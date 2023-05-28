#include "Arduino.h"
#include "SoftwareSerial.h"

#define SOFT_TX D6
#define SOFT_RX D7

class SerialControl
{
public:
    bool handle();
    void handle(byte control);
    void Startup(void (*pSendToClientsF)(const uint8_t *buffer, size_t length));
    SoftwareSerial softSerial;

private:
    boolean stoppable[2] = {true, true};
    byte currentLine[50];
    uint8_t currentLineIndex = 0;
    uint32_t recentReceivedTime[4] = {0, 0, 0, 0};
    uint32_t lastDeciTime = 0;
    void (*pSendToClients)(const uint8_t *buffer, size_t length);
    void HandleTeamComputerLong(uint8_t start);
    void HandleShortCommands();
    void HandlePlatz();
    void sendStart();
    void sendWait();
    void sendTime(uint32_t lastDeciTime, uint8_t lastLine);
    boolean HandlePlatzTime(uint8_t part, byte a, byte b, byte c);
};
