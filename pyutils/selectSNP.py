#Select SNP

#Basically takes away SNPS that are non-discriminative judged by discriminatory resolution
#SNPs is discrete!!

import sys
import copy

IFile = './wellT.tab'

File = [x.rstrip().split('\t') for x in open(IFile).readlines()]

def classify(File, data):
	classes = File[1][1:]
	Mclasses = unique(classes)
	MclassCount = {}
	for x in Mclasses:
		MclassCount[x] = 0
	M = {}
	for i in range(len(classes)):
		M[i] = classes[i]
	ones = which(data,'1')
	zeros = which(data,'0')
	onesclass = [M[X] for X in ones]
	zerosclass = [M[X] for X in zeros]
	onesclassCount = copy.deepcopy(MclassCount)
	zerosclassCount = copy.deepcopy(MclassCount)
	for x in onesclass:
		onesclassCount[x] = onesclassCount[x]+1
	for x in zerosclass:
		zerosclassCount[x] = zerosclassCount[x]+1
	print 'onesclass '+repr(onesclassCount)
	print 'zerosclass '+repr(zerosclassCount)
	return [onesclassCount,zerosclassCount]

a = []

import pylab
pylab.ion()

for x in File[2:]:
	b = classify(File,x[1:])[1]
	a.append(float(b['WELL'])/float(b['ILL']+b['WELL']+0.000001))


pylab.hist(a,100)
