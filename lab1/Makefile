CFLAGS=-g -O0 -Wall
LFLAGS=-lGL -lglut
FILES=Makefile
ARCHIVE=lab1.zip
OUT=lab1

all: $(OUT)

clean:
	$(RM) $(OUT)
	$(RM) *.o
	$(RM) *.zip

purge-o:
	$(RM) *.o

$(OUT): purge-o 
	gcc $(CFLAGS) -o $(OUT) $(LFLAGS)

.PHONY: all clean dist purge-o

dist:
	zip $(ARCHIVE) $(FILES)
