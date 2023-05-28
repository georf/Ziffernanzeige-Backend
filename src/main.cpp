#include "main.h"
SoftwareSerial softSerial;

LedDriver led_0 = LedDriver(LED_BUILTIN, fast_blink, true);
LedDriver led_1 = LedDriver(LED_1, fast_blink, false);
LedDriver led_2 = LedDriver(LED_2, fast_blink, false);

WiFiServer server(PORT);
WiFiClient serverClients[WIFI_MAX_CLIENTS];

unsigned long nextPingTime = 0;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  delay(1000);
  softSerial.begin(9600, SWSERIAL_8N1, SOFT_RX, SOFT_TX);

  Serial.print("Debug: Setting soft-AP configuration: ");
  Serial.println(WiFi.softAPConfig(IPAddress(IP_STATION), IPAddress(IP_GATEWAY), IPAddress(IP_SUBNET_MASK)) ? "Ready" : "Failed!");

  Serial.print("Debug: Setting soft-AP: ");
  pinMode(SWITCH_WIFI_CHANNEL, INPUT);
  if (digitalRead(SWITCH_WIFI_CHANNEL))
  {
    Serial.print(WIFI_SSID_A);
    Serial.print(": ");
    Serial.println(WiFi.softAP(WIFI_SSID_A, WIFI_SSID_A_PASSPHRASE) ? "Ready" : "Failed!");
  }
  else
  {
    Serial.print(WIFI_SSID_B);
    Serial.print(": ");
    Serial.println(WiFi.softAP(WIFI_SSID_B, WIFI_SSID_B_PASSPHRASE) ? "Ready" : "Failed!");
  }

  Serial.print("Debug: Soft-AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
  server.setNoDelay(true);

  led_0.set(off);
  led_1.set(off);
  led_2.set(slow_short_blink);
}

void loop()
{
  led_0.work();
  led_1.work();
  led_2.work();
  handleSerial();
  handleNewClients();

  if (nextPingTime < millis())
  {
    size_t len = 2;
    uint8_t sbuf[len] = "p\n";
    sendToClients(sbuf, len);
    nextPingTime = millis() + 1000 * 60;

    checkBatteryStatus();
  }
}

void handleSerial()
{
  if (softSerial.available())
  {
    size_t len = softSerial.available();
    uint8_t sbuf[len];
    softSerial.readBytes(sbuf, len);
    Serial.write(sbuf, len);
    Serial.flush();

    sendToClients(sbuf, len);

    led_1.set(shot);
  }

  if (Serial.available())
  {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    softSerial.write(sbuf, len);
    softSerial.flush();
    led_0.set(shot);
  }
}

void sendToClients(const uint8_t *buffer, size_t length)
{
  for (uint8_t i = 0; i < WIFI_MAX_CLIENTS; i++)
  {
    if (serverClients[i] && serverClients[i].connected())
    {
      serverClients[i].write(buffer, length);
    }
  }
}

void handleNewClients()
{
  if (!server.hasClient())
    return; // nothing to do

  for (uint8_t i = 0; i < WIFI_MAX_CLIENTS; i++)
  {
    // find bad or unused slots
    if (!serverClients[i] || !serverClients[i].connected())
    {
      if (serverClients[i])
        serverClients[i].stop();

      serverClients[i] = server.available();
      Serial.print("Debug: New client: ");
      Serial.print(i);
      Serial.print(" - ");
      Serial.println(serverClients[i].remoteIP());
      led_2.set(blink_3_times, led_2.state);
      break;
    }
  }

  // no free slot, so reject client
  WiFiClient shortClient = server.available();
  shortClient.stop();
}

void checkBatteryStatus()
{
  float voltage = analogRead(VOLTAGE_INPUT) / 1024.0 * 3.3 * 2;
  Serial.print("Debug: Battery voltage ");
  Serial.println(voltage);

  uint8_t percent = (voltage - VOLTAGE_EMPTY) / (VOLTAGE_FULL - VOLTAGE_EMPTY) * 100;
  Serial.print("Debug: Battery percent ");
  Serial.println(percent);

  if (percent < VOLTAGE_WARN_LEVEL)
  {
    if (percent >= 100)
      percent = 99;
    size_t len = 6;
    uint8_t sbuf[len];
    sbuf[0] = 'v';
    sbuf[1] = ':';
    sbuf[2] = ' ';
    sbuf[3] = (percent / 10) + 48;
    sbuf[4] = (percent % 10) + 48;
    sbuf[5] = '\n';
    sendToClients(sbuf, len);
    Serial.write(sbuf, len);
  }
}