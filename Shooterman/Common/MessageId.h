#ifndef MESSAGEID_H_
#define MESSAGEID_H_

// System messages
#define SHUT_DOWN           1
#define CHANGE_GAME_STATE   2
#define ADD_DEBUG_BUTTON    3
#define REMOVE_DEBUG_BUTTON 4
#define TOGGLE_DEBUG_BUTTON 5
#define SERVER_READY        6

#define HEARTBEAT           10

// Client to Server
#define INPUT_KEYS          100
#define NEW_USERNAME        110
#define CHARACTER_CHOOSEN   111

// Server to Client
#define SPRITE_LIST_CACHE   200
#define SPRITE_LIST         201
#define SOUND_LIST          210
#define PLAYER_DATA         220
#define PLAYER_USERNAMES    230
#define PLAYABLE_CHARACTERS 231

// Internal Client messages
#define IP_MESSAGE          1000

// Internal Server messages
#define CLIENT_DISCONNECTED 2000
#endif /* MESSAGEID_H_ */