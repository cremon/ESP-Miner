#!/bin/bash
BINFILE=./test`date +%s`.bin
echo BINFILE=$BINFILE
idf.py build && \
    ./merge_bin.sh build/$BINFILE && \
    bitaxetool --config ./config.cvs --firmware build/$BINFILE

