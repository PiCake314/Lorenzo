EX=hw1
all: $(EX)

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lfreeglut -lglew32 -lglu32 -lopengl32 -lm
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations -DRES=1
LIBS=-framework GLUT -framework OpenGL
CLEAN=rm -f $(EX) *.o *.a
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
CLEAN=rm -f $(EX) *.o *.a
endif
endif

#  Generic compile and link
%:%.cpp
	gcc $(CFLG) -o $@ $^ $(LIBS)

run:
	./$(EX)


#  Clean
clean:
	$(CLEAN)