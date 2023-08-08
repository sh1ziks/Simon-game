#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Cube
{
public:
	static int count;
	RectangleShape shape;
	int num;
	Cube(int sizeX, int sizeY);

private:

};

int Cube::count = 0;

Cube::Cube(int sizeX, int sizeY)
{
	++count;
	num = count;

	shape.setSize(Vector2f(300, 300));

	switch (num)
	{
	case 1:
		shape.setFillColor(Color(255, 0, 0));
		shape.setPosition(sizeX / 3.5, sizeY / 6);
		break;

	case 2:
		shape.setFillColor(Color(0, 255, 0));
		shape.setPosition((sizeX / 3.5) * 2, sizeY / 6);
		break;

	case 3:
		shape.setFillColor(Color(0, 0, 255));
		shape.setPosition(sizeX / 3.5, (sizeY / 6) * 3);
		break;

	case 4:
		shape.setFillColor(Color(255, 255, 0));
		shape.setPosition((sizeX / 3.5) * 2, (sizeY / 6) * 3);
		break;
	}
}