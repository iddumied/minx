@ops = { 
  "NOP" =>    0x00,
  "CALL" =>   0x01,
  "RET" =>    0x02,
  "MOV" =>    0x03,
  "MOVI" =>   0x04,
  "NOT" =>    0x05,
  "NOTR" =>   0x06,
  "AND" =>    0x07,
  "ANDI" =>   0x08,
  "ANDR" =>   0x09,
  "ANDIR" =>  0x0A,
  "OR" =>     0x0B,
  "ORI" =>    0x0C,
  "ORR" =>    0x0D,
  "ORIR" =>   0x0E,
  "DEC" =>    0x0F,
  "INC" =>    0x10,
  "LSH" =>    0x11,
  "RSH" =>    0x12,
  "PUSH" =>   0x20,
  "POP" =>    0x21,
  "DROP" =>   0x22,
  "ADD" =>    0x30,
  "ADDI" =>   0x31,
  "ADDR" =>   0x32,
  "ADDIR" =>  0x33,
  "JMP" =>    0x40,
  "JMPIZ" =>  0x41,
  "JMPNZ" =>  0x42,
  "IFZJMP" => 0x43,
}

@code = ""

loop do 
  line = gets
  nodes = line.split(" ")
  code = nodes.shift
  args = nodes.map { |a| [a.to_i(16)].pack("Q").reverse }

  break unless @ops.keys.include? code

  puts "Code: #{@ops[code]} #{args.map { |a| a.to_s() }.join('')}"
  @code << [@ops[code]].pack("Q")
  args.each do |a| @code << a end

end

puts "code is: \"#{@code}\""
puts "write to file: "
filename = gets.chop
f = File.new(filename, "w")
IO.write(f, @code)
f.close
