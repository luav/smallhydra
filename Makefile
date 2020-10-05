#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

ARDUINO_DIR = /opt/xdk/arduino

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I../smallrdf/include -I../ESPAsyncWebServer/src -I\$\(ARDUINO_DIR\)/hardware/arduino/avr/cores/arduino -I\$\(ARDUINO_DIR\)/hardware/tools -I../ESPAsyncTCP/src
CFLAGS = -Wnon-virtual-dtor -Wcast-align -Wundef -Wfloat-equal -Wunreachable-code -Weffc++ -std=c++17 -fPIC -Wzero-as-null-pointer-constant -Wmissing-include-dirs -fexceptions -fstack-protector -Werror=format-security -Wl,-z,defs -Wl,-z,now -Wl,-z,relro -Wl,-nostdlib
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = 

INC_DEBUG = $(INC) -Isrc
CFLAGS_DEBUG = $(CFLAGS) -Wshadow -Winit-self -Wredundant-decls -Winline -Wswitch-enum -Wswitch-default -Wall -Og -g -shared -fasynchronous-unwind-tables -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR) -L../smallrdf/bin/Debug
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/smallhydra.so

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -fomit-frame-pointer -O3 -shared -pipe -D_FORTIFY_SOURCE=1
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR) -L../smallrdf/bin/Release
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/smallhydra.so

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/Hydra.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/Hydra.o

all: debug release debugavr

clean: clean_debug clean_release clean_debugavr

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) -shared $(LIBDIR_DEBUG) $(OBJ_DEBUG)  -o $(OUT_DEBUG) $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/Hydra.o: src/Hydra.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/Hydra.cpp -o $(OBJDIR_DEBUG)/src/Hydra.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) -shared $(LIBDIR_RELEASE) $(OBJ_RELEASE)  -o $(OUT_RELEASE) $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/src/Hydra.o: src/Hydra.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/Hydra.cpp -o $(OBJDIR_RELEASE)/src/Hydra.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/src

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

