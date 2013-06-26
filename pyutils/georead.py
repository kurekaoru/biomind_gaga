#GEOreader
import re
import numpy as np
from kfuncs import *

def show_classes(FileObj):
	from numpy import array
	subsetTypes = {}
	subsetTypeLines = which(FileObj,'!subset_type')
	for i in subsetTypeLines:
		subsetTypes[FileObj[i].strip().split(' = ')[1]] = True
	for x in subsetTypes.keys():
		subsetChildLines = which(FileObj,'!subset_type = '+x)
		print 'Class: '+x
		for y in subsetChildLines:
			print '\tCategory: '+FileObj[y-2].strip().split(' = ')[1]+' | '+repr(len(FileObj[y-1].strip().split(' = ')[1].split(','))) + ' cases'

def getClasses(FileObj):
	descs = which(FileObj,'!subset_description')
	classes = {}
	#Find all subset types:
	typeLines = which(FileObj,'!subset_type')
	for x in typeLines:
		caseClass = (FileObj[x].strip().split(' = ')[1])
		if caseClass not in classes:
			classes[caseClass] = 1
		else:
			classes[caseClass] += 1
	return classes

def manualMap(mapobj,targetCategory,TargetClass = ''):
	out = {}
	#if targetCategory not in mapobj.keys():
	while targetCategory not in mapobj.keys():
		print 'Specified target category \''+targetCategory+'\' not in target class \''+TargetClass+'\''
		print 'Available categories:' + repr(mapobj.keys())
		targetCategory = raw_input("Enter desired category: ")
		#return mapobj
	for x in mapobj.keys():
		if x == targetCategory:
			out[x] = 1
		else:
			out[x] = 0
	return out

def MapSample(FileObj, targetClass=''):
	import operator
	out = {}
	lines = which(FileObj,'!subset_sample_id')
	classes = getClasses(FileObj)
	print classes
	sortedClass = sorted(classes.iteritems(), key=operator.itemgetter(1))
	if targetClass == '':
		targetClass = sortedClass[0][0]
		print '[targetClass] not specified, using class \'' + targetClass + '\' as target'
	for x in lines:
		category = FileObj[x-1].strip().split(" = ")[1]
		if FileObj[x+1].strip().split(" = ")[1] == targetClass:
			S = FileObj[x].strip().split(" = ")[1].split(',')
			for y in S:
				if y not in out.keys():
					out[y] = category
	return out

def MapGroups(FileObj, targetCategory='', targetClass=''):
	import operator
	out = {}
	descs = which(FileObj,'!subset_description')
	classes = getClasses(FileObj)
	categories={}
	#Assume smallest member class as targetClass unless otherwise specified
	sortedClass = sorted(classes.iteritems(), key=operator.itemgetter(1))
	if targetClass == '':
		targetClass = sortedClass[0][0]
		print 'Using class \'' + targetClass + '\' as target ('+ repr(sortedClass[0][1])+' subclasses):'
	else:
		if targetClass not in classes:
			print 'Specified target class \''+targetClass+'\' not in dataset'
			print 'Available classes:'
			print classes.keys()
			return
		else:
			print 'Using class \'' + targetClass + '\' as target ('+ repr(classes[targetClass]) +' subclasses):'
	for x in descs:
		if (FileObj[x+2].strip().split(' = ')[1]) == targetClass:
			categories[FileObj[x].strip().split(" = ")[1]] = len(categories)
			print ' '+FileObj[x].strip().split(" = ")[1] +'\t'+repr(len(FileObj[x+1].strip().split(" = ")[1].split(',')))+' cases'
	#Label cases according to target category:
	if targetCategory != '':
		categories = manualMap(categories,targetCategory,TargetClass = targetClass)
	print "Mapped categories: "+repr(categories)
	return categories

def desc(FileObj):
	sectionLines = which(FileObj,'^\^|^!')
	for x in sectionLines:
		print FileObj[x].strip()

def extractData(FileObj):
	#X = open(File).readlines()
	annotlines = which(FileObj,'^!')
	D = FileObj[annotlines[-2]+1:annotlines[-1]-1]
	return D

