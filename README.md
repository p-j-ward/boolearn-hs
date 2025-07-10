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

The positive number $\delta$ is the *margin parameter*. Large $\delta$ is more restrictive, and when sufficiently large, boolnets can represent arbitrary Boolean circuits comprised of binary AND/OR gates as well as NOT. Because weights of negative sign flip the truth value of the corresponding input, they are the NOT gates in the circuit.

To better understand the Boolean circuit correspondence and its generalization, consider a boolnet that has been trained so the margin constraint, with some $\delta$, is satisfied at all the BTFs and over all the training data. By adjusting the weights at each BTF we can then increase $\delta$, individually for each BTF, without changing the Boolean function it implements. When $\delta$ is maximized, the weight vector has the form $(\sqrt{m/n})\mathbf{w}$, where $\mathbf{w}$ has a support $s$ of coprime integers and squared norm $\mathbf{w}\cdot \mathbf{w}=n$, and there are $s$ linearly independent Boolean $x$ that satsify $|\mathbf{w}\cdot x|=1$. This $s=3$ example,

$w=(\sqrt{m/3}) (\pm 1, \pm 1, \pm 1, 0, \ldots, 0)$ ,

(and its permutations) has maximized margin $\delta=\sqrt{m/3}$. More generally, the smallest $n(s)=\mathbf{w}\cdot \mathbf{w}$, when the integer vector $\mathbf{w}$ has support $s$, is $n(s)=s$ for odd $s$ and $n(s)=s-1$ when $s$ is even. In practical terms, this means that if we impose $\delta=\sqrt{m/n}$ on all the BTFs of the boolnet for some odd $n$, then all the BTFs are equivalent to maximized/rationalized BTFs of the kind described with support at most $s=n$, when $n$ is odd. This assumes of course that all the BTFs see $s$ linearly independent input tuples during training.

Boolnets trained with margin $\delta=\sqrt{m/3}$ can represent arbitrary logic circuits comprised of binary AND/OR gates and NOT. That's because the rationalized $w$ shown above implements a majority gate and

$\mathrm{AND}(p,q) = \mathrm{MAJ}(0,p,q)\qquad \mathrm{OR}(p,q) = \mathrm{MAJ}(1,p,q)$ .

While these are sufficient to express arbitrary Boolean functions, the higher-support BTFs with smaller $\delta$ may offer more efficient representations. It's also important to bear in mind that the tight supports enabled by large $\delta$ get relaxed when BTFs do not see suffciently diverse inputs, as is often the case near the output-end of a network.

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

The numbers in the third column are the edge-weights. The program `train` we will use to learn the multiplication facts ignores these numbers. Still, we can check that the weight normalization is correct. The BTF at node `5` has five inputs with weights that have squared-norm 5, etc. Moreover, each node has either 1 or 3 nonzero weights. The `andorprob` argument (`.5`) of `layered` was used to set the probability of the case 3. When `layered` chooses (randomly) this case, the three weights are given equal magnitudes and random signs. Also, since the BTF in that case always receives input from the bias node `0`, the weights created by `layered` correspond to a random circuit of binary AND/OR/NOT gates. In a large network with setting `andorprob << .5`, about half of the nodes act as AND/OR gates taking inputs from a random pair of nodes in the layer below. The other nodes, with a single nonzero weight, just move Boolean values (or their negation) between adjacent layers.

## First training example: a binary multiplier

You've taken a look at the data and already constructed a network that at least has the correct number of input and output nodes. Will it be able to learn 16 multiplication facts? Of course not! The `2x2.net` you constructed has only 2 hidden nodes, and can therefore produce at most 4 distinct outputs (of the required 7). Construct a new `2x2.net`, as before, but with 3 layers and width 4 for both hidden layers:
```
3
4 4 4 4
```
The training algorithm takes many arguments:
```
./../src/train 
expected 11 arguments: netfile datafile items btfn beta gam epochs itermax gapstop trials id
```
The argument names exactly match the variable names in the C program. Here's what they are asking for:
* `netfile` << `2x2.net`
* `datafile` << `2x2.dat`
* `items` << `16` (number of data you wish to train with)
* `btfn` << `3.` (sets the margin via the squared norm of the rationalized BTF weight vector, as described above)
* `beta` << `.2` (time-step value of the RRR constraint satisfaction algorithm)
* `gam` << `1e-3`(RRR metric auto-tuning parameter)
* `epochs` << 10 (maximum number of lines written to the file for monitoring progress)
* `itermax` << `1e4` (maximum number of RRR iterations)
* `gapstop` << `.01` (RRR termination criterion)
* `trials` << `1` (increase this when compiling statistics from multiple runs)
* `id` << `test` (name for all the files generated by `train`)

Because of the last argument, `train` will generate the following output files:
* `test.cmd` (your command line)
* `test.gap` (file for monitoring progress)
* `test.run` (statistics when doing multiple runs)
* `test.sol` (the solution weights)

