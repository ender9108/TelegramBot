#include "TelegramBot.h"

TelegramBot::TelegramBot(WiFiClientSecure &wifiClient) {
  this->client = &wifiClient;
}

TelegramBot::TelegramBot(WiFiClientSecure &wifiClient, String token) {
  this->client = &wifiClient;
  this->token  = token;
  this->baseAction = "/bot" + this->token + "/";
}

void TelegramBot::setToken(String token) {
  this->token = token;
  this->baseAction = "/bot" + this->token + "/";
}

void TelegramBot::enableDebugMode() {
  this->debugMode = true;
}

void TelegramBot::setTimeToRefresh(long ttr) {
  this->timeToRefresh = ttr;
}

void TelegramBot::logger(String message, bool endLine) {
  if (true == this->debugMode) {
    if (true == endLine) {
        Serial.println(message);
    } else {
        Serial.print(message);
    }
  }
}

bool TelegramBot::on(int event, EventCallback callback) {
  switch (event) {
    case TELEGRAM_EVT_NEW_MSG:
      this->onNewMessage = callback;
      return true;
      break;
    default:
      this->logger("Uncknow " + String(event) + " event");
      break;
  }

  return false;
}

int TelegramBot::loop() {
  if (millis() > this->lastUpdateTime + this->timeToRefresh)  {
      this->lastUpdateTime = millis();
      this->logger(F("Checking for messages.. "));

      int newMessage = this->getUpdates(this->lastUpdateId);

      if (newMessage > 0 && this->onNewMessage != NULL) {
        this->onNewMessage(this->messages, this->lastUpdateId);
      }

      return newMessage;
  }

  return 0;
}

long TelegramBot::getLastUpdateId() {
  return this->lastUpdateId;
}

User TelegramBot::getMe() {
  this->logger("Start getMe");
  DynamicJsonDocument document = this->sendGetCommand("getMe");
  
  if (this->debugMode) {
    serializeJson(document, Serial);
    this->logger("");
  }

  if (document.containsKey("ok") && document["ok"] == false) {
    this->logger(document["message"]);
  }

  JsonObject response = document.to<JsonObject>();

  this->botUser = this->hydrateUserStruct(response);

  return this->botUser;
}

int TelegramBot::getUpdates(int offset, int limit) {
  this->logger("Start getUpdates");
  DynamicJsonDocument response = this->sendGetCommand("getUpdates?offset=" + String(offset) + "&limit=" + String(limit));

  if (response.containsKey("ok") && true == response["ok"]) {
    int size = response["result"].size();
    this->logger("Response size: " + String(size));
    if (size > 0) {
      if (size > TELEGRAM_MAX_MSG) {
        size = TELEGRAM_MAX_MSG;
      }

      int messageIndex = 0;

      for (int i = 0; i < size; i++) {
        if (this->parseMessages(response["result"][i], messageIndex)) {
          messageIndex++;
        }
      }

      return messageIndex;
    } else {
      this->logger("No message.");
    }
  }

  return 0;
}

Message* TelegramBot::getMessages(bool forceUpdate) {
  if (true == forceUpdate) {
    this->getUpdates(this->lastUpdateId);
  }

  return this->messages;
}

Message TelegramBot::getLastMessage(bool forceUpdate) {
  if (true == forceUpdate) {
    this->getUpdates(this->lastUpdateId);
  }

  return this->messages[0];
}

DynamicJsonDocument TelegramBot::sendMessage(
  long chatId, 
  String text, 
  String parseMode, 
  bool disablePreview, 
  long replyToMessageId, 
  bool disableNotification
) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["text"] = text;
  postData["parse_mode"] = parseMode;
  postData["disable_preview"] = disablePreview;
  postData["reply_to_message_id"] = replyToMessageId;
  postData["disable_notification"] = disableNotification;

  DynamicJsonDocument response = this->sendPostCommand("sendMessage", postData);

  return response;
}

DynamicJsonDocument TelegramBot::sendContact(
  long chatId, 
  String phoneNumber, 
  String firstName, 
  String lastName, 
  long replyToMessageId, 
  bool disableNotification
) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["phone_number"] = phoneNumber;
  postData["first_name"] = firstName;
  postData["last_name"] = lastName;
  postData["reply_to_message_id"] = replyToMessageId;
  postData["disable_notification"] = disableNotification;

  DynamicJsonDocument response = this->sendPostCommand("sendContact", postData);

  return response;
}

