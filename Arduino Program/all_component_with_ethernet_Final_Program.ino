#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"
#define DHTPIN 2     
#define DHTTYPE DHT11   // DHT 11
int val;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
DHT dht(DHTPIN, DHTTYPE);
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT11"));
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(7,OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(20000);
  val = digitalRead(2);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  digitalWrite(11,HIGH);
  
  delay(0);
  digitalWrite(12 ,HIGH);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  String Status;
 if(t > 34){
    digitalWrite(11,LOW);
    Status = "Temp is grater than 34";
  }
  else if(t < 32){
    digitalWrite(12,LOW);
    Status = "Temp is lower than 32";
  }
  else{
    }
    String statu;
    if(val == HIGH ) //this is temp point is 32
     {
        digitalWrite(7,HIGH); 
        statu = "OFF";
     }
     else
      {
        digitalWrite(7,LOW); //if temp goes more than 32 then fan will ON
        statu = "ON";
      }
  Serial.println(Status);
  Serial.println(statu);
  EthernetClient client = server.available();
  if (client) {
     Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
            client.println("Motor is "+statu);
            client.println("Temp is ");
            client.print(t);
            client.println("Humidity is ");
            client.print(h);
            //client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
      }
      // give the web browser time to receive the data
      delay(1);
      Serial.println("client disconnected");
    }
      // close the connection:
      client.stop();
}       
