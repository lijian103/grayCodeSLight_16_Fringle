import numpy as np
import matplotlib.pyplot as plt
import cv2

import numpy as np

a = np.array([[1, 2], [3, 4]])

print('数组 a：')
print(a)
b = np.array([[11, 12], [13, 14]])

print('数组 b：')
print(b)

print('内积：')
print(np.dot(a,b))
camin=np.array([[1454.4,0,376.855],[0,1447.26,222.825],[0,0,1]])
camex=np.array([[0.769809,-0.0158159,-57.2734],[-0.311702,0.863064,-24.8645],[0.556988,0.504847,540.21]])

print(np.dot(camin,camex))

