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

**boolearn** is a lightweight C toolkit for training **boolnets**  — networks of **Boolean threshold functions**.

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
../src/layered 2x2.wth 2x2.net

# train
../src/train 2x2.net 2x2.dat 16 3. .2 1e-3 10 1e4 .01 1 2x2a
```

This produces `2x2a.cmd`, `2x2a.gap`, `2x2a.run`, `2x2a.sol` in the current diectory. 

### Width files

These start with the number of layers, followed by the width of each layer (input to output). Here is `2x2.wth` :

````
3
4 4 4 4
````

### Network files

The header lists `number of nodes`, `number of input nodes`, `number of output nodes`, and `number of edges`. This is followed by a list of edges in the form:

`receiving node`  `sending node`  `weight`

Here is the top of `2x2.net`:

````
17  5  4  60
    5    0    0.00000000
    5    1    0.00000000
    5    2    0.00000000
    5    3    0.00000000
    5    4    0.00000000
    6    0    0.00000000
    6    1    0.00000000
    6    2    0.00000000
    6    3    0.00000000
    6    4    0.00000000

    ...
````

Node 0 is the constant-input node (not included in the count of input nodes) with value -1. The weights are all zero because the network has not been trained. The file `2x2a.sol` produced by `train` replaces these with the weights of the trained network:

````
17  5  4  60
    5    0    1.28963089
    5    1   -0.01003229
    5    2    1.29350000
    5    3   -0.01789875
    5    4    1.28968558
    6    0   -0.00669030
    6    1   -0.00508804
    6    2   -1.29501533
    6    3   -1.28961000
    6    4   -1.28832088

    ...
````

### Data files

The first line of the header gives the number of data items, the second/third lines the `type` and `number` of input/output data. Here is the top of `2x2.dat`:


````
16
2 4
2 4

1 0 0 1
0 0 1 0

0 1 1 0
0 0 1 0

...
````

The first input/output pair is the fact `2x1=2`; the second is `1x2=2`. Here are the codes for `type` of input:

`2` Boolean (0 or 1)

`0` analog (numbers between 0 and 1)

In generative models there is no input and `number` is given as zero. But there are two choices for `type`:

`1  0`  1-hot input 

`2  0`  hypercube input

In both cases the data items just have an output vector. The length of the input 1-hot or input binary-code is set by the first width in the width file.


When `number` in the output data type equals 1 the network is trained to output 1-hot vectors, as in classification. For example, here is the header of `mnistjr.dat`:

````
3823
0	64
10	1
````

There are 3823 data. The second line tells us these are analog (0) and 64 in number, and the third line tells the network to output 1-hot vectors of length 10.


## What are the command-line arguments?

Here is the command line we used for training the 2-bit multiplier and the meaning of the arguments:

````
../src/train 2x2.net 2x2.dat 16 3. .2 1e-3 10 1e4 .01 1 2x2a
````

`2x2.net`  — network file of the model

`2x2.dat`  — data file

`16`  — number of data to be used for training

`3.`  — support parameter (sigma)

`.2`  — time step parameter (beta)

`1e-3`  — metric relaxation parameter (gamma)

`10`  — maximum number of line/checkpoints in `2x2a.gap` (gap output file)

`1e4`  — maximum number of iterations of the RRR constraint solver

`.01`  — stop when gap reaches this value

`1`  — number of runs from different random starts

`2x2a`  — identifier used on all output files

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
