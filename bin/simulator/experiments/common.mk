# Some stuff that's needed by various makefiles in and below this dir

NUM_TRIALS := 100
NUM_STD_HANDS := 1000
NUMS_CMD := seq -w 0 $$(($(NUM_STD_HANDS)-1))
NUMS := $(shell $(NUMS_CMD))

