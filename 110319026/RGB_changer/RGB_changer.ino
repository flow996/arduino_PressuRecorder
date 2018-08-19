#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SSID "GameNetwork"                                   
#define PASS "805801805801"
ESP8266WebServer server(80);

const int led = BUILTIN_LED;
char led_status[200];       // status
char rBuf[50];
char gBuf[50];
char bBuf[50];              // RGB color value
String webSite;             // website in string

 // Analog input pin that the potentiometer is attached to

int sensorValue;        // value read from the pot
int outputValue;        // value output to the PWM (analog out)

unsigned long previousLED = 0;
unsigned long currentMillis = 0;
int LEDinterval =1000;
int iMode = 1;
bool turn;
int red_color,green_color,blue_color;
float red;
float green;
float blue;    
#define analogInPin A0
#define LEDR 5
#define LEDG 4
#define LEDB 14

void buildWeb(){
    webSite += "<html>";
    webSite +="<head>\n";
    webSite +="<meta charset='UTF-8'/>\n";
    webSite +="<title>Form Control RGB_LED</title>\n";
    webSite +="<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>";
    webSite +="<script>";
    webSite +="$(document).ready(function(){";
    webSite +="$(\"[type = range]\").change(function(){";
    webSite +="var redval = $(\"#red\").val();";
    webSite +="$(\"#red-text\").text(redval);";
    webSite +="var greenval = $(\"#green\").val();";
    webSite +="$(\"#green-text\").text(greenval);";
    webSite +="var blueval = $(\"#blue\").val();";
    webSite +="$(\"#blue-text\").text(blueval);";
    webSite +="});";
    webSite +="});";
    webSite +="</script>";
    webSite +="</head>\n";
    webSite += "<body style=background-color:#000000;>";
    webSite +="<div style=width:100vw;>";
    webSite +="<img src=\"http://i.imgur.com/REhfeJ1.gif\" style=width:40vw;>";
    webSite +="<img src=\"http://i.imgur.com/ldoeVrU.gif\" style=width:50vw;></div>\n";
    webSite += "<h1 style=color:#bce9e7;margin-left:30vw;>RGB LED Control form</h1>";
    webSite +="<p id=\"ledstatus\" style=font-family:Verdana;color:#ffffff;margin-left:30vw;>You can select a LED status ...</p>\n";
    webSite += "<form action=\"/form_mode\"　style=font-family:Verdana;color:#ffffff;margin-left:30vw;>";
    webSite += "<input type=\"radio\" id=\"normal\" name=\"Mode\" value=\"normal\"><label for=\"on\"> LED single  </label>\n";
    webSite += "<br>";
    webSite += "<input type=\"radio\" id\"twinkle\" name=\"Mode\" value=\"twinkle\"><label for=\"on\"> LED twinkle  </label>\n";
    webSite += "<br>";
    webSite += "<input type=\"radio\" id=\"colorful\" name=\"Mode\" value=\"colorful\"><label for=\"on\"> LED colorful  </label>\n";
    webSite += "<p>Red Color : <span id = \"red-text\">0</span></p>";
    webSite += "<input type=\"range\" id=\"red\" name=\"R-led\" min=\"0\" max=\"255\" value=\"0\">";
    webSite += "<p>Green Color : <span id = \"green-text\">0</span></p>";
    webSite += "<input type=\"range\" id=\"green\" name=\"G-led\" min=\"0\" max=\"255\" value=\"0\">";
    webSite += "<p>Blue Color : <span id = \"blue-text\">0</span></p>";
    webSite += "<input type=\"range\" id=\"blue\" name=\"B-led\" min=\"0\" max=\"255\" value=\"0\" ><br><br>";
    webSite += "<input type=\"submit\" value=\"submit\" style=background-color:#f5f5f5; color:#323233; border-radius:5px; border-color:#D4D4D4;></form>";
    webSite += "</html>";
}

void handleRoot(){
    server.send(200,"text/html",webSite);
}

void ColorPicker(){          //get color
    red_color = server.arg("R-led").toInt();
    green_color = server.arg("G-led").toInt();
    blue_color = server.arg("B-led").toInt();
    red = (red_color)*(outputValue)/255;
    green = (green_color)*(outputValue)/255;
    blue = (blue_color)*(outputValue)/255;
    analogWrite(LEDR,red);
    analogWrite(LEDG,green);
    analogWrite(LEDB,blue);
    server.send(200,"text/html",webSite);
}

void turnoff(){       //off the light
    analogWrite(LEDR, LOW);
    analogWrite(LEDG, LOW);
    analogWrite(LEDB, LOW);
}

void colorchange(){          //make color change
    red_color = server.arg("R-led").toInt();
    green_color = server.arg("G-led").toInt();
    blue_color = server.arg("B-led").toInt();  
        if(outputValue<85){      //rg change
           analogWrite(LEDR,green_color);
           analogWrite(LEDG,red_color);
           analogWrite(LEDB,blue_color);
          }
        else if((85<=outputValue)||(outputValue<170)){    //gb change
           analogWrite(LEDR,red_color);
           analogWrite(LEDG,blue_color);
           analogWrite(LEDB,green_color);
          }
        else {      //no change
           analogWrite(LEDR,red_color);
           analogWrite(LEDG,green_color);
           analogWrite(LEDB,blue_color);
        }
    server.send(200,"text/html",webSite);
}
void handleLEDStatus() {      //change mode
    if (server.arg("Mode") == "normal") {
        iMode = 1;
    }
    else if (server.arg("Mode") == "twinkle"){      
        iMode = 2; 
   }
    else if (server.arg("Mode") == "colorful"){
        iMode = 3;
    }
    server.send (200 , "text/html" ,webSite);
}

void handleNotFound(){
    server.send(404,"text/html","404 Error!");
}

void setup() {  
    Serial.begin(115200);
    // put your setup code here, to run once:
    buildWeb();
    /*WiFi.softAP(ssid, password); 
    IPAddress myIP = WiFi.softAPIP();*/
    
    /*while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
*/
    WiFi.begin(SSID, PASS); 

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      
    server.on("/",handleRoot);
    server.on("/form_mode",handleLEDStatus);
    server.onNotFound(handleNotFound);
    
    server.begin();
    Serial.println(WiFi.localIP());  
}
void loop() {
    // put your main code here, to run repeatedly:
    server.handleClient(); 

     
    sensorValue = analogRead(analogInPin);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 300, 0, 255);
    // change the analog out value:
   
  
    // print the results to the serial monitor:
    Serial.print("sensor = ");
    Serial.print(sensorValue);
    Serial.print("output = ");
    Serial.println(outputValue);

    if(iMode == 1){
       ColorPicker();
    }
    else if(iMode == 2){          
      if(millis() - previousLED >= LEDinterval){
        if(!turn){
          ColorPicker();
          turn = true;
        }
        else if(turn){
          turnoff();
          turn = false;
        }
        previousLED = millis();
      }
    }
    else if(iMode == 3){
        colorchange();
    }
   
    // wait 2 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:

}
