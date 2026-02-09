---
title: Basics
parent: Applications
nav_order: 1
---

# User's Guide

This document comprises

* A brief description of **boolnets**, networks of Boolean threshold functions (BTFs).

* Instructions for using the software package **boolearn** for machine learning on boolnets.


## Boolean threshold functions

A BTF has $m$ inputs and a single output. The inputs are Boolean, where `True/1` is encoded as $+1$ and `False/0` is encoded as $-1$. When combined as a vector $x$, the inputs have squared norm $x\cdot x = m$. Associated with each of the BTF's inputs is a real-valued weight. The corresponding $m$-component weight vector $w$ is also normalized as $w\cdot w = m$. The BTF's output, called $y$, is Boolean and given by the formula

$y = \mathrm{sgn}(w\cdot x)$

In boolearn this relationship is treated as a constraint. If it is not satisfied, all three entities ($w$, $x$, and $y$) are changed minimally, in a least-squares sense, so that the relationship holds.

Boolearn imposes an additional constraint that goes beyond how BTFs are normally used. This is a constraint on the margin between the two outputs:

$|w\cdot x| \ge \delta$

The positive number $\delta$ is the *margin parameter*. Large $\delta$ is more restrictive, and forces the network to stay clear of "gray areas" in its decision making. Large margins are useful in another respect, as expressed by the following theorem:

If a BTF $`f(x):\{-1,1\}^m\to \{-1,1\}`$ has a representation with margin $\delta=\sqrt{m/n}$ for some $n>0$, then $f$ has a representation where the support $s$ of its weights $w$ is at most the largest odd integer not exceeding $n$.

The positive number $n$ is the *BTF-norm parameter*. By defining the margin with the setting `n` << `3`, the BTF weights have support $s=3$ or less. When $s=3$, the weight vector has the form

$w=(\sqrt{m/3}) (\pm 1, \pm 1, \pm 1, 0, \ldots, 0)$ 

or its permutations. With all positive signs this implements the majority gate on the three inputs with nonzero weights. Negative weights correspond to NOT applied to their inputs. In the other case for margins defined by BTF-norm $n=3$, when the support is smaller ($s<3$), the BTF simply transmits the value of the input with the weight of largest magnitude, negating when the latter is negative. Since 

$\mathrm{AND}(p,q) = \mathrm{MAJ}(0,p,q)\qquad \mathrm{OR}(p,q) = \mathrm{MAJ}(1,p,q)$ 

we see that boolnets with BTF-norm $n=3$ implement circuits comprising NOT and 2-iput AND/OR gates. BTFs with larger $n$ can implement AND/OR with more than 2 inputs, but also gates not in the standard repertoir. The corresponding smaller and less restrictive margins translate to networks with a higher information capacity, and the possibility of representations with fewer nodes/BTFs. It is important to bear in mind that the margin referred to by the theorem is the maximum of $|w\cdot x|$ over *all possible* inputs $x$ to the BTF. Many data sets have diverse inputs, so this condition usually holds for the BTFs near the input layer. Since the diversity/entropy may decrease in the higher layers, the BTF supports in those layers may exceed the bound set by the BTF-norm parameter.

## Directory organization

Boolearn has the following directories:

`autocode cellauto decode randlogic mnist mult src`

These hold data and results for various applications. All source files are kept in the directory `src`. To create the executables, go to `src` and do

`make all`

Since the executables are placed in that same directory, to run a program `prog` from one of the applications directories you need to do

`./../src/prog ...`

where `...` are the command-line arguments for `prog`. Since it's hard to remember all the arguments, all the programs are set up so that if you enter no arguments at all, the program tells you what they should be. 


## Networks

Boolnets can have the structure of any directed acyclic graph. The software does not expect layers. Networks are one of the main inputs/outputs and are read/written to files with suffix `.net`. Because layered networks are so widely used, there is a program for generating such networks. To learn how to create a layered network and to see some small examples, go to the `mult` directory.

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
./../src/layered 
expected 3 arguments: andorprob widthfile netfile
```
The first argument is only relevant when generating synthetic data with the network. We will use this feature in a different application. The second argument, `widthfile`, is a tiny file you need to create yourself that sets the widths of the layers. Here's an example, `2x2.wth`:
```
2
4 2 4
```
This defines a network with 2 layers, 4 input nodes, 4 output nodes, and 2 internal nodes. The last argument of `layered` lets you specify the network's name. For example,
```
./../src/layered .5 2x2.wth 2x2.net
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
