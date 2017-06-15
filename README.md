# cs100-rshell

CS 100 Assignment 4 by Andrew Pirelli and Atreyu Wittman

## Introduction

This program functions similarly to a bash terminal, although as of now it does
not include all the features of bash, only the ones specified in the assignment specifications.

### Install Instructions

```
$ git clone https://github.com/apire001/rshell.git
$ cd rshell
$ git checkout hw4
$ make
$ bin/rshell
```

### Known Bugs

* If a single command has more than 2048 words, it will result in a segmentation fault.
* Parentheses should work with <, >, >> and |. NOTE: Our parentheses error checking is horrible, so it will only work with proper syntax.
* Other connectors should also work with piping
