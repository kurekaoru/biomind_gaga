#STEP2: GET RID OF SCORE
#cat $3 | cut -d ' ' -f2- | head -n1 > $4 # $4 Top Model file

python pyutils/stripModel.py $3 $4

#cut $2 -d , -f1 | transpose | sed 's/out/\n/g' |  sed '/^$/d' | sed -e 's/^[ \t]*//' | sed 's/[ \t]*$//' | sed 's/ /,/g' > $5'TS' # $2 Test input set

python pyutils/getOutput.py $2 $5'TS'

#cut $1 -d , -f1 | transpose | sed 's/out/\n/g' |  sed '/^$/d' | sed -e 's/^[ \t]*//' | sed 's/[ \t]*$//' | sed 's/ /,/g' > $5'TR' # $2 Train (Triplets) input set

python pyutils/getOutput.py $1 $5'TR'

#gedit $1&
#gedit $2&
#gedit $3&
#gedit $4&

#eval-table -C $4 -i $2 | transpose | sed 's/output/\n/g' | sed '/^$/d' | sed -e 's/^[ \t]*//' | sed 's/[ \t]*$//' | sed 's/ /,/g' > $6'TS'

#eval-table -c `python moses/eval_table_hack.py $4` -i $2 | transpose | sed 's/output/\n/g' | sed '/^$/d' | sed -e 's/^[ \t]*//' | sed 's/[ \t]*$//' | sed 's/ /,/g' > $6'TS'

####python moses/eval_table_hack.py $4 $1 $2 out.txt

#echo python moses/eval_table_hack.py $4 $1 $2 $7
python moses/eval_table_hack.py $4 $1 $2 $7

#awk 'NR==1{print;exit}' $6'TS' >> $5'TS' #TEST

#eval-table -C $4 -i $1 | transpose | sed 's/output/\n/g' | sed '/^$/d' | sed -e 's/^[ \t]*//' | sed 's/[ \t]*$//' | sed 's/ /,/g' >> $6'TR' #TRAIN

#awk 'NR==1{print;exit}' $6'TR' >> $5'TR' #TEST


