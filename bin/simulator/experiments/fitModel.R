#!/usr/bin/env Rscript

args = commandArgs(TRUE)
dataFilename = args[1]
coeffFilename = args[2]
residualsFilename = args[3]

data = read.table(dataFilename, header=TRUE)
colNames = names(data)
obs = colNames[3:length(colNames)]
res = lm(as.formula(paste("score~", paste(obs, collapse="+"))), data=data)
write(res$coefficients, file=coeffFilename, ncolumns=length(colNames)+1)
write(res$residuals, file=residualsFilename, ncolumns=1)

