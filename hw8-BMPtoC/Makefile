# Makefile for CS 2110 bmptoc Homework

PROGNAME = bmptoc
SRC = bmptoc.c
OBJ = $(SRC:%.c=%.o)
CFLAGS = -Wall -Werror -std=c99 -pedantic -Wextra
LFLAGS =

all : $(OBJ)
	@echo "[LINK] Linking $(PROGNAME)"
	@gcc -o $(PROGNAME) $^ $(LFLAGS)

debug : CFLAGS += -DDEBUG -g
debug : all
	@echo "!! Built using debug flags !!"

check: all
	./bmptoc balloon.bmp
	cp balloon.c balloon.h ../imagetester/
	cd ../imagetester && make vba

%.o : %.c
	@echo "[COMPILE] Compiling $@"
	@gcc $(CFLAGS) -c $< -o $@

clean :
	@echo "[CLEAN] Removing all compiled files"
	@rm -f *.o $(PROGNAME)

submit :
	@echo "[SUBMIT] Creating submission archive"
	@echo  "[NOTE] You still need to submit the tar file to t-square."
	@tar -czf bmptoc_submission.tar.gz bmptoc.c ../drawimage/main.c > /dev/null 2>&1
