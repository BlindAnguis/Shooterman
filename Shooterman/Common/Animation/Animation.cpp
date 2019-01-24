#include "Animation.h"

Animation::Animation(
  sf::Sprite& sprite,
  bool playOnce
) :
  mSprite(sprite),
  mPlayOnce(playOnce),
  mHasBeenPlayedOnce(false)
{
  mName = "COMMON: Animation";
}

Animation::~Animation()
{
}

void Animation::addAnimationFrame(AnimationFrame animationFrame) {
  mAnimationFrames.emplace_back(animationFrame);
}

void Animation::play() {
  //TRACE_INFO("Play: animationTime: " << mAnimationTime.getElapsedTime().asMilliseconds() << ", mTimePerAnimationFrame: " << mTimePerAnimationFrame);
  if (!mPlayOnce || !mHasBeenPlayedOnce) {
    if (mAnimationTime.getElapsedTime().asMilliseconds() > mAnimationFrames.at(mCurrentAnimationFrame).frameTime) {
      //TRACE_INFO("mCurrentAnimationFrameBefore: " << mCurrentAnimationFrame);
      mSprite.setTextureRect(mAnimationFrames.at(mCurrentAnimationFrame).animationFrame);

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
