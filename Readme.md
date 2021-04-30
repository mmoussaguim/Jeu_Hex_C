# Introduction

​	This project represents an implementation in `C` programming language of the Hex game with multiple strategies.	

# Authors

​	Here is a list of the contributers to this project:

- Mohamed Faycal Boullit
- Mamadou Moussa Guimba
- Hugo Pierre
- Jalal Izekki

This project has been accomplished under the supervision of  F. Herberteau.

# Usage

​	In order to use this project, it is necessary to have installed GSL library and set its location in `LD_LIBRARY_PATH`. The project can be run using the following commands: 

```bash
GSL_PATH=path/to/gsl make # to compile and generate the server and the players
GSL_PATH=path/to/gsl make test # to compile and run all the tests
GSL_PATH=path/to/gsl make install # to copy the main files to the install repository
make clean #to remove all the generated files and establish the initial state of the project repository

```

Where `path/to/gsl` is replaced with the path to GSL repository where `libgsl.so` is installed.	

​	The game can be started between two players `player1.so` and `player2.so` from the install repository by using the following command:

```shell
./install/server -m [M] -t [T] ./install/player1.so ./install/player2.so
```

- Use the option `-m` to specify the width of the gameboard.

- Use the option `-t` to specify the shape of the game board. The available shapes are 

  - t for a triangular board
  - c for a squared board
  - h for the hexagonal shaped board

  It is also possible to run the tests by using the command:

```bash
./install/alltests
```

# Report

​	The report describing the results of this project in more details has been written in $\LaTeX$ . All the resources can be found in the repository named `report`.

Use the following command inside the report's repository:

```bash
make # to generate a pdf version of the report
make clean # to remove the generated files after the compilation
make cleanall # to establish the initial state of the report repository
```

 