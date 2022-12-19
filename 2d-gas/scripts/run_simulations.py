# -*- coding: utf-8 -*-
"""
Created on Thr Dec 1 2022

@author: davidsantiagoquevedo
"""
import subprocess
import yaml
import time

config = yaml.safe_load(open("config.yml", "r"))["default"]

OUT_PATH = config['PATHS']['OUT_PATH'].format(dir = '2d-gas')
FIG_PATH = config['PATHS']['FIG_PATH'].format(dir = '2d-gas')
SCRIPTS_PATH = config['PATHS']['SCRIPTS_PATH'].format(dir = '2d-gas')
ENSEMBLES = config['SIMULATION']['ENSEMBLES']

flag = 'r' # e: get whole ensemble information
           # r: get random walker information

if flag == 'e':
    data_name = 'ensemble'
if flag == 'r':
    data_name = 'random_walk'

t0 = time.time()
# Compile script
subprocess.call(f'g++ {SCRIPTS_PATH}2d-gas.cpp -o {SCRIPTS_PATH}main.o', shell = True)

ensembles = 1
for e in range(ENSEMBLES):
    print("simulation: ",e)
    subprocess.call(f'{SCRIPTS_PATH}main.o {flag} -> {OUT_PATH}{data_name}{e}.dat', shell = True)
    print('Time elapsed: ', round((time.time()-t0)/60,1), ' minutes')