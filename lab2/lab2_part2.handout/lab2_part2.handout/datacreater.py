from random import random

totnum=500
file=open("/home/ethereal/Programs/CorC++/dslabs/lab2/lab2_part2.handout/input/data"+str(totnum)+".txt",'w')
px=1000
matrix=[]
for i in range(totnum):
    matrix.append([])
    for j in range(totnum):
        matrix[i].append(int(px*random()))

for i in range(totnum):
    for j in range(i+1):
        if i==j:
            matrix[i][j]=0
        else:
            matrix[i][j]=matrix[j][i]

file.write(str(totnum)+"\n")
for i in range(totnum):
    for j in range(totnum):
        file.write(str(matrix[i][j])+" ")
    file.write("\n")

file.write(str(totnum-1)+"\n")
for i in range(int(totnum-1)):
    file.write(str(i)+" ")
file.write("$\n")

file.write(str(totnum-1)+"\n")
for i in range(int(totnum/3)):
    file.write(str(i)+" ")
file.write("$\n")

file.write(str(totnum-1)+"\n")
for i in range(int(totnum/5)):
    file.write(str(i)+" ")
file.write("$\n")