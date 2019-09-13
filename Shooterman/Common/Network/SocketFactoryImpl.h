#pragma once

#include <memory>

#include "SocketFactory.h"
#include "Socket.h"

class SocketFactoryImpl : public SocketFactory {
public:
  SocketFactoryImpl();
  ~SocketFactoryImpl();

  std::shared_ptr<Socket> createTcpSocket() override;
};

