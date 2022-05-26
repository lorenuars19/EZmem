NAME=test_plugin.so

CXX = g++
# Flags for the C++ compiler: enable C++11 and all the warnings, -fno-rtti is required for GCC plugins
CXXFLAGS = -std=c++11 -Wall -fno-rtti
# Workaround for an issue of -std=c++11 and the current GCC headers
CXXFLAGS += -Wno-literal-suffix

# Determine the plugin-dir and add it to the flags
PLUGINDIR=$(shell $(CXX) -print-file-name=plugin)
CXXFLAGS += -I$(PLUGINDIR)/include

# top level goal: build our plugin as a shared library
all: ${NAME}

${NAME}: test_plugin.o
	$(CXX) $(LDFLAGS) -shared -o $@ $<

test_plugin.o : test_plugin.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<

clean:
	rm -f warn_unused.o ${NAME}

check: ${NAME}
	$(CXX) -fplugin=./${NAME} -c -x c++ /dev/null -o /dev/null

.PHONY: all clean check
