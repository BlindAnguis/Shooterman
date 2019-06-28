#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include <functional>
#include "../../Common/Trace.h"

struct AnimationFrame {
  sf::IntRect animationFrame;
  int frameTime;
  sf::Vector2f originPosition;
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

  void setAttackCallback(const std::function<void(int entityId)>& attack, int attackFrame = 3) { 
    mAttackCallback = attack;
    mAttackingFrame = attackFrame;
  }
  void setAttackFinishedCallback(const std::function<void(int entityId)>& attackFinished) { 
    mAttackFinishedCallback = attackFinished; 
  }
  void setSuperAttackIntervallCallback(const std::function<void(int entityId)>& superAttackIntervallCallback, int attackIntevall = 0) {
    mSuperAttackIntervallCallback = superAttackIntervallCallback;
    mAttackIntervall = attackIntevall;
  }

private:
  int mCurrentAnimationFrame;
  std::vector<AnimationFrame> mAnimationFrames;
  bool mPlayOnce;
  bool mHasBeenPlayedOnce;
  sf::Sprite& mSprite;
  sf::Clock mAnimationTime;
  int mEntityId;
  std::function<void(int entityId)> mAttackCallback = nullptr;
  std::function<void(int entityId)> mAttackFinishedCallback = nullptr;
  std::function<void(int entityId)> mSuperAttackIntervallCallback = nullptr;
  int mAttackingFrame;
  int mAttackIntervall;
};

