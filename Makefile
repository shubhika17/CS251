
all: gethttp git-commit

gethttp: gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp
	g++ -o gethttp -g gethttp.cpp openhttp.cpp SimpleHTMLParser.cpp

git-commit:
	git add Makefile *.h *.cpp  >> .local.git.out
	git commit -a -m "$total of $totalmax" >> .local.git.out

clean:
	rm -f *.o gethttp core
