# Build process

Just run make in the `src/` directory:
```
$ cd src
$ make
```
The default compiler is `clang` if you wish to use `gcc` run:
```
$ make CC=gcc
```

# Examples

The code is simple, does not provide many options.
It will always just count and list preimages for the given configuration or it will crash.

This will print a list of preimages for a binary CA with a **quad** neighborhood and a random rule `0x6996`:

```
./ca2d_preimages 2 2 2 0x6996 8 8 test_2x2.cas
```

This will print a list of 19804 preimages for a 3x3 configuration containing a *glider*,
the Given neighborhood and rule are for **Game of Life**:
```
./ca2d 2 3 3 0x100010001000101170116000100010117011601170116177E1668000100010117011601170116177E166801170116177E1668177E16687EE86880 3 3 gol_glider_3x3.cas
```

This one will crash, due to memory constraints:

```
./ca2d 2 3 3 0x100010001000101170116000100010117011601170116177E1668000100010117011601170116177E166801170116177E1668177E16687EE86880 10 10 gol_eden1.cas
```

# Example configurations

| file name          | siz.y | siz.x | url
|--------------------|-------|-------|------------------------------------------
| gol_goe_21x6.cas   |    21 |     6 | GoE/orphan http://wwwhomes.uni-bielefeld.de/achim/icons/orphan_6x21_125_81.gif
| gol_glider_3x3.cas |     3 |     3 | GoL glider
| gol_glider_8x8.cas |     8 |     8 | GoL glider
| one.cas            |     8 |     8 | all cells are 1 (alive in GoL terms)
| zero.cas           |     8 |     8 | all cells are 0 (dead in GoL terms)
| space_invader.cas  |    13 |    10 | Space Invader https://www.youtube.com/watch?v=IJ9dDZQmJhg

gol_eden1.cas
gol_eden_neg.cas
test1.cas
test.cas
