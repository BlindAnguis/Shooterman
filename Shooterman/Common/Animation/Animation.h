#pragma once

#include <vector>
#include <SFML\Graphics.hpp>

#include "../../Common/Trace.h"

struct AnimationFrame {
  sf::IntRect animationFrame;
  int frameTime;
};

class Animation : Trace
{
public:
  Animation(sf::Sprite& sprite, bool playOnce);
  ~Animation();

  void addAnimationFrame(AnimationFrame animationFrame);
  void play();
  bool hasBeenPlayedOnce() {
    return mHasBeenPlayedOnce;
  }

private:
  int mCurrentAnimationFrame;
  std::vector<AnimationFrame> mAnimationFrames;
  bool mPlayOnce;
  bool mHasBeenPlayedOnce;
  sf::Sprite& mSprite;
  sf::Clock mAnimationTime;
};

