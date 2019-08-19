import sys
import cv2
import numpy as np
from matplotlib import pyplot as plt
import three_dimession_image as my3D

img_name=[
    "cos_wave_Period_70_initial_phase_-2.0943951023931953.bmp",
    "cos_wave_Period_70_initial_phase_0.bmp",
    "cos_wave_Period_70_initial_phase_2.0943951023931953.bmp",

    "cos_wave_Period_64_initial_phase_-2.0943951023931953.bmp",
    "cos_wave_Period_64_initial_phase_0.bmp",
    "cos_wave_Period_64_initial_phase_2.0943951023931953.bmp",

    "cos_wave_Period_59_initial_phase_-2.0943951023931953.bmp",
    "cos_wave_Period_59_initial_phase_0.bmp",
    "cos_wave_Period_59_initial_phase_2.0943951023931953.bmp",
                  ]
def test(WrapedPhase, figureName="test"):
    initial_phase = 0
    x_axis_cos = np.arange(initial_phase, WrapedPhase.shape[1], 1)
    plt.figure(figureName)
    y_axis = WrapedPhase[600:601, :]
    plt.plot(x_axis_cos, y_axis.reshape(WrapedPhase.shape[1]))
    # plt.plot(x_axis_cos,np.unwrap(y_axis.reshape(912))) ## plt.plot(x_axis_cos,np.unwrap(2*wraped_phase)/2)

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
        img[500:700,500:900]=img[500:700,500:900]/2
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

if __name__ == "__main__":

    img = cv2.imread("./Htf_7_9/"+img_name[0])
    img = np.asarray(img, dtype="float32")
    # cv2.imshow(str(each),img)
    if img.ndim == 3:
        img=cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    test(img,figureName="test")

    img = cv2.imread("./rotateImages/" + img_name[0])
    img = np.asarray(img, dtype="float32")
    # cv2.imshow(str(each),img)
    if img.ndim == 3:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    test(img, figureName="test1")
    plt.show()