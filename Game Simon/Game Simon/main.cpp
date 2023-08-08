#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "cube.h"

using namespace sf;

void add(std::vector<int>& lit, int& count);

int main()
{
	//Переменные состояния
	bool registr = true;
	bool startPlay = false;
	bool play = false;
	bool light = false;
	bool lose = false;

	float lightTime = 0.25;
	float lightTimer = 0;
	float allTimer = 0;

	int count = 0;
	int maxScore = 0;
	int score = 0;

	std::ifstream read;
	std::ofstream file;

	// Реализация регистрации
	if (registr)
	{
		int choice;
		std::string name, password;

		std::cout << "1 - Register\t 2 - Login\n Your choice: "; std::cin >> choice;

		if (choice == 1)
		{
			std::cout << "Enter your name: "; std::cin >> name;
			std::cout << "Enter your password: "; std::cin >> password;

			file.open("D:\\PR\\Project\\Visual Studio (ready)\\Game Simon\\Data\\" + name + ".txt");
			file << name << std::endl << password;
			file.close();

			main();
		}

		else if (choice == 2)
		{
			bool status;
			std::string name, password, nameFile, passwordFile;

			std::cout << "Enter your name: "; std::cin >> name;
			std::cout << "Enter your password: "; std::cin >> password;

			read.open("D:\\PR\\Project\\Visual Studio (ready)\\Game Simon\\Data\\" + name + ".txt");
			getline(read, nameFile);
			getline(read, passwordFile);

			if (nameFile == name && passwordFile == password)
				status = true;

			else
				status = false;
			
			if (status)
			{
				std::cout << "Succesfully logged in!";
				registr = false; 
				startPlay = true;
			}

			else
			{
				int rest;
				std::cout << "False Login!" << std::endl << "1 - Restart" << std::endl; std::cin >> rest;

				if (rest == 1)
				{
					system("cls");
					main();
				}

				else
				{
					return 1;
				}
			}
		}
	}

	//Настройки окна
	ContextSettings settings;
	settings.antialiasingLevel = 6;
	
	//Иконка окна
	Image icon;
	icon.loadFromFile("Image\\GameLogo.png");

	RenderWindow window(VideoMode(1920, 1080), "Simon game", Style::Close, settings);
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	Clock clock;

	//Размер окна
	int sizeX = window.getSize().x;
	int sizeY = window.getSize().y;
	Vector2f coord(window.getSize());

	srand(time(0));

	//Порядок загорания ламп
	std::vector<int> lit;
	lit.push_back((rand() % 4) + 1);
	add(lit, count);
	add(lit, count);
	std::vector<int>::iterator it = lit.begin();

	//Создание кубов
	Cube cube1(sizeX, sizeY);
	Cube cube2(sizeX, sizeY);
	Cube cube3(sizeX, sizeY);
	Cube cube4(sizeX, sizeY);

	//Кнопки уровня сложности
	RectangleShape easyButt(Vector2f(300, 100));
	easyButt.setOrigin(easyButt.getSize().x / 2, easyButt.getSize().y / 2);
	easyButt.setFillColor(Color(255, 255, 255));
	easyButt.setPosition(coord.x - 960, coord.y - 800);

	RectangleShape normalButt(Vector2f(300, 100));
	normalButt.setOrigin(normalButt.getSize().x / 2, normalButt.getSize().y / 2);
	normalButt.setFillColor(Color(255, 255, 255));
	normalButt.setPosition(coord.x - 960, coord.y - 600);

	Font font;
	font.loadFromFile("Font\\Tektur.ttf");

	//Все тексты в игре
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

	Text difficultyText("Choose difficulty", font, 70);
	difficultyText.setOrigin(difficultyText.getGlobalBounds().width / 2, difficultyText.getGlobalBounds().height / 2);
	difficultyText.setFillColor(Color(255, 255, 255, 200));
	difficultyText.setPosition(coord.x - 960, coord.y - 1000);

	Text easyText("Easy", font, 30);
	Text normalText("Normal", font, 30);
	Text hardText("Hard", font, 30);

	FloatRect easyBounds = easyText.getGlobalBounds();
	FloatRect normalBounds = normalText.getGlobalBounds();
	FloatRect hardBounds = hardText.getGlobalBounds();

	easyText.setOrigin(easyBounds.width / 2, easyBounds.height / 2);
	easyText.setPosition(easyButt.getPosition());
	easyText.setFillColor(Color(0, 0, 0));

	normalText.setOrigin(normalBounds.width / 2, normalBounds.height / 2);
	normalText.setPosition(normalButt.getPosition());
	normalText.setFillColor(Color(0, 0, 0));

	while (window.isOpen())
	{
		Event event;

		float timer = clock.getElapsedTime().asSeconds();
		Time time = clock.restart();
		lightTimer += timer;
		if (!lose && !startPlay && !registr)
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
					if (!play && !lose && !startPlay)
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
					if (startPlay)
					{
						if (easyButt.getGlobalBounds().contains(pos.x, pos.y))
						{
							lightTime = 0.5;
							startPlay = false;
						}

						else if (normalButt.getGlobalBounds().contains(pos.x, pos.y))
						{
							lightTime = 0.25;
							startPlay = false;
						}
					}

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
								//++score;
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
								//++score;
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
								//++score;
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
								//++score;
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

							++score;
						}
					}
				}
			}
		}

		if (lightTimer > lightTime && light)
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

		if (registr)
		{

		}

		else if (startPlay)
		{
			window.draw(difficultyText);
			window.draw(easyButt);
			window.draw(easyText);
			window.draw(normalButt);
			window.draw(normalText);
		}

		else if (lose)
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