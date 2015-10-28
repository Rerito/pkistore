all:test

CPPFLAGS+=-std=c++11 -Wall -pedantic
CPPFLAGS+=-g -O2

test.o: *.hpp

%.o:%.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

%:%.o
	$(CXX) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)
