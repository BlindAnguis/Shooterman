#include "SocketFactoryImpl.h"

#include "SocketImpl.h"

SocketFactoryImpl::SocketFactoryImpl() {}

SocketFactoryImpl::~SocketFactoryImpl() {}

std::shared_ptr<Socket> SocketFactoryImpl::createTcpSocket() {
  return std::make_shared<SocketImpl>();
}
