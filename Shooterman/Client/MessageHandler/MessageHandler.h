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

  sf::Packet readInputMessage(); // Deprecated

  sf::Packet readGraphicsMessage(); // Deprecated

private:
  MessageHandler() {}

  std::list<Subscriber*> m_systemSubscriberList;
  std::list<Subscriber*> m_inputSubscriberList;
  std::list<Subscriber*> m_spriteListSubscriberList;


  std::mutex m_inputQueueMutex; // Deprecated
  std::queue<sf::Packet> m_inputQueue; // Deprecated
  std::mutex m_graphicsQueueMutex; // Deprecated 
  std::queue<sf::Packet> m_graphicsQueue; // Deprecated
};
#endif // ifndef MESSAGE_HANDLER_H