# -*- coding: utf-8 -*-
"""
Created on Mon Dec 19 2022
@author: davidsantiagoquevedo
"""
import subprocess
import sys
import yaml
import time
import pandas as pd
import matplotlib.pyplot as plt

config = yaml.safe_load(open("config.yml", "r"))["default"]

OUT_PATH = config['PATHS']['OUT_PATH'].format(dir = '2d-gas')
FIG_PATH = config['PATHS']['FIG_PATH'].format(dir = '2d-gas')
SCRIPTS_PATH = config['PATHS']['SCRIPTS_PATH'].format(dir = '2d-gas')
UTILS_PATH = config['PATHS']['UTILS_PATH'].format(dir = '2d-gas')

sys.path.append(UTILS_PATH)
import utils as ut

datContent = [i.strip().split() for i in open(f'{OUT_PATH}/random_walk0.dat').readlines()]

df_data = ut.process_dat(datContent)

x_0 = df_data.x.iloc[0]
y_0 = df_data.y.iloc[0]

df_data["r2"] = (df_data.x - x_0)**2 + (df_data.y - y_0)**2

df_data["r2"].plot()
plt.show()


