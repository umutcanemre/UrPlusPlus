CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -MMD
EXEC = a3q4
OBJECTS = board.o cliview.o gamestate.o gameviewer.o guiview.o main.o \
	path.o tilebasic.o tileblackhole.o tile.o tileend.o tilelucky.o \
	tilenull.o tilerosette.o tilestart.o tiletornado.o tokenassassin.o \
	tokenbasic.o token.o tokenflexible.o tokenspeedster.o tokensupporter.o

DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lSDL

-include ${DEPENDS}


.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
