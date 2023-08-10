#include "func.h"


float getRandomFloat(float min, float max) {
    // Seed the random number generator with the current time
    static std::mt19937 rng(static_cast<unsigned>(std::time(0)));

    // Define the distribution for floating-point numbers
    std::uniform_real_distribution<float> dist(min, max);

    // Generate and return a random float number within the specified range
    return dist(rng);
}

int getRandomInteger(int min, int max) {
    // Define the random number generator engine
    std::random_device rd;
    std::default_random_engine rng(rd());

    // Define the uniform integer distribution using the specified range
    std::uniform_int_distribution<int> dist(min, max);

    return dist(rng);
}

int processCollision() {
	//collision detection and object removal
	std::unordered_map<int, bullet*>* bulletMap = bullet::getBulletMap();
	std::unordered_map<int, enemy*>* enemyMap = enemy::getEnemyMap();
	std::unordered_map<int, player*>* playerMap = player::getPlayerMap();

	//  something...collided with...something
	std::unordered_map<int, int> collideObject;
	for (auto bullet_object = bulletMap->begin(); bullet_object != bulletMap->end(); bullet_object++) {
		//bullets from the player
		if ((bullet_object->second != NULL) && (bullet_object->second->getVelocityX() > 0)) {
			for (auto enemy_object = enemyMap->begin(); enemy_object != enemyMap->end(); enemy_object++) {

				//collision happens between enemy and bullet
				if (enemy_object->second != NULL && object::isintersect(enemy_object->second->getSprite(), bullet_object->second->getSprite())) {
					//add the bullet's id and enemy's id to the map
					collideObject[enemy_object->first] = bullet_object->first;
				}
			}

			//collision happens between bullet and the right border
			if (bullet_object->second->getPositionX() >= 1280) {
				//add the bullet's id to the map
				collideObject[bullet_object->first] = 0;
			}
		}

		//bullets from the enemy
		else if (bullet_object->second != NULL && bullet_object->second->getVelocityX() < 0) {
			for (auto player_object = playerMap->begin(); player_object != playerMap->end(); player_object++){
				//collision happens between player and the bullet
				if (player_object->second != NULL && object::isintersect(player_object->second->getSprite(), bullet_object->second->getSprite())) {
					//add the bullet's id and player's id to the map
					collideObject[player_object->first] = (bullet_object->first);
				}
			}

			//collision happens between bullet and left border
			if (bullet_object->second->getPositionX() <= 0) {
				//add the bullet's id to the map
				collideObject[bullet_object->first] = 0;
			}
		}
	}
	
	int points = 0;
	for (const auto& it : collideObject) {
		if ((it.first - playerBullet_obj < 100 && it.first - playerBullet_obj > 0) ||
			(it.first - enemyBullet_obj < 100 && it.first - enemyBullet_obj > 0)) {
			bullet::deleteObject(it.first);
		}
		else if ((bullet::getObjectPtr(it.second) != NULL) &&
			(it.first - enemy_obj < 100 && it.first - enemy_obj > 0)) {
			enemy* Enemy = enemy::getObjectPtr(it.first);
			bullet* Bullet = bullet::getObjectPtr(it.second);
			Enemy->reduceHp(Bullet->getDamageValue());
			points += Bullet->getDamageValue();
			if (Enemy->getHp() <= 0)
				enemy::deleteObject(it.first);
			bullet::deleteObject(it.second);
		}
		else if ((bullet::getObjectPtr(it.second) != NULL) &&
			(it.first - player_obj < 100 && it.first - player_obj > 0)) {
			player* Player = player::getObjectPtr(it.first);
			bullet* Bullet = bullet::getObjectPtr(it.second);
			Player->reducePlayerHp(Bullet->getDamageValue());
			points -= Bullet->getDamageValue();
			if (Player->getPlayerHp() <= 0) {
				player::deleteObject(it.first);
			}
			bullet::deleteObject(it.second);
		}
	}
	return points;
}