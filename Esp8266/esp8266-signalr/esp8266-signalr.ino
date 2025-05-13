#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <Hash.h>

#include <HX711.h>

#include <WebSocketsClient.h> //https://github.com/Links2004/arduinoWebSockets

#define DT_PIN_1 13
#define DT_PIN_2 5
#define SCK_PIN_1 15
#define SCK_PIN_2 4

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
HTTPClient http;

HX711 hx711_1;
HX711 hx711_2;

const char* ssid = "Phuong Thuong"; //Wifi name
const char* password = "Hanhattan34"; //Wifi password
const char* ip_host = "campaign-cloud-fvcccheze3ajhqdu.southeastasia-01.azurewebsites.net"; //Hub's host
const uint16_t port = 443; //wss port
const char* websocket_path = "/devices-hub"; // Hub's name
const char* notificationWebApiUrl = "localhost:7503/notifaction"; // Route to notification endpoint
volatile long weight_1 = 0;
volatile long weight_2 = 0;

const char* id_1 = "abcdefab-abcd-abcd-abcd-abcdefabcdef";
const char* id_2 = "12345678-1234-1234-1234-123456789abc";

int ws_connected = 0;
int is_weight_1_changed = 0;
int is_weight_2_changed = 0;

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
        ws_connected = 1;
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

  hx711_1.begin(DT_PIN_1, SCK_PIN_1);
  delay(1000);

  hx711_2.begin(DT_PIN_2, SCK_PIN_2);
  delay(1000);

  hx711_1.set_scale(412);
  hx711_1.tare();

  hx711_2.set_scale(412);
  hx711_2.tare();

  connectWifi();
  connectSignalRHub();

  //initNotiHttpClient();
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

// void initNotiHttpClient(){
//   http.begin(WiFiMulti, notificationWebApiUrl);  //
//   http.addHeader("Content-Type", "application/json");
// }

void connectSignalRHub(){
  Serial.println("Connecting to SignalR WebSocket...");

  webSocket.beginSSL(ip_host, port, websocket_path);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000); // Retry every 5s if disconnected
}

void loop() {
  if(!ws_connected){
    webSocket.loop();
  }
  else{
    loopTestWebSocket();

    if(hx711_1.is_ready()){
      long reading = hx711_1.get_units(10);
      is_weight_1_changed = (weight_1 - 2) > reading || (weight_1 + 2) < reading;
      if(is_weight_1_changed){
        weight_1 = reading;
      }
      Serial.println(reading);
    }

    if(hx711_2.is_ready()){
      long reading = hx711_2.get_units(10);
      is_weight_2_changed = reading < (weight_2 - 2) || reading > (weight_2 + 2);
      if(is_weight_2_changed){
        weight_2 = reading;
      }
      Serial.println(reading);
    }

    Serial.println("---");
    delay(1000);
  }
}

void process_loadcell(){
  static int count = 0;

}

void loopTestWebSocket(){
  webSocket.loop();

  if(ws_connected == 1){
    if(is_weight_1_changed){
      char buf[512];
      sprintf(buf,"{\"arguments\":[\"{\\\"SensorId\\\":\\\"abcdefab-abcd-abcd-abcd-abcdefabcdef\\\", \\\"CurrentWeight\\\": %ld}\"],\"invocationId\":\"0\",\"target\":\"SendWeightData\",\"type\":1}", weight_1);
      webSocket.sendTXT(buf);
    }

    if(is_weight_2_changed){
      char buf[512];
      sprintf(buf,"{\"arguments\":[\"{\\\"SensorId\\\":\\\"12345678-1234-1234-1234-123456789abc\\\", \\\"CurrentWeight\\\": %ld}\"],\"invocationId\":\"0\",\"target\":\"SendWeightData\",\"type\":1}", weight_2);
      webSocket.sendTXT(buf);
    }
  }
}

void loopTestPostNotiEndpoint(){
  const char* postData = "message";
  
  int httpCode = http.POST(postData);   //gửi giá trị lên api
  String payload = http.getString();    //lấy phản hồi từ api
  Serial.println(payload);
  delay(500);
}