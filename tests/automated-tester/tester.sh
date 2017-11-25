#!/bin/bash

ARGUMENT=$1
NUMBER_OF_FOLDERS=$(ls ./tests| wc -l)
TOITERATE=1
FOLDER_TOITERATE=1
TODELETE=1
printf "Results of automated-tester\n" > results.txt
restoreBack () {
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
if [ $ARGUMENT = "-d" ]; then
    restoreBack
elif [ $ARGUMENT = "-h" ]; then
    printf "______\n"
    printf "Navod\n"
    printf "Spousteni: tester se spousti prikazem sh tester.sh arg1, kde arg1\n"
    printf "odpovida jednomu z prepinacu, zatim mame 3 prepinace\n"
    printf '-'
    printf "h vypise help, -d uvede slozku a jednotlive soubory do puvodniho stavu\n"
    printf "a cesta_ke_kompileru spusti vsechny testy a vypise results + vygeneruje error soubory\n"
    printf "Tvorba testu\n"
    printf "Testy si ukladejte do jednotlivych slozek, slozka musi byt vzdy pojmenovana \n"
    printf "poradove_cislo-* a za pomlckou muze byt popis to je na vas ale cisla musi jit\n"
    printf "za sebou 1 2 3 ... zavisi na tom postupne spousteni testu a testovany soubor \n"
    printf "pojmenovavenjte poradove_cislo.code a vysledek proti kteremu se to ma porovnat\n"
    printf "poradove_cislo.correct zase plati stejna zasada cisla musi jit za seboun\n"
    printf "V pripade ze vystup z interpretu nebude odpovidat souboru .correct zustanou\n"
    printf "zachovany soubory .dif kde najdete diff .out a .correct, .ifj kde nejdete prepis\n"
    printf "do instrukci IFJcode17 a .out kde najdete vystup z interpretu\n"
elif [ "$#" -eq 1 ]; then
    if [ ! -f $ARGUMENT ]; then
        printf "Vami zadany argument \"$ARGUMENT\" neni pozice souboru\n"
    else
        restoreBack
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