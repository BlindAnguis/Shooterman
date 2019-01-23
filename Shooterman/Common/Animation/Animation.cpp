#include "Animation.h"

Animation::Animation(
  sf::Sprite& sprite,
  bool playOnce,
  int timePerAnimationFrame
) :
  mSprite(sprite),
  mPlayOnce(playOnce),
  mTimePerAnimationFrame(timePerAnimationFrame),
  mHasBeenPlayedOnce(false)
{
  mName = "COMMON: Animation";
}

Animation::~Animation()
{
}

void Animation::addAnimationFrame(sf::IntRect animationFrame) {
  mAnimationFrames.emplace_back(animationFrame);
}

void Animation::play() {
  //TRACE_INFO("Play: animationTime: " << mAnimationTime.getElapsedTime().asMilliseconds() << ", mTimePerAnimationFrame: " << mTimePerAnimationFrame);
  if (!mPlayOnce || !mHasBeenPlayedOnce) {
    if (mAnimationTime.getElapsedTime().asMilliseconds() > mTimePerAnimationFrame) {
      //TRACE_INFO("mCurrentAnimationFrameBefore: " << mCurrentAnimationFrame);
      mSprite.setTextureRect(mAnimationFrames.at(mCurrentAnimationFrame));

      if (mCurrentAnimationFrame >= mAnimationFrames.size() - 1) {
        mCurrentAnimationFrame = 0;
        mHasBeenPlayedOnce = true;
      }
      else {
        mCurrentAnimationFrame++;
      }
      //mCurrentAnimationFrame == (mAnimationFrames.size() - 1) ? mCurrentAnimationFrame = 0 : mCurrentAnimationFrame++;
      //TRACE_INFO("mCurrentAnimationFrameAfter: " << mCurrentAnimationFrame);
      mAnimationTime.restart();
    }
  } 
}
