#include "game_object.h"
#include "resource_manager.h"

GameModel::GameModel(std::string file, std::string name)
: Game_Object3D(glm::vec3(0), glm::vec3(1), glm::vec3(0.0f,0.0f,1.0f)) // The color here is stocked in HSV format
{
    this->model = ResourceManager::LoadModel(file, name);
}

void GameModel::Draw(StateManager &manager, Shader shader, glm::mat4 projection, glm::mat4 view){
	this->model.Draw(manager, shader, this->Position, this->Size, this->Rotation, this->Color, this->Alpha, projection, view);
}

void GameModel::Update(GLfloat dt, GLfloat spin){
    this->Rotation.x += dt * spin;
    this->Color.x += dt; //change the Hue
    if (this->Color.x > 1)
        this->Color.x -= floor(this->Color.x);

    GLfloat max = spin > 1000 ? 1000 : spin;
	this->Color.y = max/1000;//change the saturation
}

// Set the model in the correct position depending on its bounds so that the specified side of it is at the pos coordinate
void GameModel::SetSide(GLfloat pos, Side side_local, Side side_model){
	GLfloat newpos;
	switch(side_model){
    	case UP_SIDE :
    		newpos = pos-(this->model.Span_udb.x*this->Size.y);
    		break;
    	case DOWN_SIDE :
    		newpos = pos-(this->model.Span_udb.y*this->Size.y);
    		break;
    	case FRONT_SIDE :
    		newpos = pos-(this->model.Span_lrf.z*this->Size.x);
    		break;
    	case BACK_SIDE :
    		newpos = pos-(this->model.Span_udb.z*this->Size.x);
    		break;
    	case LEFT_SIDE :
    		newpos = pos-(this->model.Span_lrf.x*this->Size.z);
    		break;
    	case RIGHT_SIDE :
    		newpos = pos-(this->model.Span_lrf.y*this->Size.z);
	}
	switch(side_local){
    	case UP_SIDE :
    	case DOWN_SIDE :
    		this->Position.y = newpos;
    		break;
    	case FRONT_SIDE :
    	case BACK_SIDE :
    		this->Position.x = newpos;
    		break;
    	default :
    		this->Position.z = newpos;
	}
}