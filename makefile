TARGET = final

USING_CSE40166 = 1
USING_FREEGLUT = 1
USING_FREEALUT = 0
USING_OPENAL = 0
USING_FMOD = 0
USING_GLUI = 0
USING_GLEW = 1
USING_SOIL = 1

CXX    = g++
CFLAGS = -Wall -g

ifeq ($(shell uname), Darwin)
    LIBS = -framework OpenGL -framework Cocoa
else
    LIBS = -lGL -lGLU
endif

ifeq "$(USING_CSE40166)" "1"
    CSE40166 = /home/david/Documents/graphics/CSE40166
    ifneq "$(wildcard $(CSE40166))" ""
        INCPATH += -I$(CSE40166)/include
        LIBPATH += -L$(CSE40166)/lib -Wl,--rpath -Wl,$(CSE40166)/lib
	LIBS += -lCSE40166
    endif
    USING_SOIL = 1
    USING_FREEALUT = 1
    USING_OPENAL = 1
endif

ifeq "$(USING_FREEGLUT)" "1"
    ifeq ($(shell uname), Darwin)
	LIBS += -framework GLUT
    else
	FREEGLUT = /home/david/Documents/graphics/freeglut
	ifneq "$(wildcard $(FREEGLUT))" ""
            INCPATH += -I$(FREEGLUT)/include
            LIBPATH += -L$(FREEGLUT)/lib
            LIBS += -lglut
	endif
    endif
endif

ifeq "$(USING_FREEALUT)" "1"
    FREEALUT = /home/david/Documents/graphics/freealut
    ifneq "$(wildcard $(FREEALUT))" ""
	INCPATH += -I$(FREEALUT)/include
	LIBPATH += -L$(FREEALUT)/lib -Wl,--rpath -Wl,$(FREEALUT)/lib
	LIBS += -lalut
    endif
endif

ifeq "$(USING_OPENAL)" "1"
    ifeq ($(shell uname), Darwin)
	    LIBS += -framework OpenAL
    else
        OPENAL = /home/david/Documents/graphics/openal
	ifneq "$(wildcard $(OPENAL))" ""
	    INCPATH += -I$(OPENAL)/include
	    LIBPATH += -L$(OPENAL)/lib -Wl,--rpath -Wl,$(OPENAL)/lib
	    LIBS += -lopenal
	endif
    endif
endif

ifeq "$(USING_FMOD)" "1"
    FMOD = /afs/nd.edu/coursefa.12/cse/cse40166.01/fmod
    ifneq "$(wildcard $(FMOD))" ""
	INCPATH += -I$(FMOD)/include
	LIBPATH += -L$(FMOD)/lib -Wl,--rpath -Wl,$(FMOD)/lib
	LIBS += -lfmodex64
    endif
endif

ifeq "$(USING_GLUI)" "1"
    GLUI = /home/david/Documents/graphics/glui
    ifneq "$(wildcard $(GLUI))" ""
        INCPATH += -I$(GLUI)/include
        LIBPATH += -L$(GLUI)/lib -Wl,--rpath -Wl,$(GLUI)/lib
	LIBS += -lglui
    endif
endif

ifeq "$(USING_GLEW)" "1"
    GLEW = /home/david/Documents/graphics/glew
    ifneq "$(wildcard $(GLEW))" ""
        INCPATH += -I$(GLEW)/include
        LIBPATH += -L$(GLEW)/lib -Wl,--rpath -Wl,$(GLEW)/lib
	LIBS += -lGLEW
    endif
endif

ifeq "$(USING_SOIL)" "1"
    SOIL = /home/david/Documents/graphics/soil
    ifneq "$(wildcard $(SOIL))" ""
        INCPATH += -I$(SOIL)/include
        LIBPATH += -L$(SOIL)/lib -Wl,--rpath -Wl,$(SOIL)/lib
	LIBS += -lSOIL
    endif
endif

all: $(TARGET)

clean:
	rm -f *.o $(TARGET)

depend:
	rm -f Makefile.bak
	mv Makefile Makefile.bak
	sed '/^# DEPENDENCIES/,$$d' Makefile.bak > Makefile
	echo '# DEPENDENCIES' >> Makefile
	$(CXX) -MM *.cpp >> Makefile

main.o: 
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<	

.c.o: 	
	$(CXX) $(INCPATH) -c -o $@ $<

.cc.o: 	
	$(CXX) $(INCPATH) -c -o $@ $<

.cpp.o: 	
	$(CXX) $(INCPATH) -c -o $@ $<

$(TARGET): main.o
	$(CXX) $(CFLAGS) $(INCPATH) -o $@ $^ $(LIBPATH) $(LIBS)


# DEPENDENCIES
main.o: main.cpp