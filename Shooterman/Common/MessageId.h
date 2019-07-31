#ifndef MESSAGEID_H_
#define MESSAGEID_H_

namespace MessageId{
// System messages
  const int SHUT_DOWN = 1;
  const int CHANGE_GAME_STATE = 2;
  const int ADD_DEBUG_BUTTON = 3;
  const int REMOVE_DEBUG_BUTTON = 4;
  const int TOGGLE_DEBUG_BUTTON = 5;
  const int SERVER_READY = 6;

  const int HEARTBEAT = 10;

// Client to Server
  const int INPUT_KEYS = 100;
  const int NEW_USERNAME = 110;
  const int CHARACTER_CHOOSEN = 111;

// Server to Client
  const int SPRITE_LIST_CACHE = 200;
  const int SPRITE_LIST = 201;
  const int SOUND_LIST = 210;
  const int PLAYER_DATA = 220;
  const int PLAYER_USERNAMES = 230;
  const int PLAYABLE_CHARACTERS = 231;
  const int MAP_DATA = 232;

// Internal Client messages
  const int IP_MESSAGE = 1000;
  const int INFO_MESSAGE = 1001;
  const int MOUSE_MESSAGE = 1002;

// Internal Server messages
  const int CLIENT_DISCONNECTED = 2000;

};
#endif /* MESSAGEID_H_ */