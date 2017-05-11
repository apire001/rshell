#!/bin/sh
ls || exit
ls -a && exit
exit #test
echo dog && echo cat && exit
echo dog || echo cat; exit
echo dog || echo cat || exit
