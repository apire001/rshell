#!/bin/sh
ls >> testfiles/TestStore1
ps >> testfiles/TestStore2
echo this is a test > Test3
echo this sentence has five words | wc > Test4
echo cat dog > Test1
cat < testfile > Test2
echo test > Test5
ls > Test1
rm Test1
rm Test2
rm Test3
rm Test5
exit #exiting