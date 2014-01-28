#include <iostream>
#include <pHash.h>
#include <libraw/libraw.h>

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <algorithm>

using namespace std;


int raw2tiff(char *file_input,char * file_output)
{
    if(!file_input || !file_output) return 0;
    int ret;
    LibRaw  iProcessor;
    //printf("Processing file %s\n",file_input);
    iProcessor.open_file(file_input);
    if( (ret = iProcessor.open_file(file_input)) != LIBRAW_SUCCESS)
    {
        fprintf(stderr,"Cannot open_file %s: %s\n",file_input,libraw_strerror(ret));
        return 0;
    }

    iProcessor.unpack();

    ret = iProcessor.dcraw_process();

    if(LIBRAW_SUCCESS !=ret)
    {
        fprintf(stderr,"Cannot do postpocessing on %s: %s\n",
                file_input,libraw_strerror(ret));
        if(LIBRAW_FATAL_ERROR(ret))
            return 0;
    }

    //  libraw_processed_image_t * image = iProcessor->dcraw_make_mem_image();
    //  FILE * pFile;
    //  pFile = fopen ("test.bmp", "wb");
    //  fprintf (pFile, "P6\n%d %d\n%d\n", image->width, image->height, (1 << image->bits)-1);
    //  fwrite (image->data , image->data_size, 1, pFile);
    //  fclose (pFile);

   // printf("Writing file %s\n",file_output);
    if( LIBRAW_SUCCESS != (ret = iProcessor.dcraw_ppm_tiff_writer(file_output)))
        fprintf(stderr,"Cannot write %s: %s\n",file_output,libraw_strerror(ret));
    iProcessor.recycle();
}

int path_is_directory (const char* path) {
    struct stat s_buf;

    if (stat(path, &s_buf))
        return 0;

    return S_ISDIR(s_buf.st_mode);
}

void delete_folder_tree (const char* directory_name) {
    DIR*            dp;
    struct dirent*  ep;
    char            p_buf[512] = {0};

    dp = opendir(directory_name);

    while ((ep = readdir(dp)) != NULL) {
        sprintf(p_buf, "%s/%s", directory_name, ep->d_name);
        if (path_is_directory(p_buf))
            delete_folder_tree(p_buf);
        else
            unlink(p_buf);
    }

    closedir(dp);
    rmdir(directory_name);
}



char * generate_tiff(char * path,char *file_raw)
{
    char file_tiff[100];
    strcpy(file_tiff,path);
    strcat(file_tiff,file_raw);
    strcat(file_tiff, ".tiff");
    raw2tiff(file_raw,file_tiff);
    return file_tiff;
}

int main(int argc, char *argv[])
{

    if (argc < 2){
        printf("no input args\n");
        printf("expected: \"rawphash image1 image2\"\n");
        exit(1);
    }

    char *file_raw1 = argv[1];
    char *file_raw2 = argv[2];
    ulong64 hash_tiff1=0;
    ulong64 hash_tiff2=0;

    char path_tmp[100];
    strcpy(path_tmp,"tmp/");
    mkdir(path_tmp, 0755);
    char file_tiff1[100];
    char file_tiff2[100];
    file_tiff1[0] = '\0';
    file_tiff2[0] = '\0';
    strcpy(file_tiff1,path_tmp);
    strcat(file_tiff1,"1.tiff");
    strcpy(file_tiff2,path_tmp);
    strcat(file_tiff2,"2.tiff");
    raw2tiff(file_raw1,file_tiff1);
    raw2tiff(file_raw2,file_tiff2);
    ph_dct_imagehash(file_tiff1, hash_tiff1);
    ph_dct_imagehash(file_tiff2, hash_tiff2);
    int i=  ph_hamming_distance(hash_tiff1, hash_tiff2);
    cout<<i<<endl;
    //delete_folder_tree(path_tmp);
    return 0;
}

