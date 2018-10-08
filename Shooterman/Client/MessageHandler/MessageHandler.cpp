#include "MessageHandler.h"

// GAME STATE
void MessageHandler::subscribeToGameStateMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  tryToGiveId(newSubscriber);
  mGameStateSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to game state list");
}

void MessageHandler::unSubscribeToGameStateMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  for (auto it = mGameStateSubscriberList.begin(); it != mGameStateSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mGameStateSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to game state list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushGameStateMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  for (Subscriber* subscriber : mGameStateSubscriberList) {
    subscriber->sendMessage(message);
  }
}

// SYSTEM
void MessageHandler::subscribeToSystemMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  tryToGiveId(newSubscriber);
  mSystemSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to system list");
}

void MessageHandler::unSubscribeToSystemMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  for (auto it = mSystemSubscriberList.begin(); it != mSystemSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSystemSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to system list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushSystemMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSystemSubscriberLock);
  for (Subscriber* subscriber : mSystemSubscriberList) {
    subscriber->sendMessage(message);
  }
}

// INPUT
void MessageHandler::subscribeToInputMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mInputSubscriberLock);
  tryToGiveId(newSubscriber);
  mInputSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to input list");
}

void MessageHandler::unSubscribeToInputMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mInputSubscriberLock);
  for (auto it = mInputSubscriberList.begin(); it != mInputSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mInputSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to input list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushInputMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mInputSubscriberLock);
  for (Subscriber* subscriber : mInputSubscriberList) {
    subscriber->sendMessage(message);
  }
}

// SPRITE LIST
void MessageHandler::subscribeToSpriteListMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSpriteListSubscriberLock);
  tryToGiveId(newSubscriber);
  mSpriteListSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to sprite list");
}

void MessageHandler::unSubscribeToSpriteListMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSpriteListSubscriberLock);
  for (auto it = mSpriteListSubscriberList.begin(); it != mSpriteListSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSpriteListSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to sprite list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushSpriteListMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSpriteListSubscriberLock);
  for (Subscriber* subscriber : mSpriteListSubscriberList) {
    subscriber->sendMessage(message);
  }
}

// MOUSE LIST
void MessageHandler::subscribeToMouseMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mMouseSubscriberLock);
  tryToGiveId(newSubscriber);
  mMouseSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to mouse list");
}

void MessageHandler::unSubscribeToMouseMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mMouseSubscriberLock);
  for (auto it = mMouseSubscriberList.begin(); it != mMouseSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mMouseSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to mouse list");
    } else {
      ++it;
    }
  }
}

void MessageHandler::pushMouseMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mMouseSubscriberLock);
  for (Subscriber* subscriber : mMouseSubscriberList) {
    subscriber->sendMessage(message);
  }
}

// Sound LIST
void MessageHandler::subscribeToSoundMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSoundSubscriberLock);
  tryToGiveId(newSubscriber);
  mSoundSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to sound list");
}

void MessageHandler::unSubscribeToSoundMessages(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSoundSubscriberLock);
  for (auto it = mSoundSubscriberList.begin(); it != mSoundSubscriberList.end(); /**/) {
    if ((*it)->getId() == newSubscriber->getId()) {
      it = mSoundSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << newSubscriber->getId() << ") from to sound list");
    }
    else {
      ++it;
    }
  }
}

void MessageHandler::pushMouseMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSoundSubscriberLock);
  for (Subscriber* subscriber : mSoundSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void MessageHandler::unsubscribeAll(Subscriber* subscriber) {
  unSubscribeToGameStateMessages(subscriber);
  unSubscribeToSystemMessages(subscriber);
  unSubscribeToInputMessages(subscriber);
  unSubscribeToSpriteListMessages(subscriber);
  unSubscribeToMouseMessages(subscriber);
  unSubscribeToSoundMessages(subscriber);
}

void MessageHandler::tryToGiveId(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mIdGeneratorLock);
  if (subscriber->getId() == -1) {
    subscriber->setId(mCurrentId);
    mCurrentId++;
  }
}