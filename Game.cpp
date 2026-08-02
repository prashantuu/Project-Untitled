#include "Game.h"
#include <iostream>



void Game::startwindow()
{
	this->window = new RenderWindow(VideoMode(720, 960), "Space Shooter", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(165);
	this->window->setVerticalSyncEnabled(false); //upon true synchronizes your game's frame rate with monitor's refresh rate 

}

void Game::startbackground()
{
	if (!this->worldbackgroundtex.loadFromFile("Textures/background.jpg"))
		std::cout << "ERROR::GAME : BACKGROUND FILE NOT FOUND !!!!" << "\n";
	this->worldbackground.setTexture(this->worldbackgroundtex);
	this->worldbackground.setScale(2.f, 2.f);
	FloatRect localbounds = worldbackground.getLocalBounds();
	this->worldbackground.setOrigin(localbounds.width / 2.f, localbounds.height / 2.f);
	this->worldbackground.setPosition(this->window->getSize().x / 2.f, window->getSize().y / 2.f);
	this->worldbackground.rotate(90.f);
}

void Game::startsystems()
{
	this->points = 0;
}

void Game::starttextures()
{
	this->textures["BULLET"] = new Texture();
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");

	this->textures["DROIDS"] = new Texture();
	this->textures["DROIDS"]->loadFromFile("Textures/droid.png");

	this->textures["DROIDDED"] = new Texture();
	this->textures["DROIDDED"]->loadFromFile("Textures/droidded.png");

	this->textures["DEATHSTAR"] = new Texture();
	this->textures["DEATHSTAR"]->loadFromFile("Textures/deathstar.png");

	this->textures["CHARGE"] = new Texture();
	this->textures["CHARGE"]->loadFromFile("Textures/energy_charge_spritesheet_1.png");

	this->textures["LASER"] = new Texture();
	this->textures["LASER"]->loadFromFile("Textures/green_laser_elongate_spritesheet.png");

	

	


}

void Game::startstarship()
{
	this->starship = new Starship(this->window->getSize().x, this->window->getSize().y);
	 //New Pointer starship of Class type Starship

}

void Game::startBoss()
{
	this->boss = new Boss(*this->textures["DEATHSTAR"], *this->textures["CHARGE"], *this->textures["LASER"],
		this->window->getSize().x, this->window->getSize().y); 

}

void Game::startdroids()
{
	currentWave = 1;
	droidsSpawnedThisWave = 0;
	droidsAliveThisWave = 0;
	waveState = WaveState::Spawning;
	waveSpawnTimer = 0.f;
}

void Game::startGUI()
{

	this->levelFinishedText.setFont(this->font);
	this->levelFinishedText.setCharacterSize(55);   // fits comfortably in 720px width
	this->levelFinishedText.setFillColor(Color::Green);
	this->levelFinishedText.setString("LEVEL FINISHED!!");

	sf::FloatRect textBounds = this->levelFinishedText.getLocalBounds();
	this->levelFinishedText.setOrigin(
		textBounds.left + textBounds.width / 2.f,
		textBounds.top + textBounds.height / 2.f
	);
	this->levelFinishedText.setPosition(
		this->window->getSize().x / 2.f - (textBounds.width + 100.f),
		this->window->getSize().y / 2.f
	);




	//Loading Fonts
	if (!this->font.loadFromFile("Fonts/font.ttf"))
		std::cout << "ERROR ::GAME : FAILED TO LOAD FONT" << '\n';

	//starting Point Texty
	this->pointText.setPosition(600.f, 20.f);
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(20);
	this->pointText.setFillColor(Color::White);
	this->pointText.setString("Testing");

	this->GameOVertext.setFont(this->font);
	this->GameOVertext.setCharacterSize(100);
	this->GameOVertext.setFillColor(Color::Red);
	this->GameOVertext.setString("GAME OVER !!");
	this->GameOVertext.setPosition(
		this->window->getSize().x / 2.f - this->GameOVertext.getGlobalBounds().width/2.f,
		this->window->getSize().y / 2.f - this->GameOVertext.getGlobalBounds().height / 2.f
	);




	//starting ship hp
	this->playerhpbar.setSize(Vector2f(300.f, 25.f));
	this->playerhpbar.setFillColor(Color::Green);
	this->playerhpbar.setPosition(Vector2f(20.f, 20.f));

	this->playerhpbarback = this->playerhpbar;
	this->playerhpbarback.setFillColor(Color(25, 25, 25, 200));
}

Game::Game() {
	this->startwindow();
	this->startbackground();
	this->startsystems();
	this->startGUI();
	this->whisper.playbgmusic();
	this->starttextures();
	this->startBoss();
	this->startstarship();
	this->startdroids();
	

}

Game::~Game() {
	
	
	delete this->window;
	delete this->starship;
	delete this->boss;
	
	// No need to delete loaded music as CPP gonna do it automatically as the music is normal variable
	//Delete Textures
	
	for (auto &i : this->textures) {
		delete i.second;
	}

	//Delete Bullets
	for (auto *i : this->bullets) {
		delete i;
	}

	//Delete Enemies
	for (auto* i : this->droids) {
		delete i;
	}
}


//Functions while Running the Game
void Game::run() {


	while (this->window->isOpen()) {
		this->updatePollEvents();

		if (this->starship->getHP() > 0 && !this->levelFinished)
			this->update();
		else
			this->animationManager.updateAll();   // let animations finish even after game/level ends

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
			if (e.mouseButton.button == sf::Mouse::Left /*&& this->starship->canattack()*/) {
				this->bullets.push_back(new Bullet(this->textures["BULLET"], this->starship->getPos().x + 52.f, this->starship->getPos().y - 47.f, 0.f, -10.f, 1.f));
				this->whisper.playplayershoot();
			}
		}
	}

}

