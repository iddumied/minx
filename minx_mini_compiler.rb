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
OPCODE    = 2
REGISTER  = 2
VALUE     = 8
ADDRESS   = 8
MEMORY    = REGISTER 

# helper to store stuff
class Op < Struct.new :opc, :args; end

#
# These are the opcodes. You can also read them in the ByteCode.def file.
#
$ops = { 
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

  "SETB" =>   Op.new(0x17, [REGISTER, REGISTER]), 
  "SETBI" =>  Op.new(0x18, [REGISTER, VALUE]), 
  "CLRB"  =>  Op.new(0x19, [REGISTER, REGISTER]), 
  "CLRBI" =>  Op.new(0x1A, [REGISTER, VALUE]), 

  "ADD" =>    Op.new(0x20, [REGISTER, REGISTER] ), 
  "ADDI" =>   Op.new(0x21, [REGISTER, VALUE] ), 
  "ADDR" =>   Op.new(0x22, [REGISTER, REGISTER] ), 
  "ADDIR" =>  Op.new(0x23, [REGISTER, VALUE] ), 

  "SUB" =>    Op.new(0x24, [REGISTER, REGISTER] ), 
  "SUBI" =>   Op.new(0x25, [REGISTER, VALUE] ), 
  "SUBR" =>   Op.new(0x26, [REGISTER, REGISTER] ), 
  "SUBIR" =>  Op.new(0x27, [REGISTER, VALUE] ), 

  "MUL" =>    Op.new(0x28, [REGISTER, REGISTER] ), 
  "MULI" =>   Op.new(0x29, [REGISTER, VALUE] ), 
  "MULR" =>   Op.new(0x2A, [REGISTER, REGISTER] ), 
  "MULIR" =>  Op.new(0x2B, [REGISTER, VALUE] ), 

  "DIV" =>    Op.new(0x2C, [REGISTER, REGISTER] ), 
  "DIVI" =>   Op.new(0x2D, [REGISTER, VALUE] ), 
  "DIVR" =>   Op.new(0x2E, [REGISTER, REGISTER] ), 
  "DIVIR" =>  Op.new(0x2F, [REGISTER, VALUE] ), 

  "POW" =>    Op.new(0x30, [REGISTER, REGISTER] ), 
  "POWI" =>   Op.new(0x31, [REGISTER, VALUE] ), 
  "POWR" =>   Op.new(0x32, [REGISTER, REGISTER] ), 
  "POWIR" =>  Op.new(0x33, [REGISTER, VALUE] ), 

  "SQRT" =>   Op.new(0x34, [REGISTER] ), 
  "SQRTR" =>  Op.new(0x35, [REGISTER] ), 
  
  "MOD" =>    Op.new(0x36, [REGISTER, REGISTER] ), 
  "MODI" =>   Op.new(0x37, [REGISTER, VALUE] ), 
  "MODR" =>   Op.new(0x38, [REGISTER, REGISTER] ), 
  "MODIR" =>  Op.new(0x39, [REGISTER, VALUE] ), 

  "RAND" =>   Op.new(0x3A, [] ), 
  "URAND" =>  Op.new(0x3B, [] ), 

  "READSTACK"=>Op.new(0x3C, [] ), 
  "PUSH" =>   Op.new(0x3D, [REGISTER] ), 
  "POP" =>    Op.new(0x3E, [REGISTER] ), 
  "DROP" =>   Op.new(0x3F, [] ), 

  "JMP" =>    Op.new(0x40, [ADDRESS] ), 
  "JMPIZ" =>  Op.new(0x41, [REGISTER, ADDRESS] ), 
  "JMPNZ" =>  Op.new(0x42, [REGISTER, ADDRESS] ), 
  "IFZJMP" => Op.new(0x43, [ADDRESS, ADDRESS] ), 
  "EXIT" =>   Op.new(0x44, [REGISTER] ), 

  "XOR" =>    Op.new(0x45, [REGISTER, REGISTER] ), 
  "XORI" =>   Op.new(0x46, [REGISTER, VALUE] ), 
  "XORR" =>   Op.new(0x47, [REGISTER, REGISTER] ), 
  "XORIR" =>  Op.new(0x48, [REGISTER, VALUE] ), 

  "JMPR" =>   Op.new(0x49, [REGISTER] ), 
  "JMPIZR" => Op.new(0x4A, [REGISTER, REGISTER] ), 
  "JMPNZR" => Op.new(0x4B, [REGISTER, REGISTER] ), 
  "IFZJMPRR"=>Op.new(0x4C, [REGISTER, REGISTER] ), 
  "IFZJMPRA"=>Op.new(0x4D, [REGISTER, ADDRESS] ), 
  "LDADDR" => Op.new(0x4E, [] ), 
  "EXITI" =>  Op.new(0x4F, [VALUE] ), 

  "PSTACK" => Op.new(0x50, []), 
  "PREGS" =>  Op.new(0x51, []), 
  "PPROG" =>  Op.new(0x52, []), 
  "ALLOC" =>  Op.new(0x60, [REGISTER]), 
  "ALLOCI" => Op.new(0x61, [VALUE]), 
  "RESIZE" => Op.new(0x62, [MEMORY, REGISTER]), 
  "RESIZEI"=> Op.new(0x63, [MEMORY, VALUE]), 
  "FREE" =>   Op.new(0x64, [MEMORY]), 
  "PUT" =>    Op.new(0x65, [MEMORY, REGISTER, REGISTER, REGISTER]), 
  "READ" =>   Op.new(0x66, [MEMORY, REGISTER, REGISTER, REGISTER]), 
  "GETSIZE"=> Op.new(0x67, [MEMORY]), 

  "LBSH" =>   Op.new(0x80, [REGISTER]), 
  "RBSH" =>   Op.new(0x81, [REGISTER]), 
}

