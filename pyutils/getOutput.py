import sys
import numpy

I = open(sys.argv[1]).readlines()

O = open(sys.argv[2],'w')

for x in I:
	O.write(x.split(',')[0]+'\n')

O.close()
