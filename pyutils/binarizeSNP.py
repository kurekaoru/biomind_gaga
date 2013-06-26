# To replace foldSelectSPSs in openbiomind!

# SNP Binarization - Not purely mathematical so read this!!
# In Binarizing SNPs we use the fact that most SNPS only has two significant versions, as denoted 1 or 2 in the formats. during binarization we define this as ABSENCE(0) OR PRESENCE(1) OF THE WILD TYPE VARIANT (2).
# Assumptions is that the presence of recessive alleles are completely silenced by the presence of dominant version. (Thats a BIG assumption!!!)

import sys

File = sys.argv[1]
File = '../base/well.tab'
OFile = './wellT.tab'

File = [x.rstrip().split('\t') for x in open(File).readlines()]

out = [File[0]]

targetLine = ['']
for x in File[1]:
	if x == targetCategory:
		targetLine.append('1')
	else:
		targetLine.append('0')

out.append(targetLine)

def mapFunc(List):
	out = [List[0]]
	for x in List[1:]:
		if '2' in x:
			out.append('1')
		else:
			out.append('0')
	return out

for x in File[2:]:
	out.append(mapFunc(x))

OFILE = open(OFile,'w')
for x in out:
	OFILE.write('\t'.join(x)+'\n')

OFILE.close()
