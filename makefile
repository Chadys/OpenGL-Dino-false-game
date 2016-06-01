EXE = dino
CXX = g++
CXXFLAGS = -std=c++11 -Wall -W -g
packages = glfw3 glew assimp freetype2
CPPFLAGS := $(shell pkg-config --cflags $(packages))
LIBS := $(shell pkg-config --static --libs $(packages))
LDFLAGS = -lSOIL -lSDL2_mixer -lSDL2
OBJ = main.o ResourceManager.o Shader.o Texture2D.o Game.o SpriteRenderer.o Object3D.o GameLevel.o Model.o Texture3D.o GameModel.o Object2D.o Square.o TextRenderer.o Text.o StateManager.o

$(EXE): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CPPFLAGS) $(LIBS)

%.o: %.c
	$(CXX) -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(EXE) *.o
.PHONY: all clean