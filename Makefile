CC = gcc
OPTION_WALL = -Wall
OPTION_WEXTRA = -Wextra
EXE = canvascii

all: link

compile:
	$(CC) $(OPTION_WALL) $(OPTION_WEXTRA) -c $(EXE).c

link: compile
	$(CC) *.o -o $(EXE)

clean:
	rm $(EXE)
	rm *.o
	rm *.html

html:
	pandoc README.md -f markdown -t html5 -o README.html -c /home/fe6919118/inf3135-automne2021-tp1/misc/github-pandoc.css
	pandoc sujet.md -f markdown -t html5 -o sujet.html -c /home/fe691918/inf3135-automne2021-tp1/misc/github-pandoc.css
