#!/bin/sh


mpiexec -n 27 ../liblinear-1.96/trainParallel 3 9 train train ./log/QtimeRecord.log> ./log/Qtrain.log
