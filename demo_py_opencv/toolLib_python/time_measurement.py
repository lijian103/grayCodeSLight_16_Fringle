import numpy as np
import cv2
import timeit
import time
"""
import sys
sys.path.append("../../../toolLib_python/")
"""
class TimeMeasurementCv2:
    def __init__(self):
        self.tick_freq=cv2.getTickFrequency()
        self.start_time=cv2.getTickCount()

    def UpdateStartTime(self):
        self.start_time=cv2.getTickCount()

    def GetTimeComsuption(self):
        self.time_consumption=(cv2.getTickCount()-self.start_time)/self.tick_freq
        return self.time_consumption

    def PrintTimeConsumption(self):
        self.time_consumption = (cv2.getTickCount() - self.start_time) / self.tick_freq
        print(self.time_consumption)


class TimeMeasurementTimeModule:
    def __init__(self):
        self.start_time=time.time()

    def UpdateStartTime(self):
        self.start_time=time.time()

    def GetTimeComsuption(self):
        self.time_consumption=time.time()-self.start_time
        return self.time_consumption

    def PrintTimeConsumption(self):
        self.time_consumption = time.time()-self.start_time
        print(self.time_consumption)






if __name__=="main":
    tm1=time.time()
    img = cv2.imread("../Pictures/Hardware_trigger_frame/Htf_7_25/cos_wave_Period_16_initial_phase_4.71238898038469.bmp")
    img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    e1 = cv2.getTickCount()
    feq=cv2.getTickFrequency()
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            img[i][j] = j % 256
            if img[i][j] > 88:
                img[i][j]=j%256
            else:
                img[i][j] = j /256

    # your code execution
    e2 = cv2.getTickCount()
    tm2=time.time()
    delta_time = (e2 - e1)/ feq
    print(delta_time)

    #测试时间是ms级别，不够精确
    num=0
    # tm1=time.time()
    print(tm1)
    for i in range(10000):
        num=num+1
    # tm2=time.time()
    print(tm2)
    time_delta=tm2-tm1
    print(time_delta)

    img = cv2.imread('m1.png')
    # %timeit z = cv2.countNonZero(img)
    #
    # %timeit z = np.count_nonzero(img)
