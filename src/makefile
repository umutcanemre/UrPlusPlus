CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -MMD
EXEC = UrPlusPlus
OBJECTS = ai.o board.o cliview.o controller.o entityvisitor.o gamestate.o gameviewer.o guiview.o \
	human.o level1ai.o level2ai.o level3ai.o level4ai.o main.o \
	player.o tilebasic.o tileblackhole.o tile.o tilelucky.o \
	tilenull.o tilerosette.o tiletornado.o tokenassassin.o \
	tokenbasic.o token.o tokenflexible.o tokenspeedster.o tokensupporter.o \
	sdl_wrap.o gameobserver.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lSDL2 -lSDL2_image -lSDL2_ttf

-include ${DEPENDS}


.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
