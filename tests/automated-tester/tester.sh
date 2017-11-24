#!/bin/bash

RUNER="parser"
NUMBER=$(ls ./tests| wc -l)
ITERATE=$(( $NUMBER / 2 ))
TOITERATE=1
TODELETE=1
printf "Results of automated-tester\n" > results.txt
if [ "$1" = "-d" ]; then
    find ./tests -name '*.out' -print0 | xargs -0 rm
    find ./tests -name '*.ifj' -print0 | xargs -0 rm
    find ./tests -name '*.dif' -print0 | xargs -0 rm
    
else
    while [ $TOITERATE -le $ITERATE ]
    do
        ./$RUNER < ./tests/$TOITERATE.code > ./tests/$TOITERATE.ifj
        ./ic17int ./tests/$TOITERATE.ifj > ./tests/$TOITERATE.out
        if diff ./tests/$TOITERATE.out ./tests/$TOITERATE.correct; then
            printf "$TOITERATE\342\234\224\n" >> results.txt

            rm ./tests/$TOITERATE.ifj
            rm ./tests/$TOITERATE.out
        else
            printf "$TOITERATE\342\234\227\n" >> results.txt
            diff ./tests/$TOITERATE.out ./tests/$TOITERATE.correct > ./tests/$TOITERATE.dif 
        fi
        TOITERATE=$(($TOITERATE + 1))
    done
fi