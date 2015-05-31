#!/bin/sh


mpiexec -n 27 ./trainParallel 3 9 ../Q2/train ../Q6/Q2perc ../Q6/log/Q2perctimeRecord.log 14>../Q6/log/Q2train_Perceptron.log
mpiexec -n 27 ./trainParallel 3 9 ../Q2/train ../Q6/Q2lms ../Q6/log/Q2lmstimeRecord.log 15>../Q6/log/Q2train_lms.log
mpiexec -n 27 ./trainParallel 3 9 ../Q3/train ../Q6/Q3perc ../Q6/log/Q3perctimeRecord.log 14>../Q6/log/Q3train_Perceptron.log
mpiexec -n 27 ./trainParallel 3 9 ../Q3/train ../Q6/Q3lms ../Q6/log/Q3lmstimeRecord.log 15>../Q6/log/Q3train_lms.log