def extractDataToFile(File, dest='.'):
	if dest[-1] == '/':
		dest = dest[0:-1]
	O = open(dest+'/'+File+'.data','w')
	X = open(File).readlines()
	annotlines = which(X,'^!')
	D = X[annotlines[-2]+1:annotlines[-1]-1]
	writelist(O,D)
	O.close()

def featureRank(DataObj, targetCategory, logResult = False):
	out = []
	from numpy import mean, std, array, sqrt, log, concatenate
	#import scipy.stats
	from scipy import stats
	casecols = which(DataObj[1],targetCategory)
	controlcols = tuple(set(range(1,len(DataObj[1]))).difference(set(casecols)))
	for x in DataObj[2:]:
		cases = array(listToFloat(getByIndex(x,casecols)))
		controls = array(listToFloat(getByIndex(x,controlcols)))
		T = stats.ttest_ind(cases,controls)
		if logResult:
			out.append(-log(T[1]))
		else:
			out.append(T[1])		
	return out

def extractFeatureIndex(metric, cutoff):
	c = 0
	out = []
	for x in metric:
		if x > cutoff:
			out.append(c)
		c+=1
	return tuple(out)

def extractOB(obdata, lines):
	out = []
	for x in lines:
		out.append(obdata[x+2][1:])
	return numpy.array(out)

def ob_transform(FileObj, identifier='IDENTIFIER',targetClass = '',targetCategory='',enum=False, removeControls=True):
	from numpy import array, transpose
	try:
		import progressbar as P
		pbar_exists = True
	except ImportError: 
		pbar_exists = False
	MDATA = []
	#Print dataset Name
	print 'Reading dataset ' + getLineByRE(FileObj,'!dataset_title')[0].split(' = ')[1] + ' (' + getLineByRE(FileObj,'\^DATASET')[0].split(' = ')[1] + ')'
	#Extract expression table
	EDATA = extractData(FileObj)
	samp = MapSample(FileObj, targetClass = targetClass)
	cols = tokenize(EDATA[0])
	incl_cols = []
	classifier = ['']
	if identifier in cols:
		incl_cols.append(which(cols,identifier)[0])
	else:
		print 'IDENTIFIER KEY \''+identifier+'\' NOT FOUND'
		return
	for x in samp.keys():
		if x in cols:
			incl_cols.append(which(cols,x)[0])
			classifier.append(samp[x])
	header = ['']
	header.extend(getByIndex(tokenize(EDATA[0]),incl_cols[1:]))
	MDATA.append(header)
	if enum:
		M = MapGroups(FileObj,targetClass=targetClass,targetCategory=targetCategory)
		classifier = translateList(classifier,M)
	MDATA.append(classifier)
	if pbar_exists:
		widgets = ["Transforming GEO to openbiomind: ", P.Percentage(), P.Bar()]
		pbar = P.ProgressBar(maxval=len(EDATA[1:]), widgets=widgets).start()
	R = 0
	for x in EDATA[1:]:
		if removeControls:
			if not (re.search('control',tokenize(x)[1],re.IGNORECASE)):
				MDATA.append(getByIndex(tokenize(x),incl_cols))
			else:
				R+=1
		else:
			MDATA.append(getByIndex(tokenize(x),incl_cols))		
		if pbar_exists:
			pbar.update(pbar.currval + 1)
			pass
	if pbar_exists: pbar.finish()
	if removeControls and R > 0: print '[removeControls] flag on, removed '+str(R)+' control(s)'
	return array(MDATA)

