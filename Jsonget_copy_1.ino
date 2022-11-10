#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid     = "FASTWEB-cdU9qz";
const char* password = "d6zBNGPCmc";

// const char* ssid     = "TCL 20Y";
// const char* password = "123456al";

const char * Hostname = "greetingsampl.herokuapp.com";
String URI = "/drinks/1";
const uint16_t port = 80;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  // Initialize Serial port
  Serial.begin(115200);
  while (!Serial) continue;
  wifi_connect();
  
  // Connect to HTTP server
  WiFiClient client;
  client.setTimeout(10000);
  if (!client.connect(Hostname, port)) {
    Serial.println(F("Connection failed"));
    return;
                                        }

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
StaticJsonDocument<96> doc;

DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
          }

const char* description = doc["description"]; // "Tastes like grapes "
const char* name = doc["name"]; // "Grape Soda"

  Serial.println("description:-");
  Serial.println(description);
  Serial.println("nAME:-");
  Serial.println(name);
  // Disconnect
  client.stop();
}

void loop() {
  if ( (WiFi.status() == WL_CONNECTED)) 
    {
    long rnd =random(1,5);
    HTTPClient client;
    String mr="http://jsonplaceholder.typicode.com/comments?id" + String(rnd)   ;
    Serial.println("-----kkkkkkkk------------------*************************");
    Serial.println(mr);
    //client.begin("http://jsonplaceholder.typicode.com/comments?id" + String(rnd)   );
    String link="https://greetingsampl.herokuapp.com/drinks/"+ String(rnd) ;
    Serial.println("-----INNNNNNNNjhhhhhhhhtttttLink------------------*************************");
    Serial.println(link);
    client.begin("https://greetingsampl.herokuapp.com/drinks/"+ String(rnd) );
    int httpCode =client.GET();
    
    if (httpCode >0)  {
      
      String payload = client.getString(); 
      Serial.println("\nStatuscode:"+ String(httpCode));
      Serial.println("============================================================");
      //
      Serial.println("payload");
      Serial.println(payload);
    //Serial.println("you can ping me ");
    //delay(500);
    }
      else {
        Serial.println("Error on http request");
            }
  }
  else {
    Serial.write("lost connection");
        }
        delay(500);
  // not used in this example
}

void wifi_connect()
{


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
