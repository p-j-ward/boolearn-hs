<h1 align="center">
  <img src="assets/boo.png" width="40" style="vertical-align: middle;"/>
boolearn
  <img src="assets/boo.png" width="40" style="vertical-align: middle;"/>
</h1>

<!--
<p align="center">
  <img src="docs/assets/dcnet.png" width="800"/>
</p>
-->

**boolearn** is a lightweight C toolkit for training **boolnets** - networks of Boolean threshold functions.

Since Boolean threshold functions have ±1 outputs, boolnets cannot be trained by gradient-descent (back-propagation).

The training algorithm in boolearn is based on **constraints** and **projections** to these constraints.

Using divide-and-concur, the constraints fall into sets *A* and *B* that each have easy projections. A trained network is a point that lies in both *A* and *B*.

Instead of minimizing a loss, boolearn minimizes the distance between pairs of points in *A* and *B* ("the gap"). Training is complete when this distance is zero.

For details, see *Learning with Boolean threshold functions* by Elser & Lal.

**Note:** boolearn is research software and intentionally CLI-first.

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