The file whose contents you will be following closely, even as `train` is running, is the gap file, `test.gap`. For real-time reporting add an `&` at the end of your command line to put the job in the background. By doing
```
more test.gap
```
you will be able to follow the progress in long runs. The 2-bit binary multiplier application is so easy, `train` will have terminated by the time you get around to doing that. The complete gap file should look something like this:
```
         3    0.63613587    0.55792252    0.64555257    2.00700131    2.00700131    62.500 %     0.000 %
         7    0.28917853    0.43911374    0.46978192    1.28756702    1.28756702    64.062 %     0.000 %
        16    0.15349519    0.31768980    0.37626865    0.87848773    0.87848773    65.625 %     0.000 %
        40    0.12347367    0.18289424    0.30765258    0.59205003    0.56630854    73.438 %     0.000 %
       101    0.12159740    0.17687411    0.19909587    0.52520880    0.35032956    87.500 %     0.000 %
       252    0.05780539    0.10665988    0.10044701    0.29373600    0.23009609    84.375 %     0.000 %
       631    0.05448956    0.09306639    0.08987662    0.25398445    0.15553896    95.312 %     0.000 %
      1585    0.05765410    0.09512908    0.12523502    0.28574610    0.12876745    93.750 %     0.000 %
      3982    0.00343907    0.00408129    0.00714088    0.01291458    0.01001133   100.000 %     0.000 %
      4026    0.00221133    0.00248447    0.00637175    0.00984432    0.00984432   100.000 %     0.000 %
```
All the randomness in RRR comes at the start, when the weights ($w$), inputs ($x$), and outputs ($y$), are initialized. In `train` these are as random as possible: all are uniformly sampled between -1 and +1. Your `test.gap` will differ because of this.

The first column gives the RRR iteration count. If a solution had not been found after $10^4$ iterations (the setting for `itermax`) the last line would have had iteration count `10000`. In this run `train` terminated at iteration `4026`, because the lowest gap, shown in column 6, had reached the value of `gapstop` (`0.00984432 < .01`). We see that the training accuracy, shown in column 7, reaches 100 % a few iterations earlier. The generalization accuracy, in the last column, is shown as 0 % because all 16 multiplication facts were used in training, leaving none to test generalization.

Columns 2, 3, 4 show the gap broken down into contributions from the three variable types (in alphabetical order: $w$, $x$, $y$). These have a root-mean-square (rms) interpretation. For example, the `0.63613587` at the top of the second column means that in iteration 3, this was the rms difference in the constraint $A$ and constraint $B$ projections of the weights. To put this number in context, recall that the rms value of the weights is 1 by our normalization choice. By iteration 4026 this number has decreased to `0.00221133`. The evolution of columns 3 and 4 is similar, as is the context ($x$ and $y$ are valued $\pm$ 1). Column 5 is an aggregate rms gap at each BTF, where contributions from $w$, $x$, $y$ are combined. Column 6 is simply the lowest aggregate gap achieved.

In all cases (columns 2-6), when the rms gap goes to zero it means the $A$ and $B$ projections are nearly the same, or that a variable assignment has been found that satisfies both constraints. `train` uses the *divide-and-concur* constraint formulation where $A$ represents all the independent constraints at each BTF and each data item, while $B$ is responsible for concur, or the matching of one BTF's output ($y$) with another BTF's input ($x$), and the matching of the weights ($w$) across data items.

Entries to the gap file are added on a logarithmic scale, that is, the iteration count grows by a constant factor from one line to the next (`train` computes this factor from the `epochs` argument). Condensing the evolution to 10 or 50 lines of output makes it easier to digest the progress. We will see convergence to zero gap on a logarithmic time scale also in the more challenging applications.

The solution weights are written to a file that has exactly the same format as the network file we created with `layered`. The only difference is that the weights in the third column are those determined by a constraint satisfaction algorithm. Here is the top of `test.sol`:
```
17  5  4  60
    5    0   -1.28983434
    5    1   -0.00200875
    5    2   -1.29054248
    5    3    0.01983954
    5    4   -1.29245110
    6    0    1.29876608
    6    1    0.00954843
    6    2    1.29683942
    6    3    1.27723133
    6    4    0.00177617
    7    0   -1.29377298
    7    1   -1.28797003
    7    2   -0.00451735
    7    3    0.01376609
    7    4   -1.29115250

etc.
```
We see that the BTFs at nodes 5, 6, and 7 (the only ones shown) are implementing OR gates. The small weights have no effect on the BTF outputs, and the near equality (in magnitude) of the others implies they will act exactly as MAJ gates. In all three, one of the nonzero weights is to the bias input (node 0). The BTF at node 6 is therefore an OR, and by DeMorgan's rule, the BTFs at nodes 5 and 7 are AND gates. That's because every BTF $f(x)$ is self-dual, or $f(x)=-f(-x)$, so that

$\neg \mathrm{OR}(\neg p,\neg q)=\neg \mathrm{MAJ}(\neg 0,\neg p,\neg q)=\mathrm{MAJ}(0,p,q)==\mathrm{AND}(p,q)$.

Two kinds of symmetry act on the hidden nodes of layered boolnets. First, the nodes within each layer may be freely permuted (without changing the completely-connected, layered architecture). Second, because of self-duality, a boolnet's output is unchanged when the signs of all the incoming and outgoing weights, on any of the hidden BTFs, are flipped. This means that even when you expect unique solutions (say for a multiplier circuit), the weights in the solution file can and will come out differently from run to run because of symmetries. `train` calls the C time function to set the seed of the random number generator used for initialization.

Now that you know the basics of using `train`, there are many things to try, even for simple multiplier circuits. Try learning the 3-bit multiplication table, by constructing a circuit of only MAJ gates with the setting `btfn << 3.` . The 64 multiplication facts are in the file `3x3.dat`. What are good candidates for the layered architecture? Wider layers, greater depth, both? After you succeed, you can try training on just a subset of the data, by setting `items` to a number less than 64. Now the generalization accuracy, in the last column of the gap file, will not be 0 %. Don't be disappointed when you find that you need to train on nearly all the data to get 100 % on the unseen multiplication facts. The complexity of multipier circuits grows almost as fast as the number of data. The right way to learn to generalize on binary multiplication is to put arbitrary-bit-length number representation into the model - something well beyond the scope of this tutorial! Generalizing in a different direction, experiment with larger values of `btfn`. Recall that larger squared-norms correspond to smaller margins, or less constrained, higher information-capacity networks. You will find that with `btfn << 5.` there exists a multiplier cicuit with only one hidden layer of 5 nodes. From the solution file you will see that the BTFs are now implementing both 3-input and 5-input majority gates. Because `btfn` is only used for setting the margin, it is not required to be an integer. Fractional values are fine.

