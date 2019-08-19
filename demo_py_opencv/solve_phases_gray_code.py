import sys
import os
import cv2
import numpy as np
import solve_phase
from matplotlib import pyplot as plt
import grayCode
import three_dimession_image as my3D

freq_1 = 1 / 70
freq_2 = 1 / 64
freq_3 = 1 / 59

object_L = 213.5
object_d = 125
# object_T=914/70
# 这里和文章里面有歧义，文章里面是像素点数，我的理解为实物表面波的长度。相位差处也有歧义。
object_T = 2.5
waveLength_1 = freq_1
waveLength_2 = freq_2
waveLength_3 = freq_3

cv2.getTickCount
def getWrapedPhase_modify(imagePath="./cos_pictures/", Period=70, imgType=".bmp"):
    # 对于三步移相位，使用下面的初相位，可以达到完整的周期。
    # initial_phase = [np.pi/2,np.pi*2/2,np.pi*3/2,0]

    # 对于四步移相位，使用下面的初相位，可以达到完整的周期。
    initial_phase = [np.pi * 2 / 2, np.pi * 3 / 2, 0, np.pi / 2]
    imagePathList = []
    grayImages = []
    for each in range(4):
        imagePathList.append(imagePath + "cos_wave" + "_Period_" + str(Period) + "_initial_phase_" + str(
            initial_phase[each]) + imgType)
        print(imagePathList[each])
        img = cv2.imread(imagePathList[each])
        # cv2.imshow(str(each),img)

        if img.ndim == 3:
            img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        img = np.asarray(img, dtype="float32")
        grayImages.append(img)
    # WrapedPhase = np.arctan2((3 ** 0.5) * (grayImages[0] - grayImages[2]),
    #                          (2 * grayImages[1] - (grayImages[0] + grayImages[2])))
    WrapedPhase=np.zeros((grayImages[0].shape[0], grayImages[0].shape[1]), dtype="float32")
    # for i in range(grayImages[0].shape[0]):
    #     print(i)
    #     for j in range(grayImages[0].shape[1]):
    #         if -5<grayImages[0][i][j]-grayImages[1][i][j]<5 and -5<grayImages[0][i][j]-grayImages[2][i][j]<5\
    #               and   -5<grayImages[0][i][j]-grayImages[3][i][j]<5 and -5<grayImages[1][i][j]-grayImages[2][i][j]<5 :
    #             WrapedPhase[i][j]=-np.pi
    #         else:
    #             WrapedPhase[i][j] = np.arctan2((grayImages[3][i][j] - grayImages[1][i][j]),
    #                                  (grayImages[0][i][j] - grayImages[2][i][j]))
    WrapedPhase = np.arctan2((grayImages[3] - grayImages[1]),
                                   (grayImages[0] - grayImages[2]))
    WrapedPhase = cv2.medianBlur(WrapedPhase, 5)
    return WrapedPhase

def getUnWrapedPhase_grayCodeMethond(grayCodeimagePath = "../Pictures/Hardware_trigger_frame/Htf_7_24/", imgType=".bmp"):
    WrapedPhase_16 = getWrapedPhase_modify(grayCodeimagePath, 16)

    # ********显示压缩相位************************************
    solve_phase.showWrapedPhaseMap(WrapedPhase_16, imshowName=grayCodeimagePath)
    solve_phase.test(WrapedPhase_16, "WrapedPhase_16"+grayCodeimagePath)

    imagePathList = []
    grayImages = []
    for each in range(4):
        imagePathList.append(grayCodeimagePath + "grayCode_G" + str(each) + imgType)
        print(imagePathList[each])
        img = cv2.imread(imagePathList[each])
        # cv2.imshow(str(each),img)

        if img.ndim == 3:
            img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        grayImages.append(img)

    mapToGrayCode = grayCode.binMapToGray(4)

    expressAll = []
    for each in range(16):
        express = []
        temp = mapToGrayCode[each]
        for imgIndex in range(len(temp)):
            if temp[imgIndex] == "0":
                compare = "<"
            elif temp[imgIndex] == "1":
                compare = ">"
            express.append("grayImages[" + str(imgIndex) + "][i][j]" + compare + "100")
        expressAll.append(express)

    mapWrapedPhase = np.zeros((WrapedPhase_16.shape[0], WrapedPhase_16.shape[1]), dtype="uint8")

    for i in range(WrapedPhase_16.shape[0]):
        print(i)
        for j in range(WrapedPhase_16.shape[1]):

            jump=0
            # if j > 0:
            #     if WrapedPhase_16[i][j] >= WrapedPhase_16[i][j - 1]:
            #         jump = mapWrapedPhase[i][j - 1]

            for index in range(jump, 16):
                if eval(expressAll[index][0]) and eval(expressAll[index][1]) and eval(
                        expressAll[index][2]) and eval(expressAll[index][3]):
                    mapWrapedPhase[i][j] = index
                    break

    unWrapedPhase_16_1 = np.zeros((WrapedPhase_16.shape[0], WrapedPhase_16.shape[1]), dtype="uint8")
    # for i in range(WrapedPhase_16.shape[0]):
    #     for j in range(WrapedPhase_16.shape[1]):
    #         if WrapedPhase_16[i][j]<1 and WrapedPhase_16[i][j]>-1:
    #             unWrapedPhase_16_1[i][j] = 2 * np.pi * mapWrapedPhase[i][j] + WrapedPhase_16[i][j] + np.pi
    unWrapedPhase_16_1 = 2 * np.pi * mapWrapedPhase + WrapedPhase_16+np.pi
    unWrapedPhase_16_1 = np.asarray(unWrapedPhase_16_1, dtype="float32")
    unWrapedPhase_16_1 = cv2.medianBlur(unWrapedPhase_16_1, 5)
    # ********显示解压缩相位************************************
    solve_phase.showUnWrapedPhaseMap(unWrapedPhase_16_1, 16, "unWrapedPhase_16"+grayCodeimagePath)
    solve_phase.test(unWrapedPhase_16_1, "unWrapedPhase_16"+grayCodeimagePath)
    return unWrapedPhase_16_1

