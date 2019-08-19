# -*- coding: utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
import cv2
from mpl_toolkits.mplot3d import Axes3D

def plot_3D_scatter(X,Y,Z,elev=45,azim=45):
    plt.figure("3D_scatter_elev:"+str(elev)+"_azim:"+str(azim))
    ax=plt.subplot(projection='3d')
    ax.scatter3D(X,Y,Z,c='r',s=50,cmap='autumn')
    ax.view_init(elev=elev,azim=azim)#改变绘制图像的视角,即相机的位置,azim沿着z轴旋转，elev沿着y轴
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.draw()


def plot_3D_surface(X,Y,Z,elev=45,azim=45):
    fig = plt.figure("3D_surface_elev:"+str(elev)+"_azim:"+str(azim))
    ax = Axes3D(fig)
    # ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='rainbow')
    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap='rainbow')
    ax.view_init(elev=elev, azim=azim)  # 改变绘制图像的视角,即相机的位置,azim沿着z轴旋转，elev沿着y轴
    ax.set_zlabel('Z')  # 坐标轴
    ax.set_ylabel('X')
    ax.set_xlabel('Y')
    plt.draw()


if __name__ == "__main__":

    X = np.arange(0, 912, 1)
    Y = np.arange(0, 1140, 1)
    X, Y = np.meshgrid(X, Y)
    img = cv2.imread("./cos_pictures/" + "cos_wave" + "_Period_" + "1" + "_initial_phase_" + "0" + ".bmp")
    Z = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    plot_3D_surface(X, Y, Z, elev=0, azim=0)
    plot_3D_surface(X, Y, Z, elev=45, azim=45)
    # plot_3D_surface(X, Y, Z, elev=0, azim=45)
    # plot_3D_surface(X, Y, Z, elev=45, azim=0)

    plot_3D_scatter(X, Y, Z, elev=30, azim=30)
    # plt.figure("3D_scatter")
    # ax = plt.subplot(projection='3d')
    # ax.scatter(X, Y, Z, c='r')  # 绘制数据点,颜色是红色
    #
    # ax.set_zlabel('Z')  # 坐标轴
    # ax.set_ylabel('Y')
    # ax.set_xlabel('X')
    # plt.draw()
    #
    plt.show()
