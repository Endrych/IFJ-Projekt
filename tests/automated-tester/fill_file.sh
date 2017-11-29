FILE=$1
NUM=$2
TOITERATE=1
while [ $TOITERATE -le $NUM ]
do
    touch ./$FILE/$TOITERATE.code
    echo "Scope" >> ./$FILE/$TOITERATE.code 
    echo "End Scope" >> ./$FILE/$TOITERATE.code 
    touch ./$FILE/$TOITERATE.correct
    TOITERATE=$(($TOITERATE+1))
done