void Game::updateInput()
{	
	

	if (this->starship->isEntering())
		return;   // no manual control until it finishes flying in

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

	//this part is at the updatepollevents
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
			/*--counter;*/

			/*std::cout << this->bullets.size() << std::endl;*/
		}

		++counter;
	}



	//// Use an iterator to safely traverse the vector while modifying it
	//for (auto it = this->bullets.begin(); it != this->bullets.end(); ) 
	//{
	//	// 1. Update the current bullet's position
	//	(*it)->update();

	//	// 2. Bullet Culling ( top of Screen )
	//	if ((*it)->getBounds().top + (*it)->getBounds().height < 0.f)
	//	{
	//		// Delete the allocated memory to prevent memory leaks
	//		delete *it; 
	//		
	//		// erase() removes the element and returns a valid iterator to the NEXT element
	//		it = this->bullets.erase(it); 
	//	}
	//	else
	//	{
	//		// Only move to the next element if we didn't erase anything
	//		++it; 
	//	}
	//}

	
}

void Game::updateworld()
{
}

void Game::updatecollision()
{
	//Left World Collision
	if (this->starship->getBounds().left < 0.f)
	{
		this->starship->setPosition(0.f, this->starship->getBounds().top);
	}

	//Right  World Collision
	if (this->starship->getBounds().left + this->starship->getBounds().width > this->window->getSize().x)
	{
		this->starship->setPosition(this->window->getSize().x - this->starship->getBounds().width, this->starship->getBounds().top);
	}

	//Down World Collision
	if (this->starship->getBounds().top + this->starship->getBounds().height > this->window->getSize().y)
	{
		this->starship->setPosition(this->starship->getBounds().left, this->window->getSize().y - this->starship->getBounds().height);
	}

	//UP World Collision
	if (this->starship->getBounds().top < 0.f )
	{
		this->starship->setPosition(this->starship->getBounds().left,0);
	}
	
}

//Update Wave System :
void Game::updatedroids()
{
	float dt = waveClock.restart().asSeconds();

	switch (waveState)
	{
	case WaveState::Spawning:
	{
		waveSpawnTimer += dt;
		int waveIndex = currentWave - 1;

		if (droidsSpawnedThisWave < waveDroidCounts[waveIndex] &&
			waveSpawnTimer >= waveSpawnIntervals[waveIndex])
		{
			waveSpawnTimer = 0.f;
			this->droids.push_back(new Droids(this->textures["DROIDS"],
				static_cast<float>(rand() % (int)this->window->getSize().x - 20.f), -100.f));
			droidsSpawnedThisWave++;
			droidsAliveThisWave++;
		}

		// Wave cleared: all spawned, and all dead/gone
		if (droidsSpawnedThisWave >= waveDroidCounts[waveIndex] && droidsAliveThisWave <= 0)
		{
			if (currentWave < totalWaves)
			{
				currentWave++;
				droidsSpawnedThisWave = 0;
				waveCooldownTimer = 0.f;
				waveState = WaveState::WaveCooldown;
			}
			else
			{
				waveState = WaveState::AllWavesCleared;
				this->boss->triggerSpawn();   // <-- boss appears now, see Boss changes below
			}
		}
		break;
	}

	case WaveState::WaveCooldown:
	{
		waveCooldownTimer += dt;
		if (waveCooldownTimer >= waveCooldownMax)
			waveState = WaveState::Spawning;
		break;
	}

	case WaveState::AllWavesCleared:
		break;   // nothing left to spawn; boss handles itself from here
	}

	// Updating + culling existing droids (unchanged logic, but decrement alive count)
	unsigned counter = 0;
	for (auto* droid : this->droids)
	{
		droid->update();

		if (droid->getBounds().top > this->window->getSize().y)
		{
			this->starship->loseHp(2.5);
			delete this->droids.at(counter);
			this->droids.erase(this->droids.begin() + counter);
			droidsAliveThisWave--;
		}

		//Left World Collision
		else if (droid->getBounds().left < 0.f)
		{
			delete this->droids.at(counter);
			this->droids.erase(this->droids.begin() + counter);
			droidsAliveThisWave--;
		}

		//Right  World Collision
		else if (droid->getBounds().left + droid->getBounds().width > this->window->getSize().x)
		{
			delete this->droids.at(counter);
			this->droids.erase(this->droids.begin() + counter);
			droidsAliveThisWave--;
		}

		else if (droid->getBounds().intersects(this->starship->getBounds()))
		{
			this->starship->loseHp(5);
			sf::FloatRect droidBounds = this->droids.at(counter)->getBounds();
			float centerX = droidBounds.left + droidBounds.width / 2.f;
			float centerY = droidBounds.top + droidBounds.height / 2.f;
			
			this->animationManager.spawn(*(this->textures["DROIDDED"]),
				centerX,
				centerY,
				7,      // frameCount
				0.01f,  // frameDuration
				0.1f, 0.1f,  // scaleX, scaleY
				false); // loop = false, it's a one-shot death effect
			delete this->droids.at(counter);
			this->whisper.playdroidded();
			this->droids.erase(this->droids.begin() + counter);
			droidsAliveThisWave--;
		}

		++counter;
	}
}