def getUnWrapedPhaseDelta_grayCodeMethond(imagePath_NoObject="../Pictures/Hardware_trigger_frame/Htf_7_24/" ,imgPath_Object="../Pictures/Hardware_trigger_frame/Htf_7_24_1/", imgType=".bmp",flag=0):
    """
    flag=0不用重复计算UnWrapedPhase_NoObject
    :param imagePath_NoObject:
    :param imgPath_Object:
    :param imgType:
    :param flag:
    :return:
    """
    path=imagePath_NoObject+"UnWrapedPhase_NoObject.npy"
    if  os.path.exists(path):
        print("UnWrapedPhase_NoObject.npy exists")
        if flag == 0:
            UnWrapedPhase_NoObject=b = np.load(path)
            solve_phase.showUnWrapedPhaseMap(UnWrapedPhase_NoObject, 16, "unWrapedPhase_16" + imagePath_NoObject)
            solve_phase.test(UnWrapedPhase_NoObject, "unWrapedPhase_16" + imagePath_NoObject)
        else:
            UnWrapedPhase_NoObject = getUnWrapedPhase_grayCodeMethond(imagePath_NoObject, imgType)
            #可以不要 os.remove(path)，每次写入是直接覆盖的
            os.remove(path)
            np.save(path, UnWrapedPhase_NoObject)
    else:
        print("UnWrapedPhase_NoObject.npy not exists")
        UnWrapedPhase_NoObject=getUnWrapedPhase_grayCodeMethond(imagePath_NoObject, imgType)
        np.save(path,UnWrapedPhase_NoObject)

    UnWrapedPhase_Object = getUnWrapedPhase_grayCodeMethond(imgPath_Object, imgType)

    UnWrapedPhaseDelta= np.zeros((UnWrapedPhase_Object.shape[0], UnWrapedPhase_Object.shape[1]), dtype="float32")
    for i in range(UnWrapedPhase_Object.shape[0]):
        print("UnWrapedPhaseDelta_clos:",i)
        for j in range(UnWrapedPhase_Object.shape[1]):
            #不同的方式处理椒盐区，保证求差后，差的阴影部分为黑色
            if UnWrapedPhase_Object[i][j]<6.28:
                UnWrapedPhaseDelta[i][j] =0
            else:
                UnWrapedPhaseDelta[i][j]=UnWrapedPhase_NoObject[i][j]-UnWrapedPhase_Object[i][j]

    # ********显示相位差************************************
    solve_phase.showUnWrapedPhaseMap(UnWrapedPhaseDelta, 10, "UnWrapedPhaseDelta")
    solve_phase.test(UnWrapedPhaseDelta, "UnWrapedPhaseDelta" )
    return  UnWrapedPhaseDelta,UnWrapedPhase_NoObject,UnWrapedPhase_Object

