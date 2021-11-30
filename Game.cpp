#include "Game.h"

enum textures
{
	player=0,laser01,missile01,mainGun01,enemy01
};
Game::Game(RenderWindow *window)
{

	this->window = window;
	this->window->setFramerateLimit(60);
	this->dtMultiplier = 60.f;

	//Init fonts
	this->font.loadFromFile("Fonts/Dosis-Light.ttf");

	//Init texture
	backgroundTexture.loadFromFile("Textures/Backgrounds/backgorund123.png");
	this->textures.push_back(Texture());
	this->textures[player].loadFromFile("Textures/ship.png");
	this->textures.push_back(Texture());
	this->textures[laser01].loadFromFile("Textures/Guns/rayTex01.png");
	this->textures.push_back(Texture());
	this->textures[missile01].loadFromFile("Textures/Guns/missileTex01.png");
	this->textures.push_back(Texture());
	this->textures[mainGun01].loadFromFile("Textures/Guns/gun01.png");
	this->textures.push_back(Texture());
	this->textures[enemy01].loadFromFile("Textures/enemyFollow.png");
	//Backgorund
	background = new Backgrounds(&this->backgroundTexture);
	//Init players
	this->players.push_back(Player(this->textures));
	this->players.push_back(Player(this->textures, Keyboard::I, Keyboard::K, Keyboard::J, Keyboard::L, Keyboard::O));
	this->playersAlive = this -> players.size();
	//Init enemys
	Enemy e1(
		&this->textures[enemy01], this->window->getSize(),
		Vector2f (0.f,0.f),
		Vector2f(-1.f,0.f),Vector2f(0.1f,0.1f),
		0,rand()%3+1,2,1);

	this->enemiesSaved.push_back(Enemy(e1));
	this->enemySpawnTimerMax = 40;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	
	

	
	this->InitUI();

}
Game::~Game()
{
	
	delete background;

}

//Init UI
void Game::InitUI()
{
	Text tempText;

		//Follow text init

		this->followPlayerTexts.setFont(font);
		this->followPlayerTexts.setCharacterSize(14);
		this->followPlayerTexts.setColor(Color::White);
		

		

		//Static text init
		this->staticPlayerTexts.setFont(font);
		this->staticPlayerTexts.setCharacterSize(14);
		this->staticPlayerTexts.setColor(Color::White);	

	
	this->playerExpBar.setSize(Vector2f(90.f, 10.f));
	this->playerExpBar.setFillColor(Color(0.f,30.f,200.f,200.f));



	this->enemyText.setFont(this->font);
	this->enemyText.setCharacterSize(14);
	this->enemyText.setFillColor(Color::White);


	this->gameOverText.setFont(this->font);
	this->gameOverText.setFillColor(Color::Yellow);
	this->gameOverText.setCharacterSize(40);
	this->gameOverText.setString("Game Over");
	this->gameOverText.setPosition(this->window->getSize().x /2 -100.f, this->window->getSize().y /2);
}


void Game::UpdateUIPlayer(int index)
{
	
	//Fallow text

	
		this->followPlayerTexts.setPosition(this->players[index].getPosition().x, this->players[index].getPosition().y - 22.f);
		this->followPlayerTexts.setString(std::to_string(this->players[index].getPlayerNr()) + "				" + this->players[index].getHpAsString() + " \n \n \n \n \n \n " + std::to_string(this->players[index].getLevel()));


		//Exp Bar
		this->playerExpBar.setPosition(this->players[index].getPosition().x + 25.f, this->players[index].getPosition().y + 80.f);
		this->playerExpBar.setScale((static_cast<float>(this->players[index].getExp())/ this->players[index].getExpNext()),1.f);


		
	
}


void Game::UpdateUIEnemy(int index)
{
	
	this->enemyText.setPosition(this->enemies[index].getPosition());
	this->enemyText.setString(std::to_string(this->enemies[index].getHP()) + "/" + std::to_string(this->enemies[index].getHPMax()));

}



