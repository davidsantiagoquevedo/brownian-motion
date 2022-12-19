# -*- coding: utf-8 -*-
"""
Created on Thr Dec 1 2022
@author: davidsantiagoquevedo
"""
import subprocess
import sys
import yaml
import time
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import PillowWriter

config = yaml.safe_load(open("config.yml", "r"))["default"]

OUT_PATH = config['PATHS']['OUT_PATH'].format(dir = '2d-gas')
FIG_PATH = config['PATHS']['FIG_PATH'].format(dir = '2d-gas')
SCRIPTS_PATH = config['PATHS']['SCRIPTS_PATH'].format(dir = '2d-gas')
UTILS_PATH = config['PATHS']['UTILS_PATH'].format(dir = '2d-gas')

sys.path.append(UTILS_PATH)
import utils as ut

an_step = 100
t0 = time.time()
datContent = [i.strip().split() for i in open(f'{OUT_PATH}/random_walk0.dat').readlines()]

df_data = ut.process_dat(datContent)
df_data['x_diff'] = df_data.x.diff()
df_data['y_diff'] = df_data.y.diff()
###################################################
# Animation
fig, ax = plt.subplots(1, 3, figsize = (15, 5))

###################################################
# Random path
axi = ax[0]
axi.set_xticks([])
axi.set_yticks([])
l, = axi.plot([], [], 'k-')
axi.set_xlim(0, 56)
axi.set_ylim(0, 56)
xlist = []
ylist = []

# Histogram x
axx = ax[1]
xdifflist = []

# Histogram y
axy = ax[2]
ydifflist = []

# Gif
metadata = dict(title='Movie', artist='codinglikemad')
writer = PillowWriter(fps=60, metadata=metadata)

print('Time elapsed: ', round((time.time()-t0)/60,1), ' minutes')


with writer.saving(fig, f'{FIG_PATH}random_walk.gif', 100):
    # Plot the first line and cursor
    for n in range(len(df_data)):
        if n%an_step == 0:
            xlist.append(df_data.x[n])
            ylist.append(df_data.y[n])
            
            try:
                xdifflist.append(df_data.x_diff[n])
                axx.hist(xdifflist, density = True)
                ydifflist.append(df_data.y_diff[n])
                axy.hist(ydifflist, density = True)
            except:
                dummy = 0 
            
            axx.set_xlabel('Variation x')
            axx.set_ylabel('Frequency')
            axy.set_xlabel('Variation y')
            axy.set_ylabel('Frequency')

            axx.set_xlim(-0.12, 0.12)
            axx.set_ylim(0, 35)
            axy.set_xlim(-0.12, 0.12)
            axy.set_ylim(0, 35)
            
            l.set_data(xlist,ylist)
            writer.grab_frame()
            
            axx.cla()
            axy.cla()