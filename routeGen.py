#!/usr/bin/python

#Generates a routetable 
from collections import defaultdict

print("Reading in Conduits File...")

routeFile = open("conduits.txt", "r")
conduits = routeFile.readlines()
routeFile.close()
conduitDict = defaultdict(dict)
for line in conduits:
    lineTokens = line.split("\t")
    print(lineTokens)
    conduitDict[lineTokens[0]][lineTokens[1]] = int(lineTokens[2][:-1])
    conduitDict[lineTokens[1]][lineTokens[0]] = int(lineTokens[2][:-1])
print(conduitDict)
    

