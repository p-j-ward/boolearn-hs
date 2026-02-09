# Development notes

This doc is for people who want to understand performance, scaling, and how to extend the repo.

## Scaling of the compute-time

In several of the next applications the time used by `train` will be much longer than it was for the multiplier circuits. The algorithm can be highly parallelized, but this implementation is serial and runs on a single core. This has one advantage, in that the compute-time has the following simple scaling rule:

time $\propto$ `edges` $\times$ `items` $\times$ `iter`

Here `iter` $\le$ `itermax`, the number of iterations used. `train` can be parallelized so that the time is divided by `nodes` $\times$ `items`.


## Source layout (quick map)

- `boolearn/src/` — core C implementation (`train.c`, `layered.c`, `net.c`, …)
- `boolearn/*/` — small “application” folders (datasets + width files + results)
- `data/` — shared datasets (see `data/README.md`)

## Reproducibility

`train` seeds the RNG with the current time (`srand(time(NULL))`), so runs will differ by default.

If you need deterministic experiments, the simplest approach is to change the seed in `boolearn/src/train.c` (or add a seed flag / environment variable and plumb it through).
