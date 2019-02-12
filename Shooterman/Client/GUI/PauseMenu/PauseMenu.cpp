#include "PauseMenu.h"

#include <SFML/Graphics.hpp>

PauseMenu::PauseMenu() {
    mName = "CLIENT: PAUSE_MENU";
    mDebugEnabled = true;

    mComponentList.push_back(GUIComponentBuilder::createTitle("PAUSE", 250, 36));
    mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Unpause", 250, 200, GAME_STATE::PLAYING));
    mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Leave Game", 250, 260, GAME_STATE::MAIN_MENU));
    mComponentList.push_back(GUIComponentBuilder::createGameStateButton("Options", 250, 320, GAME_STATE::OPTIONS));
    mComponentList.push_back(GUIComponentBuilder::createCustomActionButton("Exit Game", 250, 380, []() {
		GameStateMessage gsm(GAME_STATE::MAIN_MENU);

    Subscriber gameStateSubscriber;
    MessageHandler::get().subscribeTo("GameState", &gameStateSubscriber);
    gameStateSubscriber.reverseSendMessage(gsm.pack());
    MessageHandler::get().unsubscribeTo("GameState", &gameStateSubscriber);

		sf::sleep(sf::milliseconds(100));
		sf::Packet shutdownMessage;
        shutdownMessage << SHUT_DOWN;
        MessageHandler::get().pushSystemMessage(shutdownMessage);
    }));
}

PauseMenu::~PauseMenu() { }