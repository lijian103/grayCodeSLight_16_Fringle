import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt
def hello():

    print("hello world!")
    figureName = "test"
    showCol = 500
    initial_phase = 0
    plt.plot([1, 2, 3, 4], [12, 3, 23, 231])
    WrapedPhase = cv2.imread("../Pictures/Grap_frame/0_20190629143411.jpg", cv2.IMREAD_GRAYSCALE)
    x_axis_cos = np.arange(initial_phase, WrapedPhase.shape[1], 1)
    plt.figure(figureName)

    y_axis = WrapedPhase[showCol:showCol + 1, :]
    plt.plot(x_axis_cos, y_axis.reshape(WrapedPhase.shape[1]))
    # plt.show()

def testTuple(WrapedPhase, figureName="test",showCol=500):
    getRow = int(WrapedPhase[-2])
    getCol = int(WrapedPhase[-1])
    # print( getRow,getCol)
    tempArry = np.empty([getRow, getCol], dtype = "float32")
    for i in range(getRow):
        for j in range(getCol):
            tempArry[i][j]=WrapedPhase[i*getCol+j]
            # pint(WrapedPhase[i*getRow+j])

    WrapedPhase=tempArry

    # img = np.asarray(WrapedPhase, dtype="uint8")
    # cv2.imshow("WrapedPhaseMap",img)

    initial_phase = 0
    x_axis_cos = np.arange(initial_phase, WrapedPhase.shape[1], 1)
    plt.figure(figureName)
    y_axis = WrapedPhase[showCol:showCol + 1, :]
    plt.plot(x_axis_cos, y_axis.reshape(WrapedPhase.shape[1]))
    plt.show()


# *******************************************************************************
def test(WrapedPhase, figureName="test", showCol=500):
    initial_phase = 0
    x_axis_cos = np.arange(initial_phase, WrapedPhase.shape[1], 1)
    plt.figure(figureName)
    y_axis = WrapedPhase[showCol:showCol + 1, :]
    plt.plot(x_axis_cos, y_axis.reshape(WrapedPhase.shape[1]))
    # plt.plot(x_axis_cos,np.unwrap(y_axis.reshape(912))) ## plt.plot(x_axis_cos,np.unwrap(2*wraped_phase)/2)

def testShowImageOneCol(path="../Pictures/Hardware_trigger_frame/Htf_7_24_2/grayCode_G2.bmp",figureName="test",showCol=500):
    a = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    test(a,figureName, showCol)
    plt.show()
# *******************************************************************************