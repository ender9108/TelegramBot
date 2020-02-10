# Examples

## Check new updates every X secondes
```c++
#include <Arduino.h>
#include <TelegramBot.h>
#include <WiFiClientSecure.h>

#define TELEGRAM_TOKEN  *****mysupertoken*****

WiFiClientSecure client;
TelegramBot tBot(client, TELEGRAM_TOKEN);

void myCallback(Update *update) {

}

void setup() {
    // Enable debug mode to show more infos in console
    tBot.enableDebugMode();

    // Add callback on new update detected
    tBot.on(TELEGRAM_EVT_NEW_MSG, myCallback);
}

void loop() {
    tBot.loop();
}
```