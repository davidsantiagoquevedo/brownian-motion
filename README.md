# 2 dimensional brownian motion
This repository contains:

1. A finite elements simulation implemented in c++ for the brownian motion exhibit by a particle confined in a two dimensional gas.

For running the simulations + a basic animation made in gnuplot run:

<code> ./scripts/2d-gas/main.o 1 -> ./results/animation.dat </code>

<code> gnuplot ./results/animation.dat </code>

The analysis of the simulated data is performed in python using pandas and pystan. 

A basic animation runs in gnuplot and the animation of the 

2. (Soon) A stochastic simulation performed in python to replicate the behavior of the system based in the analytical treatment of browian motion.
