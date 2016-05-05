#include "game_model.h"
#include "resource_manager.h"

GameModel::GameModel(std::string file, std::string name)
: Position(0), Size(1), Velocity(0.0f), Rotation_angle(0.0f, 1.0f, 0.0f), Rotation(0.0f)
{
    this->model = ResourceManager::LoadModel(file, name);
}

void GameModel::Draw(Shader shader, glm::mat4 projection, glm::mat4 view){
	this->model.Draw(shader, this->Position, this->Size, this->Rotation, this->Rotation_angle, projection, view);
}

void GameModel::SetSide(Side side, GLfloat pos){
	switch(side){
    	case UP_SIDE :
    		this->Position.y = pos-(this->model.Span_udb.x*this->Size.y);
    		break;
    	case DOWN_SIDE :
    		this->Position.y = pos-(this->model.Span_udb.y*this->Size.y);
    		break;
    	case FRONT_SIDE :
    		this->Position.x = pos-(this->model.Span_lrf.z*this->Size.x);
    		break;
    	case BACK_SIDE :
    		this->Position.x = pos-(this->model.Span_udb.z*this->Size.x);
    		break;
    	case LEFT_SIDE :
    		this->Position.z = pos-(this->model.Span_lrf.x*this->Size.z);
    		break;
    	case RIGHT_SIDE :
    		this->Position.z = pos-(this->model.Span_lrf.y*this->Size.z);
	}
}