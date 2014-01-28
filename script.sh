rm results.csv;
for filepath_original in `ls -v $1*`
do
filename_original=$(basename "$filepath_original")
extension="${filename_original##*.}"
name_original="${filename_original%.*}"
filepath_converted=$(find $2 -name "$name_original*")
echo $filepath_original
echo $filepath_converted
similarity=$(./rawphash $filepath_original $filepath_converted)
echo $similarity
echo "$filepath_original,$filepath_converted,$similarity" >> results.csv
done;
