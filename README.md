<h1 align="center">
  <img src="assets/boo.png" width="30" style="vertical-align: middle;"/>
  Boolearn
  <img src="assets/boo.png" width="30" style="vertical-align: middle;"/>
</h1>

<p align="center">
  <img src="docs/assets/dcnet.png" width="800"/>
</p>


Boolearn is a lightweight C toolkit for training and testing **Boolean threshold networks** via **constraint projections**. 

Instead of minimizing a loss, training drives a **gap** (constraint violation) to zero.

The design is intentionally CLI-first and composable.

## Quickstart

Build the tools:

```bash
cd boolearn/src
make all
```

Run the smallest tutorial example (2-bit multiplier):

```bash
cd ../mult

# generate a layered network from a width file
../src/layered 0.5 2x2.wth 2x2.net

# train
../src/train 2x2.net 2x2.dat 16 3 0.2 1e-3 10 1e4 0.01 1 run1
```

This produces `run1.cmd`, `run1.gap`, `run1.run`, `run1.sol` in the current folder.

## Mental model

- `*.dat` — datasets (booleans encoded as ±1)
- `*.net` — network graphs + weights
- `layered` — generates a fully connected layered boolnet from a `*.wth` file
- `train` — runs constraint satisfaction and writes a `*.gap` log you can watch

## Documentation

- Start here → [docs/README.md](docs/README.md)
- Applications → [docs/applications/](docs/applications/)

## Data

Datasets live under [data/](data/) (see [data/README.md](data/README.md)).
