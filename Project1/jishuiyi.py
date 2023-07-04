import rasterio as rio
import glob
import numpy as np
import matplotlib.pyplot as plt
time=[]
for i in np.arange(0,82801,3600):
  time.append(i)
files = sorted(glob.glob(r"F:\SWMM_lisflood\Project1-2-2\Project1\结果1\规划后优化方案3\res\*.wd"))#\结果2\无建筑规划
arr = []
arr1 = []
for file in files:
  with rio.open(file) as src:
    arr.append(src.read(1))
for i in np.arange(0,24,1):
  sum_ = 0;
  for k in np.arange(0,831,1):
    for j in range(0,1073,1):
      if arr[i][j][k]!=-9999:
        sum_+=arr[i][j][k]
  arr1.append(sum_*16/10000)
f=open("k1.txt","w")
f.writelines(str(arr1))
plt.plot(time, arr1,color='darkblue',label="total")
plt.legend()
plt.xlabel("time(s)")
plt.ylabel("water Depth (m)")
plt.show()