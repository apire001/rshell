#!/bin/sh
echo dog && echo cat
echo dog || echo cat
echo dog || echo cat || exit
echo dog || echo dog && exit
mkdir test || mkdir Lite
ls -a || echo A && echo eagle
echo dog; echo cat; exit
