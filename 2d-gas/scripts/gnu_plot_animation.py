# -*- coding: utf-8 -*-
"""
Created on Thr Dec 1 2022
@author: davidsantiagoquevedo
"""
import subprocess
import yaml

config = yaml.safe_load(open("config.yml", "r"))["default"]

OUT_PATH = config['PATHS']['OUT_PATH'].format(dir = '2d-gas')
FIG_PATH = config['PATHS']['FIG_PATH'].format(dir = '2d-gas')
SCRIPTS_PATH = config['PATHS']['SCRIPTS_PATH'].format(dir = '2d-gas')

filename = f"filename='{FIG_PATH}/2d-gas.gif'"
# Compile script
subprocess.call(f'g++ {SCRIPTS_PATH}2d-gas.cpp -o {SCRIPTS_PATH}main.o', shell = True)

# Create animation
subprocess.call(f'{SCRIPTS_PATH}main.o a -> {OUT_PATH}animation.dat', shell = True)
subprocess.call(f'gnuplot -e "{filename}" {OUT_PATH}animation.dat', shell = True)