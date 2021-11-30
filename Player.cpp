#include "Player.h"

unsigned Player::players = 1;
enum controls
{
	UP=0 ,DOWN,LEFT,RIGHT,SHOOT
};
enum weapons
{
	LASER = 0, MISSILE01
};

Player::Player(std::vector<Texture>& textures, int UP, int DOWN, int LEFT, int RIGHT, int SHOOT)
	:lvl(1),exp(0),
	hp(10),hpmax(10),
	damage(1),damagemax(2)
	
{
	//DT
	this->dtMultiplier = 60.f;

	//Exp gain
	this->expNext =20+ static_cast<int>((50 / 3) * ((pow(lvl, 3) - 6 * pow(lvl, 2)) + 17 * lvl - 12));
	



	//Update positions
	this->playerCenter.x = this->getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->getPosition().y + this->sprite.getGlobalBounds().height / 2;


	//Textures

	this->sprite.setTexture(textures[0]);
	this->sprite.setScale(0.13f, 0.13f);
	this->laserTexture = &textures[1];
	this->missile01Texture = &textures[2];
	this->mainGunSprite.setTexture(textures[3]);
	this->mainGunSprite.setOrigin(this->mainGunSprite.getGlobalBounds().width / 2, this->mainGunSprite.getGlobalBounds().height / 2);
	
	this->mainGunSprite.rotate(90);

	this->mainGunSprite.setPosition(this->playerCenter.x + 20.f, playerCenter.y);
	//Timers
	this->shootTimerMax = 25.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 10.f;
	this->damageTimer = this->damageTimerMax;
	
	//keys
	this->controls[controls::UP] = UP;
	this->controls[controls::DOWN] = DOWN;
	this->controls[controls::LEFT] = LEFT;
	this->controls[controls::RIGHT] = RIGHT;
	this->controls[controls::SHOOT] = SHOOT;
	//velocity,acc,stabilizer
	this->maxVelocity = 20.f;
	this->acceleration = 1.f;
	this->stablizerForce =0.3f;
	//Guns
	this->currentWeapon = LASER;
	
	//UPGRADES
	this->mainGunLevel = 0;
	this->dualMissile01 = false;
	this->dualMissile02 = false;

	
	//Nr of players

	this->playerNumber = Player::players;
	Player::players++;
	
}

Player::~Player()
{
	
}

int Player:: getDamage() const
{
	int damage = 0;
	switch (this->currentWeapon)
	{
		
	case LASER:

		damage = rand() % this->damagemax + this->damage;

		break;
	case MISSILE01:
		rand() % this->damagemax + this->damage;
		damage *= 2;

		break;
	
	default:

		damage = rand() % this->damagemax + this->damage;

		break;

	}

	return damage;
}

void Player:: UpdateLeveling()
{
	if (this->exp >= this->expNext)
	{
		this->lvl++;
		this->exp -= this->expNext;
		this->expNext = static_cast<int>((50 / 3) * ((pow(lvl, 3) - 6 * pow(lvl, 2)) + 17 * lvl - 12));
	}

}



void Player::UpdateAccessories(const float &dt)
{
	//Gun stay on player
	this->mainGunSprite.setPosition(this->mainGunSprite.getPosition().x,this->playerCenter.y);
		//Gun animation set back after firing
	if (this->mainGunSprite.getPosition().x < this->playerCenter.x + 20.f)
	{
		this->mainGunSprite.move(2.f*dt* this-> dtMultiplier+this->currentVelocity.x * dt * this->dtMultiplier, 0.f);
	}
	if (this->mainGunSprite.getPosition().x > this->playerCenter.x + 20.f)
	{
		this->mainGunSprite.setPosition(this->playerCenter.x + 20.f, playerCenter.y);
	}
}
void Player::Combat(const float& dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT])) && this->shootTimer >= this->shootTimerMax)
	{
		if (this->currentWeapon == LASER)
		{
			//Create Bullet
			if (this->mainGunLevel == 0)
			{
				this->bullets.push_back(Bullet(laserTexture, Vector2f(this->playerCenter.x + 50.f, this->playerCenter.y), Vector2f(0.2f, 0.2f), Vector2f(1.f, 0.f), 20.f, 60.f, 5.f));
			}

			//Gun animation when you shoot
			this->mainGunSprite.move(-20.f, 0.f);
		}
		//reset timer
		this->shootTimer = 0;
	}
	

}
void Player::Update(Vector2u windowBounds, const float& dt)
{
    //Update bullet timers
	if (this->shootTimer < this->shootTimerMax)
	{
		this->shootTimer += 1.f * dt * this->dtMultiplier;
	}
	if (this->damageTimer < this->damageTimerMax)
	{
		this->damageTimer += 1.f * dt * this->dtMultiplier;
	}

	//Update position
	
	this->Move(dt);
	this->Combat(dt);
	this->UpdateAccessories(dt);
}



void Player::Draw(RenderTarget &target)
{
	for (size_t i = 0; i < this->bullets.size(); i++)
	{
		this->bullets[i].Draw(target);
	}
	target.draw(this->sprite);
	target.draw(this->mainGunSprite);
}




void Player::Move(const float& dt)
{
	//UP
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP])))
	{
		this->direction.x = 0.f;
		this->direction.y = -1.f;

		if (this->currentVelocity.y > -this->maxVelocity && direction.y < 0)
			this->currentVelocity.y += direction.y * acceleration * dt * this->dtMultiplier;
	}
	//DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN])))
	{
		this->direction.x = 0.f;
		this->direction.y = 1.f;

		if (this->currentVelocity.y < this->maxVelocity && direction.y > 0)
			this->currentVelocity.y += direction.y * acceleration * dt * this->dtMultiplier;
	}
	//LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT])))
	{
		this->direction.x = -1.f;
		this->direction.y = 0.f;
		if (this->currentVelocity.x > -this->maxVelocity && direction.x < 0)
			this->currentVelocity.x += direction.x * acceleration * dt * this->dtMultiplier;

	}
	//RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT])))
	{
		this->direction.x = 1.f;
		this->direction.y = 0.f;
		if (this->currentVelocity.x < this->maxVelocity && direction.x>0)
			this->currentVelocity.x += direction.x * acceleration * dt * this->dtMultiplier;

	}

	//DRAG force
	if (this->currentVelocity.x > 0)
	{
		this->currentVelocity.x -= this->stablizerForce * dt * this->dtMultiplier;
		if (this->currentVelocity.x < 0)
			this->currentVelocity.x = 0;
	}
	if (this->currentVelocity.x < 0)
	{
		this->currentVelocity.x += this->stablizerForce * dt * this->dtMultiplier;
		if (this->currentVelocity.x > 0)
			this->currentVelocity.x = 0;
	}
	if (this->currentVelocity.y > 0)
	{
		this->currentVelocity.y -= this->stablizerForce * dt * this->dtMultiplier;
		if (this->currentVelocity.y < 0)
			this->currentVelocity.y = 0;
	}
	if (this->currentVelocity.y < 0)
	{
		this->currentVelocity.y += this->stablizerForce * dt * this->dtMultiplier;
		if (this->currentVelocity.y > 0)
			this->currentVelocity.y = 0;
	}
	//final move

	this->sprite.move(this->currentVelocity.x * dt * this->dtMultiplier, this->currentVelocity.y * dt * this->dtMultiplier);
	
	this->playerCenter.x = this->getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->getPosition().y + this->sprite.getGlobalBounds().height / 2;

}
