class OpCode 
  attr_reader :str, :code

  def initialize(str, code)
    @str = str
    @code = code 
  end

end

@opc = Array.new
def new_opcode(str, code)
  @opc << OpCode.new(str, code)
end
new_opcode("NOP",	  0x00)
new_opcode("CALL",  0x01)
new_opcode("RET",   0x02)
new_opcode("MOV",   0x03)
new_opcode("MOVI",  0x04)
new_opcode("NOT",   0x05)
new_opcode("NOTR",  0x06)
new_opcode("AND",   0x07)
new_opcode("ANDI",  0x08)
new_opcode("ANDR",  0x09)
new_opcode("ANDIR", 0x0A)
new_opcode("OR",    0x0B)
new_opcode("ORI",   0x0C)
new_opcode("ORR",   0x0D)
new_opcode("ORIR",  0x0E)
new_opcode("DEC",   0x0F)
new_opcode("INC",   0x10)
new_opcode("LSH",   0x11)
new_opcode("RSH",   0x12)
new_opcode("PUSH",  0x20)
new_opcode("POP",   0x21)
new_opcode("DROP",  0x22)
new_opcode("ADD",   0x30)
new_opcode("ADDI",  0x31)
new_opcode("ADDR",  0x32)
new_opcode("ADDIR", 0x33)
new_opcode("JMP",   0x40)
new_opcode("JMPIZ", 0x41)
new_opcode("JMPNZ", 0x42)
new_opcode("IFZJMP",0x43)

@code = ""

loop do 
  line = gets
  nodes = line.split(" ")
  code = nodes.shift
  args = nodes.map { |a| [a.to_i(16)].pack("Q").reverse }

  op = (@opc.find() { |o| o.str == code })
  break unless op 
  code = op.code

  puts "Code: #{code.to_s.to_i(16)} #{args.map { |a| a.to_s() }.join('')}"
  @code << code
  args.each do |a| @code << a end

end

puts "code is: \"#{@code}\""
puts "write to file: "
filename = gets.chop
f = File.new(filename, "w")
IO.write(f, @code)
f.close
