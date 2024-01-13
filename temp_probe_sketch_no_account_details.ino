//all tbe following liraries need to be installed and included
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WifiClient.h>
#include <ThingSpeak.h>

// put wifi name and password here (WiFi SSID is the hub name): 
#define WIFI_SSID "own_SSID_here"
#define WIFI_PASSWORD "own_Password_here"

//ThingSpeak channel  
unsigned long myChannelNumber = own_channel_number_here;  // UNCOMMENT THIS LINE
//ThingSpeak API key
const char * myWriteAPIKey = "own_API_key_here"; // UNCOMMENT THIS LINE

// 
WiFiClient client;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() !=WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Serial.println(); 
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println(String("ESP chip ID ") + ESP.getChipId());
  Serial.println();

  ThingSpeak.begin(client);
}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  //Sets the value of a specific field in the ThingSpeak channel. e.g temp. 
  //Without this line the temp is always -127.00ºC if ThingSpeak.writefields is not commented out
  ThingSpeak.setField(1, temperatureC);

  //Comments out '200' if the data has been successfuly sent
  //Serial.println(ThingSpeak.setField(1, temperatureC));

  Serial.print(temperatureC);
  Serial.println("ºC");
  
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  
  delay(3000);
}