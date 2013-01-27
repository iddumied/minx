#
# This is a minimalistic compiler for the minx virtual machine.!
#
# it does not provide automatic jumps, aliases for registers or other heavy
# stuff. It just translates simple pseudo-asm to binary which can be run on minx.
#
# You can pass a argument which should be a file with opcodes in it. As you
# might notice: there are just opcodes because I am friendly and nobody wants to
# write HEX, anyway, arguments are still hex.
#

#
# Sizes of ...
#
REGISTER  = 2
VALUE     = 8
ADDRESS   = 8
MEMORY    = REGISTER 

# helper to store stuff
class Op < Struct.new :opc, :args; end

#
# These are the opcodes. You can also read them in the ByteCode.def file.
#
@ops = { 
# opcode  =>         op    args
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
  "CMP" =>    Op.new(0x13, [REGISTER, REGISTER]), 
  "CMPI" =>   Op.new(0x14, [REGISTER, VALUE]), 
  "EQL" =>    Op.new(0x15, [REGISTER, REGISTER]), 
  "EQLI" =>   Op.new(0x16, [REGISTER, VALUE]), 
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
  "PSTACK" => Op.new(0x50, []), 
  "PREGS" =>  Op.new(0x51, []), 
  "PPROG" =>  Op.new(0x52, []), 
  "ALLOC" =>  Op.new(0x60, [REGISTER]), 
  "ALLOCI" => Op.new(0x61, [VALUE]), 
  "RESIZE" => Op.new(0x62, [MEMORY, REGISTER]), 
  "RESIZEI"=> Op.new(0x63, [MEMORY, VALUE]), 
  "FREE" =>   Op.new(0x64, [MEMORY]), 
  "PUT" =>    Op.new(0x65, [MEMORY, REGISTER, REGISTER]), 
  "READ" =>   Op.new(0x66, [MEMORY, REGISTER, REGISTER]), 
  "GETSIZE"=> Op.new(0x67, [MEMORY]), 
}

#
# decode arguments to binary
#
def create_args(opc, args)
  res = ""
  args.each_with_index do |arg, i| 
    if [REGISTER, MEMORY].include? @ops[opc].args[i]
      res << [arg.to_i(16)].pack("S") # pack for 16 bit if register address
    elsif [ADDRESS, VALUE].include? @ops[opc].args[i]
      res << [arg.to_i(16)].pack("Q") # pack for 64 bit if adress or value
    end
  end
  res
end

@code = ""

#
# compile a line
# 
def process_line(line)
    nodes = line.split(" ")
    code = nodes.shift
    args = create_args(code, nodes)

    fail "UNALLOWED OPCODE #{code}" unless @ops.keys.include? code

    @code << [@ops[code].opc].pack("S")
    @code << args
end

#
# read stdin and compile just in time to binary
#
def read_stdin
  loop do 
    process_line(gets)
  end
end

#
# read file f and compile lines to binary
#
def read_file f
  lines = File.readlines(f)
  lines.each do |line| 
    next if line.start_with? ";" or line.start_with? "#"
    next if line.strip.empty? 
    process_line(line)
  end
end

#
# run the stuff...
#

if ARGV.empty?
  read_stdin
else
  read_file( ARGV.first )
end

puts "code is: \"#{@code}\""
sleep 0.5
puts "write to file: "

if ARGV.empty?
  filename = gets.chop
  f = File.new(filename, "w")
else
  filename = ARGV.first + ".out"
  f = File.open(filename, "w")
end

puts filename
IO.write(f, @code)
f.close
