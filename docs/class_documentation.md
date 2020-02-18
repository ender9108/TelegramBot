### Class constructor

```c++
    TelegramBot(WiFiClientSecure &wifiClient);
    TelegramBot(WiFiClientSecure &wifiClient, String token);
```

### Set telegram bot token

```c++
    void setToken(String token);
```

### Enable debug mode

```c++
    void enableDebugMode();
```

### Change time to refresh

Change time loop to check updates

```c++
    void setTimeToRefresh(long ttr);
```

### Stop loop
```c++
    void pause();
```

### Restart loop

```c++
    void resume();
```

### Return the last update id (update_id)

```c++
    long getLastUpdateId();
```

### Start loop 

```c++
    int loop();
```

### Return number of update and pupulate property JsonArray updates

```c++
    int getUpdates(int offset = 0, int limit = TELEGRAM_MAX_UPDATE);
```
### Set callback on event 

Actually just TELEGRAM_EVT_NEW_UPDATE available

```c++
    bool on(int event, EventCallback callback);
```

### 

```c++
    JsonObject getMe();
```

### Send message on chat

```c++
    DynamicJsonDocument sendMessage(
        long chatId, 
        String text, 
        String parseMode = "", 
        bool disablePreview = false, 
        long replyToMessageId = 0, 
        bool disableNotification = false
    );
```
```c++
    DynamicJsonDocument sendContact(
        long chatId, 
        String phoneNumber, 
        String firstName, 
        String lastName = "", 
        long replyToMessageId = 0, 
        bool disableNotification = false
    );
```
```c++
    DynamicJsonDocument sendChatAction(long chatId, String action);
```
```c++
    DynamicJsonDocument sendLocation(
        long chatId, 
        float latitude, 
        float longitude, 
        long replyToMessageId = 0, 
        bool disableNotification = false, 
        int livePeriod = 0
    );
```
```c++
    DynamicJsonDocument editMessageReplyMarkup(
        long chatId, 
        long messageId, 
        long inlineMessageId = 0
    );
```
```c++
    DynamicJsonDocument deleteMessage(long chatId, long messageId);
```
```c++
    DynamicJsonDocument editMessageLiveLocation(
        long chatId, 
        long messageId, 
        long inlineMessageId, 
        float latitude, 
        float longitude
    );
```
```c++
    DynamicJsonDocument stopMessageLiveLocation(long chatId, long messageId, long inlineMessageId);
```
```c++
    DynamicJsonDocument forwardMessage(long chatId, long fromChatId, long messageId, bool disableNotification = false);
```
```c++
    DynamicJsonDocument kickChatMember(long chatId, long userId, long untilDate = -1);
```
```c++
    DynamicJsonDocument unbanChatMember(long chatId, long userId);
```
```c++
    DynamicJsonDocument editMessageText(long chatId, long messageId, String text, String parseMode = "", bool disablePreview = false, long inlineMessageId = 0);
```
```c++
    DynamicJsonDocument editMessageCaption(long chatId, long messageId, String caption = "", long inlineMessageId = 0);
```
```c++
    DynamicJsonDocument sendPhoto(long chatId, String photo, String caption = "", long replyToMessageId = 0, bool disableNotification = false, String parseMode = "");
```
```c++
    DynamicJsonDocument sendDocument(long chatId, String document, String caption = "", long replyToMessageId = 0, bool disableNotification = false, String parseMode = "");
``` 
```c++
    DynamicJsonDocument sendPhoto(
        long chatId, 
        int fileSize, 
        DataAvailable dataAvailableCallback, 
        GetNextByte getNextByteCallback, 
        String caption = "", 
        long replyToMessageId = 0, 
        bool disableNotification = false, 
        String parseMode = ""
    );
```
```c++
    DynamicJsonDocument sendDocument(
        long chatId, 
        int fileSize, 
        DataAvailable dataAvailableCallback, 
        GetNextByte getNextByteCallback, 
        String caption = "", 
        long replyToMessageId = 0, 
        bool disableNotification = false, 
        String parseMode = ""
    );
```