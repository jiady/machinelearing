#!/bin/sh
./minmaxPredict 3 9 ../preprocess/test.txt ../Q6/Q2lms ../Q6/log/Q2lmsans.txt > ../Q6/log/Q2lmsreport.txt
./minmaxPredict 3 9 ../preprocess/test.txt ../Q6/Q2perc ../Q6/log/Q2precans.txt > ../Q6/log/Q2precreport.txt
./minmaxPredict 3 9 ../preprocess/test.txt ../Q6/Q3lms ../Q6/log/Q3lmsans.txt > ../Q6/log/Q3lmsreport.txt
./minmaxPredict 3 9 ../preprocess/test.txt ../Q6/Q3perc ../Q6/log/Q3precans.txt > ../Q6/log/Q3precreport.txt
