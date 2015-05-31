#!/bin/sh

make
./preprocess ../rawdata/train.txt train.txt
./preprocess ../rawdata/test.txt test.txt
chmod 777 train.txt
chmod 777 test.txt
