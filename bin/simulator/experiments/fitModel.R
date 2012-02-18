#!/bin/env Rscript

args = commandArgs(TRUE)
dataFilename = args[1]

data = read.table(dataFilename, header=TRUE)
colNames = names(data)
obs = colNames[3:length(colNames)]
res = lm(as.formula(paste("score~", paste(obs, collapse="+"))), data=data)
res$coefficients
