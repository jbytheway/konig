#!/usr/bin/env Rscript

args = commandArgs(TRUE)
dataFilename = args[1]
coeffFilename = args[2]
auxFilename = args[3]
type = args[4]

data = read.table(dataFilename, header=TRUE)
colNames = names(data)
obs = colNames[3:length(colNames)]

if (type == "regression") {
  res = lm(as.formula(paste("score~", paste(obs, collapse="+"))), data=data)
  write(res$coefficients, file=coeffFilename, ncolumns=length(colNames)+1)
  write(res$residuals, file=auxFilename, ncolumns=1)
} else if (type == "decision") {
  library(MASS)
  res = lda(as.formula(paste("score~", paste(obs, collapse="+"))), data=data)
  c1 = (res$means[1,] %*% res$scaling)[1]
  c2 = (res$means[2,] %*% res$scaling)[1]
  p1 = res$prior[1]
  p2 = res$prior[2]
  threshold = ((c2*c2-c1*c1)/2-log(p2)+log(p1))/(c2-c1)
  output = cbind(as.matrix(threshold), t(res$scaling))
  write(output, file=coeffFilename, ncolumns=length(colNames)+1)
  write(predict(res, data)$class, file=auxFilename, ncolumns=1)
}