## Scaling of the compute-time

In several of the next applications the time used by `train` will be much longer than it was for the multiplier circuits. The algorithm can be highly parallelized, but this implementation is serial and runs on a single core. This has one advantage, in that the compute-time has the following simple scaling rule:

time $\propto$ `edges` $\times$ `items` $\times$ `itermax`


## Revisiting the dawn of ML: the binary autoencoder

Rumelhart, Hinton and Williams, in the famous "backpropagation" paper, were justifiably excited by the fact that their gradient descent algorithm was able to find weights that allowed information to pass through a bottleneck. Their network, called an *autoencoder*, was challenged with mapping $2^n$ random vectors to themselves, from $2^n$ input nodes to $2^n$ output nodes, but through a layer of only $n$ hidden nodes. This is impossible without nonlinearity in the activation functions, and backpropagation's ability to solve the nonconvex optimization problem was a triumph. Because sigmoids were used for the activation functions, which saturate to the values 0 and 1, the authors remarked that the training had succeeded in encoding the random vectors into binary codewords, and then decoding these back to the original vectors. We are revisiting the binary autoencoder because the gradient-descent-trained networks do not in fact deliver that result: the codes (outputs of the $n$ sigmoids in the bottleneck) are *not* binary.

Since constraint-based methods were not well known in 1985, Rumelhart et al. can't be faulted for using gradient-based methods, where 0 and 1 lie in a continuum. Boolnets do not have that limitation, and by construction deliver true binary encodings/decodings if all the constraints can be satisfied. Move into directory `autocode` to try it out yourself.

Three sets of profoundly silly data files are provided, corresponding to $n=3, 4, 5$. We will experiment with `auto16.dat`, the top of which is copied below:
```16
2 16
2 16

1 1 1 1 0 1 1 0 1 1 1 0 0 1 0 0
1 1 1 1 0 1 1 0 1 1 1 0 0 1 0 0

0 1 0 0 0 0 0 0 1 1 1 0 0 0 1 1
0 1 0 0 0 0 0 0 1 1 1 0 0 0 1 1
```
Two of the 16 matching input/output vectors are shown. Rumelhart et al. used 1-hot vectors (and $2^3=8$). Our vectors are more random. A simple 2-layer architecture with layer-widths
```
2
16 4 16
```
works for 1-hot vectors. For our data we create a network `auto16.net` having additional layers in both the encoder and decoder stages:
```
4
16 16 4 16 16
```

We will use this application to demonstrate basic hyperparameter optimization techniques. `train`, and the RRR algorithm more generally, has two hyperparameters: `beta` ($\beta$) and `gam` ($\gamma$). $\beta$ is like the gradient-step or learning-rate parameter of gradient descent. However, because RRR is *not* approximating continuous dynamics, large values like $\beta=1$ make perfect sense. In fact, RRR is the "relaxation" of the $\beta=1$ algorithm, better known as the popular "Douglas-Rachford" algorithm for convex problems. Experiments have shown that small $\beta$ helps in nonconvex problems.

To see the effects of $\beta$, we launch two runs of `train` like this:
```
./../src/train auto16.net auto16.dat 16 3. .2 1e-4 10 1e5 .05 5 test1 &

./../src/train auto16.net auto16.dat 16 3. .5 1e-4 10 1e5 .05 5 test2 &
```
These differ only in the value of $\beta$, `.2` (`test1`) and `.5` (`test2`). Some of the arguments, like `gapstop`, have been changed from what we used in the multiplier application. A new feature is enabled by the setting `trials` << `5`. The results of 5 runs, differing only in the random initialization, are summarized in the two run files. Here is `test1.run`:
```
   1       90291    0.04924928   100.000 %     0.000 %
   2       73029    0.04878647   100.000 %     0.000 %
   3       57484    0.04985247   100.000 %     0.000 %
   4       37528    0.04934596    99.219 %     0.000 %
   5       17242    0.04877118   100.000 %     0.000 %

5 / 5 solutions   55114.800000 +/- 11502.245400 iterations/solution
```
Compare this with `test2.run`, the experiments with the *larger* $\beta$ :
```
   1           0    0.18900071   100.000 %     0.000 %
   2           0    0.17517734    99.609 %     0.000 %
   3           0    0.25684861    95.312 %     0.000 %
   4           0    0.22922137    97.656 %     0.000 %
   5           0    0.30500119    93.359 %     0.000 %

 no solutions found
```
Without knowing what the columns mean, we see that smaller $\beta$ is better. All five runs in `test1` completed because the gap, shown in column 3, went below `gapstop`. The iteration counts are given in column 2. As in gap files, the two columns at the far right give the accuracies, "training" followed by "generalization" (0 % here because we trained on all the data). Notice that the training accuracy in run 4 is not perfect, even though the `gapstop` termination criterion was achieved. This can be fixed by decreasing `gapstop`, which will also require increasing `itermax`. The iteration count for non-solution runs, as in `test2`, are recorded as 0.

