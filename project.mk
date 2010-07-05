BUILD_CPPFLAGS += \
	-std=c++0x -DPHOENIX_LIMIT=6 -DBOOST_PARAMETER_MAX_ARITY=6 \
	-I$(this_srcdir) -I. -I$(top_srcdir)
# First wo arise when optimisations are on.  I don't think either of them
# is genuine.  Third is in BOOST_PARAMETER_CONSTRUCTOR macro.
BUILD_CXXFLAGS += -Wno-uninitialized -Wno-strict-aliasing -Wno-unused

