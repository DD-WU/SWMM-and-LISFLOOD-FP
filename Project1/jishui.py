import rasterio as rio
import glob
import numpy as np
import matplotlib.pyplot as plt
import sys
sys.setrecursionlimit(100000)
time=[]
for i in np.arange(0,7201,300):
  time.append(i)
files = sorted(glob.glob(r"F:\SWMM_lisflood\Project1\Project1\results-0.005541\*.wd"))
arr = []
arr1 = []
for file in files:
  with rio.open(file) as src:
    arr.append(src.read(1))
    arr1.append(src.read(1))
sum_arr1=[0]
sum_arr2=[0]
sum_arr3=[0]#30 70 80 119
for i in np.arange(1,25,1):
  sum_ = 0;
  for k in np.arange(0,34,1):
    for j in range(20):
      sum_+=arr[i][j][k]
  sum_arr1.append(sum_*25)
for i in np.arange(1,25,1):
  sum_ = 0;
  for k in np.arange(34,70,1):
    for j in range(20):
      sum_+=arr[i][j][k]
  sum_arr2.append(sum_*25)
for i in np.arange(1,25,1):
  sum_ = 0;
  for k in np.arange(70,120,1):
    for j in range(20):
      sum_+=arr[i][j][k]
  sum_arr3.append(sum_*25)
total=[]
for i in range(25):
  sum_ = 0;
  for k in np.arange(0, 120, 1):
    for j in range(20):
      sum_ += arr[i][j][k]
  total.append(sum_*25)

plt.plot(time, sum_arr1,color='red',label="d1")
plt.plot(time, sum_arr2,color='blue',label="d2")
plt.plot(time, sum_arr3,color='green',label="d3")
plt.plot(time, total,color='darkblue',label="total")
plt.legend()
plt.xlabel("time(s)")
plt.ylabel("water Depth (m^3)")
plt.show()