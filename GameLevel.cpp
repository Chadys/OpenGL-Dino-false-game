/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>
#include <cstdlib>     /* srand, rand */
#include <string>


GameLevel::GameLevel(std::unique_ptr<Tex> &bg) 
    : Bg(std::move(bg)) { }

void GameLevel::Load(const GLchar *file)
{
    std::vector<std::vector<GLint>> boxData = this->load(file);
    if (boxData.size() > 0)
        this->init(boxData);
}

void GameLevel::Load(const GLchar *file, GLuint width, GLuint height)
{
    std::vector<std::vector<GLint>> boxData = this->load(file);
    if (boxData.size() > 0)
        this->init(boxData, width, height);
}

void GameLevel::LoadMenu(GLuint width, GLuint height)
{
    this->initMenu(width, height);
}

void GameLevel::Update(GLfloat dt, GLuint width, GLuint height, glm::vec2 cam_pos){
	std::vector<Square>::iterator i = this->Squares.begin();
	while (i != this->Squares.end()){
    	i->Position+=i->Velocity*dt;
    	// Clean square out of the screen
    	if ((i->Position.x+i->Size.x < cam_pos.x && i->Velocity.x < 0) ||
    		(i->Position.x-i->Size.x > cam_pos.x+width && i->Velocity.x > 0) ||
    		(i->Position.y+i->Size.y < cam_pos.y && i->Velocity.y < 0) ||
    		(i->Position.y-i->Size.y > cam_pos.y+height && i->Velocity.y > 0))
    			i = this->Squares.erase(i);
    	else
    		i++;
    }
}

void GameLevel::Draw(StateManager &manager, SpriteRenderer &renderer, SpriteRenderer &bgrenderer, glm::mat4 projection, glm::mat4 view)
{
    for (Text &text : this->Texts)
        text.Draw(manager,*this->T_renderer);
    for (std::unique_ptr<GameObject> &box : this->Obj)
        box->Draw(manager,renderer, projection, view);
    bgrenderer.DrawSprite(manager, *this->Bg, projection, view);
}

void GameLevel::Draw(StateManager &manager, SpriteRenderer &renderer, GLuint width, GLuint height, glm::mat4 projection, glm::mat4 view, GLboolean menu)
{
    renderer.DrawSprite(manager, *this->Bg, glm::vec2(0), glm::vec2(width, height), menu, projection);
    for (std::unique_ptr<GameObject> &box : this->Obj)
       box->Draw(manager,renderer, projection, view);
    for (Square &s : this->Squares)
        s.Draw(manager,renderer, projection, view, menu);
    for (Text &text : this->Texts)
        text.Draw(manager,*this->T_renderer);
}

void GameLevel::init(std::vector<std::vector<GLint>> boxData)
{
    // Calculate dimensions
    GLuint height = boxData.size();
    GLuint width = boxData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    // Initialize level boxs based on boxData
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // Check block type from level data (2D level array)
            if (boxData[y][x] > 0 && boxData[y][x] < 10) // Red Block
            {
                GLfloat size(1);
                glm::vec3 pos(x*size,(boxData[y][x]-5)*(size/2),-(y*size));
                this->Obj.push_back(std::unique_ptr<GameObject>(new Object3D(pos, glm::vec3(size), ResourceManager::GetTexture("fence"), glm::vec3(1.0f, 0.0f, 0.0f))));
            }
        }
    }
}