The same approach can be used to find a good value for the metric auto-tuning parameter $\gamma$. The projections to constraints $A$ and $B$ at the core of the RRR algorithm minimize distances to $A$ and $B$, seen as sets in Euclidean space. Initially it's fine to use the same metric coefficients on all the variables when computing projections. However, over the course of a run it will usually be the case that some BTF, or some BTF faced with a particular training item, is more frustrated than its cohort in closing the gap - the distance between the $A$, $B$ projections of its variables. Since local optimizations often fail in nonconvex problems, the $\gamma$ hyperparameter was introduced to allow the metric coefficients to respond to gap heterogeneity. It's a heuristic that stiffens the metric on variables with a large gap, while doing the opposite with the small-gap variables, thereby shifting the job of constraint satisfaction to variables that haven't been changing much (and have small gaps). $\gamma$ sets the rate at which the metric relaxes to the nonuniform form derived from the gaps. It should be set at a small value, so as not to upset the local convergence properties of RRR. A good rule of thumb is to use a value such that `gam` $\times$ `itermax` is of order 1.

## Decoding and generative models

The decoder-half of an autoencoder is a primitive *generative model* : from just a few bits of input, arbitrarily complex (perhaps even interesting!) data vectors (text, images) can be conjured up. As a warm-up, we'll try `train` on some small random vectors, like those in the autoencoder experiments. After some experience with that, we'll move on to some images of the number 4 from the internet.

In the directory `decode` you will find the file `rand16.dat`:
```
16
2 0
2 16

0 0 0 0 1 0 0 1 1 1 1 1 1 1 0 1

0 0 1 1 0 0 1 1 0 1 1 0 1 0 0 1

etc.
```
Shown above are just two of the 16 data items (random vectors). The `2 0` and `2 16` in the header tells `train` there is no data to constrain the network inputs and there are 16 binary data that constrain the outputs (the random vectors). Thanks to the constraint formulation, `train` can work with missing inputs in much the same way it works with known inputs. For the $A$ constraint, instead of projecting node variables $y$ at the input nodes to known values ($-1$ or $+1$), the projection replaces each $y$ by *either* $-1$ or $+1$, whichever is closer (has the shorter projection distance). Everything else (projections to all the BTF constraints, concur) is unchanged.

Recalling what we found in the autoencoder experiments, it should be possible to construct a boolnet that decodes 4 bits into 16 random vectors with the following layered architecture:
```
2
4 16 16
```
Using the `rand16.net` constructed by `layered` with this width file, we run `train` as in the other examples:
```
./../src/train rand16.net rand16.dat 16 3. .5 1e-4 10 1e6 .01 1 rand16 &
```
Here is `rand16.gap`:
```
         4    0.13575713    0.22147336    0.68988313    1.08057431    1.08057431     0.000 %     0.000 %
        16    0.10156869    0.19300126    0.59327046    0.91103500    0.91103500     0.000 %     0.000 %
        64    0.07241161    0.11791873    0.46760830    0.64507023    0.61210050     0.000 %     0.000 %
       252    0.04127719    0.05865468    0.24503162    0.32627824    0.32627824     0.000 %     0.000 %
      1001    0.03505607    0.05050834    0.13844674    0.24742913    0.20913804     0.000 %     0.000 %
      3982    0.04201491    0.06611897    0.15508586    0.30804065    0.15949651     0.000 %     0.000 %
     15849    0.03484822    0.05317678    0.09555559    0.23383975    0.15949651     0.000 %     0.000 %
     63096    0.03416997    0.05492065    0.12664713    0.24905679    0.14358508     0.000 %     0.000 %
     76136    0.00183625    0.00168834    0.00602704    0.00998458    0.00998458     0.000 %     0.000 %
```
The small final gap leaves no doubt that a true decoder network has been found, even when that can't be confirmed by the accuracies (rightmost columns) when there is no input data. However, `train` provides a check by writing generator files when there is no input data. Because we used `rand16` for `id`, the generator file is `rand16.gen`. Here is how it begins:
```
16  4

 0 0 0 0	1
 1 0 0 0	1
 0 1 0 0	1
 1 1 0 0	1
 0 0 1 0	1

etc.
```
Since our network architecture specified 4 input/code bits, each line has a binary assignment to those bits. This is followed by the number of times the assignment was used in generating the data. Because the data comprised $2^4$ distinct vectros, all $2^4$ assignments had to be used (exactly once). `train` lists them as increasing binary numbers.

The most direct way to check our decoder is with the synthetic data generating program `data`. Here is what we get when we query its arguments:
```
./../src/data
expected 4 arguments: netfile genfile items id
```
For `netfile` we should *not* use `rand16.net`, since `layered` created that file with random weights (that did not get used by `train`). Instead, we should use `rand16.sol`: the same network but with solution weights. The next argument for `data` is the generator file that goes with the solution, `rand16.gen`. The argument `items` is needed when we want `data` to generate synthetic data (`items` in number) by sampling random inputs. Since here the file `rand16.gen` provides all the inputs, we tell `data` to use that instead with the setting 0 for this argument. The command
```
./../src/data rand16.sol rand16.gen 0 rand16+inputs
```
creates the data file `rand16+inputs.dat` :
```
16
2 4
2 16

 0 0 0 0
 0 0 0 0 1 0 0 0 1 0 1 1 0 1 0 1

 1 0 0 0
 1 0 0 1 1 1 0 1 1 1 0 1 1 0 1 0

 0 1 0 0
 1 1 1 1 1 1 0 1 1 1 1 1 0 0 1 1

 1 1 0 0
 1 1 1 0 1 1 0 1 1 0 0 0 1 0 1 1

 0 0 1 0
 0 0 0 0 1 0 0 1 1 1 1 1 1 1 0 1

etc.
```
All the random vectors of `rand16.dat` appear somewhere in the list, now preceded by the decoder inputs. In generative models the symmetries that previously only applied to the hidden nodes also apply to the input nodes. That explains why the first random vector of `rand16.dat` can end up as the 5th item in `rand16+inputs.dat`.