class JumpMark

  attr_accessor :name, :hex

  def initialize(name, hex)
    @name = name.gsub("\n", "").gsub(" ", "")
    @hex = hex 
  end

end

class CodeReader

  def is_comment?(line)
    line.start_with? ";" or line.start_with? "#"
  end

  def empty_line?(line)
    line.chomp.empty? or line.strip.empty?
  end

  def hex_of(i)
    "0x" + i.to_s(16).upcase
  end

end

class Compiler < CodeReader

  attr_reader :source, :jumpmarks, :code

  def initialize(filelines, jumpmarks)
    @source = filelines
    
    @jumpmarks = Array.new
  end

  def compile
    code = String.new
    @source.each do |line|
      next if is_comment?(line) 
      next if empty_line?(line)

      nodes = line.split(" ")
      opcode = nodes.shift
      args = create_args(opcode, nodes)

      fail "UNALLOWED OPCODE '#{opcode}'" unless $ops.keys.include? opcode

      code << [$ops[opcode].opc].pack("S")
      code << args
    end

    code
  end

  #
  # decode arguments to binary
  #
  def create_args(opc, args)
    res = ""
    args.each_with_index do |arg, i|
      if [REGISTER, MEMORY].include? $ops[opc].args[i]
        res << [arg.to_i(16)].pack("S") # pack for 16 bit if register address

      elsif [ADDRESS, VALUE].include? $ops[opc].args[i]
        res << [arg.to_i(16)].pack("Q") # pack for 64 bit if adress or value

      end
    end
    res
  end

end

class Preprocessor < CodeReader

  attr_reader :jumpmarks, :source

  def initialize(source)
    @source = source
    @jumpmarks = Array.new
    preprocess
    translate_jumpmarks
  end

  def preprocess 
    offset = 0
    @source.each do |line|
      next if is_comment?(line)
      next if empty_line?(line)

      matched = line.match(/[a-z_]*:/)
      if matched
        @jumpmarks << JumpMark.new(matched.string.gsub(":", ""), hex_of(offset))

      else 
        opcode = line.split(" ").first
        if $ops[opcode].nil? then fail "Opcode not found: '#{opcode}'" end
        offset += OPCODE + $ops[opcode].args.inject { |x, sum| sum += x }

      end

    end
  end

  #
  # translate jumpmarks to it's hexadecimal jump addresses.
  #
  def translate_jumpmarks
    @jumpmarks.each do |mark|
      @source.map! do |line|
        if line.include? mark.name and not line.include? ":"
          line.gsub!(mark.name, mark.hex)

        elsif line.include? "#{mark.name}:"
          #
          # remove jumpmarks
          # 
          line.gsub!(mark.name + ":", "; JUMPMARK: #{mark.name}")

        end
        
        line
      end
    end
  end

end

if __FILE__ == $0
  if ARGV.empty? 
    puts "No arguments."
    exit 1
  end

  f = File.open(ARGV.first, "r")
  source = f.readlines
  pre = Preprocessor.new(source)

  #
  # print preprocessed source of -E is set
  #
  if ARGV.include? "-E"
    puts "Marks:" 
    puts pre.jumpmarks.map { |m| m.name }.join(", ")
    puts "\nPreprocessed:"
    puts pre.source
    exit 0
  end

  com = Compiler.new(pre.source, pre.jumpmarks)

  code = com.compile

  new_filename = ARGV.first + ".out"

  nf = File.open(new_filename, "w+")
  nf.write(code)
  nf.close
  
  puts "#{ARGV.first} -> #{new_filename}"
end