void GameLevel::init(std::vector<std::vector<GLint>> boxData, GLuint width, GLuint height)
{
    // Calculate dimensions
    GLuint n_block = boxData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    // Initialize level boxs based on boxData
     for (GLuint i = 0 ; i < n_block ; i++){
         if (boxData[0][i] > 0) // Block
         {
             GLuint type_block = rand() % 6;
             Texture2D tex = ResourceManager::GetTexture("grass"+to_string(type_block));
             //the size of the texture is ok for a 1024x768 screen, adapt it
             glm::vec2 size = glm::vec2(tex.Width,tex.Height)*((GLfloat)width/1024);

             GLfloat x = (width/2-size.x/2) + i*size.x;

             GLfloat dino_height = ResourceManager::GetTexture("dino1").Height/8*((GLfloat)width/1024);
             GLfloat y = (height/2-size.y/2)-(((boxData[0][i]%10)-5)*size.y)+dino_height+(size.y-dino_height)/2;

             this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y), size)));

             if ((boxData[0][i] > 10 && boxData[0][i] < 20) || boxData[0][i] > 30){   // Fern 
                 tex = ResourceManager::GetTexture("fern");
                 this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y-size.y), size)));
             }
             if (boxData[0][i] > 20){   // Vine 
                 tex = ResourceManager::GetTexture("vine");
                 this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y), size)));
             }
         }
     }
}

// void GameLevel::initMenu(GLfloat width, GLfloat height)
// {
//     Texture2D tex = ResourceManager::GetTexture("button");
//     width /= 3;
//     height /= 2;
//     GLfloat b_width, b_height, x, y, pas_y;
//     b_width = width;
//     b_height = (1.0/3)*height;
//     x = width*2.5 - b_width/2;
//     y = height - b_height/2;
//     pas_y = (1.0/3 * height)/2;

//     this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y + pas_y), glm::vec2(b_width, b_height))));
//     this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm::vec2(x,y + pas_y*3), glm::vec2(b_width, b_height))));
//     this->Obj.push_back(std::unique_ptr<GameObject>(new Object2D(tex, glm:"zéaq'":vec2(x,y + pas_y*5), glm::vec2(b_width, b_height))));
// }
void GameLevel::initMenu(GLfloat width, GLfloat height)
{
    GLfloat b_width, b_height, x, y, b_x, b_y, pas_y, size_factor;
    b_width = 0.75*(width/3);
    b_height = 0.65*(1.0/3)*(height/2);
    x = (width/3)*2.25;
    y = 1.05*(height/2);
    b_x = x - b_width/2;
    b_y = y - b_height/2;
    pas_y = (1.0/3 * height)/4.6;
    // The font size is adapted for a 1920x1080 screen, adapt it (the screen size choosen is relatively big since text get pixelized if they're to much enlarged)
    size_factor = (GLfloat)width/1920;

    this->Squares.push_back(Square(glm::vec2(b_x,b_y + pas_y), glm::vec2(b_width, b_height), glm::vec3(0.0f,0.6f,1.0f), glm::vec3(0.0f,0.1f,0.2f), V_GRAD));
    this->Texts.push_back(Text("Nouvelle Partie", glm::vec2(x,y + pas_y), size_factor));
    
    this->Squares.push_back(Square(glm::vec2(b_x,b_y + pas_y*3), glm::vec2(b_width, b_height), glm::vec3(0.6f,0.6f,0.6f), glm::vec3(0.2f,0.2f,0.2f), V_GRAD));
    this->Texts.push_back(Text("Continuer", glm::vec2(x,y + pas_y*3), size_factor, glm::vec3(0.2f)));

    this->Squares.push_back(Square(glm::vec2(b_x,b_y + pas_y*5), glm::vec2(b_width, b_height), glm::vec3(0.0f,0.6f,1.0f), glm::vec3(0.0f,0.1f,0.2f), V_GRAD));
    this->Texts.push_back(Text("Quitter", glm::vec2(x,y + pas_y*5), size_factor));

    this->Texts.push_back(Text("RAPTOR", glm::vec2(width/4.25, height/6), size_factor*3, glm::vec3(0.3f,0.3f,1.0f)));
    this->Texts.push_back(Text("SIMULATOR", glm::vec2(width/4.25, height/6+50*size_factor*3), size_factor*2, glm::vec3(0.3f,0.3f,1.0f)));

    this->T_renderer.reset(new TextRenderer(width, height));
    this->T_renderer->Load("fonts/Futura_Bold_Font/a_FuturaOrto-Bold_2258.ttf",50);
    this->Select(0);
}


