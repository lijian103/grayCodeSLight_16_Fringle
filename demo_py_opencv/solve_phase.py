import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt
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


def getObjectHeight(deta, object_L, object_d, object_T, k):
    objectHeight = object_L * object_T * k * deta / (2 * np.pi * object_d + object_T * deta)
    return objectHeight


def getObjectHeight_1(deta, object_L, object_d, k):
    objectHeight = object_L * k * deta / (object_d + k * deta)
    return objectHeight


def getWrapedPhase(imagePath="./cos_pictures/", Period=70, imgType=".bmp"):
    initial_phase = [-2 * np.pi / 3, 0, 2 * np.pi / 3]
    imagePathList = []
    grayImages = []
    for each in range(3):
        imagePathList.append(imagePath + "cos_wave" + "_Period_" + str(Period) + "_initial_phase_" + str(
            initial_phase[each]) + imgType)
        print(imagePathList[each])
        img = cv2.imread(imagePathList[each])
        img = np.asarray(img, dtype="float32")
        # cv2.imshow(str(each),img)

        if img.ndim == 3:
            img=cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        grayImages.append(img)
    # WrapedPhase = np.zeros((img.shape[0], img.shape[1]), dtype="float32")
    # for i in range(img.shape[0]):
    #     for j in range(img.shape[1]):
    #         WrapedPhase[i][j] =np.arctan2(3 ** 0.5 * (grayImages[0][i][j] - grayImages[2][i][j]), (
    #                     2 * grayImages[1][i][j] - (grayImages[0][i][j] + grayImages[2][i][j])))
    WrapedPhase = np.arctan2((3 ** 0.5) * (grayImages[0] - grayImages[2]),
                             (2 * grayImages[1] - (grayImages[0] + grayImages[2])))
    # WrapedPhase = cv2.medianBlur(WrapedPhase, 5)
    return WrapedPhase


def showWrapedPhaseMap(WrapedPhase, imshowName="0"):
    """
    # show WrapedPhaseMap
    :param WrapedPhase:
    :return:
    """
    WrapedPhaseMap = (WrapedPhase + np.pi) * 255 / (2 * np.pi)
    img = np.asarray(WrapedPhaseMap, dtype="uint8")
    cv2.imshow("WrapedPhaseMap_" + imshowName, img)


def test(WrapedPhase, figureName="test",showCol=500):
    initial_phase = 0
    x_axis_cos = np.arange(initial_phase, WrapedPhase.shape[1], 1)
    plt.figure(figureName)
    y_axis = WrapedPhase[showCol:showCol+1, :]
    plt.plot(x_axis_cos, y_axis.reshape(WrapedPhase.shape[1]))
    # plt.plot(x_axis_cos,np.unwrap(y_axis.reshape(912))) ## plt.plot(x_axis_cos,np.unwrap(2*wraped_phase)/2)

def testShowImageOneCol(path="../Pictures/Hardware_trigger_frame/Htf_7_24_2/grayCode_G2.bmp",showCol=500):
    a = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    test(a, "unWrapedPhase_a", showCol)
    plt.show()

def unWrapPhase_lib(WrapedPhase):
    """
    此种方法求解方正的图片没什么问题，但对于倾斜的图像存在很大的问题，而大部分的图片都是倾斜的，
    因此此方法在实际应用中并不合适。
    :param WrapedPhase:
    :param Period:
    :return:
    """
    unWrapedPhase = np.zeros((WrapedPhase.shape[0], WrapedPhase.shape[1]), dtype="float32")
    for each in range(WrapedPhase.shape[0]):
        unWrapedPhase[each, :] = np.unwrap(WrapedPhase[each, :])
    return unWrapedPhase


def showUnWrapedPhaseMap(unWrapedPhase, Period, imshowName="0"):
    """
    # show UnWrapedPhaseMap
    :param UnWrapedPhase:
    :return:
    """
    unWrapedPhaseMap = (unWrapedPhase) * 255 / (Period * 2 * np.pi)
    img = np.asarray(unWrapedPhaseMap, dtype="uint8")
    cv2.imshow("unWrapedPhaseMap_" + "Period:" + str(Period) + "_" + imshowName, img)


