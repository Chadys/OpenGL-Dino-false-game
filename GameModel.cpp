#include "game_object.h"
#include "resource_manager.h"

GameModel::GameModel(std::string file, std::string name)
: Game_Object3D(glm::vec3(0), glm::vec3(1))
{
    this->model = ResourceManager::LoadModel(file, name);
}

void GameModel::Draw(StateManager &manager, Shader shader, glm::mat4 projection, glm::mat4 view){
	this->model.Draw(manager, shader, this->Position, this->Size, this->Rotation, this->Rotation_angle, this->Alpha, projection, view);
}

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