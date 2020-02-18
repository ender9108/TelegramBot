# Examples

## Check new updates every 30 secondes
```c++
#include <Arduino.h>
#include <ArduinoJson.h>
#include <TelegramBot.h>
#include <WiFiClientSecure.h>

#define TELEGRAM_TOKEN  *****mysupertoken*****

WiFiClientSecure client;
TelegramBot tBot(client, TELEGRAM_TOKEN);

void onNewUpdateCallback(JsonArray updates, int newMessages) {
    Serial.println("New messages arrived !!");
    // Display JsonArray updates on serial port
    serializeJson(updates, Serial);
}

void setup() {
    // Enable debug mode to show more infos in console
    tBot.enableDebugMode();

    // Set ttr at 30s (default: 10s)
    tBot.setTimeToRefresh(30000);

    // Add callback on new update detected
    tBot.on(TELEGRAM_EVT_NEW_MSG, onNewUpdateCallback);
}

void loop() {
    tBot.loop();
}
```

## Send message
```c++
#include <Arduino.h>
#include <ArduinoJson.h>
#include <TelegramBot.h>
#include <WiFiClientSecure.h>

#define TELEGRAM_TOKEN  *****mysupertoken*****

WiFiClientSecure client;
TelegramBot tBot(client, TELEGRAM_TOKEN);
long chatId = 12345;

void setup() {
}

void loop() {
    tBot.sendMessage(chatId, "Hello, I tried to send a message with TelegramBot library !!");
}
```