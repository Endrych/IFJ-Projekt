#!/bin/bash

ARGUMENT=$1
NUMBER_OF_FOLDERS=$(ls ./tests| wc -l)
TOITERATE=1
FOLDER_TOITERATE=1
TODELETE=1
printf "Results of automated-tester\n" > results.txt
if [ $ARGUMENT = "-d" ]; then
    find ./tests -name '*.out' -print0 | xargs -0 rm
    find ./tests -name '*.ifj' -print0 | xargs -0 rm
    find ./tests -name '*.dif' -print0 | xargs -0 rm
elif [ $ARGUMENT = "-h" ]; then
    printf "Navod:\n"
elif [ "$#" -eq 1 ]; then
    if [ ! -f $ARGUMENT ]; then
        printf "Vami zadany argument \"$ARGUMENT\" neni pozice souboru\n"
    else
        sh tester.sh -d
        while [ $FOLDER_TOITERATE -le $NUMBER_OF_FOLDERS ]
        do
            FOLDER_NAME=$(ls ./tests | grep $FOLDER_TOITERATE-)
            printf "____________\n" >> results.txt
            printf "Folder $FOLDER_NAME:\n" >> results.txt
            NUMBER=$(ls ./tests/$FOLDER_NAME| wc -l)
            ITERATE=$(( $NUMBER / 2 ))
            while [ $TOITERATE -le $ITERATE ]
            do
                ./$ARGUMENT < ./tests/$FOLDER_NAME/$TOITERATE.code > ./tests/$FOLDER_NAME/$TOITERATE.ifj
                RETURN_CODE=$?
                ./ic17int ./tests/$FOLDER_NAME/$TOITERATE.ifj > ./tests/$FOLDER_NAME/$TOITERATE.out
                if diff ./tests/$FOLDER_NAME/$TOITERATE.out ./tests/$FOLDER_NAME/$TOITERATE.correct; then
                    printf "$TOITERATE\342\234\224 - Return code: $RETURN_CODE\n" >> results.txt

                    rm ./tests/$FOLDER_NAME/$TOITERATE.ifj
                    rm ./tests/$FOLDER_NAME/$TOITERATE.out
                else
                    printf "$TOITERATE\342\234\227 - Return code: $?\n" >> results.txt
                    diff ./tests/$FOLDER_NAME/$TOITERATE.out ./tests/$FOLDER_NAME/$TOITERATE.correct > ./tests/$FOLDER_NAME/$TOITERATE.dif 
                fi
                TOITERATE=$(($TOITERATE + 1))
            done
            TOITERATE=1
            FOLDER_TOITERATE=$(($FOLDER_TOITERATE + 1))
        done
    fi
else
    printf "_______________________________________________\n"
    printf "|Je vyzadovan pouze jeden argument!|           |\n"
    printf "|Pro napovedu zadejte -h                       |\n"
    printf "|Pro smazani vysledku testu -d                 |\n"
    printf "|Pro spusteni programu zadejte pozici kompileru|\n"
    printf "¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n"
    
fi