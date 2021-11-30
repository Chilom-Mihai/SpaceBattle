#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Game.h"
#include "Player.h"
using namespace sf;

int main()
{
    RenderWindow window(sf::VideoMode(1920, 1080), "Space Battle",Style::Default);
	Clock clock;
	float dt = 0.f;

	Game game(&window);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				window.close();
		}

		dt = clock.restart().asSeconds();
		game.Update(dt);
		game.Draw();
	}

    return 0;
}