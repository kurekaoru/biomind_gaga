echo " +=======================================================================+";
echo " | biomindGA2 C++ DIRTY HACK                          //2012 Biomind LLC |";
echo " +=======================================================================+";

rm -rf temp
mkdir -p temp
g++ -I/usr/local/include -I/usr/local/include/cppconn -I/usr/local/lib -l mysqlcppconn -o temp/batch batch.cpp
temp/batch $2
g++ -I/usr/local/include -I/usr/local/include/cppconn -I/usr/local/lib -l mysqlcppconn -o temp/X $1

export filename=$2

export KEY=$(ls temp | grep -o '^[ ]*[0-9a-fA-F]\{8\}-[0-9a-fA-F]\{4\}-[0-9a-fA-F]\{4\}-[0-9a-fA-F]\{4\}-[0-9a-fA-F]\{12\}')

rm temp/$KEY

export MAPP_NR_CPUS=1

# use >1 for multiprocessing

function mapp() {
	if [[ -z $MAPP_NR_CPUS ]] ; then
		local MAPP_NR_CPUS=$(grep "processor	:" < /proc/cpuinfo | wc -l)
	fi
	local mapp_pid=$(exec bash -c 'echo $PPID')
	local mapp_funname=$1
	local -a mapp_params
	mapp_params=("$@")
	local mapp_nr_args=${#mapp_params[@]}
	local mapp_current=0
	function mapp_trap() {
		#echo "PROGRESS: $((mapp_current*100/mapp_nr_args))%" 1>&2
		if [[ $mapp_current -lt $mapp_nr_args ]] ; then
			let mapp_current+=1
		(
			sleep 5
			$mapp_funname "${mapp_params[$mapp_current]}"
			kill -USR1 $mapp_pid
		) &
		fi
	}

	trap mapp_trap SIGUSR1

	while [[ $mapp_current -lt $mapp_nr_args ]]; do
		wait
		if [[ $mapp_current -lt $mapp_nr_args && $? -lt 127 ]] ; then
			sleep 1
			local mapp_tmp_count=$mapp_current
			wait
			if [[ $mapp_tmp_count -eq $mapp_current ]] ; then
				#echo "   MAPP_FORCE" 1>&2
				for i in $(seq 1 ${MAPP_NR_CPUS}) ; do
					sleep 5
					mapp_trap
				done
			fi
		fi
	done

	for i in $(seq 1 ${MAPP_NR_CPUS}) ; do
		wait
	done

	trap - SIGUSR1
	unset -f mapp_trap
}

function flipper() {
	temp/X $filename $KEY
}

#GET NUMBER OF TASKS FROM MYSQL SERVER AND FEED IT INTO NEXT STEP
export NUMOFTASKS=$(expr `mysql -u kaoru -prbdWK2uk -e 'select distinct(totalTasks) from GA_sessions.batch where batchId = "'$KEY'"' | grep -E '^[0-9]+$'`)

#mapp flipper {0..($NUMOFTASKS)}

mapp flipper $(seq 0 $NUMOFTASKS)

echo -e ' ALL PROCESSES COMPLETED'
rm -rf temp

#REMOVING KEY `ls | grep -o '[0-9a-fA-F]\{8\}-[0-9a-fA-F]\{4\}-[0-9a-fA-F]\{4\}-[0-9a-fA-F]\{4\}-[0-9a-fA-F]\{12\}'`


#ssh -L 3307:127.0.0.1:3306 kaoru@71.84.65.212
#mysql -ukaoru -prbdWK2uk --port 3307 --host 127.0.0.1