## Classic classification: MNIST

Though boolearn was created for learning Boolean data, without modification it can also work with analog data. Go to the directory `mnist`. Here is what you will see at the top of the file `mnistjr.dat`:
```
3823
1	64
10	1

0.00	0.06	0.38	0.94	0.75	0.06	0.00	0.00	0.00	0.44	...
0

etc.
```
There are 3823 data items, and `1    64` means there are 64 inputs of type `1` (analog), that `train` reads as floating-point numbers. The `10    1` that follows means there is just a single 10-valued output. As you've surely guessed, each line of 64 floating-point numbers is the 8 $\times$ 8 grayscale image of some MNIST numeral, and the integer that follows is its class (0 - 9). This is a smaller and downsampled version of the original 28 $\times$ 28, 16-bit data set.

When `train` sees output-type `c    1` ($c > 2$) in the header, it replaces the output classes with $c$-component 1-hot vectors, the position of the 1 marking the class. The boolnet must therefore have $c$ output nodes. For `mnistjr.dat` we already get interesting results with the simple 2-layer network generated by the following width file:
```
2
64 64 10
```
Regarding the analog input, instead of converting `0`/`1` to -1/+1 (binary data), when the input type is `1`, `train` expects floating point numbers in the range [0, 1] and maps these linearly into the range [-1, +1]. Nothing changes in the constraint satisfaction algorithm aside from the node variables $y$ getting projected to continuous values at the input nodes (to the data values). The $y$ variables at all the other nodes continue to be Boolean.

MNIST is our first experiment with generalization. We train on just the first `N` data items with the command line
```
./../src/train mnistjr.net mnistjr.dat N 5. .2 1e-3 10 2e5 .05 1 mnistjr_N &
```
and compare results when `N` is 128, 256, and 512. Since the data is randomly ordered, when training with $N$ items the algorithm sees about $N/10$ examples of each kind of digit. Accuracies are the average of $h/n$, where $h=1$ if the Boolean output "hits" the correct class, $h=0$ otherwise, and $n$ is the number of 1's in the Boolean output (since the network's output is not automatically 1-hot).

We set the squared-BTF-norm parameter `btfn` equal to 5. instead of 3. to increase the network's information capacity. We also do not have any reason to believe the class of an image can be derived by simple logic applied to the pixels!

Here is the gap file for the first experiment, `mnistjr_128.gap`:
```
         4    0.56477771    0.30823892    0.78820497    5.24328543    5.24328543    11.558 %    11.534 %
        12    0.09648800    0.10657777    0.75997095    1.38984998    1.38984998    11.496 %    11.562 %
        39    0.02545053    0.11357696    0.57923384    1.17616036    1.11352254    15.326 %    13.250 %
       132    0.01937969    0.03810052    0.47594831    0.49728139    0.49728139    41.016 %    22.186 %
       448    0.01193183    0.03570415    0.12622912    0.34270020    0.30723177    87.109 %    58.378 %
      1516    0.00662057    0.01501151    0.05308913    0.14469507    0.11386118    98.438 %    66.434 %
      4704    0.00253154    0.00534298    0.03195912    0.04966517    0.04966517   100.000 %    64.116 %
```
We see that `gapstop << .05` was sufficient to achieve 100 % training accuracy. The generalization accuracy (last column) is quite poor, but not unreasonable considering the algorithm had seen only about 13 examples of each digit.

Here is `mnistjr_256.gap`:
```
         4    0.57101266    0.30858423    0.78821356    5.28799928    5.28799928    13.368 %    13.672 %
        12    0.09760769    0.10688701    0.76048299    1.39619607    1.39619607    13.462 %    13.785 %
        39    0.02494075    0.11312082    0.57755355    1.17121735    1.11203583    17.083 %    16.379 %
       132    0.02040713    0.03879405    0.48144063    0.51302355    0.50623700    29.766 %    23.613 %
       448    0.01313315    0.03713970    0.13869806    0.36377545    0.33968349    78.776 %    63.410 %
      1516    0.00820760    0.01884961    0.07209018    0.18372815    0.15938517    96.289 %    79.011 %
      5138    0.00488509    0.01342561    0.05341729    0.12208419    0.08948161   100.000 %    79.941 %
     17412    0.00339781    0.01012794    0.03774996    0.08981532    0.05537790   100.000 %    82.156 %
     21965    0.00238985    0.00504257    0.03423034    0.04697091    0.04697091   100.000 %    81.777 %
```
By doubling the number of examples, the generalization accuracy improved significantly. Here are the results of the last experiment, `mnistjr_512.gap`:
```
         4    0.57559547    0.30868422    0.79026226    5.32053817    5.32053817     9.500 %     9.622 %
        12    0.09836629    0.10700868    0.76313371    1.40087505    1.40087505     9.711 %     9.579 %
        39    0.02571347    0.11401144    0.58595867    1.18510394    1.11712214     6.342 %     8.063 %
       132    0.02236669    0.04057387    0.49177903    0.54179812    0.53477924    15.609 %    11.953 %
       448    0.01374754    0.03811167    0.14755095    0.37789494    0.35447807    77.897 %    68.793 %
      1516    0.00766799    0.01903549    0.07717108    0.18763667    0.17829736    95.703 %    87.277 %
      5138    0.00520713    0.01602103    0.06093098    0.14520066    0.12032428    97.070 %    88.332 %
     17412    0.00461367    0.01314067    0.05198234    0.12034325    0.09789872    99.219 %    90.496 %
     59011    0.00393315    0.01066006    0.04317845    0.09790033    0.06967737    99.219 %    88.750 %
    167957    0.00224249    0.00510358    0.04063471    0.04844574    0.04844574    98.535 %    90.063 %
```
Since now the training accuracy is struggling to reach 100 %, there is not much point in increasing the number of training examples. To improve generalization one has to improve the model.


