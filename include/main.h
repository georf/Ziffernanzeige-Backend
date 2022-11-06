#include <Arduino.h>
// #include <EEPROM.h>
#include "SoftwareSerial.h"
#include "LedDriver.h"
#include <ESP8266WiFi.h>

#define SOFT_TX D6
#define SOFT_RX D7

#define LED_1 D8
#define LED_2 D5

#define SWITCH_WIFI_CHANNEL D1
#define SWITCH_UNUSED D2

#define VOLTAGE_INPUT A0
#define VOLTAGE_FULL 4.15
#define VOLTAGE_EMPTY 3.0
#define VOLTAGE_WARN_LEVEL 10

#define IP_SUBNET 10, 0, 112
#define IP_STATION IP_SUBNET, 42
#define IP_GATEWAY IP_SUBNET, 1
#define IP_SUBNET_MASK 255, 255, 255, 0
#define PORT 11242

#define WIFI_SSID_A "Ziffernanzeige-A"
#define WIFI_SSID_A_PASSPHRASE "usi3taQuoh6mahyaiSha"
#define WIFI_SSID_B "Ziffernanzeige-B"
#define WIFI_SSID_B_PASSPHRASE "yai2inohch4OhP1du5iu"
#define WIFI_MAX_CLIENTS 10

void handleSerial();
void handleNewClients();
void checkBatteryStatus();
void sendToClients(const uint8_t *buffer, size_t length);