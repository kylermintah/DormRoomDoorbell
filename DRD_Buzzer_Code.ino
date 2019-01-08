#include <WiFi101.h>
#include "String.h"
#include <SPI.h>

char ssid[] = "AndroidAP"; //  your network SSID (name) 
char pass[] = "abcdefgh";  // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;
int status = WL_IDLE_STATUS;
int previous = 0;


//thingspeak server
char server[] = "api.thingspeak.com";


WiFiClient client;


//API key for the Thingspeak ThingHTTP
const String apiKey = "2GFWH5AQ2S4VS12B";

//the number the message should be sent to
const String sendNumber = "+16466375618";


const int pinBuzzer = 9;
static boolean doorman = false;

int data = 0;

void setup() {
//SERIAL SETUP
  Serial.begin(9600);
  
//PINModeSETUP
  pinMode (pinBuzzer, OUTPUT);
 
  
//WIFI SECTION
//set up WiFi:
  if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  // attempt to connect to Wifi network:
 while (status != WL_CONNECTED) { 
  //  Serial.print("Attempting to connect to SSID: ");
   // Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    status = WiFi.begin(ssid, pass);
  
    // wait 10 seconds for connection:
    delay(1);
  } 
  //Serial.println("Connected to wifi");
 // printWifiStatus();

}


//loopSTART
void loop() {
//Serial.print(BTSerial.available());


 while(Serial.available())  {  // Send data only when you receive data:
  
    delay(1);
    data = Serial.parseInt();      //Read the incoming data and store it into variable data
    
    if(data == 1)  {          //Checks whether value of data is equal to 1 
      digitalWrite(pinBuzzer, HIGH);  //If value is 1 then LED turns ON
      
      tone(pinBuzzer,76);
      digitalWrite(pinBuzzer, HIGH);
      previous = 1;
      
    }
    
    else {    //Checks whether value of data is equal to 0
         //If value is 0 then LED turns OFF
      if (previous == 1){
        noTone(pinBuzzer);
        digitalWrite(pinBuzzer, LOW);
       sendSMS(sendNumber, URLEncode("DRD: There is someone at your door"));
        previous=0;
      }
    }
      Serial.flush();
  }                        
   
}
//loopEND
void sendSMS(String number,String message)
{
  // Make a TCP connection to remote host
  if (client.connect(server, 80))
  {


    //should look like this...
    //api.thingspeak.com/apps/thinghttp/send_request?api_key={api key}&number={send to number}&message={text body}
    //VWSJIJ92077SKE9G&13473219561&message=DRD

    client.print("GET /apps/thinghttp/send_request?api_key=");
    client.print(apiKey);
    client.print("&number=");
    client.print(number);
    client.print("&message=");
    client.print(message);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  }
  else
  {
   // Serial.println(F("Connection failed"));
  } 


  // Check for a response from the server, and route it
  // out the serial port.
  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      Serial.print(c);
    }      
  }
  Serial.println();
  client.stop();
}

//URLencodeSTART

String URLEncode(const char* msg)
{
  const char *hex = "0123456789abcdef";
  String encodedMsg = "";


  while (*msg!='\0'){
    if( ('a' <= *msg && *msg <= 'z')
      || ('A' <= *msg && *msg <= 'Z')
      || ('0' <= *msg && *msg <= '9') ) {
      encodedMsg += *msg;
    } 
    else {
      encodedMsg += '%';
      encodedMsg += hex[*msg >> 4];
      encodedMsg += hex[*msg & 15];
    }
    msg++;
  }
  return encodedMsg;
}
//URLencodeEND

//printWifiStatusSTART

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());


  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);


  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
//printWifiStatusEND
  
  

