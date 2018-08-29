from col import colorcorrection
import cv2
import os
import pandas as pd

rep_species = "FLV/"
rep_crope = rep_species + "Crope"
rep_crope_colorcorrection = rep_species + "Crope_color_correction"
rep_colochecker = rep_species + "Colorchecker_summary"
rep_colorchecker_colorcorrection = rep_species + "Colorchecker_summary_correction"
rep_tab = rep_species + "ColorChecker_tab"

rep_tab_ref = "ref.csv"
tab_ref = pd.read_csv(rep_tab_ref, sep=";")
filename_list = os.listdir(rep_crope)

for name in filename_list:
    crope = cv2.imread(rep_crope + "/" + name)
    colorchecker = cv2.imread(rep_colochecker + "/" + name)
    name = name.split(".")[0]
    tab = pd.read_csv(rep_tab+ "/" + name + ".txt", sep=";")
    crope_color_correct = colorcorrection(crope, tab, tab_ref)
    colorchecker_color_correct = colorcorrection(colorchecker, tab, tab_ref)
    cv2.imwrite(rep_crope_colorcorrection + "/" + name + ".jpg", crope_color_correct)
    cv2.imwrite(rep_colorchecker_colorcorrection + "/" + name + ".jpg", colorchecker_color_correct)
    print(name + ": Done")

