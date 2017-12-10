#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Roomba.h>

  // ******************************************************************************** //
  // MQTT + Wifi                                                                      //
  // ******************************************************************************** //
  
  const char* ssid = "Sziliwifi";
  const char* password = "YFFWFNZZ";
  const char* mqtt_server = "192.168.137.1";
  
  WiFiClient espClient;
  PubSubClient client(espClient);
  
  long lastMsg = 0;
  char msg[10];
//  char message_buff[100];
//  int value = 0;

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
          /*
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
          */

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


//  uint8_t bump_n_wheel [10];
//  uint8_t bump_n_wheel_old;
//  bool newdata;

  uint8_t msg_byte [10];
  



  void setup() {
    //pinMode(LED_BUILTIN, OUTPUT);
    pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    digitalWrite(BUILTIN_LED, HIGH);
    
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);


    for (int i = 0; i < 10; i++)
        msg [i] = 0;

    
    
    roomba.reset();
    delay(5000);
    roomba.start();
    delay(1000);
    roomba.fullMode();
    //roomba.clean();
    delay(1000);  
    
  }

  
  void loop() {

      
      if (!client.connected()) {
        reconnect();
      }
      client.loop();


/*
      delay(100);
      //roomba.start();
      delay(100);
      roomba.getSensors(7, bump_n_wheel, 1);
      if (!newdata)
      {
          if (bump_n_wheel[0] != bump_n_wheel_old)
          {
              bump_n_wheel_old = bump_n_wheel[0];
              newdata = true;
          }
              
      }
      else
      {
          int char_written;
          char_written = snprintf (msg, 10, "%d", bump_n_wheel[0]);
          client.publish("Roomba_bumpwheel", msg);
          newdata = false;
      }
 }
      */
      
      

      
      // 2 másodpercentként történik a publikálás
      long now = millis(); 
      if (now - lastMsg > 1000) {
        lastMsg = now;

        //roomba.start();
        roomba.getSensors(43, msg_byte, 2); 
        uint16_t velocity = (msg_byte[0] << 8) | msg_byte[1];
        snprintf (msg, 10, "%d", velocity);
        client.publish("Roomba/velocity", msg);

        msg[0] = 0;
        msg[1] = 0;
        
               
        roomba.getSensors(25, msg_byte, 2);
        uint16_t batteryCharge = (msg_byte[0] << 8) | msg_byte[1];
        snprintf (msg, 10, "%d", batteryCharge);
        client.publish("Roomba/batteryCharge", msg);

        roomba.getSensors(24, msg_byte, 1);
        uint8_t temperature = msg_byte[0];
        snprintf (msg, 10, "%d", temperature);
        client.publish("Roomba/temperature", msg);
        
        
//        if (roomba.getSensors(7, bump_n_wheel, 1))   // bumps n wheels
//          Serial.println("Uj erkezett:");
//        else
//          Serial.println("regi:"); 
//
//        int char_written;
//        char_written = snprintf (msg, 10, "%d", bump_n_wheel[0]);
//        client.publish("Roomba_bumpwheel", msg);
//        Serial.println(msg);
//        Serial.println(char_written);
      }
  }


/**************************************************************************************************************************************************************/
// Halott kod. Spooky....
/**************************************************************************************************************************************************************/
      /*
      // 2 másodpercentként történik a publikálás
      long now = millis(); 
      if (now - lastMsg > 2000) {
        
        lastMsg = now;


        if (roomba.getSensors(7, bump_n_wheel, 1))   // bumps n wheels
          Serial.println("Kiraly!!!! uj adat:");
        else
          Serial.println("regi:");

        Serial.print("Publish message: ");
        Serial.println(bump_n_wheel[0]);

        bump_n_wheel[0] = 7;

        
        for (int i = 0; i < 10; i++)
        {
          c [i] = (char)bump_n_wheel[i];
        }

        
        //client.publish("penis", c);

      }

      */
      

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
      
      
  

