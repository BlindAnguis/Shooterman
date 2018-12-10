#pragma once

#include "../Task.h"
#include "../DoneTask.h"
#include "../../../MessageHandler/MessageHandler.h"
#include "../../../MessageHandler/PrivateCommunication.h"
#include "../../../../Common/Messages/ClientMainAndNetworkHandlerMessages.h"

class SetupNetworkConnectionTask : public Task {
public:
  SetupNetworkConnectionTask(PrivateCommunication* mainToNhCommunication, Subscriber* subscriber, std::string ip, unsigned short port) {
    mName = "SetupNetworkConnectionTask";
    mMainToNhCommunication = mainToNhCommunication;
    mSubscriber = subscriber;
    mIP = ip;
    mPort = port;
  }

  ~SetupNetworkConnectionTask() {}

  Task* execute() {
    SetupSocketConnectionData sscd(mIP, mPort);
    mMainToNhCommunication->pushMessage(sscd.pack());
    // TODO: Clean up mMainToNhCommunication
    return new DoneTask();
  }

private:
  Subscriber* mSubscriber;
  PrivateCommunication* mMainToNhCommunication;
  std::string mIP;
  unsigned short mPort;
};