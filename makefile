CC    ?= clang
CXX   ?= clang++

EXE = minidl
DEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++11

CFLAGS = -Wno-deprecated-register -O0  $(DEBUG) $(CSTD) 
CXXFLAGS = -O0  $(DEBUG) $(CXXSTD)

CPPOBJ = main src/dl_driver src/Onthology
SOBJ = parser lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				parser.tab.cc parser.tab.hh \
				location.hh position.hh \
			    stack.hh parser.output parser.o \
				lexer.o lexer.yy.cc $(EXE)\

.PHONY: all
all:$(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) parser.o lexer.o

parser: build/parser.yy
	bison -d -v build/parser.yy
	$(CXX) $(CXXFLAGS) -c -o parser.o parser.tab.cc

lexer: build/lexer.l
	flex --outfile=lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c lexer.yy.cc -o lexer.o

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)
