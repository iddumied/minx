for file in $(ls asm | grep out | grep -v endless); do

	echo -n "Run $file"
	./bin/minx asm/$file 
	if [[ $? -ne 0 ]]; then
		echo "\t:: error with $file"
	else
		echo ""
	fi

done
