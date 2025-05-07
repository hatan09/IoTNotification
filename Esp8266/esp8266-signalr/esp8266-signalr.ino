#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <Hash.h>

#include <WebSocketsClient.h> //https://github.com/Links2004/arduinoWebSockets

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
HTTPClient http;

const char* ssid = "Strypper 2.4G"; //Wifi name
const char* password = "Welkom01"; //Wifi password
const char* ip_host = "4r4kl0fp-7251.asse.devtunnels.ms"; //Hub's host
const uint16_t port = 443; //wss port
const char* websocket_path = "/iot-hub"; // Hub's name
const char* notificationWebApiUrl = "localhost:7503/notifaction"; // Route to notification endpoint

int ws_connected = 0;

// const int LED = 2;
const int BTN = 0;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n", payload);
        webSocket.sendTXT("{\"protocol\":\"json\",\"version\":1}");
        connected = 1;
      }
      break;
    case WStype_TEXT:
      Serial.printf("[WSc] get text: %s\n", payload);
      // if (strcmp((char*)payload, "LED_ON") == 0) {
      //   digitalWrite(LED, 0); // Khi client phát sự kiện "LED_ON" thì server sẽ bật LED
      // } else if (strcmp((char*)payload, "LED_OFF") == 0) {
      //   digitalWrite(LED, 1); // Khi client phát sự kiện "LED_OFF" thì server sẽ tắt LED
      // }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  // pinMode(LED, OUTPUT);
  pinMode(BTN, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266 Application");
  for(uint8_t t = 4; t > 0; t--) {
    Serial.println("[SETUP] BOOT WAIT %d...\n");
    delay(1000);
  }

  connectWifi();
  connectSignalRHub();

  initNotiHttpClient();
}

void connectWifi(){
  Serial.println("Connecting to WiFi...");
  WiFiMulti.addAP(ssid, password);
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void initNotiHttpClient(){
  http.begin(notificationWebApiUrl);  //
  http.addHeader("Content-Type", "application/json");
}

void connectSignalRHub(){
  Serial.println("Connecting to SignalR WebSocket...");

  webSocket.beginSSL(ip_host, port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000); // Retry every 5s if disconnected
}

void loop() {
  loopTestPostNotiEndpoint();
}

void loopTestWebSocket(){
  webSocket.loop();

  if(ws_connected == 1){
    webSocket.sendTXT("{\"arguments\":[\"TestGroup\"],\"invocationId\":\"0\",\"target\":\"SendMessage\",\"type\":1}");
    delay(500);
  }
}

void loopTestPostNotiEndpoint(){
  postData = "message";
  
  int httpCode = http.POST(postData);   //gửi giá trị lên api
  String payload = http.getString();    //lấy phản hồi từ api
  Serial.println(payload);
  delay(500);
}