#pragma once

#include <memory>

#include "Socket.h"

class SocketFactory {
public:
  virtual ~SocketFactory() {}

  virtual std::shared_ptr<Socket> createTcpSocket() = 0;
};