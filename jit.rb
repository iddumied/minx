REGISTER  = 2
VALUE     = 8
ADDRESS   = 8

class Op < Struct.new :opc, :args; end

@ops = { 
  "NOP" =>    Op.new(0x00, [] ), 
  "CALL" =>   Op.new(0x01, [ADDRESS] ), 
  "RET" =>    Op.new(0x02, [] ), 
  "MOV" =>    Op.new(0x03, [REGISTER, REGISTER] ), 
  "MOVI" =>   Op.new(0x04, [REGISTER, VALUE] ), 
  "NOT" =>    Op.new(0x05, [REGISTER] ), 
  "NOTR" =>   Op.new(0x06, [REGISTER] ), 
  "AND" =>    Op.new(0x07, [REGISTER, REGISTER] ), 
  "ANDI" =>   Op.new(0x08, [REGISTER, VALUE] ), 
  "ANDR" =>   Op.new(0x09, [REGISTER, REGISTER] ), 
  "ANDIR" =>  Op.new(0x0A, [REGISTER, VALUE] ), 
  "OR" =>     Op.new(0x0B, [REGISTER, REGISTER] ), 
  "ORI" =>    Op.new(0x0C, [REGISTER, VALUE] ), 
  "ORR" =>    Op.new(0x0D, [REGISTER, REGISTER] ), 
  "ORIR" =>   Op.new(0x0E, [REGISTER, VALUE] ), 
  "DEC" =>    Op.new(0x0F, [REGISTER] ), 
  "INC" =>    Op.new(0x10, [REGISTER] ), 
  "LSH" =>    Op.new(0x11, [REGISTER] ), 
  "RSH" =>    Op.new(0x12, [REGISTER] ), 
  "PUSH" =>   Op.new(0x20, [REGISTER] ), 
  "POP" =>    Op.new(0x21, [REGISTER] ), 
  "DROP" =>   Op.new(0x22, [] ), 
  "ADD" =>    Op.new(0x30, [REGISTER, REGISTER] ), 
  "ADDI" =>   Op.new(0x31, [REGISTER, VALUE] ), 
  "ADDR" =>   Op.new(0x32, [REGISTER, REGISTER] ), 
  "ADDIR" =>  Op.new(0x33, [REGISTER, VALUE] ), 
  "JMP" =>    Op.new(0x40, [ADDRESS] ), 
  "JMPIZ" =>  Op.new(0x41, [REGISTER, ADDRESS] ), 
  "JMPNZ" =>  Op.new(0x42, [REGISTER, ADDRESS] ), 
  "IFZJMP" => Op.new(0x43, [ADDRESS, ADDRESS] ), 
}



def create_args(opc, args)
  res = ""
  args.each_with_index do |arg, i| 
    if @ops[opc].args[i] == REGISTER
      res << [arg.to_i(16)].pack("S") # pack for 16 bit if register address
    elsif @ops[opc].args[i] == ADDRESS || @ops[opc].args[i] == VALUE
      res << [arg.to_i(16)].pack("Q") # pack for 64 bit if adress or value
    end
  end
  res
end

@code = ""

loop do 
  line = gets
  nodes = line.split(" ")
  code = nodes.shift
  args = create_args(code, nodes)

  break unless @ops.keys.include? code

  @code << [@ops[code].opc].pack("S")
  @code << args

end

puts "code is: \"#{@code}\""
puts "write to file: "
filename = gets.chop
f = File.new(filename, "w")
IO.write(f, @code)
f.close
