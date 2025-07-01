# User's Guide

This document comprises

1) a brief description of Boolean threshold function networks (boolnets), and,

2) complete instructions for using the software package **boolearn** for machine learning on boolnets.


## Boolean threshold functions

A BTF has m inputs and a single output. The inputs are Boolean, where True/1 is encoded as +1 and False/0 is encoded as -1. When combined as a vector x, the inputs have squared norm x.x = m. Associated with each of the BTF's inputs is a real-valued weight. The corresponding m-component weight vector w is normalized to also have squared norm m. The BTF's output, called y, is Boolean and given by the formula

$y = \mathrm{sgn}(w\cdot x)$

In boolearn this relationship is treated as a constraint. If it is not satisfied, all three entities (w, x, and y) are changed minimally in a least-squares sense, so that the relationship holds.

Boolearn imposes an additional constraint that goes beyond how BTFs are normally used. This is a constraint on the margin between the two outputs:

$|w\cdot x| \ge \delta$

The positive number delta is the margin parameter. Large delta is more restrictive, and when sufficiently large, boolnets can represent arbitrary Boolean circuits comprised of binary-And/Or gates as well as Not gates. The latter correspond to weights with negative sign.

The previous statement is best explained as follows. Consider the following weight vectors:

$\sqrt{m/1}\quad (1,0,\quad\ldots\quad)$

$\sqrt{m/3}\quad (1,1,1,0,\quad\ldots\quad)$

$\sqrt{m/5}\quad (1,1,1,1,1,0,\quad\ldots\quad)$

$\sqrt{m/7}\quad (1,1,1,1,1,1,1,0,\quad\ldots\quad)$

$\sqrt{m/7}\quad (2,1,1,1,0,\quad\ldots\quad)$

etc.

The ... means padded with 0 to have m components. The prefactor provides the correct normalization. Without the prefactor these weight vectors have margin 1. This margin is realized for s linearly independent Boolean inputs x, where s is the support of the weight vector. This property makes these weight vectors extremal in the following sense. Suppose the margin parameter is set at sqrt(m/5). In that case all the BTFs in the network are equivalent to one of the top three BTFs in the above list, including of course permutations and sign changes applied to their components. BTFs that realize the equality-case of the margin constraint over all 2^m inputs will have weights with support s = 5. All BTFs not equivalent to one of the top three are eliminated because their margins are too small. This restriction is relaxed if a BTF does not see all 2^m inputs in the course of training.

The prefactor denominators 1,3,5, ... are the squared norms of the BTF types. A BTFs with squared norm 1 is a dictator: its output is dictated by just one of its inputs, with an optional Not. BTFs with squared norm 3 are 3-input majority gates, again with optional Nots applied to the inputs.

Boolnets with margin sqrt(m/3) imposed on all its m-input BTFs can represent arbitrary logic circuits comprised of binary And/Or gates and Not. That's because

And(p,q) = Maj(0,p,q)
 Or(p,q) = Maj(1,p,q)

While these are sufficient to express an arbitrary Boolean function, the higher squared-norm BTFs provide more efficient representations. For example, the 5th case above, with support s = 4, is equivalent to a ternary And/Or.


## Directory organization

**Boolearn** has the following directories:

`autocode cellauto decode logic mnist mult src`

These hold data and results for various applications. All source files are kept in the directory src. To create the executables, go to src and do

`make all`

Since the executables are placed in that same directory, to run the program prog from one of the applications directories you need to do

`./../src/prog ...`

where `...` are the command-line arguments for prog. Since it's hard to remember all the arguments, all the programs are set up so that if you enter no arguments at all, the program tells you what they should be. 


## Networks

The networks of boolearn can have the structure of any directed acyclic graph. The software does not expect layers. Networks are one of the main inputs/outputs and read/written to files with suffix .net. Because layered networks are so widely used, there is a program for generating such networks. To learn how to create a layered network and to see some small examples, go to the mult directory.

Data files have suffix .dat. Here's the top of 2x2.dat:
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
The 16 in the header tells you there are 16 data items. The 2 4 that follows tells you that the network expects 4 binary (2-valued) inputs. Because of the 2 4 in the next line, the network produces 4 binary outputs. From the two data items shown you can see that this is just a collection of 2-bit x 2-bit multiplication facts (in some random order). For example, from the first two lines of data we learn that 2x1 = 2, while the next two lines express 1x2=2.

To learn the multiplication table up to 2-bits you need to create a network that takes 4 Boolean inputs and produces the same number of outputs. The program layered will let you create such a network. Here is what happens when you make a query for the command-line arguments:

./../src/layered 
expected 3 arguments: andorprob widthfile netfile

The first argument is only relevant if you also want to set the network weights for generating synthetic data. We will use this feature in a different application. The second argument, widthfile, is a tiny file you need to create yourself that sets the widths of the layers. Here's an example, 2x2.wth:

2
4 2 4

This creates a network with 2 layers, 4 input nodes, 4 output nodes, and 2 internal nodes. The last argument of layered lets you specify the network's name. For example,

./../src/layered .5 2x2.wth 2x2.net

produces the following network (2x2.net):
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
The first line tells you the network has 11 nodes of which 5 are inputs and 4 are outputs. Since there are no BTFs at the inputs, the network has 11 - 5 = 2 + 4 = 6 BTFs. Why 5 inputs instead of 4? Input 0 is a special constant input with value 0 (False) that serves as a bias input to all the BTFs (each BTF learns its own bias-weight). Because the nodes spanning layers are completely connected, the network has altogether 6 + 2x4 + 4x2 = 22 edges.

In software, the plural forms of the graph objects are reserved for their number. Thus nodes = 11, innodes = 5, outnodes = 4, edges = 22.

The 22 lines after the header give the node labels (0 through nodes-1) for each edge. Labels 0 through innodes-1 are reserved for the network inputs. The remaining nodes, having BTFs, are labeled so that by evaluating the corresponding BTFs in ascending order (5, 6, ..., 10), their inputs are either network inputs or the outputs of previously evaluated BTFs. The first node in each line of the network file is the receiving node, and explains why the lowest label is 5 in our example.

The numbers in the third column are the edge-weights. The program train we will use to learn the multiplication facts ignores these numbers. Still, we can check that the weight normalization is correct. The BTF at node 5 has five inputs with weights that have squared norm 5, etc. Moreover, each node has either 1 or 3 nonzero weights. The andorprob argument (.5) of layered was used to set the probability of the case 3. When that case is used by layered, the three weights are given equal magnitudes and random signs. Also, since the BTF in that case always receives input from the bias node 0, the weights created by layered correspond to a random circuit of binary And/Or/Not gates. In a large network with setting andorprob = .5, about half of the nodes act as And/Or gates taking inputs from a random pair of nodes in the layer below. The other nodes, with a single nonzero weight, just move Boolean variables (or their negation) between adjacent layers.
