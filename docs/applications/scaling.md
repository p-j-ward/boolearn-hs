---
title: Compute-time scaling
parent: Applications
nav_order: 3
---

## Scaling of the compute-time

In several of the next applications the time used by `train` will be much longer than it was for the multiplier circuits. The algorithm can be highly parallelized, but this implementation is serial and runs on a single core. This has one advantage, in that the compute-time has the following simple scaling rule:

time $\propto$ `edges` $\times$ `items` $\times$ `iter`

Here `iter` $\le$ `itermax`, the number of iterations used. `train` can be parallelized so that the time is divided by `nodes` $\times$ `items`.
