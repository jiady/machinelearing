#!/bin/sh
traindata="../preprocess/train.txt"
testdata="../preprocess/test.txt"

./train -s 14 $traindata> Q1perctrain.log
./predict $testdata train.txt.model outlmsq1.txt >Q1precpredict.log
rm ./train.txt.model
rm ./outlmsq1.txt
./train  -s 15 $traindata> Q1lmstrain.log
./predict $testdata train.txt.model outlmsq1.txt >Q1lmspredict.log
rm ./train.txt.model
rm ./outlmsq1.txt
