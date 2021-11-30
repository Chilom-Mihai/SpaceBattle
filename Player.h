#pragma once
#include "Bullet.h"

class Player
{
private:
	unsigned playerNumber;

	float dtMultiplier;
	
	Vector2f playerCenter;
	Sprite sprite;
	RectangleShape hitbox;
	std::vector <Bullet> bullets;
	
	//Accesories
	
	Sprite mainGunSprite;
	//Texture* bulletTexture;
	Texture* missile01Texture;
	Texture* missile02Texture;
	Texture* laserTexture;

	float shootTimer;
	float shootTimerMax;
	float damageTimer;
	float damageTimerMax;
	int controls[5];
	Vector2f currentVelocity;
	float maxVelocity;
	float acceleration;
	Vector2f direction;
	float stablizerForce;
	int hp;
	int lvl;
	int exp;
	

	

	int damage;
	int damagemax;
	int hpmax;
	int expNext;




	
	int currentWeapon;
	//UPPGRADES
	int mainGunLevel;
	bool dualMissile01;
	bool dualMissile02;
public:

	Player(std::vector<Texture> &textures, int UP = Keyboard::W, int DOWN = Keyboard::S, int LEFT = Keyboard::A, int RIGHT = Keyboard::D, int SHOOT = Keyboard::Space);
	virtual ~Player();
	
	
	//Accessorys
	inline std::vector<Bullet>& getBullets()
	{
		return this->bullets;
	}
	inline const Vector2f& getPosition()const
	{
		return this->sprite.getPosition();
	}
	inline const String getHpAsString()const
	{
		return std::to_string(this->hp) + "/" + std::to_string(this->hpmax);
	}
	 int getDamage() const;
	
	inline FloatRect getGlobalBounds() const
	{
		return this->sprite.getGlobalBounds();
	}
	inline const int& getHp() const
	{
		return this->hp;
	}
	inline const int& getHpMax() const
	{
		return this->hpmax;
	}
	inline void takeDamage(int damage) 
	{
		this->hp -= damage;
	}
	inline bool isAlive() const
	{
		return this->hp > 0;	
	}
	inline const int& getPlayerNr() const
	{
		return this->playerNumber;
	}

	inline const int& getLevel() const
	{
		return this->lvl;
	}
	inline const int& getExp() const
	{
		return this->exp;
	}
	inline const int& getExpNext() const
	{
		return this->expNext;
	}
	inline void gainExp(int exp)  
	{
		 this->exp +=exp;
		 this->UpdateLeveling();
	}


	//Functions
	void UpdateLeveling();
	void Update(Vector2u windowBounds, const float& dt);
	void Draw(RenderTarget &target);
	void Move(const float& dt);
	void Combat(const float& dt);
	void UpdateAccessories(const float& dt);
	static unsigned players;


};

