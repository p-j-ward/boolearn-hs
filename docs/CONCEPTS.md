# Concepts

This doc is about the *ideas* behind Boolearn: boolean threshold functions, margins, and why training is framed as **constraint satisfaction** (tracked by a “gap”) instead of minimizing a loss.

## TL;DR

- Booleans are represented as **±1** (`True/1` → +1, `False/0` → −1).
- Each node is a **Boolean threshold function (BTF)**: $y = \mathrm{sgn}(w\cdot x)$.
- Boolearn treats this as a **constraint** and “repairs” violated constraints by minimal changes.
- A **margin** constraint $|w\cdot x| \ge \delta$ helps avoid ambiguous / low-confidence decisions.
- The `btfn` hyperparameter is used to set margins via the *BTF-norm* idea described below.

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


## Where next?

- If you want to run stuff → [USER_GUIDE.md](USER_GUIDE.md)
- If you want worked tutorials → [EXAMPLES.md](EXAMPLES.md)
