import os

d0="./full/"
dir='b2'
for filename in os.listdir(d0+dir):
    os.rename(d0+"/"+dir+"/"+filename,d0+"/"+dir+"/"+dir+"_"+filename)
