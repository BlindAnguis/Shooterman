#pragma once

#include <vector>
#include <SFML\Graphics.hpp>

#include "../../Common/Trace.h"

class Animation : Trace
{
public:
  Animation(sf::Sprite& sprite, bool playOnce, int timePerAnimationFrame);
  ~Animation();

  void addAnimationFrame(sf::IntRect animationFrame);
  void play();
  bool hasBeenPlayedOnce() {
    return mHasBeenPlayedOnce;
  }

private:
  int mCurrentAnimationFrame;
  int mTimePerAnimationFrame;
  std::vector<sf::IntRect> mAnimationFrames;
  bool mPlayOnce;
  bool mHasBeenPlayedOnce;
  sf::Sprite& mSprite;
  sf::Clock mAnimationTime;
};

