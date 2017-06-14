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

* If a single command has more than 2048 words, it will result in a segmentation fault
* The "$ " string for the terminal command lines may be output in the wrong place if you use the <, >, >> or | operators. This is due to the parent process not waiting for the child processes to finish, we have tried many different execution and waiting  methods but are unsure how to fix it. Regardless of this bug, the program will continue to run and work properly aside from this. Only happens once per redirection command.
* 