std::vector<std::vector<GLint>> GameLevel::load(const GLchar *file)
{
    // Clear old data
    this->Obj.clear();
    this->Squares.clear();
    this->Texts.clear();
    // Load from file
    GLuint boxCode;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<GLint>> boxData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<GLint> row;
            while (sstream >> boxCode) // Read each word seperated by spaces
                row.push_back(boxCode);
            boxData.push_back(row);
        }
    }
    return boxData;
}

//MENU ONLY : Active Continue button
void GameLevel::ActiveContinue(){
	this->Squares[1].Color1 = glm::vec3(0.0f,0.6f,1.0f);
	this->Squares[1].Color2 = glm::vec3(0.0f,0.1f,0.2f);
	this->Texts[1].Color = glm::vec3(0);
}

//MENU ONLY : Change the selected button depending on the arrow pressed and the previous selected button
void GameLevel::ChangeButton(GLboolean up){
	//First button selected
	if (this->Texts[0].Size_factor > this->Texts[1].Size_factor){
		if(up)
			return;
		this->Deselect(0);
		if(this->Texts[1].Color.x == 0.2f) // button continue deactivated
			this->Select(2);
		else
			this->Select(1);
	}
	//Second button selected
	else if (this->Texts[1].Size_factor > this->Texts[2].Size_factor){
		this->Deselect(1);
		if(up)
			this->Select(0);
		else
			this->Select(2);
	}
	//Last button selected
	else{
		if(!up)
			return;
		this->Deselect(2);
		if(this->Texts[1].Color.x == 0.2f) // button continue deactivated
			this->Select(0);
		else
			this->Select(1);
	}
}

//MENU ONLY : select a button
void GameLevel::Select(GLuint n){
	glm::vec2 new_size = this->Squares[n].Size * 1.2f;
	this->Squares[n].Position -= (new_size-this->Squares[n].Size)*0.5f;
	this->Squares[n].Size = new_size;
	std::swap(this->Squares[n].Color1, this->Squares[n].Color2);
	this->Texts[n].Size_factor *= 1.2f;
	this->Texts[n].Color = glm::vec3(1);
}

//MENU ONLY : deselect a button
void GameLevel::Deselect(GLuint n){
	glm::vec2 new_size = this->Squares[n].Size / 1.2f;
	this->Squares[n].Position += (this->Squares[n].Size-new_size)*0.5f;
	this->Squares[n].Size = new_size;
	std::swap(this->Squares[n].Color1, this->Squares[n].Color2);
	this->Texts[n].Size_factor /= 1.2f;
	this->Texts[n].Color = glm::vec3(0);
}

void GameLevel::AddCircles(GLuint width, GLuint height, glm::vec2 cam_pos){
	glm::vec2 pos, size, velocity;
	glm::vec3 color;
	GLfloat screen_factor = width/1920;
	
	for (GLuint i = 0 ; i < 50 ; i++){
		for (GLuint j = 0 ; j < 4 ; j++){
			switch (i){
				case 0 : //create circles to the left
					pos.x = rand() %-50 -200;
					pos.y = rand() %(100+height) -100;
					break;
				case 1 : //create circles to the right
					pos.x = rand() %(width+200) + width+50;
					pos.y = rand() %(100+height) -100;
					break;
				case 2 : //create circles up
					pos.x = rand() %(100+width) -100;
					pos.y = rand() %-50 -200;
					break;
				default : //create circles down
					pos.x = rand() %(100+width) -100;
					pos.y = rand() %(200+height) + height+50;
			}
			size = glm::vec2(rand() %150 +30);
			color = glm::vec3((GLfloat) rand() / RAND_MAX, (GLfloat) rand() / RAND_MAX, (GLfloat) rand() / RAND_MAX);
			velocity = glm::normalize(glm::vec2(width/2, height/2) - pos) * (GLfloat)(rand()%200 +125);

			this->Squares.push_back(Square((pos+cam_pos)*screen_factor, size*screen_factor, color, glm::vec3(1), RADIAL_GRAD, velocity*screen_factor, GL_TRUE));
		}
	}
}