import cv2
import numpy as np
import pandas as pd

SIZE_KERNEL_GAUSSIAN_BLUR = 51
CROPE_THRESH = 1600

def findcolorchecker(img, ref, mask = None, crope_thresh = 1600, size_kernel_gaussian_blur = 1):

    if mask.size != 0:
        thresh = mask
    else:
        croped = cv2.resize(img[0:crope_thresh, :], (0, 0), fx=0.25, fy=0.25)
        ret, thresh = cv2.threshold(cv2.GaussianBlur(cv2.cvtColor(croped, cv2.COLOR_BGR2GRAY), (size_kernel_gaussian_blur, size_kernel_gaussian_blur), 0), 0, 255,
                                    cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

    im, contours, hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    # cv2.imshow("thresh", thresh)
    # cv2.waitKey()
    area_size = []

    for i in range(len(contours)):
        output = np.zeros(croped.shape)
        cv2.drawContours(output, contours, i, (255, 255, 255), thickness=-1)
        area_size.append(np.count_nonzero(output))

    area_size = np.asarray(area_size)
    rect = cv2.minAreaRect(contours[np.where(area_size == max(area_size))[0][0]])
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    box = np.maximum(box, 0, box)
    # croped_copy = croped
    # cv2.drawContours(croped_copy, [box],0, [0, 0, 255], thickness=2)
    # cv2.imshow("red rectangle", croped_copy)
    # cv2.waitKey()
    distToTopLeftCorner = np.zeros(4)
    for i in range(4):
        distToTopLeftCorner[i] = pow(box[i, 0], 2) + pow(box[i, 1], 2)
    idx_topleftcorner = np.where(distToTopLeftCorner == min(distToTopLeftCorner))[0]
    box = box[
          [idx_topleftcorner, (idx_topleftcorner + 1) % 4, (idx_topleftcorner + 2) % 4, (idx_topleftcorner + 3) % 4], :]

    width = ref.shape[1]
    height = ref.shape[0]

    homography, mask = cv2.findHomography(box, np.array([[0, 0], [width, 0], [width, height], [0, height]]))

    result = cv2.warpPerspective(croped, homography, (width, height))

    return result

def extractColor(colorchecker, ref):
    keypoints = [cv2.KeyPoint(x, y, 5)
                 for y in np.linspace(22, ref.shape[0] - 22, 4)
                 for x in np.linspace(22, ref.shape[1] - 22, 6)]
    # cv2.drawKeypoints(colorchecker, keypoints, colorchecker)
    # cv2.imshow("plot", colorchecker)
    # cv2.waitKey()
    keypoints = np.int0(np.reshape([kp.pt for kp in keypoints], (-1, 2))[:, (1, 0)])

    col = []
    for i in range(keypoints.shape[0]):
        sample = colorchecker[(keypoints[i, 0] - 1):(keypoints[i, 0] + 1), (keypoints[i, 1] - 1):(keypoints[i, 1] + 1)]
        col.append(np.mean(sample, axis=(0, 1)))

    tab = pd.DataFrame(np.vstack(col), columns=["B", "G", "R"])
    color_palette = np.uint8(np.reshape(np.vstack(col), [4, 6, 3]))
    color_palette = cv2.resize(color_palette, (0, 0), fx=100, fy=100, interpolation=cv2.INTER_NEAREST)

    return tab, color_palette