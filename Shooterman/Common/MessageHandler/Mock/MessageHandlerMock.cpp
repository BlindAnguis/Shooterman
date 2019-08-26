#include "gmock/gmock.h"

#include "../MessageHandler.h"

class MessageHandlerMock : public MessageHandler {
public:
  MOCK_METHOD2(publishInterface, void(std::string name, Interface* interface));
  MOCK_METHOD1(unpublishInterface, void(std::string name));

  MOCK_METHOD2(subscribeTo, bool(std::string name, Subscriber* subscriber));
  MOCK_METHOD3(subscribeToWithTimeoutImpl, void(std::string interfaceName, Subscriber* subscriber, int timeoutLength));
  virtual void subscribeToWithTimeout(std::string interfaceName, Subscriber* subscriber, int timeoutLength = 5000) {
    subscribeToWithTimeoutImpl(interfaceName, subscriber, timeoutLength);
  }
  MOCK_METHOD2(unsubscribeTo, void(std::string name, Subscriber* subscriber));

  MOCK_METHOD0(getPublishedInterfaces, std::list<std::string>());
};