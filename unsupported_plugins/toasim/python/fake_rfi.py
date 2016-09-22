#!/usr/bin/python
import toasim
import sys,random


nreal = 1
header = toasim.header()

header.parfile_name=sys.argv[1]
header.timfile_name=sys.argv[2]

par=open(sys.argv[1])
tim=open(sys.argv[2])

header.orig_parfile=par.read()
header.idealised_toas=tim.read()

reals=list()

i=0
while i < len(sys.argv):
    if sys.argv[i] == "--nreal":
        i+=1
        nreal=int(sys.argv[i])
        continue
    i+=1


r=0
while r < nreal:
    offsets=list()
    ntoa=0
    for line in header.idealised_toas.split("\n"):
        if line.startswith(" "):
            elems=line.strip().split()
            error=float(elems[3])*1e-6
            toa=elems[2]
            offsets.append(random.lognormvariate(-6,3)*error) # *random.uniform(-1,1))
            ntoa+=1
    r+=1
    print "\b\b\b\b\b\b\b\b",
    print "%d"%r,
    reals.append(toasim.correction(header,offsets,0,0,0,""))
header.ntoa=ntoa
header.nrealisations=nreal
print "\nWriting...."


header.orig_parfile=""
header.idealised_toas=""
file=open(header.timfile_name+".addRFI","w")
header.write(file)
for real in reals:
    real.write(file)
file.close()

    
