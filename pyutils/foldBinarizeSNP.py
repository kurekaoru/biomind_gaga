import sys
import numpy
import random
import os
from georead import *

INFILE = sys.argv[1]
OUTDIR = sys.argv[2]
TARGET = sys.argv[3]
NFOLDS = int(sys.argv[4])

if OUTDIR[-1] == '/': OUTDIR = OUTDIR[0:-1]

def mapFunc(List):
	out = [List[0]]
	for x in List[1:]:
		if '2' in x:
			out.append('1')
		else:
			out.append('0')
	return out

File = [x.rstrip().split('\t') for x in open(INFILE).readlines()]

out = [File[0]]

targetLine = ['']
for x in File[1]:
	if x == TARGET:
		targetLine.append('1')
	else:
		targetLine.append('0')

out.append(targetLine)

for x in File[2:]:
	out.append(mapFunc(x))

NDATA 		= len(out[0][1:])
TSSIZE 		= NDATA/2
ALLLINES 	= range(1,len(out[1][1:]))
TESTLINES 	= [0]+[x+1 for x in random.sample(range(NDATA),TSSIZE)]
TRAINLINES 	= [0]+complement(TESTLINES,ALLLINES)

os.system('mkdir -p '+OUTDIR)

for i in range(NFOLDS):
	TSFILE = open(OUTDIR+'/test'+str(i)+'.tab','w')
	TRFILE = open(OUTDIR+'/train'+str(i)+'.tab','w')
	#TSFILE = open(OUTDIR+'/out'+repr(i)+'.test','w')
	#TRFILE = open(OUTDIR+'/out'+repr(i)+'.train','w')
	for x in out:
		TSFILE.write('\t'.join(getByIndex2(x,TESTLINES))+'\n')
		TRFILE.write('\t'.join(getByIndex2(x,TRAINLINES))+'\n')
	TSFILE.close()
	TRFILE.close()
