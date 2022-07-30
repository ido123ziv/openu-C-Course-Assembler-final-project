#!/bin/bash
MY_PATH="/home/ubuntu/openu-C-Course-Assembler-final-project"
if [[ $MY_PATH == $(pwd) ]]; then 
    echo good to go!; 
else 
    cd $MY_PATH  
fi
find ./Tests -type f -name "*.as" | while read line 
do 
    FP=$(echo $line | cut -d '/' -f 3 | cut -d '.' -f 1)
    echo File is: $FP
    ./assembler Tests/$FP > outputs/$FP
done
