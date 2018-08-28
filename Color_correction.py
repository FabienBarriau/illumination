import numpy as np
from sklearn import linear_model
import os
import pandas as pd
from sklearn import linear_model
import cv2
import matplotlib.pyplot as plt

def colorcorrection(crope, tab, tab_ref):

    X_blue = tab[['B']].values[18:24]
    X_green = tab[['G']].values[18:24]
    X_red = tab[['R']].values[18:24]

    y_blue = tab_ref[['B']].values[18:24]
    y_green = tab_ref[['G']].values[18:24]
    y_red = tab_ref[['R']].values[18:24]

    lr_blue = linear_model.LinearRegression(fit_intercept=True)
    lr_green = linear_model.LinearRegression()
    lr_red = linear_model.LinearRegression()
    lr_blue.fit(X_blue, y_blue)
    lr_green.fit(X_green, y_green)
    lr_red.fit(X_red, y_red)

    crope[:, :, 0] = np.uint8(crope[:, :, 0] * lr_blue.coef_[0][0] + lr_blue.intercept_[0])
    crope[:, :, 1] = np.uint8(crope[:, :, 1] * lr_green.coef_[0][0] + lr_green.intercept_[0])
    crope[:, :, 2] = np.uint8(crope[:, :, 2] * lr_red.coef_[0][0] + lr_red.intercept_[0])

    return crope

