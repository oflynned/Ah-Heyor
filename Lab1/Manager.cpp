#pragma once
#include "GLIncludes.h"
#include <iostream>

// singleton instance
class Manager {
private:
	enum GameState {
		PLAYING, WON, LOST
	};

	GameState gameState;

	//prevent initialisation by constructor
	Manager() {}

public:

	static Manager getInstance() {
		static Manager* instance = new Manager();
		return *instance;
	}

	void update() {
	}

	void setGameState(GameState gameState) { this->gameState = gameState; }

	void checkGameState(int cansLeft, int health) {
		gameState = GameState::PLAYING;

		if (health < 1) {
			gameState = GameState::LOST;
		} 
		if (cansLeft == 0 && health > 0) {
			gameState = GameState::WON;
		} 
		
		if (DEV_MODE) {
			std::cout << "Game state: " << gameState << ", " 
				<< cansLeft << "/" << SPAWN_CAN_SIZE << ", " 
				<< health << "/" << TOTAL_HEALTH << "(" 
				<< (int)(((float) health / (float) TOTAL_HEALTH) * 100) << "%)" << std::endl;
		}
	}
};