void Game::Update(const float &dt)
{
	if (this->playersAlive>0)
	{
		//Update timers
		if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		{
			this->enemySpawnTimer+=2.f*dt *this-> dtMultiplier;
		}
		//Spawn enemies
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			this->enemies.push_back(Enemy(&this->textures[enemy01], this->window->getSize(),
				Vector2f(0.f, 0.f),
				Vector2f(-1.f, 0.f), Vector2f(0.1f, 0.1f),
				0, rand() % 3 + 1, 3, 1));
			//Reset timer
			this->enemySpawnTimer = 0;
		}
		for (size_t i = 0; i < this->players.size(); i++)
		{
			if (this->players[i].isAlive())
			{
				//UPDATE PLAYERS
				this->players[i].Update(this->window->getSize(),dt);

				//bullet update/delete bullets
				for (size_t k = 0; k < this->players[i].getBullets().size(); k++)
				{
					this->players[i].getBullets()[k].Update(dt);


					//Enemy collision check
					for (size_t j = 0; j < this->enemies.size(); j++)
					{
						if (this->players[i].getBullets()[k].getGlobalBounds().intersects(this->enemies[j].getGlobalBounds()))
						{
							this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
							
							//Enemy take damage
							if (this->enemies[j].getHP() > 0)
								this->enemies[j].takeDamage(this->players[i].getDamage());

							//Enemy dead
							if (this->enemies[j].getHP() <= 0)
							{
								this->players[i].gainExp(this->enemies[j].getHPMax() + (rand()%this->enemies[j].getHPMax()+1));
								this->enemies.erase(this->enemies.begin() + j);
								
							}
								return;
						}

					}
					//Delete bullets from screen
					if (this->players[i].getBullets()[k].getPosition().x > this->window->getSize().x)
					{
						this->players[i].getBullets().erase(this->players[i].getBullets().begin() + k);
						return;
					}


				}


			}
		}
		//this->enemies[i].Update();
		
		for (size_t i = 0; i < this->enemies.size(); i++)
		{
			this->enemies[i].Update(dt);

			//Enemy player colision
			for (size_t k = 0; k < this->players.size(); k++)
			{
				if (this->players[k].isAlive())
				{
					if (this->players[k].getGlobalBounds().intersects(this->enemies[i].getGlobalBounds()))
					{
						
						int damage = this->enemies[i].getDamage();
						
						this->players[k].takeDamage(damage);
						
						this->textTags.push_back(
							TextTag( 
								&this->font,
								std::to_string(damage), 
								Color::Red, 
								Vector2f(this->players[k].getPosition().x + 20.f,
									this->players[k].getPosition().y - 20.f),
								20)
						);
						
						if (!this->players[k].isAlive())
						{
							this->playersAlive--;
						}
						this->enemies.erase(this->enemies.begin() + i);
						return;
					}
				}
			}

			if (this->enemies[i].getPosition().x < 0 - this->enemies[i].getGlobalBounds().width)
			{
				this->enemies.erase(this->enemies.begin() + i);
				return; //return
			}
		}
	}
}


void Game::DrawUI()
{


}


void Game::Draw()
{
	this->window->clear();

	background->Draw(*window);
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		this->enemies[i].Draw(*this->window);
		//UI
		this->UpdateUIEnemy(i);
		this->window->draw(this->enemyText);
	}
	for (size_t i = 0; i < this->players.size(); i++)
	{
		if (this->players[i].isAlive())
		{
		
				
				this->window->draw(this->followPlayerTexts);
				//Update UI
				this->UpdateUIPlayer(i);

				this->players[i].Draw(*this->window);
				this->window->draw(this->playerExpBar);
		}
	}

	

	//GAME OVER TEXT
	if (this->playersAlive <= 0)
	{

		this->window->draw(this->gameOverText);
	}




	this->window->display();
}