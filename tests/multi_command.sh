#!/bin/sh
echo dog && echo cat
echo dog || echo cat
echo dog || echo cat || exit
echo dog || echo dog && exit
echo dog; echo cat; exit
