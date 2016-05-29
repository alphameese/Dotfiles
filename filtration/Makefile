EXECUTABLE = filtration
SOURCE_DIR = src
OBJECT_DIR = build

CPPFILES = $(wildcard $(SOURCE_DIR)/*.cpp)

OBJFILES = $(addprefix $(OBJECT_DIR)/,  $(notdir $(CPPFILES:.cpp=.o)))

CC = g++

CFLAGS = -std=c++11 -g -c -Wall

LDFLAGS = -g

PNG_PATH = libpng-1.6.21
INCLUDE += -I$(PNG_PATH)/include
LINK_LIBS +=  -L$(PNG_PATH)/lib/ -lpng -lz
PNG_LIB = $(PNG_PATH)/lib/libpng.a

LINK_LIBS += -L/usr/X11R6/lib -lX11

CURR_DIR = $(shell pwd)
PNG_BUILD_PATH = $(CURR_DIR)/$(PNG_PATH)

all: setup $(EXECUTABLE)

rebuild: clean all

setup: | $(OBJECT_DIR)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

$(EXECUTABLE): $(OBJFILES) $(PNG_LIB)
	$(CC) $(LDFLAGS) $(OBJFILES) $(LINK_LIBS) -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CC) $(CFLAGS) $(INCLUDE)  -o $@ $<

$(PNG_LIB):
	cd $(PNG_PATH)/src; ./configure --prefix=$(PNG_BUILD_PATH) --enable-shared=no
	$(MAKE) -C $(PNG_PATH)/src install

clean:
	\rm -rf $(OBJECT_DIR) $(EXECUTABLE)
