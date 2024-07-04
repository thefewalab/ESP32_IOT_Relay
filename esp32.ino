#include <WiFi.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <DHT.h>  // Example for a DHT sensor, replace with your sensor library
#include <OneWire.h>
#include <DallasTemperature.h>

// Replace with your network credentials
const char* ssid = "orionspace_2";
const char* password = "CLB2C85750";

const int relay1Pin = 16;
const int relay2Pin = 15;
const int relay3Pin = 17;

const int pin35 = 35;
const int pin36 = 36;


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



// GPIO where the DS18B20 is connected to
const int oneWireBus = 21;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Variables to store temperature values
String temperatureF = "";
String temperatureC = "";

// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;


String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);

  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
  }
  return String(tempC);
}

String readDSTemperatureF() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempF = sensors.getTempFByIndex(0);

  if(int(tempF) == -196){
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Fahrenheit: ");
    Serial.println(tempF);
  }
  return String(tempF);
}

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  else if(var == "TEMPERATUREF"){
    return temperatureF;
  }
  return String();
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

   // Start the DS18B20 sensor
  sensors.begin();

  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  pinMode(relay3Pin, OUTPUT);
  pinMode(oneWireBus, INPUT);

  pinMode(pin35, INPUT_PULLUP);
  pinMode(pin36, INPUT_PULLUP);

  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();

  // Initialize LittleFS
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

   // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html");
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/jquery-3.7.0.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/jquery-3.7.0.js", "application/javascript");
  });

  // Route to load image.png file
  server.on("/logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/logo.png", "image/png");
  });

  // Route to set GPIO to HIGH
  server.on("/bulb-true", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay1Pin, HIGH);    
    //request->send(LittleFS, "/index.html");
  });
  
  // Route to set GPIO to LOW
  server.on("/bulb-false", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay1Pin, LOW);    
    //request->send(LittleFS, "/index.html");
  });

  // Route to set GPIO to HIGH
  server.on("/ac-true", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay2Pin, HIGH);    
    //request->send(LittleFS, "/index.html");
  });
  
  // Route to set GPIO to LOW
  server.on("/ac-false", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay2Pin, LOW);    
    //request->send(LittleFS, "/index.html");
  });

  // Route to set GPIO to HIGH
  server.on("/light-true", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay3Pin, HIGH);    
    //request->send(LittleFS, "/index.html");
  });
  
  // Route to set GPIO to LOW
  server.on("/light-false", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(relay3Pin, LOW);    
    //request->send(LittleFS, "/index.html");
  });

  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureF.c_str());
  });
  
  // Handle switch statuses
  server.on("/pin-status", HTTP_GET, [](AsyncWebServerRequest *request){
    int status35 = digitalRead(pin35);
    int status36 = digitalRead(pin36);
    String status35_str = (status35 == LOW) ? "pressed" : "released";
    String status36_str = (status36 == LOW) ? "pressed" : "released";
    String json = "{\"status35\":\"" + status35_str + "\", \"status36\":\"" + status36_str + "\"}";
    request->send(200, "application/json", json);
  });


  // Start server
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    temperatureF = readDSTemperatureF();
    lastTime = millis();
  } 


}
