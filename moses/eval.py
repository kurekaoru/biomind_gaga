import sys

#for arg in sys.argv:
#    print arg

def list_s2i(list):
	out=[]
	for i in list:
		out.append(eval(i))
	return tuple(out)

path = sys.argv[1]

fold = sys.argv[2]

sol = sys.argv[3]

#x = open(sys.argv[1]+"/gtuple-folds/model"+sol+".moses").readlines()[0]

#TR = open(sys.argv[1]+"/gtuple-folds/out"+sol+".mosesTR").readlines()
#TS = open(sys.argv[1]+"/gtuple-folds/out"+sol+".mosesTS").readlines()

x = open(sys.argv[1]+"/gtuple-folds/model"+fold+".moses").readlines()[0]
TR = open(sys.argv[1]+"/gtuple-folds/out"+fold+".mosesTR").readlines()
TS = open(sys.argv[1]+"/gtuple-folds/out"+fold+".mosesTS").readlines()

outfile = open(sys.argv[1]+"/task/current/fold_"+fold+"/sol_"+sol+"/outfinal.txt",'w')
#print 'CREATED '+sys.argv[1]+'/task/current/fold_'+fold+'/sol_'+sol+'/outfinal.txt'+'\n'

real_TR = list_s2i(TR[0].strip().split(','))
real_TS = list_s2i(TS[0].strip().split(','))

evaltable_TR = []
for i in TR[1:2]:
	confusion_TR = [0,0,0,0]
	model = list_s2i(i.strip().split(','))
	for j in range(len(real_TR)):
		if (real_TR[j] == 1 and model[j] == 1):
			confusion_TR [0] += 1
		elif (real_TR[j] == 1 and model[j] == 0):
			confusion_TR [1] += 1
		elif (real_TR[j] == 0 and model[j] == 1):
			confusion_TR [2] += 1
		elif (real_TR[j] == 0 and model[j] == 0):
			confusion_TR [3] += 1
	evaltable_TR.append(tuple(confusion_TR))


evaltable_TS = []
for i in TS[1:2]:
	confusion_TS = [0,0,0,0]
	model = list_s2i(i.strip().split(','))
	for j in range(len(real_TS)):
		if (real_TS[j] == 1 and model[j] == 1):
			confusion_TS [0] += 1
		elif (real_TS[j] == 1 and model[j] == 0):
			confusion_TS [1] += 1
		elif (real_TS[j] == 0 and model[j] == 1):
			confusion_TS [2] += 1
		elif (real_TS[j] == 0 and model[j] == 0):
			confusion_TS [3] += 1
	evaltable_TS.append(tuple(confusion_TS))

outfile.write("Model:\n")
outfile.write(x+'\n')
outfile.write("Global Train Matrix:\n")
outfile.write(repr(evaltable_TR[0][0])+"\t"+repr(evaltable_TR[0][1])+"\n")
outfile.write(repr(evaltable_TR[0][2])+"\t"+repr(evaltable_TR[0][3])+"\n")
outfile.write("Accuracy: ")
outfile.write(repr(float(evaltable_TR[0][0]+evaltable_TR[0][3])/float(sum(evaltable_TR[0]))))

outfile.write("\nGlobal Test Matrix:\n")
outfile.write(repr(evaltable_TS[0][0])+"\t"+repr(evaltable_TS[0][1])+"\n")
outfile.write(repr(evaltable_TS[0][2])+"\t"+repr(evaltable_TS[0][3])+"\n")
outfile.write("Accuracy: ")
outfile.write(repr(float(evaltable_TS[0][0]+evaltable_TS[0][3])/float(sum(evaltable_TS[0]))))

outfile.close()
