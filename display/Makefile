PLATFORM = $(shell uname)

CFLAGS= -g -Wall -l m
LDFLAGS=

ifeq ($(PLATFORM),Darwin)
## Mac OS X
CFLAGS += -m64 -isystem/usr/local/include
LDFLAGS+= -m64 -lc -framework AGL -framework OpenGL -framework GLUT \
	-framework Foundation
else
## Linux
INCLUDEPATH  = -I/usr/include/GL/ 
LIBPATH = -L/usr/lib #-L/usr/X11R6/lib
LDFLAGS+=  -lGL -lglut #-lrt -lGLU -lX11 -lm  -lXmu -lXext -lXi
endif


CC = gcc
GRAPHICS = $(LIBPATH) $(LDFLAGS)


default: compile

compile: display.o list.o
	$(CC) $? -lm $(GRAPHICS) -o view

list.o: llist.c
	$(CC) $(CFLAGS) -c $< -o $@

display.o: Display.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ view

# # CC = gcc
# # CFLAGS = -g -Wall -std=c99 -l m

# # default:	compile

# # compile:	view.o grid.o
# # 	$(CC) $(CFLAGS) view.o grid.o -o view


# # view.o:		view.c view.h grid.o
# # 	$(CC) $(CFLAGS) -c $< -o $@

# # grid.o:		grid.c grid.h
# # 	$(CC) $(CFLAGS) -c $< -o $@

# # clean:
# # 	$(RM) *.o *~ view