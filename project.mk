BUILD_CPPFLAGS += \
	-std=c++0x -DPHOENIX_LIMIT=6 -I$(this_srcdir) -I. -I$(top_srcdir)
# These warnings arise when optimisations are on.  I don't think either of them
# is genuine
BUILD_CXXFLAGS += -Wno-uninitialized -Wno-strict-aliasing

