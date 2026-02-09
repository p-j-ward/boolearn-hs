---
title: Binary autoencoder
parent: Applications
nav_order: 4
---

## Revisiting the dawn of ML: the binary autoencoder

Rumelhart, Hinton and Williams, in the famous "backpropagation" paper, were justifiably excited by the fact that their gradient descent algorithm was able to find weights that allowed information to pass through a bottleneck. Their network, called an *autoencoder*, was challenged with mapping $2^k$ random vectors to themselves, from $2^k$ input nodes to $2^k$ output nodes, but through a layer of only $k$ hidden nodes. This is impossible without nonlinearity in the activation functions, and backpropagation's ability to solve the nonconvex optimization problem was a triumph. Because sigmoids were used for the activation functions, which saturate to the values 0 and 1, the authors remarked that the training had succeeded in encoding the random vectors into binary codewords, and then decoding these back to the original vectors. We are revisiting the binary autoencoder because the gradient-descent-trained networks do *not* in fact deliver that result: the codes (outputs of the $k$ sigmoids in the bottleneck) are *not* binary.

Since constraint-based methods were not well known in 1985, Rumelhart et al. can't be faulted for using gradient-based methods, where 0 and 1 lie in a continuum. Boolnets do not have that limitation, and by construction deliver true binary encodings/decodings if all the constraints are satisfied. Move into directory `autocode` to try it out yourself.

Three sets of profoundly silly data files are provided, corresponding to $k=3, 4, 5$. We will experiment with `auto16.dat`, the top of which is copied below:
```16
2 16
2 16

1 1 1 1 0 1 1 0 1 1 1 0 0 1 0 0
1 1 1 1 0 1 1 0 1 1 1 0 0 1 0 0

0 1 0 0 0 0 0 0 1 1 1 0 0 0 1 1
0 1 0 0 0 0 0 0 1 1 1 0 0 0 1 1

etc.
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
These differ only in the value of $\beta$, `.2` (`test1`) and `.5` (`test2`). Some of the arguments, like `gapstop`, have been changed from what we used in the multiplier application. A new feature is enabled by the setting `trials` << `5`. The results of 5 runs, differing only in the random initialization, are summarized in the two "run files". Here is `test1.run`:
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

Smallness of the gap, like smallness of the loss (in gradient methods), does not translate directly to high training accuracy. Still, thanks to the interpretability of the gap (from the nice normalizations of the variables as discussed above), the relationship is quite good. You will find that bringing the gap below 0.1 will in most cases result in 100 % training accuracy, or something very close to that.

The same multi-run approach can be used to find a good value for the metric auto-tuning parameter $\gamma$. The projections to constraints $A$ and $B$ at the core of the RRR algorithm minimize distances to $A$ and $B$, seen as sets in Euclidean space. Initially it's fine to use the same metric coefficients on all the variables when computing projections. However, over the course of a run it will usually be the case that some BTF, or some BTF faced with a particular training item, is more frustrated than its cohort in closing the gap - the distance between the $A$, $B$ projections of its variables. Since local optimizations often fail in nonconvex problems, the $\gamma$ hyperparameter was introduced to allow the metric coefficients to respond to gap heterogeneity. It's a heuristic that stiffens the metric on variables with a large gap, while doing the opposite with the small-gap variables, thereby shifting the job of constraint satisfaction to variables that haven't been changing much (and have small gaps). $\gamma$ sets the rate at which the metric relaxes to the nonuniform form derived from the gaps. It should be set at a small value, so as not to upset the local convergence properties of RRR. A good rule of thumb is to use a value such that `gam` $\times$ `itermax` is of order 1.
