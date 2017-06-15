#!/bin/sh
ls | cat
ls -a | grep src
ls | cat | wc
ls | wc
ps | wc
cat < testfiles/test1 | tr A-Z a-z | wc > testfiles/test1
cat < testfiles/test2 | tee Output1
echo hello world this is a test | wc
exit #exiting