def unWrapPhase_doubleFre(WrapedPhase_1, WrapedPhase_2, waveLength_1, waveLength_2, flag=0):
    """
    存在一定的问题，最后一列展开相位后为0，按道理应该为最大值，可用 # test(unWrapedPhase_1)测试
    :param WrapedPhase_1:
    :param WrapedPhase_2:
    :param waveLength_1:
    :param waveLength_2:
    :param flag:
    :return:
    """
    if (waveLength_1 > waveLength_2):
        print("need waveLength_2>waveLength_1")
        sys.exit(-1)
    unWrapedPhase = np.zeros((WrapedPhase_1.shape[0], WrapedPhase_1.shape[1]), dtype="float32")
    if flag == 0:
        for i in range(WrapedPhase_1.shape[0]):
            for j in range(WrapedPhase_1.shape[1]):
                if WrapedPhase_1[i][j] >= WrapedPhase_2[i][j]:
                    unWrapedPhase[i][j] = (WrapedPhase_1[i][j] - WrapedPhase_2[i][j]) * waveLength_2 / (
                            waveLength_2 - waveLength_1)
                else:
                    unWrapedPhase[i][j] = (2 * np.pi + WrapedPhase_1[i][j] - WrapedPhase_2[i][j]) * waveLength_2 / (
                            waveLength_2 - waveLength_1)
    elif flag == 1:
        for i in range(WrapedPhase_1.shape[0]):
            for j in range(WrapedPhase_1.shape[1]):
                if WrapedPhase_1[i][j] >= WrapedPhase_2[i][j]:
                    unWrapedPhase[i][j] = (WrapedPhase_1[i][j] - WrapedPhase_2[i][j]) * waveLength_1 / (
                            waveLength_2 - waveLength_1)
                else:
                    unWrapedPhase[i][j] = (2 * np.pi + WrapedPhase_1[i][j] - WrapedPhase_2[i][j]) * waveLength_1 / (
                            waveLength_2 - waveLength_1)
    return unWrapedPhase


