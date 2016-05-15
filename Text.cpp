#include "text.h"

Text::Text(std::string t, glm::vec2 position, GLfloat size, glm::vec3 color)
	: text(t), Position(position), Size(size), Color(color){ }
	
void Text::Draw(TextRenderer &renderer){
	renderer.RenderText(this->text, this->Position.x, this->Position.y, this->Size, this->Color);
}