//NONE WAVE SYSTEM : 

//void Game::updatedroids()
//{
//	//Spawning Droids
//	this->spawnTimer += 0.5f;
//	if (this->spawnTimer >= this->spawnTimerMax)
//	{
//		this->droids.push_back(new Droids(this->textures["DROIDS"],rand() % this->window->getSize().x - 20.f, -100.f)); // Spawing of Enemies
//		this->spawnTimer = 0.f; 
//		
//	}
//		
//	//Updating Droids
//	unsigned counter = 0;
//	for (auto* droid : this->droids) {
//		droid->update();
//
//
//
//		if (droid->getBounds().top /*+ droid->getBounds().height*/ > this->window->getSize().y)
//		{
//
//			delete this->droids.at(counter);
//			this->droids.erase(this->droids.begin() + counter);
//			/*--counter;*/
//
//
//		}
//		// Enemy Colliding with Player
//		else if (droid->getBounds().intersects(this->starship->getBounds()))
//		{
//			this->starship->loseHp(2);
//			sf::FloatRect droidBounds = this->droids.at(counter)->getBounds();
//			float centerX = droidBounds.left + droidBounds.width / 2.f;
//			float centerY = droidBounds.top + droidBounds.height / 2.f;
//
//			this->animationManager.spawn(*(this->textures["DROIDDED"]),
//				centerX,
//				centerY,
//				7,      // frameCount
//				0.01f,  // frameDuration
//				0.1f, 0.1f,  // scaleX, scaleY
//				false); // loop = false, it's a one-shot death effect
//			delete this->droids.at(counter);
//			this->whisper.playdroidded();
//			this->droids.erase(this->droids.begin() + counter);
//			/*--counter;*/
//			
//		}
//
//		++counter;
//	}
//
//}


