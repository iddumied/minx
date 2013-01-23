for file in $(ls asm | grep out | grep -v endless); do

	./bin/minx asm/$file 
	if [[ $? -ne 0 ]]; then
		echo ":: error with $file"
	else
		echo ":: running: $file"
	fi

done
