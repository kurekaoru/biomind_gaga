import sys
import numpy

I = open(sys.argv[1]).readlines()

O = open(sys.argv[2],'w')

O.write(' '.join(I[0].split(' ')[1:]))

O.close()
