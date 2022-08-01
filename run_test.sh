#!/bin/bash
MY_PATH="/home/ido/openu-C-Course-Assembler-final-project"
if [[ $MY_PATH == $(pwd) ]]; then 
    echo good to go!; 
else 
    cd $MY_PATH  
fi
rm Tests/*.am*
rm Tests/*.ext*
rm Tests/*.ent*
mv Tests/expected.ob Tests/expected.save
rm Tests/*.ob*
mv Tests/expected.save Tests/expected.ob
find ./Tests -type f -name "*.as" | while read line 
do 
    FP=$(echo $line | cut -d '/' -f 3 | cut -d '.' -f 1)
    echo File is: $FP
    ./assembler Tests/$FP > outputs/$FP
done
