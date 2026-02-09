# Contributing

Thanks for taking a look.

## Quick dev loop

```bash
cd boolearn/src
make all
```

Run an example (multiplier):

```bash
cd ../mult
../src/layered .5 2x2.wth 2x2.net
../src/train 2x2.net 2x2.dat 16 3 0.2 1e-3 10 1e4 0.01 1 run1
```

## Style & expectations

- Keep the CLI interface stable whenever possible (argument order matters).
- Prefer small, readable changes over sweeping refactors.
- If you add a new “application”, please add a short page under `docs/applications/`.
