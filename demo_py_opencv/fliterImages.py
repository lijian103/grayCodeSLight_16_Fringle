import os
import glob
import shutil
import cv2
import numpy as np
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

# os.mkdir("./fliterImages")
for e1 in img_name:
    shutil.copy("./Htf_7_9/"+e1, "./fliterImages/"+e1)

images = glob.glob("./fliterImages/*.bmp")
for e1 in img_name:
    img = cv2.imread("./fliterImages/"+e1,0)
    # median = cv2.medianBlur(img, 5)
    # blur = cv2.GaussianBlur(img, (5, 5), 0)
    # kernel = np.ones((5, 5), np.float32) / 25
    # dst = cv2.filter2D(img, -1, kernel)
    dst = cv2.bilateralFilter(img, 9, 75, 75)
    cv2.imwrite("./fliterImages/"+e1,dst)