def unWrapPhase_multipleFre(WrapedPhase_1, WrapedPhase_2, WrapedPhase_3, waveLength_1, waveLength_2, waveLength_3,
                            flag=0):
    waveLength_12 = (waveLength_1 * waveLength_2) / (waveLength_2 - waveLength_1)
    waveLength_23 = (waveLength_2 * waveLength_3) / (waveLength_3 - waveLength_2)
    if flag == 0:
        unWrapedPhase_1_2 = unWrapPhase_doubleFre(WrapedPhase_1, WrapedPhase_2, waveLength_1, waveLength_2, 0)
        unWrapedPhase_1_2_normalization = unWrapedPhase_1_2 * (waveLength_2 - waveLength_1) / waveLength_2

        unWrapedPhase_2_3 = unWrapPhase_doubleFre(WrapedPhase_2, WrapedPhase_3, waveLength_2, waveLength_3, 0)
        unWrapedPhase_2_3_normalization = unWrapedPhase_2_3 * (waveLength_3 - waveLength_2) / waveLength_3

        unWrapedPhase_1_2__2_3 = unWrapPhase_doubleFre(unWrapedPhase_1_2_normalization, unWrapedPhase_2_3_normalization,
                                                       waveLength_12, waveLength_23, 0)
        # # showUnWrapedPhaseMap 的6和70是指它解包的周期
        # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3, 6, "1_2__2_3_normalization")
        unWrapedPhase_1_2__2_3_Un_normalization = unWrapedPhase_1_2__2_3 / (
                (waveLength_2 - waveLength_1) / waveLength_2)
        # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3_Un_normalization, 70, "1_2__2_3_Un_normalization")

    if flag == 1:
        unWrapedPhase_1_2 = unWrapPhase_doubleFre(WrapedPhase_1, WrapedPhase_2, waveLength_1, waveLength_2, 1)
        unWrapedPhase_1_2_normalization = unWrapedPhase_1_2 * (waveLength_2 - waveLength_1) / waveLength_1

        unWrapedPhase_2_3 = unWrapPhase_doubleFre(WrapedPhase_2, WrapedPhase_3, waveLength_2, waveLength_3, 0)
        unWrapedPhase_2_3_normalization = unWrapedPhase_2_3 * (waveLength_3 - waveLength_2) / waveLength_3

        unWrapedPhase_1_2__2_3 = unWrapPhase_doubleFre(unWrapedPhase_1_2_normalization, unWrapedPhase_2_3_normalization,
                                                       waveLength_12, waveLength_23, 0)
        # # showUnWrapedPhaseMap 的6和70是指它解包的周期
        # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3, 6, "1_2__2_3_normalization")
        unWrapedPhase_1_2__2_3_Un_normalization = unWrapedPhase_1_2__2_3 / (
                (waveLength_2 - waveLength_1) / waveLength_1)
        # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3_Un_normalization, 64, "1_2__2_3_Un_normalization")
    if flag == 2:
        unWrapedPhase_1_2 = unWrapPhase_doubleFre(WrapedPhase_1, WrapedPhase_2, waveLength_1, waveLength_2, 1)
        unWrapedPhase_1_2_normalization = unWrapedPhase_1_2 * (waveLength_2 - waveLength_1) / waveLength_1

        unWrapedPhase_2_3 = unWrapPhase_doubleFre(WrapedPhase_2, WrapedPhase_3, waveLength_2, waveLength_3, 1)
        unWrapedPhase_2_3_normalization = unWrapedPhase_2_3 * (waveLength_3 - waveLength_2) / waveLength_2

        unWrapedPhase_1_2__2_3 = unWrapPhase_doubleFre(unWrapedPhase_1_2_normalization, unWrapedPhase_2_3_normalization,
                                                       waveLength_12, waveLength_23, 1)
        # # showUnWrapedPhaseMap 的5和70是指它解包的周期
        # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3, 5, "1_2__2_3_normalization")
        unWrapedPhase_1_2__2_3_Un_normalization = unWrapedPhase_1_2__2_3 / (
                (waveLength_3 - waveLength_2) / waveLength_2)
        # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3_Un_normalization, 59, "1_2__2_3_Un_normalization"+)
    return unWrapedPhase_1_2__2_3_Un_normalization, unWrapedPhase_1_2__2_3


def getWrapedPhaseMultipleFringe(imagePath="./Htf_7_9_2/", Period_1=70, Period_2=64, Period_3=59):
    WrapedPhase_70 = getWrapedPhase(imagePath, Period_1)
    showWrapedPhaseMap(WrapedPhase_70, imshowName=imagePath + str(Period_1))
    WrapedPhase_64 = getWrapedPhase(imagePath, Period_2)
    showWrapedPhaseMap(WrapedPhase_64, imshowName=imagePath + str(Period_2))
    WrapedPhase_59 = getWrapedPhase(imagePath, Period_3)
    showWrapedPhaseMap(WrapedPhase_59, imshowName=imagePath + str(Period_3))
    return WrapedPhase_70, WrapedPhase_64, WrapedPhase_59




