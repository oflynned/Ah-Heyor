#pragma once

// singleton instance
class Manager {
private:
	enum GameState {
		PLAYING, WON, LOST
	};

	GameState gameState;

	const int ONE_SECOND = 1000;
	int timeLeft = ONE_SECOND * 60;

	int health = 1000;

	static Manager* mInstance;

	//prevent initialisation by constructor
	Manager() {}

public:
	static Manager* getInstance() {
		mInstance = mInstance ? mInstance : new Manager;
		return mInstance;
	}

	void update() {
		if(gameState == GameState::PLAYING)
			timeLeft--;
	}

	void setGameState(GameState gameState) { this->gameState = gameState; }
};