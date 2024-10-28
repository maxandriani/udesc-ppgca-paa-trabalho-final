# Weblist

Trabalho final da disciplina de PAA do Programa de mestrado da UDESC.

Criação de uma estrutura mista de árvores octogonais e listas lincadas para armazenamento arbitrário de dados e balanceamento.

[PAA_2024__Weblist.pdf](./docs/PAA_2024__WebList.pdf)

## Getting Started

This Makefile configuration will generate dynamic link files on `source` folder. However, all the `elf` binaries are targeted to `/bin` folder.

You should change the `PROGRAM_NAME` variable inside `./Makefile` file to the name of your program.

## Build Artifacts

``` sh
# Compile the entire program
make

# Cleanup binary files and erase ./bin folder
make clean

# Runs cleanup before tries to compile.
make ci
```