if __name__ == "__main__":
    # 验证lib求解相位，产生的图片，可以很好地解相位
    # WrapedPhase_5 = getWrapedPhase(imagePath="./cos_pictures/", Period=5)
    # showWrapedPhaseMap(WrapedPhase_5, Period=5)
    # unWrapedPhase=unWrapPhase_lib(WrapedPhase_5, Period=5)
    # showUnWrapedPhaseMap(unWrapedPhase,5)
    #
    # 验证lib求解相位，相机拍摄的图片，不能很好地解相位
    # WrapedPhase_70 = getWrapedPhase(imagePath="./Htf_7_9/", Period=70)
    # showWrapedPhaseMap(WrapedPhase_70, Period=70)
    # unWrapedPhase_1_2__2_3=unWrapPhase_lib(WrapedPhase_70, Period=70)
    # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3, 70, "1_2__2_3")
    # test(unWrapedPhase_1_2__2_3)

    # 此部分封装，使用下面函数代替
    # WrapedPhase_70 = getWrapedPhase(imagePath="./Htf_7_9_2/", Period=70)
    # showWrapedPhaseMap(WrapedPhase_70, imshowName="70")
    # WrapedPhase_64 = getWrapedPhase(imagePath="./Htf_7_9_2/", Period=64)
    # showWrapedPhaseMap(WrapedPhase_64, imshowName="64")
    # WrapedPhase_59 = getWrapedPhase(imagePath="./Htf_7_9_2/", Period=59)
    # showWrapedPhaseMap(WrapedPhase_59, imshowName="59")
    # WrapedPhase_70, WrapedPhase_64, WrapedPhase_59 = getWrapedPhaseMultipleFringe(imagePath="./cos_pictures/",
    #                                                                               Period_1=70,
    #                                                                               Period_2=64, Period_3=59)

    # 此部分已经封装为函数unWrapPhase_multipleFre
    # unWrapedPhase_1_2 = unWrapPhase_doubleFre(WrapedPhase_70, WrapedPhase_64, waveLength_1, waveLength_2, 0)
    # unWrapedPhase_1_2_normalization = unWrapedPhase_1_2 * (waveLength_2 - waveLength_1) / waveLength_2
    #
    # unWrapedPhase_2_3 = unWrapPhase_doubleFre(WrapedPhase_64, WrapedPhase_59, waveLength_2, waveLength_3, 0)
    # unWrapedPhase_2_3_normalization = unWrapedPhase_2_3 * (waveLength_3 - waveLength_2) / waveLength_3
    #
    # waveLength_12 = (waveLength_1 * waveLength_2) / (waveLength_2 - waveLength_1)
    # waveLength_23 = (waveLength_2 * waveLength_3) / (waveLength_3 - waveLength_2)
    # unWrapedPhase_1_2__2_3 = unWrapPhase_doubleFre(unWrapedPhase_1_2_normalization, unWrapedPhase_2_3_normalization,
    #                                                  waveLength_12, waveLength_23, 0)
    # # showUnWrapedPhaseMap 的6和70是指它解包的周期
    # showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3, 6, "1_2__2_3")
    # unWrapedPhase_1_2__2_3_Un_normalization = unWrapedPhase_1_2__2_3 / ((waveLength_2 - waveLength_1) / waveLength_2)

    # ******************************************测试************************
    # WrapedPhase_64 = getWrapedPhase("./cos_pictures/", 64)
    # showWrapedPhaseMap(WrapedPhase_64, imshowName="./cos_pictures" + str(64))
    # test(WrapedPhase_64, "detaUnWrapedPhase")


    # **********************************没有物体放入的情况*******************************************
    WrapedPhase_70_No_Object, WrapedPhase_64_No_Object, WrapedPhase_59_No_Object = \
        getWrapedPhaseMultipleFringe(imagePath="../../../Pictures/Hardware_trigger_frame/Htf_7_16/", Period_1=70,
                                     Period_2=64, Period_3=59)
    # imagePath="../../../Pictures/Hardware_trigger_frame/Htf_7_9/"
    unWrapedPhase_1_2__2_3_Un_normalization_No_Object, unWrapedPhase_1_2__2_3_No_Object = \
        unWrapPhase_multipleFre(WrapedPhase_70_No_Object, WrapedPhase_64_No_Object, WrapedPhase_59_No_Object,
                                waveLength_1, waveLength_2, waveLength_3, flag=0)
    #  showUnWrapedPhaseMap 的6和70是指它解包的周期
    showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3_No_Object, 6, "1_2__2_3_normalization_No_Object")
    showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3_Un_normalization_No_Object, 70, "1_2__2_3_Un_normalization_No_Object")
    medianBlur_unWrapedPhase_1_2__2_3_Un_normalization_No_Object = cv2.medianBlur(
        unWrapedPhase_1_2__2_3_Un_normalization_No_Object, 5)
    showUnWrapedPhaseMap(medianBlur_unWrapedPhase_1_2__2_3_Un_normalization_No_Object, 70,
                         "medianBlur_1_2__2_3_Un_normalization_No_Object")
    test(medianBlur_unWrapedPhase_1_2__2_3_Un_normalization_No_Object, "1")

    # **********************************有物体放入的情况*******************************************
    WrapedPhase_70, WrapedPhase_64, WrapedPhase_59 = \
        getWrapedPhaseMultipleFringe(imagePath="../../../Pictures/Hardware_trigger_frame/Htf_7_16_1/", Period_1=70,
                                     Period_2=64, Period_3=59)
    unWrapedPhase_1_2__2_3_Un_normalization, unWrapedPhase_1_2__2_3 = \
        unWrapPhase_multipleFre(WrapedPhase_70, WrapedPhase_64, WrapedPhase_59, waveLength_1, waveLength_2,
                                waveLength_3, flag=0)

    #  showUnWrapedPhaseMap 的6和70是指它解包的周期
    showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3, 6, "1_2__2_3_normalization")
    showUnWrapedPhaseMap(unWrapedPhase_1_2__2_3_Un_normalization, 70, "1_2__2_3_Un_normalization")
    medianBlur_unWrapedPhase_1_2__2_3_Un_normalization = cv2.medianBlur(
        unWrapedPhase_1_2__2_3_Un_normalization, 5)
    showUnWrapedPhaseMap(medianBlur_unWrapedPhase_1_2__2_3_Un_normalization, 70,
                         "medianBlur_1_2__2_3_Un_normalization")
    test(medianBlur_unWrapedPhase_1_2__2_3_Un_normalization, "2")

    detaUnWrapedPhase_1_2__2_3_Un_normalization = \
        medianBlur_unWrapedPhase_1_2__2_3_Un_normalization_No_Object - medianBlur_unWrapedPhase_1_2__2_3_Un_normalization
    medianBlur = cv2.medianBlur(detaUnWrapedPhase_1_2__2_3_Un_normalization, 5)
    detaUnWrapedPhase_1_2__2_3_Un_normalization = medianBlur
    showUnWrapedPhaseMap(detaUnWrapedPhase_1_2__2_3_Un_normalization + 10, 30,
                         "detaUnWrapedPhase_1_2__2_3_Un_normalization")
    test(detaUnWrapedPhase_1_2__2_3_Un_normalization, "detaUnWrapedPhase")

    # 相位常量测试
    # for i in range(detaUnWrapedPhase_1_2__2_3_Un_normalization.shape[0]):
    #     for j in range(detaUnWrapedPhase_1_2__2_3_Un_normalization.shape[1]):
    #         detaUnWrapedPhase_1_2__2_3_Un_normalization[i][j]=34

    objectHeight = getObjectHeight(detaUnWrapedPhase_1_2__2_3_Un_normalization, object_L, object_d, object_T=2.5, k=1)
    test(objectHeight, "objectHeight")
    for i in range(objectHeight.shape[0]):
        for j in range(objectHeight.shape[1]):
            if objectHeight[i][j] < 0:
                objectHeight[i][j] = 0

    for i in range(objectHeight.shape[0]):
        for j in range(objectHeight.shape[1]):
            if objectHeight[i][j] > 40:
                objectHeight[i][j] = 0

    showUnWrapedPhaseMap(objectHeight + 5, 8, "objectHeight")
    test(objectHeight, "objectHeight_discard_low0")
    # print(objectHeight.shape)
    # print(objectHeight.shape[0])

    # medianBlur=cv2.medianBlur(detaUnWrapedPhase_1_2__2_3_Un_normalization, 5)
    #
    # test(medianBlur,"medianBlur")

    # ************************************************************
    Z = objectHeight[0:830, 160:]
    X = np.arange(0, Z.shape[1], 1)
    Y = np.arange(0, Z.shape[0], 1)
    X, Y = np.meshgrid(X, Y)
    my3D.plot_3D_surface(Y, X, Z, elev=45, azim=-35)
    # my3D.plot_3D_surface(Y,X,Z,elev=45,azim=-10)
    plt.show()
