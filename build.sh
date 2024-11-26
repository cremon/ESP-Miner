#!/bin/bash
BINFILE=./build/test`date +%s`.bin
IDFCMD=idf.py

for i in $IDFCMD bitaxetool npm; do
  command -v $i >/dev/null 2>&1 || { echo >&2 "I require $i but it's not installed.  Aborting."; exit 1; }
done

if [[ ! -e config.csv ]]; then
  echo "no config.csv file found. Please copy one from config-*.csv"
  exit 1
fi

$IDFCMD build && \
    ./merge_bin.sh $BINFILE && \
    bitaxetool --config ./config.csv --firmware $BINFILE

