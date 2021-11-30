#pragma once

#include "Player.h"
#include"Enemy.h"
#include"Backgrounds.h"

class Game
{
private:
	RenderWindow* window;
	float dtMultiplier;
	Backgrounds* background;
	Texture backgroundTexture;
	
	//UI
	//Text
	Font font;
	Text followPlayerTexts;
	Text staticPlayerTexts;
	Text enemyText;
	Text gameOverText;

	//Bars
	RectangleShape playerExpBar;

	//Texttags
	std::vector<TextTag>textTags;


	//Players
	std::vector<Player> players;
	int playersAlive;

	//Enemies
	std::vector<Enemy> enemies;
	std::vector<Enemy> enemiesSaved;
	float enemySpawnTimer;
	float enemySpawnTimerMax;

	

	//Textures
	std::vector<Texture>textures;
	


public:
	Game(RenderWindow *window);

	virtual ~Game();

	//Accessories
	inline RenderWindow& getWindow()
	{
		return *this->window;
	}

	//Functions
	void InitUI();
	void UpdateUIPlayer(int index);
	void Update(const float& dt);
	void Draw();
	void DrawUI();
	void UpdateUIEnemy(int index);

	
};

