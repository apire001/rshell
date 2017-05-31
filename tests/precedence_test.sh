#!/bin/sh
echo A; echo B; echo C
ls -a || echo A
echo A; echo B && echo C
echo A && echo B || echo C && echo D
(echo A && echo B) || (echo C && echo D)
cd .. || ls -a && mkdir temp || pwd
((cd .. || ls -a ) && mkdir temp) || pwd
ls && ((echo G || echo H) || echo moose)
(echo moose || echo dov) && test -e temp
ls && (test -d temp/name.cc && echo m) || test -f temp/tinker
(cd .. || echo done) || exit