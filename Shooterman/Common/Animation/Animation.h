#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include <functional>
#include "../../Common/Trace.h"

struct AnimationFrame {
  sf::IntRect animationFrame;
  int frameTime;
};

class Animation : Trace
{
public:
  Animation(sf::Sprite& sprite, bool playOnce, int entityId);
  ~Animation();

  void addAnimationFrame(AnimationFrame animationFrame);
  void play();

  bool hasBeenPlayedOnce() {
    return mHasBeenPlayedOnce;
  }

  int getCurrentAnimationFrame() {
    return mCurrentAnimationFrame;
  }

  void reset() {
    mHasBeenPlayedOnce = false;
    mCurrentAnimationFrame = 0;
  }

  void setAttackCallback(const std::function<void(int entityId)>& attack) { mAttackCallback = attack; }

private:
  int mCurrentAnimationFrame;
  std::vector<AnimationFrame> mAnimationFrames;
  bool mPlayOnce;
  bool mHasBeenPlayedOnce;
  sf::Sprite& mSprite;
  sf::Clock mAnimationTime;
  int mEntityId;
  std::function<void(int entityId)> mAttackCallback = nullptr;
};

