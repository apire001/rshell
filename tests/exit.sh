#!/bin/sh
ls || exit
(cd .. || echo free) && echo should echo
ls && (echo moose && exit)
(echo M && echo G) || (echo Q  && echo F) 
test -e /test/file/path || exit
test -d /test/file/path || exit
[ -f /test/file/path ] || exit
[ -e /test/file/path ] || exit
ls && (echo moose && exit)
(exit && echo A) || (echo B  && echo C)
(echo A || echo B) || exit
exit