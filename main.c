#include<WiFi.h>
#include<HTTPClient.h>

const char* ssid = "Jayesh's ESP32"; // setup ssid and password
const char* password = "jayeshm7";
bool login=false;

WiFiServer server(80); // initialize WiFi server

// Success message
String html_success ="<!DOCTYPE html>\
<html> \
  <body> \
      Welcome Home Jayesh Mirani \
  </body> \
</html>";

// Failure message
String html_fail ="<!DOCTYPE html>\
<html> \
  <body> \
      Incorrect Password. Ttry again after 3 LED blinks. \
  </body> \
</html>";


int touch_val,touch_bool=0;
void setup()
{
  // put your setup code here, to run once:
  pinMode(2,OUTPUT); // pin to be used for Blinking inbuilt ESP32 LED 3 times
  pinMode(4,INPUT); // pin for inbuilt touch sensor in ESP32
  Serial.begin(115200); // Start the Serial communication with serial monitor with default baud rate for ESP32 as 115200 

  WiFi.softAP(ssid, password); // Connect local server to ESP32 WiFi network
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address : ");
  Serial.println(IP);
  server.begin();
}

void loop()
{
   // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(client)
  {
    int correct=1;
    if(login==false)
    {
    touch_val = touchRead(4); // read touch value
    int i;
    for(i=0;i<3;i++)
    {
       digitalWrite(2,HIGH); // LED blink process 3 times to notify user to enter password
       delay(1000);
       digitalWrite(2,LOW); 
       delay(1000);
    }
    Serial.println("Enter Password");
    int j;
    int pw_arr[4]= {1,0,0,0};
        for(j=0;j<4 && correct;j++)
        {
            Serial.println("Reading bit no " + String(4-j));
            delay(1000);
            touch_val= touchRead(4);
            
            if(touch_val < 16)
                touch_bool=1;
            Serial.print("touch_bool: ");
            Serial.println(touch_bool);
            if(touch_bool != pw_arr[j])
            {
              correct=0;
            }
            touch_bool=0;
            Serial.print("Current bit is correct: ");
            Serial.println(correct);
        }
    }
    if(correct==1)
    {
        Serial.println("Welcome Home Jayesh Mirani");
        client.print(html_success);
        login=true;
    }
    else
    {
        Serial.println("Incorrect password. Try again");
        client.print(html_fail);
    }
  }
}
