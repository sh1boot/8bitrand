#!/bin/sh

# typical usage:
#   ./configlist.sh | xargs -L1 -IXXX sh -c 'XXX -t 0x100000'

DUT=./8bitrand
for lag in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 ; do
    for width in -n -w ; do
        index=0
        while $DUT $width$index -r$lag 0 2>/dev/null ; do
            echo $DUT $width$index -r$lag
            index=$((index+1))
        done
    done
    echo $DUT -a0x00aa -r$lag
    echo $DUT -a0x0155 -r$lag
    echo $DUT -a0xaa55 -r$lag
done
