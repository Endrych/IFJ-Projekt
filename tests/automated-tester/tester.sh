#!/bin/bash

ARGUMENT=$1
if [ $# -eq 2 ]; then
    FOLDER_NUMBER=$2
fi
NUMBER_OF_FOLDERS=$(ls ./tests| wc -l)
TOITERATE=1
FOLDER_TOITERATE=1
TODELETE=1
restoreBack () {
    printf "Results of automated-tester\n" > results.txt
    OUT_NUM=$(find ./tests -name '*.out' | wc -l)
    IFJ_NUM=$(find ./tests -name '*.ifj' | wc -l)
    DIF_NUM=$(find ./tests -name '*.dif' | wc -l)
    if [ $OUT_NUM -ne 0 ]; then
        find ./tests -name '*.out' -print0 | xargs -0 rm
    fi
    if [ $IFJ_NUM -ne 0 ]; then
        find ./tests -name '*.ifj' -print0 | xargs -0 rm
    fi
    if [ $DIF_NUM -ne 0 ]; then
        find ./tests -name '*.dif' -print0 | xargs -0 rm
    fi
}
if [ "$ARGUMENT" = "-d" ]; then
    restoreBack
elif [ $# -eq 2 ];then
    if [ ! -f $ARGUMENT ]; then
        printf "Vami zadany argument \"$ARGUMENT\" neni pozice souboru\n"
    else
        restoreBack
        FOLDER_NAME=$(ls ./tests | grep $FOLDER_NUMBER- | sort -n)
        printf "____________\n" >> results.txt
        printf "$FOLDER_NAME:\n" >> results.txt
        printf "¯¯¯¯¯¯¯¯¯¯¯¯\n" >> results.txt
        
        NUMBER=$(ls ./tests/$FOLDER_NAME| wc -l)
        ITERATE=$(( $NUMBER / 2 ))
        while [ $TOITERATE -le $ITERATE ]
        do
            ./$ARGUMENT < ./tests/$FOLDER_NAME/$TOITERATE.code > ./tests/$FOLDER_NAME/$TOITERATE.ifj 
            COMPILER_RETURN_CODE=$?
            ./ic17int ./tests/$FOLDER_NAME/$TOITERATE.ifj  > ./tests/$FOLDER_NAME/$TOITERATE.out
            INTERPRET_RETURN_CODE=$?
            if diff ./tests/$FOLDER_NAME/$TOITERATE.out ./tests/$FOLDER_NAME/$TOITERATE.correct;  then
                printf "$TOITERATE\342\234\224 - Return codes- C: $COMPILER_RETURN_CODE I: $INTERPRET_RETURN_CODE\n" >> results.txt

                rm ./tests/$FOLDER_NAME/$TOITERATE.ifj
                rm ./tests/$FOLDER_NAME/$TOITERATE.out
            else
                printf "$TOITERATE\342\234\227 - Return codes- C: $COMPILER_RETURN_CODE I: $INTERPRET_RETURN_CODE\n" >> results.txt
                diff ./tests/$FOLDER_NAME/$TOITERATE.out ./tests/$FOLDER_NAME/$TOITERATE.correct  > ./tests/$FOLDER_NAME/$TOITERATE.dif 
            fi
            TOITERATE=$(($TOITERATE + 1))
        done
    fi
elif [ "$ARGUMENT" = "-h" ]; then
    cat ./help_print
elif [ "$#" -eq 1 ]; then
    if [ ! -f $ARGUMENT ]; then
        printf "Vami zadany argument \"$ARGUMENT\" neni pozice souboru\n"
    else
        restoreBack
        while [ $FOLDER_TOITERATE -le $NUMBER_OF_FOLDERS ]
        do
            FOLDER_NAME=$(ls ./tests | grep $FOLDER_TOITERATE- | sort -n |  head -n 1)
            printf "____________\n" >> results.txt
            printf "$FOLDER_NAME:\n" >> results.txt
            printf "¯¯¯¯¯¯¯¯¯¯¯¯\n" >> results.txt
            NUMBER=$(ls ./tests/$FOLDER_NAME| wc -l)
            ITERATE=$(( $NUMBER / 2 ))
            while [ $TOITERATE -le $ITERATE ]
            do
                ./$ARGUMENT < ./tests/$FOLDER_NAME/$TOITERATE.code > ./tests/$FOLDER_NAME/$TOITERATE.ifj 
                COMPILER_RETURN_CODE=$?
                ./ic17int ./tests/$FOLDER_NAME/$TOITERATE.ifj  > ./tests/$FOLDER_NAME/$TOITERATE.out
                INTERPRET_RETURN_CODE=$?
                if diff ./tests/$FOLDER_NAME/$TOITERATE.out ./tests/$FOLDER_NAME/$TOITERATE.correct;  then
                    printf "$TOITERATE\342\234\224 - Return codes- C: $COMPILER_RETURN_CODE I: $INTERPRET_RETURN_CODE\n" >> results.txt

                    rm ./tests/$FOLDER_NAME/$TOITERATE.ifj
                    rm ./tests/$FOLDER_NAME/$TOITERATE.out
                else
                    printf "$TOITERATE\342\234\227 - Return codes- C: $COMPILER_RETURN_CODE I: $INTERPRET_RETURN_CODE\n" >> results.txt
                    diff ./tests/$FOLDER_NAME/$TOITERATE.out ./tests/$FOLDER_NAME/$TOITERATE.correct  > ./tests/$FOLDER_NAME/$TOITERATE.dif 
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

clear