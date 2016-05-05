EXE = chat
CXX = g++
CXXFLAGS = -std=c++11 -Wall -W -g
packages = glfw3 glew assimp
CPPFLAGS := $(shell pkg-config --cflags $(packages))
LIBS := $(shell pkg-config --static --libs $(packages))
LDFLAGS = -lSOIL

$(EXE): main.cpp ResourceManager.cpp Shader.cpp Texture2D.cpp Game.cpp SpriteRenderer.cpp GameObject.cpp GameLevel.cpp Model.cpp Texture3D.cpp GameModel.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(EXE) *.o
.PHONY: all clean
	