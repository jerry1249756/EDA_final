# EDA_final



## File structure
```
src/
|  Annealing.cpp
|  Annealing.h
|  enum.h
|  extern.h
|  FM_alg.cpp
|  FM_alg.h
|  Kraftwerk2.cpp
|  Kraftwerk2.h
|  LA.cpp
|  LA.h
|  Legalizaion.cpp
|  Legalization.h
|  main.cpp
|  module.cpp
|  module.h
|  Neighborood.cpp
|  Neighborood.h
|  partition.cpp
|  partition.h
|  place_terminal.cpp
|  place_terminal.h
bin/
input.txt
README.md
Problem B_evaluator_0525
makefile
```

## How to compile
(1) Install relative library:
Go to
```
https://eigen.tuxfamily.org/index.php?title=Main_Page
```
to download the latest stable release of Eigen library. Rename the folder as ```Eigen``` and place it to ```/usr/local/include```.

(2) Simply type 
```
make
```
in the Linux terminal.

The  .o files and the binary would be created.

## How to run
Create a blank ```output.txt``` in the root folder and type
```
./bin/partition ./input.txt ./output.txt
```
in the Linux terminal.
