#include "ToggleDebugButtonMessage.h"

#include "../../Common/MessageId.h"

ToggleDebugButtonMessage::ToggleDebugButtonMessage(TRACE_LEVEL traceLevel) {
  mTraceLevel = traceLevel;
}

ToggleDebugButtonMessage::ToggleDebugButtonMessage(sf::Packet packet) {
  unpack(packet);
}

ToggleDebugButtonMessage::~ToggleDebugButtonMessage() { }

sf::Packet ToggleDebugButtonMessage::pack() {
  sf::Packet packet;
  packet << TOGGLE_DEBUG_BUTTON;
  packet << mTraceLevel;
  return packet;
}

void ToggleDebugButtonMessage::unpack(sf::Packet packet) {
  int traceLevel = -1;
  packet >> traceLevel;
  mTraceLevel = (TRACE_LEVEL)traceLevel;
}

TRACE_LEVEL ToggleDebugButtonMessage::getTraceLevel() {
  return mTraceLevel;
}