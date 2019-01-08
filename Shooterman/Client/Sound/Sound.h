#ifndef SOUND_H
#define SOUND_H

#include <thread>

#include <SFML/Graphics.hpp>

#include "../../Common/MessageHandler/MessageHandler.h"

class Sound {
public:
  Sound();

  //void shutDown();

private:
  bool m_running = true;
  std::thread* m_soundThread;

  void playSound();
  void playSound2();

};

#endif // SOUND_H