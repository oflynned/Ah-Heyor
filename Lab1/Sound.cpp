#pragma once
#include "windows.h"
#include "GLIncludes.h"
#include "File.cpp"

#include <thread>

class Sound {
public:
	enum Sounds {
		rambling, sfx, music
	};

private:
	std::string ramblingsVals[8] = {
		"Bob's Your Uncle",
		"Dya Know What I Mean",
		"Ehhhh",
		"Getting Into Politics",
		"Green and White Shirt",
		"I Have Yeah",
		"I Used To Buhhh",
		"It's Actually a Good Point"
	};

	std::string musicVals[1] = { 
		"Hardbass"
	};

	std::string sfxVals[3] = {
		"Police",
		"Picking Up",
		"Running"
	};

	const int NUM_RAMBLINGS = 8;
	const int NUM_MUSIC = 1;
	const int NUM_SFX = 3;

	int currWait = 0, waitUntil = 0;

	int generateRand(int limit) {
		srand((unsigned)time(0));
		return rand() % limit;
	}

	int randomWait() {
		// wait should be 10-20s
		srand((unsigned)time(0));
		return (rand() % 10000) + 10000;
	}

public:
	Sound() {}
	~Sound() {}

	void playAudio(Sounds sounds) {
		std::string sound;
		int element = 0;

		switch (sounds) {
		case Sounds::rambling:
			element = generateRand(NUM_RAMBLINGS);
			sound = File::getAbsoluteSoundPath(
				"Ramblings/" + ramblingsVals[element] + ".wav");
			sndPlaySound((const char*)sound.c_str(), SND_ASYNC | SND_NOSTOP);
			break;
		case Sounds::music:
			element = generateRand(NUM_MUSIC);
			sound = File::getAbsoluteSoundPath(
				"Music/" + musicVals[element] + ".wav");
			sndPlaySound((const char*)sound.c_str(), SND_ASYNC | SND_LOOP | SND_NOSTOP);
			break;
		case Sounds::sfx:
			element = generateRand(NUM_SFX);
			sound = File::getAbsoluteSoundPath(
				"Sfx/" + sfxVals[element] + ".wav");
			sndPlaySound((const char*)sound.c_str(), SND_ASYNC | SND_NOSTOP);
			break;
		default:
			break;
		}

		waitUntil = randomWait();
	}

	void shouldPlay() {
		if (currWait < waitUntil) {
			std::cout << currWait << "/" << waitUntil << std::endl;
			currWait++;
		}
		else {
			currWait = 0;
			playAudio(Sounds::rambling);
		}
	}
};