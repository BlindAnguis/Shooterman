#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <queue>
#include <list>

#include <SFML/Network.hpp>

#include "Common/Subscriber.h"

class MessageHandler {
public:
  static MessageHandler& get() {
    static MessageHandler instance;
    return instance;
  }

  void subscribeToSystemMessages(Subscriber* newSubscriber);
  void pushSystemMessage(sf::Packet message);

  void subscribeToInputMessages(Subscriber* newSubscriber);
  void pushInputMessage(sf::Packet message);

  void subscribeToSpriteListMessages(Subscriber* newSubscriber);
  void pushSpriteListMessage(sf::Packet message);

private:
  MessageHandler() {}

  std::list<Subscriber*> m_systemSubscriberList;
  std::list<Subscriber*> m_inputSubscriberList;
  std::list<Subscriber*> m_spriteListSubscriberList;
};
#endif // ifndef MESSAGE_HANDLER_H