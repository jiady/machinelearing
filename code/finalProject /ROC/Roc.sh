#!/bin/sh

make
./Roc ../Q1/out.txt Q1Roc.txt
./Roc ../liblinear-1.96/Q2ans.txt Q2Roc.txt
./Roc ../liblinear-1.96/Q3ans.txt Q3Roc.txt
./Roc ../Q6/log/Q3precans.txt Q3precRoc.txt 
