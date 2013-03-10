for file in $(ls asm | grep out | grep -v endless); do

	echo -n "Run $file"
	./bin/minx asm/$file --fast
	if [[ $? -ne 0 ]]; then
		echo " :: error with $file"
	else
		echo ""
	fi

done
