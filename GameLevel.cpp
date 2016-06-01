/*******************************************************************
** Taken and adapted from learnopengl.com (part of a Breakout game) 
******************************************************************/
#include "game_level.h"

#include <fstream>
#include <sstream>
#include <string>

#define RandRange(max, min) (std::uniform_int_distribution<>((min), (max))(gen))
#define fRandRange(max, min) ((GLfloat)std::uniform_real_distribution<>((min), (max))(gen))

//Static init
std::random_device  GameLevel::rd;
std::mt19937		GameLevel::gen(rd());


GameLevel::GameLevel(std::unique_ptr<Tex> &bg) 
    : Bg(std::move(bg)) { }

std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>> GameLevel::Load(const GLchar *file)
{
    std::vector<std::vector<GLint>> boxData = this->load(file);
    if (boxData.size() > 0)
        return this->init(boxData);
    return std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>>();
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
    renderer.DrawSprite(manager, this->Obj[0]->Tex, this->Obj.size(), projection, view);
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

std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>> GameLevel::init(std::vector<std::vector<GLint>> boxData)
{
    // Calculate dimensions
    GLuint height = boxData.size();
    GLuint width = boxData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    std::vector<glm::vec3> v, c; // will contain all the cube positions and colors for instanced rendering
    GLfloat size(1);
    GLint min = boxData[0][0], max = min;

    // Initialize level boxs based on boxData
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // Check block type from level data (2D level array)
            if (boxData[y][x] > 0 && boxData[y][x] < 10) // Red Blocks
            {
                glm::vec3 pos(x*size,(boxData[y][x]-5)*(size/2),-(y*size));
    			this->Obj.push_back(std::unique_ptr<GameObject>(new Object3D(pos, glm::vec3(size), ResourceManager::GetTexture("fence"), glm::vec3(1.0f, 0.0f, 0.0f))));
                v.push_back(pos);
                c.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
                if (boxData[y][x] > max)
                	max = boxData[y][x];
                if (boxData[y][x] < min)
                	min = boxData[y][x];
            }
        }
    }
    // Add random boxes
    glm::vec3 rainbow[] = {
    	glm::vec3(1.0f,0.0f,0.0f), //RED
    	glm::vec3(1.0f,0.5f,0.0f), //ORANGE
    	glm::vec3(1.0f,1.0f,0.0f), //YELLOW
    	glm::vec3(0.0f,1.0f,0.0f), //GREEN
    	glm::vec3(0.0f,1.0f,1.0f), //CYAN
    	glm::vec3(0.0f,0.0f,1.0f), //BLUE
    	glm::vec3(0.5f,0.0f,1.0f) //VIOLET
    };
    for (GLuint i = 0; i < 100000; ++i){
        glm::vec3 pos(RandRange(1000, -1000)*size,(RandRange(max,min)-5)*(size/2),RandRange(1000, -1000)*size);
        glm::vec3 color(rainbow[RandRange(6,0)]);
    	this->Obj.push_back(std::unique_ptr<GameObject>(new Object3D(pos, glm::vec3(size), ResourceManager::GetTexture("fence"), color)));
        v.push_back(pos);
        c.push_back(color);
    }
    return std::pair<std::vector<glm::vec3>,std::vector<glm::vec3>>(v,c);
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
    fstream.close();
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

void GameLevel::AddCircles(GLint width, GLint height, glm::vec2 cam_pos){
	glm::vec2 pos, size, velocity;
	glm::vec3 color;
	GLfloat screen_factor = width/1920;
	
	for (GLuint i = 0 ; i < 4 ; i++){
		for (GLuint j = 0 ; j < 50 ; j++){
			size = glm::vec2(fRandRange(150, 30));
			switch (i){
				case 0 : //create circles to the left
					pos.x = fRandRange(-size.x, -(size.x+300));
					pos.y = fRandRange(-pos.x+height, pos.x);
					break;
				case 1 : //create circles to the right
					pos.x = fRandRange(width+300, width);
					pos.y = fRandRange(pos.x+height, -pos.x);
					break;
				case 2 : //create circles up
					pos.y = fRandRange(-size.y, -300-size.y);
					pos.x = fRandRange(-pos.y+width, pos.y);
					break;
				default : //create circles down
					pos.y = fRandRange(300+height, height);
					pos.x = fRandRange(pos.y+width, -pos.y);
			}
			color = glm::vec3((GLfloat) rand() / RAND_MAX, (GLfloat) rand() / RAND_MAX, (GLfloat) rand() / RAND_MAX);
			velocity = glm::normalize(glm::vec2(width/2, height/2) - pos) * fRandRange(500, 200);

			this->Squares.push_back(Square((pos+cam_pos)*screen_factor, size*screen_factor, color, glm::vec3(1), RADIAL_GRAD, velocity*screen_factor, GL_TRUE));
		}
	}
}