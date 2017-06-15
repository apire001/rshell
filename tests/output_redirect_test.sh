#!/bin/sh
ls > Output1
ps > Output2
echo this is a test > Output3
echo this sentence has five words | wc > Output4
echo hello world > OutputTest1
cat < OutputRed_test > Output2
echo test > Output5
ls > Output1
rm Output1
rm Output2
rm Output3
rm Output5
exit