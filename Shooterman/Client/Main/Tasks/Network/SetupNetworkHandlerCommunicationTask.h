#pragma once

#include "SetupNetworkConnectionTask.h"
#include "../Task.h"
#include "../../../MessageHandler/MessageHandler.h"
#include "../../../MessageHandler/PrivateCommunication.h"

class SetupNetworkHandlerCommunicationTask : public Task {
public:
  SetupNetworkHandlerCommunicationTask(std::string ip, unsigned short port) {
    mName = "SetupNetworkHandlerCommunicationTask";
    mIP = ip;
    mPort = port;
    mMainToNhCommunication = new PrivateCommunication();
    MessageHandler::get().publishInterface("mainToNhCommunication", mMainToNhCommunication);
  }

  ~SetupNetworkHandlerCommunicationTask() { }

  Task* execute() {
    if (MessageHandler::get().subscribeTo("nhToMainCommunication", &mSubscriber)) {
      TRACE_INFO("Connection with Network Handler established");
      mNeedRerun = false;
      return new SetupNetworkConnectionTask(mMainToNhCommunication, &mSubscriber, mIP, mPort);
    }

    if (retries < 0) {
      TRACE_INFO("Connection with Network Handler timeout, stop retrying");
      MessageHandler::get().unpublishInterface("mainToNhCommunication");
      return new DoneTask();
    }

    mNeedRerun = true;
    retries--;
    return this;
  }

private:
  int retries = 600; // About 10 seconds
  Subscriber mSubscriber;
  PrivateCommunication* mMainToNhCommunication;
  std::string mIP;
  unsigned short mPort;
};