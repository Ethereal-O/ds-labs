import random
import numpy
nodenum=2000
edgenum=100000
wmax=50

random.seed(1)
edge=[]
isexisted={}
while(len(edge)<edgenum):
    v1=random.randint(1,nodenum-1)
    v2=random.randint(1,nodenum-1)
    w=random.randint(1,wmax+1)
    if(v1==v2 or str(v1)+"+"+str(v2) in isexisted):
        continue 
    tmp=[v1,v2,w]
    edge.append(tmp)
    isexisted[str(v1)+"+"+str(v2)]=True
    isexisted[str(v2)+"+"+str(v1)]=True
    

with open("./data/data+"+str(nodenum)+"+"+str(edgenum),"w") as file:
    file.write(str(nodenum)+" "+str(edgenum)+"\n")
    for each in edge:
        file.write(str(each[0])+" "+str(each[1])+" "+str(each[2])+"\n")
