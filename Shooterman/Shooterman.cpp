// ConsoleApplication2.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

sf::Color getColor() {

	int color;
	srand(time(NULL));

	color = rand() % 10 + 1;

	switch (color) {
	case 1:
		return sf::Color::Blue;
	case 2:
		return sf::Color::Cyan;
	case 3:
		return sf::Color::Red;
	case 4:
		return sf::Color::Magenta;
	case 5:
		return sf::Color::White;
	case 6:
		return sf::Color::Yellow;
	default:
		return sf::Color::Green;
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
	window.setFramerateLimit(60);
	sf::CircleShape shape(50.f);
	shape.setPosition(200, 200);
	shape.setFillColor(sf::Color::Green);
	float velocityX = 5;
	float velocityY = 2;

	while (window.isOpen())
	{
		if ((shape.getPosition().x + shape.getRadius() * 2) > 500.f || shape.getPosition().x < 0)
		{
			velocityX *= -1;
			shape.setFillColor(getColor());
		}
		if ((shape.getPosition().y + shape.getRadius() * 2) > 500.f || shape.getPosition().y < 0)
		{
			velocityY *= -1;
			shape.setFillColor(getColor());
		}
		shape.setPosition(shape.getPosition() + sf::Vector2f(velocityX, velocityY));

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}