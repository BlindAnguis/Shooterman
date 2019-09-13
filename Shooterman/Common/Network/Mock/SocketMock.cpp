#include "gmock/gmock.h"

#include "../Socket.h"
#include "../SocketFactory.h"

class SocketMock : public Socket {
public:
  MOCK_METHOD3(connect, Soc::Status(std::string ipAddress, std::uint16_t port, int timeout));
  MOCK_METHOD0(disconnect, void());

  MOCK_METHOD1(setBlocking, void(bool blocking));

  MOCK_METHOD1(send, void(sf::Packet& packet));
  MOCK_METHOD1(receive, Soc::Status(sf::Packet& packet));
};

class SocketFactoryMocker : public SocketFactory {
public:
  SocketFactoryMocker(std::shared_ptr<SocketMock> socketMock) {
    mSocketMock = socketMock;
  }

  std::shared_ptr<Socket> createTcpSocket() {
    return mSocketMock;
  }

private:
  std::shared_ptr<SocketMock> mSocketMock;
};