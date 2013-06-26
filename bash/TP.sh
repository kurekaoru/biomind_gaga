tp() {
	awk '{for (i=1; i<=NF; i++) { arr[i] = arr[i]"\t"$i; }} END { for(i=1; i<=NF; i++) print arr[i]; }' $1
}

#cp $1 temp/temp.tab

cp $1 $3

head -2 $1 | tail -1 | tp | shuf | tp > temp/$2.tab

export X=$(head -2 $1 | tail -1 | tp | shuf | tp)

#sed "2 c\\$X" -i temp/temp.tab

sed "2 c\\$X" -i $3