## Generalization with synthetic data I: random AND/OR circuits

A critical (but little used) test of machine learning algorithms is training on models that can exactly represent the data. Generalization in this setting is simply the question, "How little data is sufficient to exactly reconstruct the model ?". We will test `train` on the task of reconstructing the weights of a boolnet from the data it generates and some general characteristics of its architecture. This exercise also demonstrates new features of the programs `layered` and `data`.

Go to the directory `randlogic`. You will not see any `.dat` files because you will make your own. Start by creating a boolnet using `3.wth`:
```
3
32 32 32 32
```
With the setting `andorprob << .5` in `layered`, all the BTFs are AND/OR gates with probability 1/2 and otherwise just copy a Boolean value from the layer below:
```
./../src/layered .5 3.wth 3.net
```
Negative weights correspond to NOT gates and are used with probabilty 1/2.

So far we have only used `data` with the setting `items << 0`, which is an instruction to generate as many data items as there are assignments in a `.gen` file. By setting `items << M` for some positive M, `data` will instead generate M data items by randomly sampling binary inputs to the boolnet. A dummy argument must be provided for the `.gen` file:
```
./../src/data 3.net dummy.gen 10000 3
```
Here is the top of the data file `3.dat`:
```
10000
2 32
2 32

 1 1 1 1 0 1 0 1 1 0 0 0 1 1 1 1 1 0 1 0 1 0 0 0 0 0 0 0 0 1 0 1
 0 0 0 0 0 1 1 0 1 1 0 1 0 0 1 0 0 0 1 1 1 0 0 0 0 0 0 1 0 0 1 0

 1 0 0 1 1 0 1 0 0 1 0 1 1 1 0 0 1 1 0 1 0 0 1 0 0 1 0 0 0 1 1 1
 0 0 1 0 1 1 0 0 0 0 0 0 1 0 1 0 1 1 0 0 0 1 0 0 0 0 0 1 1 1 0 1

etc.
```

