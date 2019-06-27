#include "Animation.h"


Animation::Animation(
  sf::Sprite& sprite,
  bool playOnce,
  int entityId
) :
  mSprite(sprite),
  mPlayOnce(playOnce),
  mHasBeenPlayedOnce(false),
  mEntityId(entityId)
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
      AnimationFrame af = mAnimationFrames.at(mCurrentAnimationFrame);
      mSprite.setTextureRect(af.animationFrame);
      mSprite.setOrigin(af.originPosition);

      if (mCurrentAnimationFrame == mAttackingFrame && mAttackCallback != nullptr) {
        mAttackCallback(mEntityId);
      }

      if (mCurrentAnimationFrame >= (int)mAnimationFrames.size() - 1) {
        mCurrentAnimationFrame = 0;
        mHasBeenPlayedOnce = true;
        if (mAttackFinishedCallback != nullptr) {
          mAttackFinishedCallback(mEntityId);
        }
      } else {
        mCurrentAnimationFrame++;
      }
      //TRACE_INFO("mCurrentAnimationFrameAfter: " << mCurrentAnimationFrame);
      mAnimationTime.restart();
    }
  }
}
