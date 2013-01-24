for file in $(ls asm | grep -v out | grep ".asm"); do 
	ruby minx_mini_compiler.rb ./asm/$file
done
