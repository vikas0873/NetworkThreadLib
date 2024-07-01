#ifndef NETWORK_THREAD_H
#define NETWORK_THREAD_H

#include <Arduino.h>

class NetworkThread {
public:
    static void begin();
    static void sendData(const String& data);
    static String receiveData();
    static bool isConnected();
};

#endif