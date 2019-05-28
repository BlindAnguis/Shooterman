#pragma once

#include "Message.h"

enum TRACE_LEVEL { DEB_1, DEB_2, DEB_3, DEB_4, SEND_REC, FUNC };

class ToggleDebugButtonMessage : Message {
public:
  ToggleDebugButtonMessage(TRACE_LEVEL traceLevel);
  ToggleDebugButtonMessage(sf::Packet packet);
  ~ToggleDebugButtonMessage();

  sf::Packet pack() override;
  void unpack(sf::Packet packet) override;

  TRACE_LEVEL getTraceLevel();

private:
  TRACE_LEVEL mTraceLevel;
};