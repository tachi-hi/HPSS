#!usr/bin/bash

SEP=./HPSS_smoothness_eval # MODIFY THIS SUITABLY

RES=my_result.txt # MODIFY THIS SUITABLY
CSV=result.csv # MODIFY THIS SUITABLY

#if [ -e comment_out.txt ]; then 
	if [ -e ${RES} ]
	then
		rm ${RES}
	fi

	echo Inst,filename,InstType,SmoothType,smoothness,_>> ${RES}

	for input in `ls ./wav/*.WAV`
	do
		touch tmp_H.txt tmp_P.txt tmp.txt; rm tmp_H.txt tmp_P.txt tmp.txt

		echo $input
		echo $i $input >> tmp_H.txt; echo a >> tmp_H.txt; echo alpha >> tmp_H.txt
		echo $i $input >> tmp_P.txt; echo a >> tmp_P.txt; echo beta >> tmp_P.txt

		cp $input tmp_.wav
		sox tmp_.wav -c 1 -r 16000 tmp.wav
		$SEP -i tmp.wav -d 1 - g 0.5 -M 5 -f 1024 -s 256 -W hanning >> tmp.txt

		sed -e "2d" tmp.txt >> tmp_H.txt
		sed -e "1d" tmp.txt >> tmp_P.txt
		echo "_" >> tmp_H.txt
		echo "_" >> tmp_P.txt
		cat tmp_H.txt >> ${RES}
		cat tmp_P.txt >> ${RES}
	done
#fi

cat ${RES} | perl -pe 's/\n/,/g' | perl -pe "s/,_,/\n/g" | perl -pe "s|./wav/||g" | perl -pe "s|\.WAV||g" > ${CSV}
echo "Finished. Now edit the CSV file manually."


