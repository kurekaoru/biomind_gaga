#!/usr/bin/python
# Filename: X.py
# Personalized module of handy functions...
# Kaoru Ng | kurekaoru@gmail.com

version = '0.1'

def insertOrCount(M,K):
	if K in M:
		M[K] = M[K]+1
	else:
		M[K] = 1

def union(S1,S2):
	M = {}
	for x in S1:
		M[x] = True
	for x in S2:
		M[x] = True
	return M.keys()

def intersection(L1,L2):
	M = {}
	O = {}
	for x in L1:
		M[x] = True
	for x in L2:
		if x in M: O[x] = True
	return O.keys()

def groupIntersection(Group):
	if len(Group) == 1:
		return Group
	else:
		out = Group[0]
		for x in Group[1:]:
			out = intersection(out,x)
		return out

def complement(L1,L2):
	M = {}
	O = {}
	for x in L1:
		M[x] = True
	for x in L2:
		if x not in M: O[x] = True
	return O.keys()

def exportList(List,fileName):
	F = open(fileName,'w')
	F.write(repr(len(List))+'\n')
	F.write(','.join(List))
	F.close()

def mask(arr):
	out = []
	dic = {'none':True,'null':True}
	for x in arr:
		if x in dic:
			out.append(1)
		else:
			out.append(0)
	return out

def delNulls(arr):
	out = []
	dic = {'none':True,'null':True}
	for x in arr:
		if x not in dic:
			out.append(x)
	return out

def mat_normalize(mat):
	mat = array(mat)
	return mat/mat.max()

def getByIndex2(L,inds):
	out = []
	for x in inds:
		out.append(L[x])
	return out

def writelist(fileobj, list):
	for x in list:
		fileobj.write(x)

def writetable(fileobj, table, linesep='\n', fieldsep='\t'):
	for x in table:
		fileobj.write(fieldsep.join(x)+linesep)

def writecsv(filename, table, linesep='\n', fieldsep='\t'):
	F = open(filename,'w')
	for x in table:
		F.write(fieldsep.join(list(x))+linesep)
	F.close()

def which(L,regex):
	import re
	out = []
	c=0
	for x in L:
		if re.match(regex,x):
			out.append(c)
		c+=1
	return tuple(out)

def getLineByRE(L,regex, strip=True):
	lines = which(L,regex)
	if strip:
		return [L[i].strip() for i in lines]
	else:
		return [L[i] for i in lines]

def unique(L):
    out = []
    R = set()
    for x in L:
        if x not in R:
            R.add(x)
            out.append(x)
    return out

def isUnique(L):
	out = []
	R = set()
	for x in L:
		if x not in R:
		    R.add(x)
		    out.append(x)
	if L == out:
		return True
	else:
		return False

def tokenize(string):
	return string.strip().split('\t')

def translateList(List,Map,permissive = True):
	out = []
	if permissive:
		for x in List:
			if x in Map:
				out.append(Map[x])
			else:
				out.append(x)
	else:
		for x in List:
			if x in Map:
				out.append(Map[x])
			else:
				print 'Key error, abort!'
				return
	return out

def translateList2(List,Map,permissive = True):
	out = []
	if permissive:
		for x in List:
			if x in Map and Map[x] != '':
				out.append(Map[x])
			elif Map[x] == '':
				out.append(x)
			else:
				out.append(x)
	else:
		for x in List:
			if x in Map:
				out.append(Map[x])
			else:
				print 'Key error, abort!'
				return
	return out

def whichIn(subset, superList):
	out = []
	c = 0
	for x in superList:
		if x in subset:
			out.append(c)
		c += 1
	return out

def getByIndex(L,index,sort=False):
	import types
	out = []
	ind = index
	if not hasattr(index, '__iter__'):
		ind = index.split(',')
	indr = []
	for x in ind:
		if type(x) == int:
			indr.append(x)
		else:
			if ':' not in x:
				indr.append(int(x))
			elif ':' in x:
				indr.extend(range(int(x.split(':')[0]),int(x.split(':')[1])))
	if len(unique(indr))!=len(indr):
		print 'WARNING: index list contains replicated elements'
	if sort:
		indr.sort()
	for x in indr:
		out.append(L[x])
	return out

def sortMap(Map, descend=True):
	import operator
	from numpy import array
	K = []
	V = []
	S = sorted(Map.iteritems(), key=operator.itemgetter(1))
	for x in S:
		K.append(x[0])
		V.append(x[1])
	if descend:
		K.reverse()
		V.reverse()
	return array([K,V])

###############################################################################TYPE CONVERTERS

def matSort(M, indCol):
	import numpy
	O = M[:, indCol].argsort()
	S = numpy.take(M, O, 0)
	return S

def removeNull(mat,replacement):
	for x in mat:
		for i in range(len(x)):
			if x[i] == 'null':
				x[i] = replacement

def listToInt(LIST):
	return tuple([int(x) for x in LIST])

def listToFloat(LIST):
	return tuple([float(x) for x in LIST])

def listToStr(LIST):
	return tuple([str(x) for x in LIST])

# End of X.py
