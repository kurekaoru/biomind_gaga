P = '0<(+(*(-1 +(1 $rs913871#PPP2R2D) +(*(+(1 *(-1 $rs913871#PPP2R2D)) $rs6998593#RNF139) *(+($rs6998593#RNF139 $rs913871#PPP2R2D) $rs4876948#TRAF2))) $rs4876948#TRAF2 $rs913871#PPP2R2D))'

def formatcombo(P):
	M = P[2:]
	print M

formatcombo(P)

c=[]
d=[]
count = 0
pos = 0
for x in P[2:]:
	if x == '(':
		count+=1
		c.append(count)
		d.append(pos)
	elif x == ')':
		count -=1
		c.append(count)
		d.append(pos)
	pos +=1
