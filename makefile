EXE = chat
CXX = g++
CXXFLAGS = -std=c++11 -Wall -W -g
packages = glfw3 glew assimp freetype2
CPPFLAGS := $(shell pkg-config --cflags $(packages))
LIBS := $(shell pkg-config --static --libs $(packages))
LDFLAGS = -lSOIL

$(EXE): main.cpp ResourceManager.cpp Shader.cpp Texture2D.cpp Game.cpp SpriteRenderer.cpp Object3D.cpp GameLevel.cpp Model.cpp Texture3D.cpp GameModel.cpp Object2D.cpp Square.cpp TextRenderer.cpp Text.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	rm -f $(EXE) *.o
.PHONY: all clean
	