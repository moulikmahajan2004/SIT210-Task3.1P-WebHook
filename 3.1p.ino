//These are the libraries 
#include <Arduino.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// DHT22 sensor connected to digital pin 4
#define DHTPIN 4   
// Using the DHT22 type 
#define DHTTYPE DHT22 

//keys to establish connections
//wifi name and the password
#define WIFI_NAME "Moulik"
#define WIFI_PWD "12345678"

//apikey for integration
#define MY_API_KEY "P9L2NFLJ18OB1RPW"

DHT dht(DHTPIN, DHTTYPE);

//wifipasswordwith the variabledefined above
char ssid[] = WIFI_NAME;
char pass[] = WIFI_PWD;

//chaanel id and api key for verification to use the api
unsigned long channel_ID =2251754; 
const char *apiKey = MY_API_KEY;

//created the variable wificlient with wifficlient type
WiFiClient wifiClient;

void setup() {
  //serial communication between the pc and nano
  Serial.begin(9600);
  //dht,begin funvtion
  dht.begin();
 WIFICONNECTION_FUNCTION();
}

void loop() {
  //variables od temp and the huni 
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();

//if sensor connected successfully then it return the sensor value else it print the sensor is not connected successfully
  if (isnan(temp) || isnan(humi)) {
    Serial.println("Sensor not connected properly values can not be fetched !!!!!");
    delay(2000);
    return;
  }
//printing the values
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("°C, Humi: ");
  Serial.print(humi);
  Serial.println("%");

//wficlient varaile is passed in the thingspeak begin function
  ThingSpeak.begin(wifiClient);
  //field number along with the value is sent to the thingspeak
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humi);

//Browser code is the sent when therese is stable connection few codes are set on the particular protocols like 200 is when the connection is succesfull and the data is sent
  int Browser_code = ThingSpeak.writeFields(channel_ID, apiKey);

  if (Browser_code == 200) {
    Serial.println("Data successfully sent to thingspeak");
  } else {
    //this printed  when the error is occured while sending along with it the code is also printed
    Serial.println(Browser_code)
    Serial.println("Error while sending the data to the thing speak");
  }

  delay(30000); 
}
//created the seprate function to reduce the workload as the seprate function will reduce the work load asn is called only once after the connection there is no need 
void WIFICONNECTION_FUNCTION() {
  //checking the status of the wifi
  int status = WL_IDLE_STATUS;
  //if the status is not equal yo Wl_connected then trying to connectying to wifi is printed
  while (status != WL_CONNECTED) {
    Serial.print("Trying to Connect to WIFI...");
    //wifi.befin are the built in function in library where the ssid or wifiname and the pass hich is passsword is passed
    status = WiFi.begin(ssid, pass);
    delay(500);
  }
  Serial.println();
  //if connected then wifi is connected 
  Serial.println("Successfully connectde to the wifi");
}