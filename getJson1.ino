#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
  // Connect to HTTP server
//WiFiClient client;
 
const char* ssid     = "";
const char* password = " ";


const char *  Hostname = "worldtimeapi.org";
String        URI = "/api/timezone/asia/kolkata";

// const char * Hostname = "greetingsampl.herokuapp.com";
// String URI_m = "/drinks/";



// const char * Hostname = "worldtimeapi.org";
// String URI = "/api/timezone/asia/kolkata";
const uint16_t port = 80;



void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  while (!Serial) continue;



  Serial.print("Connecting to ");
  Serial.println(ssid);
 


  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  



}

void loop() {

  // Connect to HTTP server
  WiFiClient client;
  client.setTimeout(10000);
  if (!client.connect(Hostname, port)) {
    Serial.println(F("Connection failed"));
    return;
  }
Serial.println(F("Prima di send request------------------------------------!"));
  Serial.println(F("Connected!"));
  // Send HTTP request
  client.println("GET " + URI + " HTTP/1.0");
  client.println("Host: " + (String)Hostname);
  client.println("Connection: close");
  if (client.println() == 0) {
    Serial.println("Failed to send request");
    client.stop();
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.0 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    client.stop();
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  // Allocate the JSON document
  StaticJsonDocument<768> doc;

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    client.stop();
    return;
  }


  String datetime = doc["datetime"]; // 2022-02-11T15:21:21.732473+05:30

  int Day = doc["day_of_week"];

  Serial.print("datetime:");
  Serial.println(datetime);
  Serial.print("Day:");
  Serial.println(Day);
Serial.println("Disconnect:");
  // Disconnect
  client.stop();



  // /***************************************************************************
  //     FOR TIME
  // ***************************************************************************/

  // String Time = datetime.substring(11, 16);
  // String Hour = datetime.substring(11, 13);

  // //24 hour to 12 hour conversion
  // int h = Hour.toInt();
  // if (h > 12)
  //   h = h - 12;
  // else
  //   h = h;
  // Serial.println(h);
  // Hour = (String)h;
  // Time = Time.substring(2);
  // if (h > 10)
  //   Time = Hour + Time;
  // else
  //   Time = "0" + Hour + Time;
  // Serial.print("time - "); Serial.println(Time);

  // /***************************************************************************
  //   FOR DATE
  // ***************************************************************************/

  // String Date = datetime.substring(8, 10);
  // String    Day_str = "";

  // if (Day == 0)
  //   Day_str = "Sun";
  // if (Day == 1)
  //   Day_str = "Mon";
  // if (Day == 2)
  //   Day_str = "Tue";
  // if (Day == 3)
  //   Day_str = "Wed";
  // if (Day == 4)
  //   Day_str = "Thu";
  // if (Day == 5)
  //   Day_str = "Fri";
  // if (Day == 6)
  //   Day_str = "Sat";

  // Serial.print("date - "); Serial.println(Date);
  // Serial.print("day - "); Serial.println(Day_str);
Serial.println("delay 100 - ");
  delay(100);

  // not used in this example
}