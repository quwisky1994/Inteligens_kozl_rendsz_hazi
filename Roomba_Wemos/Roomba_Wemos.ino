#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Roomba.h>

  // ******************************************************************************** //
  // MQTT + Wifi                                                                      //
  // ******************************************************************************** //
  
  const char* ssid = "SmartHome";
  const char* password = "virslibabszalon";
  const char* mqtt_server = "iot.eclipse.org";
  
  WiFiClient espClient;
  PubSubClient client(espClient);
  
  long lastMsg = 0;
  char msg[50];
  char msgspeed[2];
  char message_buff[100];
  int value = 0;

void setup_wifi() {
          delay(10);
          // We start by connecting to a WiFi network
          Serial.println();
          Serial.print("Connecting to ");
          Serial.println(ssid);
        
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

void callback(char* topic, byte* payload, unsigned int length) {
          Serial.print("Message arrived [");
          Serial.print(topic);
          Serial.print("] ");
          for (int i = 0; i < length; i++) {
            Serial.print((char)payload[i]);
          }
          Serial.println();
        
          // Switch on the LED if an 1 was received as first character
          if ((char)payload[0] == '1') {
            digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on
          } else {
            digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off 
          }

}

void reconnect() {
        // Loop until we're reconnected
        while (!client.connected()) {
          Serial.print("Attempting MQTT connection...");
          // Attempt to connect
          if (client.connect("ESP8266Client")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "hello world");
            // ... and resubscribe
            client.subscribe("inTopic");
          } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
          }
        }
}





  // ******************************************************************************** //
  // Foprogram                                                                        //
  // ******************************************************************************** //

  // Rx: feher  - D2
  // Tx: szurke - D1
  // Roomba inicializalas           rxPin, txPin,
  Roomba roomba (new SoftwareSerial(   D1,   D2), Roomba::Baud115200);

  bool stp = true;




  void setup() {
    //pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    digitalWrite(BUILTIN_LED, HIGH);
    
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

    /*
    delay(1000);
    roomba.reset();
    delay(1000);
    roomba.start();
    delay(1000);
    roomba.fullMode();
    delay(1000);  
    */
  }
  
  void loop() {

      
      if (!client.connected()) {
        reconnect();
      }
      
      client.loop();

      long now = millis();
      if (now - lastMsg > 2000) {
        lastMsg = now;
        ++value;
        snprintf (msg, 75, "%d", 321);
        msgspeed[0] = 0x01;                 // 321 = 0x141
        msgspeed[1] = 0x41;
        Serial.print("Publish message: ");
        Serial.println((char*)msgspeed);
        
        client.publish("penis", msgspeed);
      }
      
    /*
      if (stp)
      {
        roomba.driveDirect(200, 50);    // turning right
        digitalWrite(LED_BUILTIN, LOW);
        delay(1000);
        stp = false;
        roomba.driveDirect(0, 0);
        digitalWrite(LED_BUILTIN, HIGH); 
      }
      else
      {
        for (int i = 0; i < 5; i++)
        {
          digitalWrite(LED_BUILTIN, LOW); 
          delay(500);
          digitalWrite(LED_BUILTIN, HIGH); 
          delay(500);     
        }
        roomba.stop();
      }
      */
      
  }

