MOVI 0x02 0x00 

MOVI 0xB1 0x00 
DEC 0xB1 

# if akku is zero jump to EOP, else to 0x00
IFZJMPRA 0xB1 0x00