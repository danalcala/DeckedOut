PROG = DeckedOut
LIBS = -lglut -lGLU -lGL
SRCS = DeckedOut.cpp
OBJS = $(SRCS:.cpp=.o)

$(PROG) : $(OBJS)
	g++ -o $@ $(OBJS) $(LIBS)

clean:
	rm $(OBJS) $(PROG)
