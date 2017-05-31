#!/bin/sh
test -e /test/file/path
test -f /test/file/path
test -d /test/file/path
[ -e /test/file/path ]
[ -f /test/file/path ]
[ -e /test/file/path ]
test -e /test/file/path && echo yes
test -f /test/file/path && echo yes
test -d /test/file/path && echo yes
[ -e /test/file/path ] && echo yes
[ -f /test/file/path ] && echo yes
[ -d /test/file/path ] && echo yes
test -e /test/file/path || echo I
test -f /test/file/path || echo I am
test -d /test/file/path || echo I am Sparta
[ -e /test/file/path ] || echo I need coffee
[ -f /test/file/path ] || echo asap
[ -d /test/file/path ] || ls
ls && test -e /test/file/path
ls && test -e /test/file/path || echo why
echo yes && echo no || test -f /test/file/path