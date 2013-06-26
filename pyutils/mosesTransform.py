import sys
import numpy

IFILE = sys.argv[1]
OFILE = sys.argv[2]

IN = numpy.array([x.rstrip().split('\t') for x in open(IFILE).readlines()])

IN_T = [x[1:] for x in numpy.transpose(IN)]
IN_T[0][0] = 'out'

OUT = open(OFILE,'w')

for x in IN_T:
	OUT.write(','.join(x)+'\n')

OUT.close()
