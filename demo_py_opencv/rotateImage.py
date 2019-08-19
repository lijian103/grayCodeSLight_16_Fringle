import os
import glob
import shutil
import cv2

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
# os.mkdir("./rotateImages")
for e1 in img_name:
    shutil.copy("./cos_pictures/"+e1, "./rotateImages/"+e1)

images = glob.glob("./rotateImages/*.bmp")
for e1 in img_name:
    img = cv2.imread("./rotateImages/"+e1,0)
    rows,cols = img.shape
    M = cv2.getRotationMatrix2D((cols/2,rows/2),10,1)
    dst = cv2.warpAffine(img,M,(cols,rows))
    cv2.imwrite("./rotateImages/"+e1,dst)