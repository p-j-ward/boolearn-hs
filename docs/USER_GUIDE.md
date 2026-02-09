# User Guide

> [!TIP]
> **Build + run in ~2 minutes**
>
> ```bash
> cd boolearn/src
> make all
>
> cd ../mult
> ../src/layered 0.5 2x2.wth 2x2.net
> ../src/train 2x2.net 2x2.dat 16 3 0.2 1e-3 10 1e4 0.01 1 run1
> ```
>
> This writes `run1.cmd`, `run1.gap`, `run1.run`, `run1.sol` in the current folder.

## Repo layout

- `boolearn/src/` — C sources + `makefile` (build outputs land here too)
- `boolearn/*/` — example / application folders (`mult`, `mnist`, `decode`, …)
- `data/` — datasets (`*.dat`) mirrored per-application for convenience
- `docs/` — documentation (you are here)
- `notebooks/` — optional analysis notebooks (you can add later)

> [!NOTE]
> Some older instructions you may find online refer to `mult/` or `src/` at the repo root.
> In this repo, those live under `boolearn/` (e.g. `boolearn/mult`, `boolearn/src`).

## Build

From the repo root:

```bash
cd boolearn/src
make all
```

That builds a few CLI tools (notably `layered` and `train`) as binaries inside `boolearn/src/`.

## Running the tools

Boolearn is CLI-first. A nice usability feature is that **every tool prints its expected arguments when run with no args**.

Example:

```bash
cd boolearn/mult
../src/train
```

## Files you’ll see

- `*.wth` — a *width file* describing layer sizes (used by `layered`)
- `*.net` — a boolnet definition + weights (input/output of tools)
- `*.dat` — dataset files (inputs/outputs per item, with a header)
- `*.gap` — progress log during training (watch this during long runs)
- `*.sol` — learned weights / solution
- `*.cmd` — the exact command line used (for reproducibility)
- `*.run` — multi-run statistics when `trials > 1`
- `*.gen` — generated data in “zero-input data” mode (only for some setups)

## Directory organization (detail)

Boolearn’s application folders live under `boolearn/`:

`autocode  cellauto  decode  randlogic  mnist  mult  src`

These hold data and results for various applications. All source files are kept in the directory `boolearn/src`. To create the executables, go to `boolearn/src` and do:

```bash
make all
```

Since the executables are placed in that same directory, to run a program `prog` from one of the application directories you need to do:

```bash
../src/prog ...
```

where `...` are the command-line arguments for `prog`. Since it's hard to remember all the arguments, all the programs are set up so that if you enter no arguments at all, the program tells you what they should be.

## Networks and data files (detail)

Boolnets can have the structure of any directed acyclic graph. The software does not expect layers. Networks are one of the main inputs/outputs and are read/written to files with suffix `.net`. Because layered networks are so widely used, there is a program for generating such networks. To learn how to create a layered network and to see some small examples, go to the `boolearn/mult` directory.

Data files have suffix `.dat`. Here's the top of `2x2.dat`:
```
16
2 4
2 4

1 0 0 1
0 0 1 0

0 1 1 0
0 0 1 0

etc.
```
The `16` in the header tells you there are $16$ data items. The `2 4` that follows tells you that the network expects 4 binary (2-valued) inputs. Because of the `2 4` in the next line, the network produces 4 binary outputs. From the two data items shown, you can see that this is just a collection of (2-bit) $\times$ (2-bit) multiplication facts (in some random order). For example, from the first two lines of data we learn that 2 $\times$ 1 = 2, while the next two lines express 1 $\times$ 2 = 2.

To learn the multiplication table up to 2-bits you need to create a network that takes 4 Boolean inputs and produces the same number of outputs. The program `layered` will let you create such a network. Here is what happens when you make a query for the command-line arguments:
```
../src/layered 
expected 3 arguments: andorprob widthfile netfile
```
The first argument is only relevant when generating synthetic data with the network. We will use this feature in a different application. The second argument, `widthfile`, is a tiny file you need to create yourself that sets the widths of the layers. Here's an example, `2x2.wth`:
```
2
4 2 4
```
This defines a network with 2 layers, 4 input nodes, 4 output nodes, and 2 internal nodes. The last argument of `layered` lets you specify the network's name. For example,
```
../src/layered .5 2x2.wth 2x2.net
```
produces the network `2x2.net`:
```
11  5  4  22
    5    0    0.00000000
    5    1    2.23606798
    5    2    0.00000000
    5    3    0.00000000
    5    4    0.00000000
    6    0   -1.29099445
    6    1   -1.29099445
    6    2   -1.29099445
    6    3    0.00000000
    6    4    0.00000000
    7    0    1.00000000
    7    5    1.00000000
    7    6    1.00000000
    8    0    0.00000000
    8    5   -1.73205081
    8    6    0.00000000
    9    0   -1.00000000
    9    5    1.00000000
    9    6   -1.00000000
   10    0   -1.00000000
   10    5    1.00000000
   10    6   -1.00000000
```
The first line tells you the network has 11 nodes of which 5 are inputs and 4 are outputs. Since there are no BTFs at the inputs, the network has 11 - 5 = 6 BTFs (one for each internal and output node). Why 5 inputs instead of 4? Node `0` is an extra node not included in any layer that serves as a bias input to all the BTFs of the network. Its value is constant (`False/0`) and BTFs tune their bias by adjusting the corresponding weight. The program `layered` forms completely connected networks within each layer. The total number of edges in our example is therefore 6 + 2 $\times$ 4 + 4 $\times$ 2 = 22.

In software, the plural forms of the graph objects are reserved for their number. Thus `nodes = 11`, `innodes = 5`, `outnodes = 4`, `edges = 22`.

The 22 lines after the header give the node labels (`0` through `nodes-1`) for each edge. Labels `0` through `innodes-1` are reserved for the network inputs. The remaining nodes, having BTFs, are labeled so that by evaluating the corresponding BTFs in ascending order (5, 6, $\ldots$ , 10), their inputs are either network inputs or the outputs of previously evaluated BTFs. The first node in each line of a network file is the receiving node, and explains why the lowest label is `5` in our example.

The numbers in the third column are the edge-weights. The program `train` we will use to learn the multiplication facts ignores these numbers. Still, we can check that the weight normalization is correct. The BTF at node `5` has five inputs with weights that have squared-norm 5, etc. Moreover, each node has either 1 or 3 nonzero weights. The `andorprob` argument (`.5`) of `layered` was used to set the probability of the case 3. When `layered` chooses (randomly) this case, the three weights are given equal magnitudes and random signs. Also, since the BTF in that case always receives input from the bias node `0`, the weights created by `layered` correspond to a random circuit of NOT and 2-input AND/OR gates. In a large network with setting `andorprob` << `.5`, about half of the nodes act as AND/OR gates taking inputs from a random pair of nodes in the layer below. The other nodes, with a single nonzero weight, just move Boolean values (or their negation) between adjacent layers.


## Next steps

- Follow the multiplier tutorial → [EXAMPLES.md](EXAMPLES.md#first-training-example-a-binary-multiplier)
- Learn what “gap”, margins, and BTF-norm mean → [CONCEPTS.md](CONCEPTS.md)
