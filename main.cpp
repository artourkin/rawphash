#include <QCoreApplication>
#include <iostream>
#include <pHash.h>
#include <libraw/libraw.h>

#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <algorithm>

using namespace std;

struct ph_imagepoint{
    ulong64 hash;
    char *id;
};

struct ph_imagepoint* ph_malloc_imagepoint(){

    return (struct ph_imagepoint*)malloc(sizeof(struct ph_imagepoint));

}

int process_image(char *file)
{
        // Let us create an image processor
        LibRaw * iProcessor = new LibRaw();

        // Open the file and read the metadata
        iProcessor->open_file(file);

        // The metadata are accessible through <a href="API-datastruct-eng.html">data fields of the class</a>
        printf("Image size: %d x %d\n",iProcessor->imgdata.sizes.width,iProcessor->imgdata.sizes.height);

        // Let us unpack the image
        iProcessor->unpack();
        int ret = iProcessor->dcraw_process();
        // Convert from imgdata.rawdata to imgdata.image:
        //iProcessor.raw2image();
        int error= iProcessor->dcraw_ppm_tiff_writer("tmp1.tiff");
        // And let us print its dump; the data are accessible through <a href="API-datastruct-eng.html">data fields of the class</a>

        //fprintf(error,"Cannot write: %s\n",libraw_strerror(ret));
        // Finally, let us free the image processor for work with the next image
        iProcessor->recycle();
}

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();
    ulong64 myhash1=0;
    ulong64 myhash2=0;
    ph_imagepoint *dp = NULL;
    ph_dct_imagehash("/home/artur/elsa.jpg", myhash1);
    ph_dct_imagehash("/home/artur/elsa.jpg", myhash2);
    dp = ph_malloc_imagepoint();
    cout<<myhash1<<endl;
    cout<<myhash2<<endl;
    int i=  ph_hamming_distance(myhash1, myhash2);
     cout<<i<<endl;
    process_image("/home/artur/test.dng");
    return 0;
}

