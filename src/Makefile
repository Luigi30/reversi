CC = m68k-amigaos-gcc -g
CXX = m68k-amigaos-g++ -g
CFLAGS = -noixemul -fbaserel -m68000 -Os -Wall -fomit-frame-pointer -fpermissive
CXXFLAGS = -Os -Wall -fomit-frame-pointer

INCDIR = ../include
OBJDIR = obj
BINDIR = ../bin

BINS = reversi

_OBJS = main.o reversi.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
	
$(BINDIR)/reversi: $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^
	
$(OBJDIR)/main.o: main.cpp reversi.hpp
	$(CXX) -c $(CFLAGS) -o $@ $<
	
$(OBJDIR)/reversi.o: reversi.cpp reversi.hpp
	$(CXX) -c $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINDIR)/$(BINS)
	rm -f $(OBJDIR)/*.o *~