To see how well `train` can learn to generalize, we run
```
./../src/train 3.net 3.dat N 3. .2 1e-3 20 1e6 .1 1 3_N &
```
with various numbers of training items `N`. Here is `3_64.gap`:
```
         2    0.87529153    0.48048943    0.82754204    5.78797802    5.78797802    51.123 %    50.553 %
         4    0.54679478    0.32646915    0.82125022    3.73990161    3.73990161    50.732 %    50.633 %
         8    0.22541255    0.17727525    0.81676621    1.82939005    1.82939005    50.977 %    50.604 %
        16    0.04757007    0.13271608    0.79204898    1.15017350    1.15017350    51.172 %    50.753 %
        32    0.04727803    0.13673757    0.67647838    1.12740468    1.12525797    52.539 %    51.523 %
        64    0.07060727    0.10585284    0.63656250    1.00003649    1.00003649    55.615 %    54.695 %
       126    0.05441507    0.08451410    0.50823741    0.81627446    0.80678849    68.701 %    59.454 %
       252    0.03549788    0.05583900    0.38084336    0.57289717    0.57289717    82.666 %    67.673 %
       502    0.02474204    0.04337193    0.22425908    0.38546762    0.35407650    96.582 %    73.690 %
      1001    0.01483602    0.02662393    0.11488884    0.21917124    0.17914247    99.902 %    75.119 %
      1996    0.01111909    0.02340811    0.07526052    0.17152670    0.11683637   100.000 %    75.168 %
      2624    0.00558595    0.01399110    0.04026800    0.09658450    0.09658450   100.000 %    75.262 %
```
While `train` had no trouble finding weights that explained the 64 training items, the generalization accuray is only about 75 %. Here is `3_128.gap`:
```
         2    0.89042781    0.48181004    0.82937318    5.86757642    5.86757642    51.245 %    49.735 %
         4    0.55461353    0.32735790    0.82137888    3.78018733    3.78018733    51.489 %    49.826 %
         8    0.22893335    0.17805455    0.81677223    1.84618540    1.84618540    51.489 %    49.746 %
        16    0.04866120    0.13433903    0.80324575    1.16777861    1.16777861    51.440 %    49.713 %
        32    0.04734262    0.13836873    0.68216808    1.13829920    1.13440710    52.246 %    49.860 %
        64    0.07140019    0.11011376    0.63841413    1.01848292    1.01848292    57.861 %    53.196 %
       126    0.05702851    0.08625804    0.50667320    0.82639642    0.82131521    66.943 %    59.595 %
       252    0.03904006    0.05914671    0.38959642    0.59821074    0.59821074    78.320 %    69.044 %
       502    0.02978486    0.04919612    0.26725811    0.45136591    0.44274693    90.332 %    77.394 %
      1001    0.02492949    0.04477638    0.20542979    0.37804571    0.35732688    96.777 %    80.582 %
      1996    0.02185683    0.04042680    0.16673781    0.32628430    0.30494001    99.048 %    81.242 %
      3982    0.02153710    0.03822944    0.15252195    0.30684743    0.26405269    99.658 %    81.065 %
      7944    0.02003797    0.03601045    0.14323283    0.28810483    0.25085647    99.902 %    81.056 %
     15849    0.01840440    0.03635869    0.13278433    0.27914639    0.23678951    99.902 %    80.858 %
     31623    0.01992024    0.03482909    0.12841078    0.27320948    0.21402439   100.000 %    81.523 %
     63096    0.01531696    0.02970551    0.11637412    0.23348392    0.18456477   100.000 %    83.000 %
    125893    0.01396139    0.02832132    0.10152249    0.21514271    0.16419973    99.976 %    83.946 %
    251189    0.01224530    0.02499934    0.08321597    0.18535536    0.14057288    99.951 %    84.692 %
    501188    0.01007479    0.02083242    0.06990876    0.15444499    0.11472015   100.000 %    84.967 %
    817382    0.00628811    0.01309673    0.04647657    0.09759826    0.09759826   100.000 %    86.923 %
```
Here too we see that the number of training items is insufficient to generalize. But here is `3_256.gap`:
```
         2    0.90229764    0.48171511    0.82805122    5.92674008    5.92674008    49.011 %    49.264 %
         4    0.56167745    0.32761093    0.82312405    3.81591423    3.81591423    48.865 %    49.336 %
         8    0.23178765    0.17806169    0.81794455    1.85916112    1.85916112    48.889 %    49.334 %
        16    0.04874372    0.13487024    0.80701672    1.17375651    1.17375651    48.792 %    49.410 %
        32    0.04825189    0.14111984    0.69291974    1.15945077    1.14734218    49.475 %    49.928 %
        64    0.07419989    0.11249898    0.64637198    1.04017244    1.04017244    54.016 %    53.587 %
       126    0.05925930    0.08736252    0.50413181    0.83347688    0.83347688    62.158 %    61.449 %
       252    0.04127459    0.06082053    0.39862955    0.61639720    0.61639720    73.853 %    70.664 %
       502    0.03355723    0.05405592    0.29702344    0.50103291    0.49331515    85.767 %    79.215 %
      1001    0.02890492    0.05167730    0.24474568    0.44234401    0.43905824    92.200 %    82.739 %
      1996    0.02881509    0.05188016    0.23198115    0.43344811    0.41265873    95.752 %    84.164 %
      3982    0.02684411    0.05026736    0.21162312    0.40810918    0.38686851    97.217 %    86.104 %
      7944    0.02477993    0.04832899    0.19561842    0.38447882    0.34898300    98.438 %    90.477 %
     15849    0.02111318    0.04318731    0.16399549    0.33359454    0.30434831    99.390 %    94.654 %
     31623    0.01427799    0.03405128    0.10935889    0.24304791    0.22754060    99.890 %    99.021 %
     63096    0.01039166    0.02910907    0.08487580    0.19670310    0.16421829    99.988 %    99.764 %
    125893    0.00964354    0.02567983    0.07714577    0.17519590    0.14028439    99.878 %    99.717 %
    251189    0.00905458    0.02478916    0.06987461    0.16330250    0.11430751    99.915 %    99.789 %
    304208    0.00585157    0.01410334    0.05066723    0.09620216    0.09620216    99.988 %    99.754 %
```
Both accuracies reach 100 % when a smaller setting for `gapstop` is used. If we repeat these experiments with another set of random gates (on the same architecture), the results will be the same: a transition to perfect generalization when the number of training items reaches 256.


## Generalization with synthetic data II: cellular automata

While 2-input AND/OR with NOT can express any Boolean function, the random circuits created by `layered` may not be your first choice to study generalization. The program `data_cellauto` offers an alternative. This generates random binary input strings and the output string that results when some number of steps of a 1D cellular automaton rule is applied to the input. Here is what you get when you query for the arguments:
```
./../src/data_cellauto 
expected 6 arguments: inbits rule size steps items id
```
`inbits` is the number of input bits and `rule` is the Wolfram encoding of the rule that gets applied to the input bits. For example, `inbits << 3` and `rule << 232` is the majority rule applied to three adjacent bits to determine the next center bit (one time step). `size` is the 1D (periodic) extent of the automaton and `steps` is the number of time steps between the input and output strings. `items` is the number of data, each one generated from a random sample of the inputs. `id` is the name of the data file, before `.dat` is appended.

The difficulty of reconstructing automaton rules varies greatly. Let's start with an easy one:
```
./../src/data_cellauto 3 232 32 3 10000 232_3
```
This generates 3-step data for the "majority" rule on a 1D (periodic) world of 32 cells. Here is the top of `232_3.dat`
```
10000
2 32
2 32

 1 1 1 1 0 0 1 1 1 1 0 1 0 0 1 1 1 0 0 1 0 1 0 0 1 0 0 0 1 0 0 1
 1 1 1 1 0 0 1 1 1 1 1 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1

 0 1 0 0 1 1 0 1 0 0 0 1 0 1 0 0 1 1 1 1 0 1 1 1 0 0 1 1 0 1 0 0
 0 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 1 1 1 1 1 1 1 1 0 0 1 1 1 0 0 0

etc.
```
Since we know that rule-232 is just a 3-input majority gate, to represent 3 time-step data a boolnet (`232_3.net`) with width file `232_3.wth`
```
3
32 32 32 32
```
is sufficient, even when setting ``btfn << 3.` in `train`:
```
./../src/train 232_3.net 232_3.dat 256 3. .2 1e-3 10 1e6 .1 1 232_3_256 &
```
Here we are training on 256 data. From `232_3_256.gap` we see that generalization is near perfect:
```
         4    0.56131819    0.32791040    0.82291499    3.81506389    3.81506389    49.829 %    49.843 %
        16    0.04849535    0.13484975    0.80913955    1.17567319    1.17567319    49.695 %    49.964 %
        64    0.07296111    0.11078892    0.64275129    1.02835360    1.02835360    53.320 %    52.239 %
       252    0.04491122    0.06456939    0.41082153    0.64735119    0.63913453    71.570 %    67.581 %
      1001    0.03420579    0.05704794    0.27947821    0.50041807    0.48215516    87.744 %    76.278 %
      3982    0.03015137    0.05452534    0.24073174    0.45304484    0.43162546    93.005 %    78.511 %
     15849    0.02730792    0.05230340    0.22161633    0.42427911    0.39984982    95.935 %    81.341 %
     63096    0.01686062    0.03850565    0.13269482    0.28437433    0.26912555    98.474 %    92.050 %
    109932    0.00577884    0.01366643    0.04564975    0.09992491    0.09992491    99.902 %    98.266 %
