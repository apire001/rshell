#!/bin/sh
cat < testfiles/test1
wc < testfiles/test2
tr A-Z a-z < testfiles/test3
grep it < testfiles/test4
cat < testfiles/test3 > TestOutput
cat < testfiles/test1 | tr A-Z a-z
grep 1 < testfiles/test4
grep 2 < testfiles/test2 | wc
exit #exiting  