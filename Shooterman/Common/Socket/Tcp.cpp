#include "Tcp.h"

TcpSocket::TcpSocket(std::string ip, unsigned short port) {
  mName = "TCP";
  mTcpSocket = new sf::TcpSocket();
  mIp = sf::IpAddress(ip);
  mPort = port;
}

TcpSocket::TcpSocket(sf::TcpSocket* socket) {
  mName = "TCP";
  mTcpSocket = socket;
  mIp = socket->getRemoteAddress();
  mPort = socket->getRemotePort();
}

TcpSocket::~TcpSocket() {
  delete mTcpSocket;
}

bool TcpSocket::connect() {
  TRACE_INFO("COnnection: " << mIp << " " << mPort);
  sf::Socket::Status status = mTcpSocket->connect(mIp, mPort);
  //mTcpSocket->setBlocking(false);

  TRACE_INFO("Socket status is: " << status);
  if (status != sf::Socket::Done) {
    TRACE_ERROR("Could not connect to server with address: " << mIp);
    return false;
  } else {
    TRACE_INFO("Socket connected");
  }
  return true;
}

void TcpSocket::disconnect() {
  mTcpSocket->disconnect();
}

std::vector<sf::Packet> TcpSocket::read() {
  std::vector<sf::Packet> packets;
  sf::Packet packet;
  while (mTcpSocket->receive(packet) == sf::Socket::Done) {
    TRACE_INFO("Reading tcp socket");
    packets.push_back(packet);
  }
  TRACE_INFO("Reading tcp socket Done");
  return packets;
}

void TcpSocket::send(sf::Packet packet) {
  mTcpSocket->send(packet);
}