//HP Lose System :
void Game::updateCombat()
{
	for (int i = 0; i < this->droids.size(); ++i)
	{
		bool droidHitThisFrame = false;
		for (size_t k = 0; k < this->bullets.size() && !droidHitThisFrame; k++)
		{
			if (this->droids[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				this->droids[i]->takeDamage(this->bullets[k]->getDamage());

				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				if (this->droids[i]->isDead())
				{
					this->points += this->droids[i]->getPoints();

					sf::FloatRect db = this->droids[i]->getBounds();
					this->animationManager.spawn(*(this->textures["DROIDDED"]),
						db.left + db.width / 2.f, db.top + db.height / 2.f,
						7, 0.05f, 0.1f, 0.1f, false);

					delete this->droids[i];
					this->whisper.playdroidded();
					this->droids.erase(this->droids.begin() + i);
					this->droidsAliveThisWave--;
					i--;
				}

				droidHitThisFrame = true;
			}
		}
	}
}











//Instant Kill System :

//void Game::updateCombat() 
//{
//	
//	for (int i = 0; i < this->droids.size(); ++i)
//	{
//
//		bool droids_deleted = false;
//		for (size_t k = 0; k < this->bullets.size() && droids_deleted == false ; k++) {
//			if (this->droids[i]->getBounds().intersects(this->bullets[k]->getBounds()))
//			{
//
//				this->points += this->droids[i]->getPoints();
//				
//				
//				
//				sf::FloatRect droidBounds = this->droids[i]->getBounds();
//				float centerX = droidBounds.left + droidBounds.width / 2.f;
//				float centerY = droidBounds.top + droidBounds.height / 2.f;
//
//				this->animationManager.spawn(*(this->textures["DROIDDED"]),
//					centerX,
//					centerY,
//					7,      // frameCount
//					0.01f,  // frameDuration
//					0.1f, 0.1f,  // scaleX, scaleY
//					false); // loop = false, it's a one-shot death effect
//
//				delete this->droids[i];
//				this->whisper.playdroidded();
//				this->droids.erase(this->droids.begin() + i);
//
//				i--;
//
//				delete this->bullets[k];
//				
//				this->bullets.erase(this->bullets.begin() + k);
//
//				droids_deleted = true;
//			}
//		}
//	}
//}

void Game::updateGUI()
{
	std::stringstream ss;
	ss << "Points : "<<this->points;

	this->pointText.setString(ss.str());

	//update ship gui
	float hppercent = static_cast<float>(this->starship->getHP()) / this->starship->getHPmax();
	float hplength = static_cast<float>(this->playerhpbarback.getSize().x);

	this->playerhpbar.setSize(Vector2f( hplength  * hppercent, this->playerhpbarback.getSize().y));
}

void Game::update()
{
	
	

	this->updateInput();

	this->starship->update();
	
	//Boss Related Updates
	if (this->boss->consumeSpawnEvent())
	{
		this->whisper.stopbgmusic();   
		this->whisper.playbossmusic();
	}

	FloatRect shipBounds = this->starship->getBounds();
	Vector2f shipCenter(shipBounds.left + shipBounds.width / 2.f,
		shipBounds.top + shipBounds.height / 2.f);

	this->boss->update(shipCenter, shipBounds);
	if (this->boss->consumeChargeStartedEvent()) {
		this->boss->setChargeSoundDuration(this->whisper.getChargeSoundDuration());
		this->whisper.playbosscharge();
	}
	if (this->boss->consumeHitEvent())
		this->starship->loseHp(this->boss->getLaserDamage());

	// Bullets vs boss
	for (auto it = this->bullets.begin(); it != this->bullets.end(); )
	{
		if (this->boss->isActive() && this->boss->getBounds().intersects((*it)->getBounds()))
		{
			this->boss->takeDamage(/*(*it)->getDamage()*/10);

			sf::FloatRect hitBounds = (*it)->getBounds();
			float hitX = hitBounds.left + hitBounds.width / 2.f;
			float hitY = hitBounds.top + hitBounds.height / 2.f;

			this->animationManager.spawn(*(this->textures["DROIDDED"]),
				hitX, hitY-50,
				7, 0.05f,
				0.1f, 0.1f,
				false);

			delete* it;
			it = this->bullets.erase(it);
		}
		else ++it;
	}
	// Boss just died — spawn ONE big blast scaled to its size, and flag level complete
	if (this->boss->consumeDeathEvent())
	{
		sf::FloatRect bossBounds = this->boss->getBounds();
		float centerX = bossBounds.left + bossBounds.width / 2.f;
		float centerY = bossBounds.top + bossBounds.height / 2.f;

		float frameW = this->textures["DROIDDED"]->getSize().x / 7.f;
		float desiredWidth = bossBounds.width * 1.4f;   // a bit bigger than the boss itself
		float bigScale = desiredWidth / frameW;

		this->animationManager.spawn(*(this->textures["DROIDDED"]), centerX, centerY, 7, 0.08f, bigScale, bigScale, false);

		this->levelFinished = true;
	}






	this->updatecollision();

	this->updatebullets();
	this->updatedroids();

	this->updateCombat();
	this->animationManager.updateAll();
	  




	this->updateGUI();  //Do it arnd last so that all updated info is shown

	this->updateworld();
}

void Game::renderworld()
{
	this->window->draw(this->worldbackground);
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerhpbarback);
	this->window->draw(this->playerhpbar);
}

void Game::render()
{
	this->window->clear(); //Clear the previous frame

	//Draw Background
	this->renderworld();

	//Render all the stuffs
	this->starship->render(*this->window);
	
	this->boss->render(*this->window);

	for (auto* bullet : this->bullets) {
		bullet->render(this->window);  
	}

	for (auto* droid : this->droids) {
		droid->render(this->window);
	}


	//Render Explosion
	animationManager.renderAll(*this->window); 

	this->renderGUI(); 
	
	//Game Over Screen
	if (this->starship->getHP() <= 0)
		this->window->draw(this->GameOVertext);

	//Level Finished Screen
	if (this->levelFinished)
		this->window->draw(this->levelFinishedText);

	this->window->display(); // Display the frame
}
