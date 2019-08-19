import numpy as np
from matplotlib import pyplot as plt
import cv2

Image_rows = 1140
Image_cols = 912
# Image_cols = 896

def binMapToGray(bits):
    MapToGray = []
    binFromDec = []
    B = []
    G = []
    for dec in range(2 ** bits):
        binFromDec.append("{:b}".format(dec))
        binFromDec[dec] = binFromDec[dec].zfill(bits)
        for i in range(bits):
            B.append(binFromDec[dec][-(i + 1)])
        for i in range(bits - 1):
            G.append(str(int(B[i]) ^ int(B[i + 1])))
        G.append(B[bits - 1])
        G.reverse()
        G = "".join(G)
        MapToGray.append(str(G))
        B = []
        G = []
    return MapToGray

def grayCodeTOImgCode(grayCode):
    G0 = ""
    G = []
    for img_num in range(len(grayCode[0])):
        for each in grayCode:
            G0 = G0 + each[img_num]
        G.append(G0)
        G0 = ""
    return G

def grayCodeTOImgs(G,Image_rows = 1140,Image_cols = 896):
    imgs = []
    img_region = []
    Image_cols_Origin=Image_cols
    Image_cols=Image_cols//(2 ** len(G))*(2 ** len(G))
    img = np.zeros((Image_rows, Image_cols_Origin, 3), dtype="uint8")
    for each in range(2 ** len(G)):
        img_region.append(img[0:Image_rows, each * Image_cols // (2 ** len(G)):(each + 1) * Image_cols // (2 ** len(G))])

    if Image_cols_Origin != Image_cols:
        img_surplus_region = img[0:Image_rows,  Image_cols :]
        img_surplus_region[:]=0

    img = img_region[0]
    for img_num in range(len(G)):
        for i in range(2 ** len(G)):
            if G[img_num][i] == "0":
                img_region[i][:] = 20
            elif G[img_num][i] == "1":
                img_region[i][:] = 255

        for i in range(1, 2 ** len(G)):
            img = np.concatenate((img, img_region[i]), axis=1)

        if Image_cols_Origin != Image_cols:
            img = np.concatenate((img, img_surplus_region), axis=1)

        imgs.append(img)
        img = img_region[0]
    return imgs

def generateGrayCodeImgs(imgs):
    imgIndex=0
    for each in imgs:

        cv2.imwrite(
            "./grayCode_pictures/" + "grayCode" + "_G" + str(imgIndex) + ".bmp",
            each)
        cv2.imshow( "./grayCode_pictures/" + "grayCode" + "_G" + str(imgIndex) + ".bmp", each)

        imgIndex=imgIndex+1

if __name__ == "__main__":
    # bits=4
    # grayCode=binMapToGray(4)
    # G=grayCodeTOImgCode(grayCode)
    # imgs=grayCodeTOImgs(G,Image_rows = 1140,Image_cols = 912)
    # generateGrayCodeImgs(imgs)
    retval	=	cv2.getTickFrequency(		)





