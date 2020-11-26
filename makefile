CC    ?= clang
CXX   ?= clang++

EXE = test/tbox_test.out
DEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++11

CFLAGS = -Wno-deprecated-register -O0  $(DEBUG) $(CSTD) 
CXXFLAGS = -Wno-deprecated-register -O0  $(DEBUG) $(CXXSTD)

CPPOBJ = test/tbox_test src/dl_driver src/Onthology
SOBJ = build/parser build/lexer

FILES = $(addsuffix .cpp, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))
SOBJS = $(addsuffix .o, $(OBJS))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				parser.tab.cc parser.tab.hh \
				location.hh position.hh \
			    stack.hh parser.output parser.o \
				lexer.o lexer.yy.cc $(EXE)\

all:$(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(SOBJS)


parser: build/parser.yy
	bison -d -v build/parser.yy
	$(CXX) $(CXXFLAGS) -c -o build/parser.o build/parser.tab.cc

lexer: lexer.l
	flex --outfile=lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c lexer.yy.cc -o lexer.o

.PHONY: test
test:
	cd test && ./test0.pl

.PHONY: clean
clean:
	rm -rf $(CLEANLIST)