#!/bin/sh

rm ./Q3timeRecord.log
mpiexec -n 27 ./trainParallel 3 9 ../Q3/train ../Q3/train Q3timeRecord.log> Q3train.log