DynamicJsonDocument TelegramBot::sendChatAction(long chatId, String action) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["action"] = action;

  DynamicJsonDocument response = this->sendPostCommand("sendChatAction", postData);

  return response;
}

DynamicJsonDocument TelegramBot::sendLocation(
  long chatId, 
  float latitude, 
  float longitude, 
  long replyToMessageId, 
  bool disableNotification, 
  int livePeriod
) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["latitude"] = latitude;
  postData["longitude"] = longitude;
  postData["reply_to_message_id"] = replyToMessageId;
  postData["disable_notification"] = disableNotification;
  postData["live_period"] = livePeriod;

  DynamicJsonDocument response = this->sendPostCommand("sendLocation", postData);

  return response;
}

DynamicJsonDocument TelegramBot::editMessageReplyMarkup(long chatId, long messageId, long inlineMessageId) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["message_id"] = messageId;
  postData["inline_message_id"] = inlineMessageId;

  DynamicJsonDocument response = this->sendPostCommand("editMessageReplyMarkup", postData);

  return response;
}

DynamicJsonDocument TelegramBot::deleteMessage(long chatId, long messageId) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["message_id"] = messageId;

  DynamicJsonDocument response = this->sendPostCommand("deleteMessage", postData);

  return response;
}

DynamicJsonDocument TelegramBot::editMessageLiveLocation(long chatId, long messageId, long inlineMessageId, float latitude, float longitude) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(5);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["message_id"] = messageId;
  postData["inline_message_id"] = inlineMessageId;
  postData["latitude"] = latitude;
  postData["longitude"] = longitude;

  DynamicJsonDocument response = this->sendPostCommand("editMessageLiveLocation", postData);

  return response;
}

DynamicJsonDocument TelegramBot::stopMessageLiveLocation(long chatId, long messageId, long inlineMessageId) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["message_id"] = messageId;
  postData["inline_message_id"] = inlineMessageId;

  DynamicJsonDocument response = this->sendPostCommand("stopMessageLiveLocation", postData);

  return response;
}

DynamicJsonDocument TelegramBot::forwardMessage(long chatId, long fromChatId, long messageId, bool disableNotification) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["from_chat_id"] = messageId;
  postData["message_id"] = messageId;
  postData["disable_notification"] = disableNotification;

  DynamicJsonDocument response = this->sendPostCommand("forwardMessage", postData);

  return response;
}

DynamicJsonDocument TelegramBot::kickChatMember(long chatId, long userId, long untilDate) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(3);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["user_id"] = userId;

  if (untilDate != -1) {
    postData["until_date"] = untilDate;
  }

  DynamicJsonDocument response = this->sendPostCommand("kickChatMember", postData);

  return response;
}

DynamicJsonDocument TelegramBot::unbanChatMember(long chatId, long userId) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["user_id"] = userId;

  DynamicJsonDocument response = this->sendPostCommand("unbanChatMember", postData);

  return response;
}

DynamicJsonDocument TelegramBot::editMessageText(long chatId, long messageId, String text, String parseMode, bool disablePreview, long inlineMessageId) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["message_id"] = messageId;
  postData["text"] = text;
  postData["parse_mode"] = parseMode;
  postData["disable_preview"] = disablePreview;
  postData["inline_message_id"] = inlineMessageId;

  DynamicJsonDocument response = this->sendPostCommand("editMessageText", postData);

  return response;
}

DynamicJsonDocument TelegramBot::editMessageCaption(long chatId, long messageId, String caption, long inlineMessageId) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(4);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["message_id"] = messageId;
  postData["caption"] = caption;
  postData["inline_message_id"] = inlineMessageId;

  DynamicJsonDocument response = this->sendPostCommand("editMessageCaption", postData);

  return response;
}

DynamicJsonDocument TelegramBot::sendPhoto(
  long chatId, 
  String photo, 
  String caption, 
  long replyToMessageId, 
  bool disableNotification, 
  String parseMode
) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["photo"] = photo;
  postData["caption"] = caption;
  postData["reply_to_messageId"] = replyToMessageId;
  postData["disable_notification"] = disableNotification;
  postData["parse_mode"] = parseMode;

  DynamicJsonDocument response = this->sendPostCommand("editMessageCaption", postData);

  return response;
}

