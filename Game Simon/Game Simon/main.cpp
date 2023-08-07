#include <SFML/Graphics.hpp>
#include <vector>
#include <sstream>
#include "cube.h"

using namespace sf;

void add(std::vector<int>& lit, int& count);

int main()
{
	ContextSettings settings;
	settings.antialiasingLevel = 6;

	RenderWindow window(VideoMode(1920, 1080), "Simon", Style::Close, settings);

	Clock clock;

	int sizeX = window.getSize().x;
	int sizeY = window.getSize().y;
	Vector2f coord(window.getSize());

	srand(time(0));

	bool play = false;
	bool light = false;
	bool lose = false;
	float lightTimer = 0;
	float allTimer = 0;
	int count = 0;
	int score = 0;

	std::vector<int> lit;
	lit.push_back((rand() % 4) + 1);
	add(lit, count);
	add(lit, count);
	std::vector<int>::iterator it = lit.begin();

	Cube cube1(sizeX, sizeY);
	Cube cube2(sizeX, sizeY);
	Cube cube3(sizeX, sizeY);
	Cube cube4(sizeX, sizeY);

	Font font;
	font.loadFromFile("Font\\Tektur.ttf");

	Text guideText("\t\tSimon game\n1 - Start\tEscape - Exit", font, 50);
	guideText.setFillColor(Color(255, 255, 255));
	guideText.setPosition(coord.x - 1225, coord.y - 1070);

	Text loseText("You lose!", font, 200);
	loseText.setFillColor(Color(255, 255, 255));
	loseText.setPosition(coord.x - 1400, coord.y - 750);

	Text scoreText("", font, 60);
	scoreText.setFillColor(Color(255, 255, 255));
	scoreText.setPosition(coord.x - 1900, coord.y - 100);

	Text timeText("", font, 60);
	timeText.setFillColor(Color(255, 255, 255));
	timeText.setPosition(coord.x - 1900, coord.y - 170);

	Text restartText("Press E for restart", font, 40);
	restartText.setFillColor(Color(255, 255, 255));
	restartText.setPosition(coord.x - 1150, coord.y - 500);

	while (window.isOpen())
	{
		Event event;

		float timer = clock.getElapsedTime().asSeconds();
		Time time = clock.restart();
		lightTimer += timer;
		if (!lose)
			allTimer += timer;

		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
					window.close();

				if (event.key.code == Keyboard::Num1 || event.key.code == Keyboard::Numpad1)
				{
					if (!play && !lose)
						light = true;
				}

				if (event.key.code == Keyboard::E && lose)
				{
					count = 0;
					lit.clear();

					lit.push_back((rand() % 4) + 1);
					add(lit, count);
					add(lit, count);
					it = lit.begin();

					play = false;
					light = false;
					lose = false;
					lightTimer = 0;
					allTimer = 0;
					score = 0;
				}
			}

			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
				{
					if (play)
					{
						switch (*it)
						{
						case 1:
							if (cube1.shape.getGlobalBounds().contains(pos.x, pos.y))
							{
								++it;
								lightTimer = 0;
								cube1.shape.setFillColor(Color(255, 150, 150));
								++score;
							}

							else
							{
								play = false;
								lose = true;
							}
							break;

						case 2:
							if (cube2.shape.getGlobalBounds().contains(pos.x, pos.y))
							{
								++it;
								lightTimer = 0;
								cube2.shape.setFillColor(Color(150, 255, 150));
								++score;
							}

							else
							{
								play = false;
								lose = true;
							}
							break;

						case 3:
							if (cube3.shape.getGlobalBounds().contains(pos.x, pos.y))
							{
								++it;
								lightTimer = 0;
								cube3.shape.setFillColor(Color(150, 150, 255));
								++score;
							}

							else
							{
								play = false;
								lose = true;
							}
							break;

						case 4:
							if (cube4.shape.getGlobalBounds().contains(pos.x, pos.y))
							{
								++it;
								lightTimer = 0;
								cube4.shape.setFillColor(Color(255, 255, 200));
								++score;
							}

							else
							{
								play = false;
								lose = true;
							}
							break;
						}

						if (it == lit.end())
						{
							play = false;
							add(lit, count);
							it = lit.begin();
						}
					}
				}
			}
		}


		if (lightTimer > 0.5 && light)
		{
			cube1.shape.setFillColor(Color(255, 0, 0));
			cube2.shape.setFillColor(Color(0, 255, 0));
			cube3.shape.setFillColor(Color(0, 0, 255));
			cube4.shape.setFillColor(Color(255, 255, 0));


			switch (*it)
			{
			case 1:
				cube1.shape.setFillColor(Color(255, 150, 150));
				break;

			case 2:
				cube2.shape.setFillColor(Color(150, 255, 150));
				break;

			case 3:
				cube3.shape.setFillColor(Color(150, 150, 255));
				break;

			case 4:
				cube4.shape.setFillColor(Color(255, 255, 200));
				break;
			}

			++it;
			if (it == lit.end())
			{
				light = false;
				play = true;

				it = lit.begin();
			}
			lightTimer = 0;
		}

		if (!light && lightTimer > 0.25)
		{
			cube1.shape.setFillColor(Color(255, 0, 0));
			cube2.shape.setFillColor(Color(0, 255, 0));
			cube3.shape.setFillColor(Color(0, 0, 255));
			cube4.shape.setFillColor(Color(255, 255, 0));
		}

		std::ostringstream scoreString, timeString;
		scoreString << score; timeString << allTimer;
		scoreText.setString("Score: " + scoreString.str());
		timeText.setString("Time: " + timeString.str());

		window.clear();
		if (lose)
		{
			window.draw(loseText);
			window.draw(scoreText);
			window.draw(restartText);
			window.draw(timeText);
		}

		else
		{
			window.draw(guideText);
			window.draw(scoreText);
			window.draw(timeText);
			window.draw(cube1.shape);
			window.draw(cube2.shape);
			window.draw(cube3.shape);
			window.draw(cube4.shape);
		}
		window.display();
	}
}

void add(std::vector<int>& lit, int& count)
{
	int num = (rand() % 4) + 1;

	while (lit[count] == num)
	{
		num = (rand() % 4) + 1;
	}

	lit.push_back(num);
	++count;
}