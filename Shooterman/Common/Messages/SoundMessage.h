#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "Message.h"
#include "../MessageId.h"
#include "../../Common/Sounds.h"

class SoundMessage : Message {
public:
  SoundMessage();
  ~SoundMessage();

  void addSound(Sounds sound);

  Sounds getSound(unsigned int position);

  sf::Packet pack();

  void unpack(sf::Packet packet);

  int getSize();

private:
  int mSize;
  std::vector<Sounds> mSounds;
};
