
import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt
import toolLib_python.time_measurement as tm

plt.figure("1")
plt.plot([1, 2, 3, 4], [12, 3, 23, 231])


plt.figure("2")
plt.plot([1, 2, 3, 4], [12, 3, 23, 231])
plt.ion()
plt.ioff()


img = cv2.imread("../Pictures/Hardware_trigger_frame/Htf_7_25/cos_wave_Period_16_initial_phase_4.71238898038469.bmp")
img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
ti=tm.TimeMeasurementCv2()
for i in range(img.shape[0]):
    # print(i)
    for j in range(img.shape[1]):
        img[i][j] = j % 256
        if img[i][j] > 88:
            img[i][j]=j%256
        else:
            img[i][j] = j /256
ti.PrintTimeConsumption()

# %timeit z = cv2.countNonZero(img)

# %timeit z = np.count_nonzero(img)
