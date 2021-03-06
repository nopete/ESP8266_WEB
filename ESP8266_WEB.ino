// Inspired by http://allaboutee.com/2014/12/30/esp8266-and-arduino-webserver/
#include <SoftwareSerial.h>
 
#define DEBUG true
 
SoftwareSerial esp8266(8,9); // make RX Arduino line is pin 8, make TX Arduino line is pin 9.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 8
                             // and the RX line from the esp to the Arduino's pin 9

static const String AP_NAME=REPLACE!!! // "AP_NAME" ;
static const String AP_PASSWD=SUBSTITUTE!!! // AP_PASSWD;

void setup()
{
  uint32_t baud = 9600;
  Serial.begin(9600);
  esp8266.begin(115200);
  esp8266.print("AT+CIOBAUD=");
  esp8266.println(baud);
  esp8266.begin(baud);
  Serial.print("SETUP!! @");
  Serial.println(baud);
  
 
  
 // sendData("AT+RST\r\n",2000,DEBUG); // reset module
  //sendData("AT+CWMODE=2\r\n",1000,DEBUG); // configure as access point
  sendData("AT+CWMODE=1\r\n",1000,DEBUG); // configure as a client
//  sendData("AT+CWLAP\r\n",1000,DEBUG); // configure as a client

  sendData("AT+CWJAP=\"",100,DEBUG);
  sendData(AP_NAME,100, DEBUG);
  sendData("\",\"",100, DEBUG);
  sendData(AP_PASSWD,100, DEBUG);
  sendData("\"\r\n",10000,DEBUG); // configure as access point

  sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
}
 
void loop()
{
  if(esp8266.available()) // check if the esp is sending a message 
  {
    /*
    while(esp8266.available())
    {
      // The esp has data so display its output to the serial window 
      char c = esp8266.read(); // read the next character.
      Serial.write(c);
    } */
    
    if(esp8266.find("+IPD,"))
    {
     delay(1000);
 
     int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
     
     String webpage = "<h1>Hello</h1>&lth2>World!</h2><button>LED1</button>";
 
     String cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
     
     webpage="<button>LED2</button>";
     
     cipSend = "AT+CIPSEND=";
     cipSend += connectionId;
     cipSend += ",";
     cipSend +=webpage.length();
     cipSend +="\r\n";
     
     sendData(cipSend,1000,DEBUG);
     sendData(webpage,1000,DEBUG);
 
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendData(closeCommand,3000,DEBUG);
    }
  }
}
 
 
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    if (debug)
    {
      Serial.print(">>>>> SENDING ");
      Serial.println(command);
    }
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(">>>>> RECEIVED ");
      Serial.print(response);
    }
    
    return response;
}

