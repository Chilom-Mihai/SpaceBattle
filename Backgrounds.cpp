#include "Backgrounds.h"

Backgrounds::Backgrounds(Texture* texture)
{
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
}

Backgrounds::~Backgrounds()
{

}
void Backgrounds::Draw(RenderTarget& target)
{
	target.draw(this->sprite);
}