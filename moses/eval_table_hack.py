import os
import sys

model = open(sys.argv[1]).readlines()[0].strip()
M = model[:]

observed_tr = [int(x.strip().split(',')[0]) for x in open(sys.argv[2]).readlines()[1:]]
observed_ts = [int(x.strip().split(',')[0]) for x in open(sys.argv[3]).readlines()[1:]]

if model[0:2] == '0<':
	model = model[3:-1]

outmodel = open(sys.argv[1],'w')

outmodel.write(model)

outmodel.close()

os.system('eval-table -u out -C '+sys.argv[1]+' -i '+ sys.argv[2] +' > ' + sys.argv[2] + '_')
#print ('eval-table -u out -C '+sys.argv[1]+' -i '+ sys.argv[2] +' > ' + sys.argv[2] + '_')
os.system('eval-table -u out -C '+sys.argv[1]+' -i '+ sys.argv[3] +' > ' + sys.argv[3] + '_')
#print ('eval-table -u out -C '+sys.argv[1]+' -i '+ sys.argv[2] +' > ' + sys.argv[2] + '_')

output_tr = [float(x.strip()) for x in open(sys.argv[2] + '_').readlines()[1:]]
output_ts = [float(x.strip()) for x in open(sys.argv[3] + '_').readlines()[1:]]

call_tr = [1 if x > 0 else 0 for x in output_tr]
call_ts = [1 if x > 0 else 0 for x in output_ts]

confusion_tr = [0,0,0,0]
confusion_ts = [0,0,0,0]

for x in range(len(observed_tr)):
	if observed_tr[x] == 1 and call_tr[x] == 1:
		confusion_tr[0] += 1
	if observed_tr[x] == 1 and call_tr[x] == 0:
		confusion_tr[1] += 1
	if observed_tr[x] == 0 and call_tr[x] == 1:
		confusion_tr[2] += 1
	if observed_tr[x] == 0 and call_tr[x] == 0:
		confusion_tr[3] += 1

for x in range(len(observed_ts)):
	if observed_ts[x] == 1 and call_ts[x] == 1:
		confusion_ts[0] += 1
	if observed_ts[x] == 1 and call_ts[x] == 0:
		confusion_ts[1] += 1
	if observed_ts[x] == 0 and call_ts[x] == 1:
		confusion_ts[2] += 1
	if observed_ts[x] == 0 and call_ts[x] == 0:
		confusion_ts[3] += 1

outfile = open(sys.argv[4],'w')

outfile.write('Model:\n')

outfile.write(M+'\n\n')

outfile.write("Global Train Matrix:\n")

outfile.write(str(confusion_tr[0])+'\t'+str(confusion_tr[1])+'\n')
outfile.write(str(confusion_tr[2])+'\t'+str(confusion_tr[3])+'\n')
outfile.write("Accuracy: ")
outfile.write(repr(float(confusion_tr[0]+confusion_tr[3])/float(sum(confusion_tr))))

outfile.write("\n\nGlobal Test Matrix:\n")

outfile.write(str(confusion_ts[0])+'\t'+str(confusion_ts[1])+'\n')
outfile.write(str(confusion_ts[2])+'\t'+str(confusion_ts[3])+'\n')
outfile.write("Accuracy: ")
outfile.write(repr(float(confusion_ts[0]+confusion_ts[3])/float(sum(confusion_ts))))

outfile.close()

#print model
