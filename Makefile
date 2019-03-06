OBJDIR          = obj

CC              = g++

CLIENT_SRC      = $(wildcard *.cpp)
CLIENT_OBJ      = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CLIENT_SRC))

CXXFLAGS        = -m32 -Wall -Wextra -pedantic -fPIC -O2 -Os -std=c++11
LDFLAGS         = -m32 -lpthread

TARGET          = rpcgen

.PHONY: clean

all: rpcgen

rpcgen: $(OBJDIR) $(CLIENT_OBJ) Makefile
	$(CC) $(LDFLAGS) -o $(TARGET) $(CLIENT_OBJ)
	strip $(TARGET)

uninstall:
	sudo rm -f  /usr/local/bin/$(TARGET)

install: rpcgen uninstall
	sudo cp -f $(TARGET) /usr/local/bin/$(TARGET)

clean:
	rm -rf $(OBJDIR)
	rm -f  $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o :%.cpp Makefile
	$(CC) $(CXXFLAGS) -c $< -o $@