if __name__ == "__main__":
    # imagePath="../Pictures/Hardware_trigger_frame/Htf_7_24_1/"
    # WrapedPhase_16 = getWrapedPhase_modify(imagePath, 16)
    #
    # # ********************************************
    # solve_phase.showWrapedPhaseMap(WrapedPhase_16, imshowName=imagePath + str(16))
    # solve_phase.test(WrapedPhase_16, "WrapedPhase_16")
    #
    #
    # grayCodeimagePath ="../Pictures/Hardware_trigger_frame/Htf_7_24_1/"
    # imgType = ".bmp"
    # imagePathList = []
    # grayImages = []
    # for each in range(4):
    #     imagePathList.append(grayCodeimagePath + "grayCode_G" + str(each) + imgType)
    #     print(imagePathList[each])
    #     img = cv2.imread(imagePathList[each])
    #     # cv2.imshow(str(each),img)
    #
    #     if img.ndim == 3:
    #         img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    #
    #     grayImages.append(img)
    #
    # mapToGrayCode = grayCode.binMapToGray(4)
    #
    # expressAll = []
    # for each in range(16):
    #     express = []
    #     temp = mapToGrayCode[each]
    #     for imgIndex in range(len(temp)):
    #         if temp[imgIndex] == "0":
    #             compare = "<"
    #         elif temp[imgIndex] == "1":
    #             compare = ">"
    #         express.append("grayImages[" + str(imgIndex) + "][i][j]" + compare + "100")
    #     expressAll.append(express)
    #
    # mapWrapedPhase = np.zeros((WrapedPhase_64.shape[0], WrapedPhase_64.shape[1]), dtype="uint8")
    #
    # for i in range(WrapedPhase_16.shape[0]):
    #     print(i)
    #     for j in range(WrapedPhase_16.shape[1]):
    #         jump = 0
    #         if j > 0:
    #             if WrapedPhase_16[i][j] >= WrapedPhase_16[i][j - 1]:
    #                 jump = mapWrapedPhase[i][j - 1]
    #             else:
    #                 jump = 0
    #         else:
    #             jump = 0
    #         for index in range(jump, 16):
    #             if eval(expressAll[index][0]) and eval(expressAll[index][1]) and eval(
    #                     expressAll[index][2]) and eval(expressAll[index][3]) :
    #                 mapWrapedPhase[i][j] = index
    #                 break
    #
    # unWrapedPhase_16_1 = 2 * np.pi * mapWrapedPhase + WrapedPhase_16+np.pi
    # unWrapedPhase_16_1 = np.asarray(unWrapedPhase_16_1, dtype="float32")
    # # unWrapedPhase_16_1 = cv2.medianBlur(unWrapedPhase_16_1, 5)
    # # ********显示解压缩相位************************************
    # solve_phase.showUnWrapedPhaseMap(unWrapedPhase_16_1, 16, "unWrapedPhase_16" + grayCodeimagePath)
    # solve_phase.test(unWrapedPhase_16_1, "unWrapedPhase_16" + grayCodeimagePath)

    #  #*******************************************************
    UnWrapedPhaseDelta,UnWrapedPhase_NoObject,UnWrapedPhase_Object=getUnWrapedPhaseDelta_grayCodeMethond(imagePath_NoObject="../Pictures/Hardware_trigger_frame/Htf_7_25/",
                                          imgPath_Object="../Pictures/Hardware_trigger_frame/Htf_7_25_2/",
                                          imgType=".bmp",flag=0)

    # **********************************************求高度************
    objectHeight = solve_phase.getObjectHeight(UnWrapedPhaseDelta, object_L, object_d, object_T=2.5, k=1)
    objectHeight = cv2.medianBlur(objectHeight, 5)
    for i in range(objectHeight.shape[0]):
        for j in range(objectHeight.shape[1]):
            if objectHeight[i][j] < 0:
                objectHeight[i][j] = 0

    for i in range(objectHeight.shape[0]):
        for j in range(objectHeight.shape[1]):
            if objectHeight[i][j] > 7:
                objectHeight[i][j] = 0
    objectHeight = cv2.medianBlur(objectHeight, 5)
    solve_phase.test(objectHeight, "objectHeight")
    solve_phase.showUnWrapedPhaseMap(objectHeight, 8, "objectHeight")


    # ************************************************************
    Z = objectHeight[0:830, 160:]
    X = np.arange(0, Z.shape[1], 1)
    Y = np.arange(0, Z.shape[0], 1)
    X, Y = np.meshgrid(X, Y)
    my3D.plot_3D_surface(Y, X, Z, elev=45, azim=0)
    # my3D.plot_3D_surface(Y,X,Z,elev=45,azim=-10)
    plt.show()


