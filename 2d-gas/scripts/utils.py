# -*- coding: utf-8 -*-
"""
Created on Mon Dec 19 2022
@author: davidsantiagoquevedo
"""
import pandas as pd

def process_dat(datContent):
    """Read .dat contents from c++ simulations
    """
    df = pd.DataFrame(datContent)
    df[['x','y','vx','vy']] = df[0].str.split(",", n = 3, expand = True)
    df['x'] = pd.to_numeric(df['x'])
    df['y'] = pd.to_numeric(df['y'])
    df['vx'] = pd.to_numeric(df['vx'])
    df['vy'] = pd.to_numeric(df['vy'])
    df = df[['x','y','vx','vy']]
    return df