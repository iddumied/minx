#
# Test some bit operations here
#

MOVI 0xB0 0x00
MOVI 0xB1 0x01
MOVI 0xB2 0x02

AND 0xB1 0xB2
OR 0xB1 0xB2

# set bit 2
SETB 0xB1 0xB2
# clear bit 2
CLRB 0xB1 0xB2 

# value in 0xB1 should be 0x01 again
EQLI 0xB1 0x01 
# if so, in akku there is 1 now, else 0

DEC 0x02
# and now there is zero if everything worked fine, else 0xFFFFFFFFFFFFFFFF

#
# It's so damn easy
#
EXIT 0x02
