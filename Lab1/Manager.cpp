#pragma once
#include "GLIncludes.h"
#include <iostream>
#include "Sound.cpp"

// singleton instance
class Manager {
public:
	enum GameState {
		PLAYING, WON, LOST
	};

private:
	int health = TOTAL_HEALTH;
	bool hasPlayedPolice = false;

	Sound sfx;
	GameState gameState;

	// hide constructors
	Manager() {}
	~Manager() {}
	Manager(const Manager &);
	Manager & operator=(const Manager &);

	static Manager* instance;

public:
	static Manager& getInstance() {
		static Manager manager;
		return manager;
	}

	void setGameState(Manager::GameState gameState) { this->gameState = gameState; }
	void checkGameState(int cansLeft) {
		gameState = GameState::PLAYING;

		if (health < 1) {
			gameState = GameState::LOST;
		}
		if (cansLeft == 0 && health > 0) {
			gameState = GameState::WON;
		}

		if (DEV_SHOW_STATE) {
			std::cout << "Game state: " << gameState << ", "
				<< cansLeft << "/" << SPAWN_CAN_SIZE << ", "
				<< health << "/" << TOTAL_HEALTH << "("
				<< (int)(((float)health / (float)TOTAL_HEALTH) * 100) << "%)" << std::endl;
		}

		if (gameState == GameState::PLAYING) {
			decrementHealth();
		}

		if (gameState == GameState::LOST && !hasPlayedPolice) {
			sfx.playAudio(Sound::sfx, Sound::POLICE_SND);
			hasPlayedPolice = true;
		}
	}

	int getHealth() { return this->health; }

	void decrementHealth() {
		this->health--;
		if (health < 0)
			health = 0;
	}

	void incrementHealth() {
		this->health += INCREMENT_HEALTH;
		if (health > TOTAL_HEALTH) health = TOTAL_HEALTH;
	}
};