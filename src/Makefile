CC = m68k-amigaos-gcc -g
CXX = m68k-amigaos-g++ -g
CFLAGS = -noixemul -m68000 -Os -Wall -fomit-frame-pointer -Wno-pointer-sign -Wno-int-conversion -fpermissive
CXXFLAGS = -noixemul -m68000 -Os -Wall -fomit-frame-pointer -fpermissive -Wno-write-strings

INCDIR = ../include
OBJDIR = obj
BINDIR = ../bin

BINS = reversi

_OBJS = main.o reversi.o board.o mcarlo.o serial.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))
	
$(BINDIR)/reversi: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	
$(OBJDIR)/main.o: main.cpp $(INCDIR)/main.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $@ $<
	
$(OBJDIR)/reversi.o: reversi/reversi.cpp $(INCDIR)/reversi/reversi.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $@ $<
	
$(OBJDIR)/board.o: reversi/board.cpp $(INCDIR)/reversi/board.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $@ $<

$(OBJDIR)/mcarlo.o: reversi/mcarlo.cpp $(INCDIR)/reversi/mcarlo.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $@ $<

$(OBJDIR)/serial.o: serial.cpp $(INCDIR)/serial.h
	$(CXX) -c $(CXXFLAGS) -I$(INCDIR) -o $@ $<

clean:
	rm -f $(BINDIR)/$(BINS)
	rm -f $(OBJDIR)/*.o *~