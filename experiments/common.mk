# Some stuff that's needed by various makefiles in and below this dir
SHELL = bash # for pipefail amd <() syntax

NUM_TRIALS := 100
NUM_STD_HANDS := 1000
NUMS_CMD := seq -w 0 $$(($(NUM_STD_HANDS)-1))
NUMS := $(shell $(NUMS_CMD))

SIM := $(top_builddir)bin/simulator/konig-simulator
FEAT := $(top_builddir)bin/features/konig-features

