#include "Sound.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>




Sound::Sound() {
	std::cout << "Starting SOUND module..." << std::endl;
	m_soundThread = new std::thread(&Sound::playSound2, this);
	std::cout << "Starting SOUND module done" << std::endl;
}

//int loadSound()
//{
//	sf::SoundBuffer buffer;
//	if (!buffer.loadFromFile("sound.wav"))
//		return -1;

//	...

	//	return 0;
//}




void Sound::playSound() {
	std::cout << "Playing sound" << std::endl;

	while (m_running) {
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			sf::SoundBuffer* buffer = new sf::SoundBuffer();
			
			std::cout << "loadbufferA" << std::endl;
			//buffer.loadFromFile("C:\\Users\\jonssjon\\source\\repos\\roadrunners\\Shooterman\\nokity.wav")
			
			//if (!buffer->loadFromFile("C:\\Users\\jonssjon\\source\\repos\\roadrunners\\Shooterman\\nokity.wav")) {
			if (!buffer->loadFromFile("C:\\Users\\jonssjon\\source\\repos\\roadrunners\\Shooterman\\boing_x.wav")) {
				std::cout << "ladda filjaveln sjalv, idiot" << std::endl;
				return;
			}
			sf::Sound sound;
			std::cout << "setbuffer" << std::endl;
			sound.setBuffer(*buffer);
			sound.setLoop(true);
			std::cout << "Play." << std::endl;
			sound.play();
			//sound.setPlayingOffset(sf::seconds(2));
			sf::sleep(sf::milliseconds(3000));
		}
	}
}

void Sound::playSound2() {
	std::cout << "Playing sound" << std::endl;
	std::string SoundEffect;

	while (m_running) {
		sf::SoundBuffer* buffer = new sf::SoundBuffer();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			SoundEffect = "C:\\Users\\jonssjon\\source\\repos\\roadrunners\\Shooterman\\boing_x.wav";
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			SoundEffect = "C:\\Users\\jonssjon\\source\\repos\\roadrunners\\Shooterman\\bomb_x.wav";
		}
		std::cout << "loadbufferA" << std::endl;
		if (SoundEffect.length()!=0) {
			if (!buffer->loadFromFile(SoundEffect)) {
				std::cout << "ladda filjaveln sjalv, idiot" << std::endl;
				return;
			}
			sf::Sound sound;
			std::cout << "setbuffer" << std::endl;
			sound.setBuffer(*buffer);
			//sound.setLoop(true);
			std::cout << "Play." << std::endl;
			sound.play();
			//sound.setPlayingOffset(sf::seconds(2));
			sf::sleep(sf::milliseconds(300));
			SoundEffect = "";
		}
	}
}


//void Sound::shutDown() {
//	std::cout << "Shutdown of SOUND module requested..." << std::endl;
//	m_running = false;
//	m_soundThread->join();
//	std::cout << "Shutdown of SOUND module done" << std::endl;
//}