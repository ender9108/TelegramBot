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
    void setTimeToRefresh(long ttr);
    void pause();
    void resume();
    long getLastUpdateId();
    int loop();
    int getUpdates(int offset = 0, int limit = TELEGRAM_MAX_UPDATE);
    bool on(int event, EventCallback callback);
    JsonObject getMe();
    DynamicJsonDocument sendMessage(long chatId, String text, String parseMode = "", bool disablePreview = false, long replyToMessageId = 0, bool disableNotification = false);
    DynamicJsonDocument sendContact(long chatId, String phoneNumber, String firstName, String lastName = "", long replyToMessageId = 0, bool disableNotification = false);
    DynamicJsonDocument sendChatAction(long chatId, String action);
    //getUserProfilePhotos(int userId, int offset = 0, int limit = 100)
    //setWebhook(String url, DataAvailable dataAvailableCallback, GetNextByte getNextByteCallback)
    //deleteWebhook()
    DynamicJsonDocument sendLocation(long chatId, float latitude, float longitude, long replyToMessageId = 0, bool disableNotification = false, int livePeriod = 0);
    DynamicJsonDocument editMessageReplyMarkup(long chatId, long messageId, long inlineMessageId = 0);
    DynamicJsonDocument deleteMessage(long chatId, long messageId);
    DynamicJsonDocument editMessageLiveLocation(long chatId, long messageId, long inlineMessageId, float latitude, float longitude);
    DynamicJsonDocument stopMessageLiveLocation(long chatId, long messageId, long inlineMessageId);
    DynamicJsonDocument forwardMessage(long chatId, long fromChatId, long messageId, bool disableNotification = false);
    DynamicJsonDocument kickChatMember(long chatId, long userId, long untilDate = -1);
    DynamicJsonDocument unbanChatMember(long chatId, long userId);
    DynamicJsonDocument editMessageText(long chatId, long messageId, String text, String parseMode = "", bool disablePreview = false, long inlineMessageId = 0);
    DynamicJsonDocument editMessageCaption(long chatId, long messageId, String caption = "", long inlineMessageId = 0);
    DynamicJsonDocument sendPhoto(long chatId, String photo, String caption = "", long replyToMessageId = 0, bool disableNotification = false, String parseMode = "");
    DynamicJsonDocument sendDocument(long chatId, String document, String caption = "", long replyToMessageId = 0, bool disableNotification = false, String parseMode = "");
    
    /* @todo implement */
    
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