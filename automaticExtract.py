from col import extractColor
from col import findcolorchecker
import os
import cv2
import pandas as pd

rep_species = "FLV/"
rep_img = rep_species + "Original"
rep_colorchecker = rep_species + "ColorChecker"
rep_summary = rep_species + "ColorChecker_summary"
rep_tab = rep_species + "ColorChecker_tab"

ref = cv2.imread("CCPPPatches.jpg")
names_list = os.listdir(rep_img)

for name in names_list:
    img = cv2.imread(rep_img + "/" + name)
    colorchecker = findcolorchecker(img, ref)
    cv2.imwrite(rep_colorchecker + "/" + name, colorchecker)
    tab, color_palette = extractColor(colorchecker, ref)
    cv2.imwrite(rep_summary + "/" + name, color_palette)
    tab.to_csv(rep_tab + "/" + name.split(".")[0] + ".csv")
    print(name + " : Done")
