#!/bin/sh

rm ./Q2timeRecord.log
mpiexec -n 27 ./trainParallel 3 9 ../Q2/train ../Q2/train Q2timeRecord.log>Q2train.log