```
If you need more convincing that `train` has truly reconstructed rule-232, take a look at the top of `232_3_256.sol`:
```
129  33  32  3168
   33    0   -0.00491884
   33    1    0.00725799
   33    2    0.01029105
   33    3   -0.01294057
   33    4   -0.06124797
   33    5   -0.04233782
   33    6   -0.04341197
   33    7   -0.04459403
   33    8   -0.00804814
   33    9    0.05470183
   33   10    0.07238804
   33   11    0.01162374
   33   12    0.03966960
   33   13    0.00055208
   33   14   -0.02863902
   33   15   -0.01628740
   33   16    0.00023360
   33   17    0.04007809
   33   18   -0.00619625
   33   19   -0.04576495
   33   20   -0.00141638
   33   21    3.29598814
   33   22    3.30508793
   33   23    3.34254136
   33   24    0.06310564
   33   25    0.05824260
   33   26    0.02588341
   33   27    0.00033820
   33   28   -0.03006870
   33   29   -0.00410884
   33   30   -0.05914210
   33   31    0.04243144
   33   32   -0.03754063

etc.
```
This shows all the weights going to node 33, the first hidden node. It shouldn't be a mystery why all except three of these, from the input nodes 1-32, are close to zero. The nonzero weights, from consecutive nodes, are nearly equal and implement a 3-input majority gate - one step of the automaton. Note that node 33 is *not* required to hold the successor of node 1 because the roles of the hidden nodes have a layer-wise permutation symmetry. The permutation selected in a solution is determined by the random initialization of `train`. In this solution node 33 holds the successor of node 22.

A harder rule to reconstruct is Wolfram's "chaos" rule with code 30:
```
./../src/data_cellauto 3 30 16 1 10000 30_1
```
Because it's harder, we decrease the size to 16 and time steps to 1. This rule has formula $q_{t+1}=p_t\oplus (q_t \lor r_t)$, where $p,q,r$ are consecutive cells and $\oplus$ / $\lor$ are exclusive/regular OR. To compute this with 2-input AND/OR and NOT one needs *two* intermediate stages, each holding two bits. The first intermediate stage holds $p$ and the OR of $q$ and $r$, which we call $s$. The second stage holds the AND of $p$ and $\neg s$ in one bit, the AND of $\neg p$ and $s$ in the other bit. The OR of the bits in the second stage is the exclusive OR of $p$ and $s$. This should convince you that to implement just one time step of the rule-30 automaton, in a 16-cell world, we need a boolnet `30_1.net` with the following width specification:
```
3
16 32 32 16
```
Training on 256 data items,
```
./../src/train 30_1.net 30_1.dat 256 3. .2 1e-3 10 1e6 .1 1 30_1_256 &
```
results in the following gap file, `30_1_256.gap`:
```
         4    0.56121250    0.33602990    0.84513041    3.47287712    3.47287712    52.148 %    52.398 %
        16    0.04911581    0.14486534    0.81501924    1.14732108    1.14732108    52.100 %    52.521 %
        64    0.08313015    0.12538073    0.69099621    1.06406108    1.06406108    53.857 %    54.032 %
       252    0.05359075    0.07538043    0.42371195    0.66054043    0.66054043    64.014 %    60.276 %
      1001    0.04587595    0.07252703    0.33393721    0.57492250    0.56901972    73.584 %    61.376 %
      3982    0.04389475    0.07187849    0.32025610    0.55890084    0.52774508    82.837 %    69.404 %
     15849    0.02640796    0.05012771    0.18270749    0.35371384    0.33102048    97.363 %    94.154 %
     63096    0.01636335    0.03361414    0.12138043    0.23419192    0.19729820    99.121 %    98.423 %
    251189    0.01164226    0.02921470    0.09163289    0.19062306    0.14375005    99.561 %    99.309 %
    755382    0.00549876    0.01577849    0.04568708    0.09834139    0.09834139    99.878 %    99.659 %
```
Keeping in mind the layer-wise permutation symmetry of solutions, `30_1_256.sol` will convince you that `train` has reconstructed a circuit that fits the above description perfectly. Unlike the rule-232 example that needed only the 3-input majority gate and zero bias weights (from node 0), in the rule-30 solution you will see that in all the BTFs that have three nonzero weights, one is to node 0 (because they are implementing 2-input AND and OR). It is remarkable that `train` is able to find this solution given only the "hint" that there is a solution with `btfn << 3`! Previous work on learning automaton rules with neural networks relied heavily on a convolutional structure for the weights, which was absent in this demonstration.
