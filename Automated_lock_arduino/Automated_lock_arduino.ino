#include<SoftwareSerial.h>
#include<Servo.h>

  SoftwareSerial esp8266(2,3);
  Servo myservo;
#define baudi 115200
#define baud 9600
#define DEBUG true

  int servo = 12;
  int blueLED = 13;


void setup()
{
  
    pinMode(blueLED,OUTPUT);
    digitalWrite(blueLED,HIGH);
    
    
    myservo.attach(servo);
    myservo.write(120);
    
    Serial.begin(baudi);
    esp8266.begin(baudi);
    sendData("AT+CIOBAUD=9600\r\n",1000,DEBUG);
    delay(2000);
    Serial.begin(baud);
    esp8266.begin(baud);
    InitWifiModule();
    digitalWrite(blueLED,LOW);
    delay(500);
    digitalWrite(blueLED,HIGH);
    delay(500);
    digitalWrite(blueLED,LOW);
    
    
}


void loop() {

  if(esp8266.available())
  {
    if(esp8266.find("+IPD,"))
    {
      delay(1000);
      int connectionId = esp8266.read()-48;

      esp8266.find("pin=");

      int pinNumber = (esp8266.read()-48*10);
      pinNumber+=(esp8266.read()-48);

      int statusMot = esp8266.read()-48;
      myservo.write(statusMot*90+30);
      Serial.println(connectionId);
      Serial.println(pinNumber);
      Serial.print("      ");
      Serial.println(statusMot);
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand+="\r\n";
      sendData(closeCommand,1000,DEBUG);
      
    }
  }
}
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    esp8266.print(command);
    long int time = millis();
    while((time+timeout)>millis())
    {
      while(esp8266.available())
      {
        char c = esp8266.read();

        response+=c;
      }
    }
    if(debug)
    {
      Serial.print(response);
    }
    return response;
}

void InitWifiModule()
{
  delay(3000);
  sendData("AT+CWMODE=1\r\n",1500,DEBUG);
  delay(1000);
  sendData("AT+CIFSR\r\n",1500,DEBUG);
  delay(1000);
  sendData("AT+CIPMUX=1\r\n",1500,DEBUG);
  delay(1000);
  sendData("AT+CIPSERVER=1,80\r\n",1500,DEBUG);

}
