#pragma once
#include"Bullet.h"
class Backgrounds
{
private:
	Texture *texture;
	Sprite sprite;





public:
	Backgrounds(Texture* texture);
	virtual ~Backgrounds();

	void Draw(RenderTarget& target);
};