DynamicJsonDocument TelegramBot::sendDocument(
  long chatId, 
  String document, 
  String caption, 
  long replyToMessageId, 
  bool disableNotification, 
  String parseMode
) {
  const size_t CAPACITY = JSON_OBJECT_SIZE(6);
  StaticJsonDocument<CAPACITY> doc;

  JsonObject postData = doc.to<JsonObject>();
  postData["chat_id"] = chatId;
  postData["document"] = document;
  postData["caption"] = caption;
  postData["reply_to_messageId"] = replyToMessageId;
  postData["disable_notification"] = disableNotification;
  postData["parse_mode"] = parseMode;

  return this->sendPostCommand("sendDocument", postData);
}

bool TelegramBot::parseMessages(JsonObject message, int index) {
  int updateId = message["update_id"];

  if (this->lastUpdateId != updateId) {
    this->lastUpdateId = updateId;

    for (int i = (TELEGRAM_MAX_MSG-2); i >= 0; i--) {
      this->messages[(i+1)] = this->messages[i];
    }

    this->messages[0] = this->hydrateMessageStruct(message["message"]);

    return true;
  }

  return false;
}

Message TelegramBot::hydrateMessageStruct(JsonObject jsonMessage) {
  Message msg;

  msg.id = jsonMessage["message_id"].as<int>();
  msg.date = jsonMessage["date"].as<int>();
  msg.text = jsonMessage["text"].as<String>();
  //msg.type = jsonMessage["entities"]["type"].as<String>();

  msg.from = this->hydrateUserStruct(jsonMessage["from"]);
  msg.chat = this->hydrateChatStruct(jsonMessage["chat"]);

  return msg;
}

User TelegramBot::hydrateUserStruct(JsonObject jsonUser) {
  User user;
  user.id = jsonUser["id"].as<int>();
  user.firstName = jsonUser["first_name"].as<String>();
  user.lastName = jsonUser["last_name"].as<String>();
  user.username = jsonUser["username"].as<String>();
  user.languageCode = jsonUser["language_code"].as<String>();
  user.isBot = jsonUser["is_bot"].as<bool>();

  return user;
}

Chat TelegramBot::hydrateChatStruct(JsonObject jsonChat) {
  Chat chat;
  chat.id = jsonChat["id"];

  return chat;
}

DynamicJsonDocument TelegramBot::sendGetCommand(String action) {
  DynamicJsonDocument response(4096);

  HTTPClient httpClient;
  this->logger("CALL -> " + String(TELEGRAM_HOST)+(this->baseAction + action));
  httpClient.begin(*this->client, String(TELEGRAM_HOST), TELEGRAM_PORT, (this->baseAction + action));
  int httpCode = httpClient.GET();

  if (httpCode == HTTP_CODE_OK) {
    String payload = httpClient.getString();
    this->logger(payload);
    DeserializationError error = deserializeJson(response, payload);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return this->buildJsonResponseError(httpCode, F("deserializeJson() failed"));
    }
  } else {
    return this->buildJsonResponseError(httpCode, httpClient.getString());
  }

  httpClient.end();

  return response;
}

DynamicJsonDocument TelegramBot::sendPostCommand(String action, JsonObject payloadObject) {
  DynamicJsonDocument response(512);

  HTTPClient httpClient;
  this->logger("CALL -> " + String(TELEGRAM_HOST)+(this->baseAction + action));
  httpClient.begin(*this->client, String(TELEGRAM_HOST), TELEGRAM_PORT, (this->baseAction + action));
  httpClient.addHeader(F("Content-Type"), F("application/json"));

  String postData;
  serializeJson(payloadObject, postData);

  int httpCode = httpClient.POST(postData);

  if (httpCode == HTTP_CODE_OK) {
    String payload = httpClient.getString();
    this->logger(payload);
    DeserializationError error = deserializeJson(response, payload);

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return this->buildJsonResponseError(httpCode, F("deserializeJson() failed"));
    }
  } else {
    return this->buildJsonResponseError(httpCode, httpClient.getString());
  }

  httpClient.end();

  return response;
}

DynamicJsonDocument TelegramBot::buildJsonResponseError(int statusCode, String message) {
  const int capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(1);
  DynamicJsonDocument response(capacity);
  response["ok"] = false;
  response["statusCode"] = statusCode;
  response["message"] = message;

  return response;
}