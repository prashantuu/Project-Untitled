#include "Game.h"
#include <iostream>



void Game::startwindow()
{
	this->window = new RenderWindow(VideoMode(720, 1280), "Space Shooter", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(165);
	this->window->setVerticalSyncEnabled(false); //upon true synchronizes your game's frame rate with monitor's refresh rate 

}

void Game::starttextures()
{
	this->textures["BULLET"] = new Texture();
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");

}

void Game::startstarship()
{
	this->starship = new Starship; //New Pointer starship of Class type Starship



}

Game::Game() {
	this->startwindow();
	this->starttextures();
	this->startstarship();

}

Game::~Game() {
	
	delete this->window;
	delete this->starship;

	//Delete Textures
	
	for (auto &i : this->textures) {
		delete i.second;
	}

	//Delete Bullets
	for (auto *i : this->bullets) {
		delete i;
	}

}


//Functions while Running the Game
void Game::run() {
	while (this->window->isOpen()) {  // Updates and Renders Frame everytime window is opened
		this->update();    
		this->render();
	}

}

void Game::updatePollEvents()
{
	Event e;
	while (this->window->pollEvent(e)) {
		if (e.Event::type == Event::Closed || e.Event::KeyPressed && e.Event::key.code == Keyboard::Escape) // Checks if window is close or escape is pressed to close window
			this->window->close();

		if (e.Event::type == Event::MouseButtonPressed) {
			if (e.mouseButton.button == sf::Mouse::Left) {
				this->bullets.push_back(new Bullet(this->textures["BULLET"], this->starship->getPos().x + 52.f, this->starship->getPos().y - 47.f, 0.f, -10.f, 1.f));
			}
		}
	}

}

void Game::updateInput()
{	
	

	

	//Move Player
	if (Keyboard::isKeyPressed(Keyboard::A))
		this->starship->move(-1.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::D))
		this->starship->move(1.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::W))
		this->starship->move(0.f, -1.f);
	if (Keyboard::isKeyPressed(Keyboard::S))
		this->starship->move(0.f, 1.f);


	/*if (e.Event::type == Event::MouseButtonPressed) {
		if (e.mouseButton.button == sf::Mouse::Left) {
			this->bullets.push_back(new Bullet(this->textures["BULLET"], this->starship->getPos().x + 50, this->starship->getPos().y - 65, 0.f, -10.f, 1.f));
		}
	}*/
}       

void Game::updatebullets()
{
	unsigned counter = 0;
	for (auto* bullet : this->bullets) {
		bullet->update();

		// Bullet Culling ( top of Screen )

		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			//Delete Bullet out of the screen
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}

		++counter;
	}

	
}

void Game::update()
{
	
	this->updatePollEvents();
	this->updateInput();
	this->updatebullets();


}

void Game::render()
{
	this->window->clear(); //Clear the previous frame

	//Render all the stuffs
	this->starship->render(*this->window);

	for (auto* bullet : this->bullets) {
		bullet->render(this->window);  
	}


	this->window->display(); // Display the frame
}