def moses_transform(FileObj, targetCategory, identifier='IDENTIFIER', cutoff = 8,select=True):
	from numpy import array, transpose
	try:
		import progressbar as P
		pbar_exists = True
	except ImportError: 
		pbar_exists = False
	MDATA = []
	#Extract expression table
	EDATA = extractData(FileObj)
	samp = MapSample(FileObj)
	target = MapGroups(FileObj,targetCategory)
	if target == False:
		return
	cols = tokenize(EDATA[0])
	incl_cols = []
	header = ['']
	header.extend(getByIndex(tokenize(EDATA[0]),incl_cols[1:]))
	classifier = ['out']
	if identifier in cols:
		incl_cols.append(which(cols,identifier)[0])
	else:
		print 'IDENTIFIER KEY \''+identifier+'\' NOT FOUND'
		return
	for x in samp.keys():
		if x in cols:
			incl_cols.append(which(cols,x)[0])
			classifier.append(str(target[samp[x]]))
	MDATA.append(header)
	MDATA.append(classifier)
	if pbar_exists:
		widgets = ["Transforming GEO to moses: ", P.Percentage(), P.Bar()]
		pbar = P.ProgressBar(maxval=len(EDATA[1:]), widgets=widgets).start()
	for x in EDATA[1:]:
		MDATA.append(getByIndex(tokenize(x),incl_cols))
		if pbar_exists:
			pbar.update(pbar.currval + 1)
			pass
	if pbar_exists: pbar.finish()
	if select:
		z = featureRank(MDATA, '1')
		I = extractFeatureIndex(z, cutoff)
		MDATA2 = [MDATA[1]]
		for x in I: 
			MDATA2.append(MDATA[x+2])
		return transpose(array(MDATA2))
	return transpose(array(MDATA))

def readMap(File):
	return open(File).readlines()[1].strip().split(',')

def binarize(data,metric='median'):
	from numpy import array,median
	data = array(data).astype(float)
	if metric == 'median':
		return (data < median(data)).astype(int)

def binarizeDataset(dataset, metric='median'):
	modes = ['median']
	if metric not in modes:
		print 'Binarization metric has to be one of the following:'
		print '|'.join(modes)
	else:
		print 'Binarize dataset done using metric: ['+metric+'])'
		return [binarize(x) for x in dataset]

def concatDatasets(ListOfDatasets,index,binarize=False):
	from numpy import array, nan, nanmax
	out = []
	Ind = index [:]
	#Convert everything to float and normalize!
	D = []
	for x in ListOfDatasets:
		Dataset = array(x)[2:,1:]
		removeNull(Dataset,nan)
		Dataset = Dataset.astype(float)
		##################################################NORMALIZATION STEP (CRUCIAL)
		if binarize:
			Dataset = binarizeDataset(Dataset)
		else:
			Dataset /= nanmax(Dataset)
		##################################################NORMALIZATION STEP (CRUCIAL)
		D.append(Dataset)
	header = [""]
	classifier = [""]
	for x in ListOfDatasets:
		header+=x[0][1:]
		classifier+=x[1][1:]
	out.append(header)
	out.append(classifier)
	for x in Ind:
		L = [ListOfDatasets[0][x][0]]
		for y in D:
			L+=y[x-2]
		out.append(L)
	return out

def ob2moses(dataset):
	from numpy import array, transpose
	out = []
	T = transpose(dataset)
	features = T[0][1:]
	features[0] = 'out'
	out.append(features)
	for x in T[1:]:
		out.append(array(x[1:]).astype(float))
	return array(out)

def binarizeMoses(mosesObj):
	labels = numpy.array([eval(x) for x in mosesObj[1:,0]]).astype(int)
	dat = mosesObj[1:,1:].astype(float)
	datB = binarizeDataset(dat)
	out = list([M_ob[0]])
	for i in range(len(labels)):
		out.append([labels[i]]+list(datB[i]))
	return numpy.array(out)


if __name__ == '__main__':
	mtest = moses_transform(X, 'psoriasis', cutoff=30, identifier='IDENTIFIER')
	writecsv('test2.moses',mtest,fieldsep=',')
	################################################################################LOAD FILE
	File = 'GDS3539_full.soft'
	X = open(File).readlines()
	obtest = ob_transform(X, identifier='IDENTIFIER')
	z = featureRank(obtest, 'psoriasis')
	I = extractFeatureIndex(z, 8)
	ob2 = obtest[0:2]
	for x in I:
		ob2.append(obtest[x+2])
	writecsv('test.ob',ob2)

