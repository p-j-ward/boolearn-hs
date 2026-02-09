---
title: Cellular automata
parent: Applications
nav_order: 8
---

## Generalization with synthetic data II: cellular automata

While 2-input AND/OR with NOT can express any Boolean function, the random circuits created by `layered` may not be your first choice to study generalization. The program `data_cellauto` offers an alternative. This generates random binary input strings and the output string that results when some number of steps of a 1D cellular automaton rule is applied to the input. Here is what you get when you query for the arguments:
```
./../src/data_cellauto 
expected 6 arguments: inbits rule size steps items id
```
`inbits` is the number of input bits and `rule` is the Wolfram encoding of the rule that gets applied to the input bits. For example, `inbits` << `3` and `rule` << `232` is the majority rule applied to three adjacent bits to determine the next center bit (one time step). `size` is the 1D (periodic) extent of the automaton and `steps` is the number of time steps between the input and output strings. `items` is the number of data, each one generated from a random sample of the inputs. `id` is the name of the data file, before `.dat` is appended.

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
is sufficient, even when setting `btfn` << `3.` in `train`:
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
Because it's harder, we decrease the size to 16 and time steps to 1. This rule has formula $q_{t+1}=p_t\oplus (q_t \lor r_t)$, where $p,q,r$ are consecutive cells and $\oplus$ / $\lor$ are exclusive/regular OR. To compute this with 2-input AND/OR and NOT one needs *two* intermediate stages, each holding *two* bits. The first intermediate stage holds $p$ and the OR of $q$ and $r$, which we call $s$. The second stage holds the AND of $p$ and $\neg s$ in one bit, the AND of $\neg p$ and $s$ in the other bit. The OR of the bits in the second stage is the exclusive OR of $p$ and $s$. This should convince you that to implement just one time step of the rule-30 automaton, in a 16-cell world, we need a boolnet `30_1.net` with the following width specification:
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
Keeping in mind the layer-wise permutation symmetry of solutions, `30_1_256.sol` will convince you that `train` has reconstructed a circuit that fits the above description perfectly. Unlike the rule-232 example that needed only the 3-input majority gate and zero bias weights (from node 0), in the rule-30 solution you will see that in all the BTFs that have three nonzero weights, one is to node 0 (because they are implementing 2-input AND and OR). It is remarkable that `train` is able to find this solution given only the "hint" that there is a solution with `btfn` << `3`! Previous work on learning automaton rules with neural networks relied heavily on a convolutional structure for the weights, which was absent in this demonstration.
