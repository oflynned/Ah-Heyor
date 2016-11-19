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
	const int RAMBLING_SIZE = 8;
	const int MUSIC_SIZE = 1;
	const int SFX_SIZE = 4;

	static const const int BURP = 3;

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

	std::string sfxVals[4] = {
		"Police",
		"Picking Up",
		"Running",
		"Burp"
	};

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
	static const int BURP_SND = 3;

	Sound() {}
	~Sound() {}

	void playAudio(Sounds sounds, int index = -1) {
		std::string sound;
		int element = 0;
		unsigned int attributes = SND_ASYNC | SND_NOSTOP;

		switch (sounds) {
		case Sounds::rambling:
			element = generateRand(RAMBLING_SIZE);
			sound = File::getAbsoluteSoundPath(
				"Ramblings/" + ramblingsVals[element] + ".wav");
			sndPlaySound((const char*)sound.c_str(), attributes);
			break;
		case Sounds::music:
			element = generateRand(MUSIC_SIZE);
			sound = File::getAbsoluteSoundPath(
				"Music/" + musicVals[element] + ".wav");
			sndPlaySound((const char*)sound.c_str(), attributes | SND_NOSTOP);
			break;
		case Sounds::sfx:
			element = index == -1 ? generateRand(SFX_SIZE) : index;
			sound = File::getAbsoluteSoundPath(
				"Sfx/" + sfxVals[element] + ".wav");
			sndPlaySound((const char*)sound.c_str(), attributes);
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