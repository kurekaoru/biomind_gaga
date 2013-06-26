# To replace foldSelectSPSs in openbiomind!

import sys
import numpy
import random

File = sys.argv[1]
#File = './wellT.tab'
File = [x.rstrip().split('\t') for x in open(File).readlines()]
NFOLDS = 3
TSSIZE = int(10)

def complement(L1,L2):
	M = {}
	O = {}
	for x in L1:
		M[x] = True
	for x in L2:
		if x not in M: O[x] = True
	return O.keys()

NDATA = len(F[1][1:])
ALLLINES = range(1,len(F[1][1:]))
TESTLINES = [0]+[x+1 for x in random.sample(range(NDATA),TSSIZE)]
TRAINLINES = [0]+complement(TESTLINES,ALLLINES)
STEM = 'out'

def getByIndex2(L,inds):
	out = []
	for x in inds:
		out.append(L[x])
	return out

for i in range(NFOLDS):
	TSFILE = open('test'+str(i)+'.tab','w')
	TRFILE = open('train'+str(i)+'.tab','w')
	for x in File:
		TSFILE.write('\t'.join(getByIndex2(x,TESTLINES))+'\n')
		TRFILE.write('\t'.join(getByIndex2(x,TRAINLINES))+'\n')
	TSFILE.close()
	TRFILE.close()
			
