#include <stdbool.h>
#include <iostream>

#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "messages.h"
#include "func.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Platypus Scuffed Edition", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	window.setFramerateLimit(60);
	sf::Clock clock;
	sf::Event event;
 
	// enumeration for scene changes
	enum part { start, tutorial, transition, singleMulti, play, pause };
	// enumeration for objectType
	enum objectType {
		player_obj = 100,
		playerBullet_obj = 200,
		enemy_obj = 300,
		enemyBullet_obj = 400
	};

	// variables that'll be used inside the main game
	bool gas_1 = false;				// decides when the player object will reduce its velocityY value
	bool gas_2 = false;
	bool shoot_1 = false;			// decides when the player object will execute the shoot() function
	bool shoot_2 = false;
	int level = 0;					// as the name implies, to differentiate levels
	float currentPoint = 0;			// as the name implies, to save the point calculation result
	bool generateEnemy = false;		// decides whether to generate new enemies

	// variables that'll be used in the start menu
	int scene = start;				// decide what scene is being run
	int choice = 0;					// variables that will later change the scene in the start menu
	int stepTutorial = 1;			// saves the tutorial step that is being described

	// main game loop
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (scene) {
				case start:
				{
					if (event.key.code == sf::Keyboard::Up) {
						choice--;
						if (choice < 0)
							choice = 0;
					}
					if (event.key.code == sf::Keyboard::Down) {
						choice++;
						if (choice > 2)
							choice = 2;
					}
					if (event.key.code == sf::Keyboard::Enter) {
						if (choice == 0) {
							scene = singleMulti;
						}
						if (choice == 1) {
							scene = tutorial;
						}
						if (choice == 2) {
							window.close();
						}
						choice = 0;
					}
					break;
				}
				case tutorial:
				{
					if (event.key.code == sf::Keyboard::Enter) {
						stepTutorial++;
					}
					break;
				}
				case singleMulti:
				{
					if (event.key.code == sf::Keyboard::Up) {
						choice--;
						if (choice < 0)
							choice = 0;
					}
					if (event.key.code == sf::Keyboard::Down) {
						choice++;
						if (choice > 1)
							choice = 1;
					}
					if (event.key.code == sf::Keyboard::Enter) {
						if (choice == 0) {
							player* Player = new player(1, 100, 100, 0, 0, 60, 29, 0.0002f);
							Player->setPlayerHp(100);
						}
						if (choice == 1) {
							player* Player_1 = new player(1, 100, 100, 0, 0, 60, 29, 0.0002f);
							Player_1->setPlayerHp(100);

							player* Player_2 = new player(2, 100, 100, 0, 0, 60, 29, 0.0002f);
							Player_2->setPlayerHp(100);
						}
						scene = transition;
						choice = 0;
					}
				}
				case play:
				{
					if (event.key.code == sf::Keyboard::Z) {
						gas_1 = true;
					}
					if (event.key.code == sf::Keyboard::X) {
						shoot_1 = true;
					}
					if (event.key.code == sf::Keyboard::C) {
						if (player::getObjectPtr(player_obj + 1)->getBulletCount() >= 30) {
							player::getObjectPtr(player_obj + 1)->resetBulletCount();
						}
					}

					if (event.key.code == sf::Keyboard::Comma) {
						gas_2 = true;
					}
					if (event.key.code == sf::Keyboard::Period) {
						shoot_2 = true;
					}
					if (event.key.code == sf::Keyboard::Slash) {
						if (player::getObjectPtr(player_obj + 2)->getBulletCount() >= 30) {
							player::getObjectPtr(player_obj + 2)->resetBulletCount();
						}
					}

					
					if (event.key.code == sf::Keyboard::Space) {
						scene = pause;
					}
					break;
				}
				case pause:
				{
					if (event.key.code == sf::Keyboard::Space) {
						scene = play;
						clock.restart();
					}
					break;
				}
				case transition:
				{
					if (event.key.code == sf::Keyboard::Enter && level != -1) {
						level++;
						generateEnemy = true; shoot_1 = false; shoot_2 = false; gas_1 = false; gas_2 = false;
						scene = play;
						//Player->setPosition(100, 100);	Player->setVelocity(0.0f, 0.0f);
						//Player->setPlayerHp(100);		Player->resetBulletCount();
					}
					if (event.key.code == sf::Keyboard::R && (level == -1 || level == 3)) {
						level = 0; shoot_1 = false; shoot_2 = false; gas_1 = false; gas_2 = false;
						//Player->setPosition(100, 100);	Player->setVelocity(0.0f, 0.0f);
						//Player->setPlayerHp(100);		Player->resetBulletCount();
						currentPoint = 0;
						scene = start;
					}
					break;
				}
				}
				break;

			case sf::Event::KeyReleased:
				switch (scene) {
				case play:
				{
					if (event.key.code == sf::Keyboard::Z) {
						gas_1 = false;
					}
					if (event.key.code == sf::Keyboard::X) {
						shoot_1 = false;
					}
					if (event.key.code == sf::Keyboard::Comma) {
						gas_2 = false;
					}
					if (event.key.code == sf::Keyboard::Period) {
						shoot_2 = false;
					}
					break;
				}
				}
				break;
			}
		}

		//outside pollEvent
		switch (scene) {
		case start:
		{
			window.draw(text::startMenuChoice(choice));
			break;
		}

		case tutorial:
		{
			sf::RectangleShape rectangle;
			rectangle.setSize(sf::Vector2f(960, 540));
			rectangle.setPosition(100, 100);
			sf::Texture tutorial;
			tutorial.loadFromFile("images/keyboardNjirr.jpg");
			rectangle.setTexture(&tutorial);
			window.draw(rectangle);

			window.draw(text::tutorialStep(stepTutorial));

			if (stepTutorial == 5)
			{
				scene = start;
				stepTutorial = 1;
			}
			break;
		}

		case singleMulti:
		{
			window.draw(text::singleMultiChoice(choice));
			break;
		}

		case transition:
		{
			switch (level) {
			case -1:
			{
				bullet::clearObject();
				enemy::clearObject();
				window.draw(text::lose());
				break;
			}
			case 0: window.draw(text::startLevel(1)); break;
			case 1: window.draw(text::startLevel(2)); break;
			case 2: window.draw(text::startLevel(3)); break;
			case 3: window.draw(text::win()); break;
			}
			break;
		}

		case pause:
		{
			window.draw(text::pauseText());
			break;
		}

		case play:
		{
			if (player::getObjectPtr(101) != NULL) {
				player* player_1 = player::getObjectPtr(101);
				if (gas_1)
					player_1->thrust();
				if (shoot_1 && player_1->getBulletCount() <= 30)
					player_1->shoot();
				if (player_1->getBulletCount() >= 30)
					window.draw(text::bulletEmpty(1));
			}

			if (player::getObjectPtr(102) != NULL) {
				player* player_2 = player::getObjectPtr(102);
				if (gas_2)
					player_2->thrust();
				if (shoot_2 && player_2->getBulletCount() <= 30)
					player_2->shoot();
				if (player_2->getBulletCount() >= 30)
					window.draw(text::bulletEmpty(2));
			}

			//pause the game if the window lost its focus
			if (!window.hasFocus()) {
				scene = pause;
			}


			//level mechanics and enemy object creation
			if (generateEnemy) {
				bullet::clearObject();
				clock.restart();
				switch (level) {
				case 1:
				{
					for (int i = 1; i < getRandomInteger(2, 4); i++) {
						enemy* Enemy = new enemy(i, getRandomFloat(400, 1280), getRandomFloat(0, 720), getRandomFloat(-0.3, 0.3), getRandomFloat(0.1, 0.3), 60, 29, 0.0f);
					}
					break;
				}
				case 2:
				{
					for (int i = 1; i < getRandomInteger(4, 7); i++) {
						enemy* Enemy = new enemy(i, getRandomFloat(400, 1280), getRandomFloat(0, 720), getRandomFloat(-0.6, 0.6), getRandomFloat(-0.6, 0.6), 60, 29, 0.0f);
					}
					break;
				}
				case 3:
				{
					for (int i = 1; i < getRandomInteger(7, 11); i++) {
						enemy* Enemy = new enemy(i, getRandomFloat(400, 1280), getRandomFloat(0, 720), getRandomFloat(-0.9, 0.9), getRandomFloat(-0.9, 0.9), 60, 29, 0.0f);
					}
					break;
				}
				}
				generateEnemy = false;
				break;
			}

			//level up when the enemy is 0
			if (enemy::getEnemyMap()->empty()) {
				scene = transition;
				break;
			}

			//enemy's attack algorithm
			std::unordered_map<int, enemy*>* enemyMap = enemy::getEnemyMap();
			std::unordered_map<int, player*>* playerMap = player::getPlayerMap();
			for (auto enemy_object = enemyMap->begin(); enemy_object != enemyMap->end(); enemy_object++) {
				enemy* Enemy = enemy_object->second;
				for (auto player_object = playerMap->begin(); player_object != playerMap->end(); player_object++) {
					player* Player = player_object->second;
					if ((Enemy->getPositionY() < Player->getPositionY() + 5) &&
						(Enemy->getPositionY() > Player->getPositionY() - 5)) {
						Enemy->shoot();
					}
				}
			}

			//collision detection and object removal
			currentPoint += processCollision();

			if (player::getPlayerMap()->empty()) {
				level = -1;
				scene = transition;
				break;
			}

			//update & draw
			double dt = clock.restart().asSeconds() * 1500;
			player::updateNDrawAllObject(dt, window);
			enemy::updateNDrawAllObject(dt, window);
			bullet::updateNDrawAllObject(dt, window);

			window.draw(text::score(currentPoint));
			break;
		}
		}
		window.display();
		window.clear(sf::Color(255, 255, 255));
	}
	return 0;
}