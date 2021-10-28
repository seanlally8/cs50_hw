import re
import sys
import csv
import string

count = 0
countf = 0
string = "ABACAABBBACBACBACAA"
key = "BAC"


lista = [x for x in range(len(string)) if string.startswith(key, x)]

print(lista)

for i in range(len(lista)):
    count = 0
    tmp = lista[i]
    for j in range(tmp, len(string), len(key)):
        if string.startswith(key, j):
            count += 1
        else:
            break
    if count > countf:
        countf = count

print(countf)
