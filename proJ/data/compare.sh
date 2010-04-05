#! /bin/bash


ls correctres > tmp1
ls frame1 > tmp2
echo "Has the list of files that have changed?"
diff tmp1 tmp2

echo "Have the individual files changed?"
for i in `ls correctres | xargs`
do
	diff correctres/${i} frame1/${i}
done
	
