#!/bin/sh
lib="liblinear-1.96"
traindata="../preprocess/train.txt"
testdata="../preprocess/test.txt"

../$lib/train $traindata > train.log
#../$lib/predict $testdata train.txt.model out.txt > report.txt

#../$lib/train -w1 3 -w-1 1 $traindata > trainw.log
#../$lib/predict $testdata train.txt.model out.txt > reportw.txt

#../$lib/train -s 4 $traindata > trains.log
#../$lib/predict $testdata train.txt.model out.txt > reports.txt

#../$lib/train -B 1 $traindata > trainb.log
#../$lib/predict $testdata train.txt.model out.txt > reportb.txt


