TARGET          := rpcgen
INSTALLDIR      := $(PREFIX)/usr/local/bin

MAKEFILE        := $(lastword $(MAKEFILE_LIST))

OBJDIR          := build/obj

CC              := g++
STRIP           := strip

VPATH           := . export
INCLUDEPATH     := . export
DEFINES         := 

IFLAGS          := $(patsubst %,-I%,$(INCLUDEPATH))
DFLAGS          := $(patsubst %,-D%,$(DEFINES))

SOURCES         := $(wildcard *.cpp)
OBJECTS         := $(patsubst %.cpp,$(OBJDIR)/%.o,$(notdir $(SOURCES)))

CXXFLAGS        := -m32 -Wall -Wextra -pedantic -O2 -Os -std=gnu++11 $(IFLAGS) $(DFLAGS)
LDFLAGS         := -m32 -s -Wl,--build-id=none

### ============================================================================

.PHONY: all clean install uninstall

all: $(TARGET)

### Build ======================================================================

$(TARGET): $(OBJDIR) $(OBJECTS) $(MAKEFILE)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJECTS)
	$(STRIP) --strip-all $(TARGET)

clean:
	rm -rf $(OBJDIR)
	rm -f  $(TARGET)

### Deployment =================================================================

install: $(TARGET)
	install $(TARGET) $(INSTALLDIR)/$(TARGET)

uninstall:
	rm -f $(INSTALLDIR)/$(TARGET)

### Objects ====================================================================

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : %.cpp $(MAKEFILE)
	$(CC) $(CXXFLAGS) -c $< -o $@
