CXX = g++
CPPFLAGS += $(shell xmlrpc-c-config c++2 --cflags) -g
CXXFLAGS += 
LDFLAGS +=
LIBS += $(shell xmlrpc-c-config c++2 abyss-server --libs)

all: driver duel simple

driver: driver.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o driver driver.cpp $(LIBS)

simple: simple.cpp protocol_common.h
	$(CXX) $(CPPFLAGS) -o simple simple.cpp

duel: duel.cpp
	$(CXX) $(CPPFLAGS) -o duel duel.cpp

.PHONY: clean
clean:
	rm -rf *.o driver duel simple
