#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "EddaNetwork23";  // Enter targeted WIFI name
const char* password = "Zeynepbora2";  //Enter targeted WIFI password

ESP8266WebServer server(80); //We created a webserver object by using port 80

uint8_t LED1pin = D7; //Selecting the pins
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);//Setting the pins as output pins
  pinMode(LED2pin, OUTPUT);

  
  Serial.println(ssid);
  Serial.println(" Ağına Bağlanılıyor");

  
  WiFi.begin(ssid, password);//We access to wifi that has been specified earlier

  //We check if the connection is established
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print("."); // Until the connection is established, we print dots to serial port
  }
  Serial.println("");
  Serial.println("Ağ Bağlantısı Sağlandı..!");
  Serial.print("IP Adresiniz: ");  Serial.println(WiFi.localIP()); //We learn the local IP from serial port and then we use the IP for browser access

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin(); //Start the server
  Serial.println("HTTP Sunucusu Başlatıldı");
}
float temp_val;
void loop() {
  server.handleClient();
  float temp_adc_val;
  temp_adc_val = analogRead(A0);  /* Read Temperature */
  temp_val = (temp_adc_val / 1023.0)*5000; /* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */
  Serial.print("Temperature = ");
  Serial.print(temp_val);
  Serial.print(" Degree Celsius\n");
  delay(1000);
  //Change the pin voltages according to relevant variables(HIGH or LOW)
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
}

void handle_OnConnect() {//Deafult entries
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Durumu: OFF | GPIO6 Durumu: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); //Updates the html codes
}

void handle_led1on() {//Updates the variables after button event on browser
  LED1status = HIGH;
  Serial.println("GPIO7 Durumu: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); //Updates the html codes
}

void handle_led1off() {//Updates the variables after button event on browser
  LED1status = LOW;
  Serial.println("GPIO7 Durumu: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); //Updates the html codes
}

void handle_led2on() {//Updates the variables after button event on browser
  LED2status = HIGH;
  Serial.println("GPIO6 Durumu: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); //Updates the html codes
}

void handle_led2off() {//Updates the variables after button event on browser
  LED2status = LOW;
  Serial.println("GPIO6 Durumu: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); //Updates the html codes
}

void handle_NotFound(){//404 not found case
  server.send(404, "text/plain", "Sayfa Bulunamadı");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){ // Updating the html codes
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr+="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\"><meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\"><title>Smart Plug Control Center</title>    <style>html {font-family: Helvetica;display: inline-block;            margin: 0px auto;            text-align: center;        }        body {            margin-top: 50px;            background-repeat:no-repeat;            background-image:url('https://images.pexels.com/photos/36717/amazing-animal-beautiful-beautifull.jpg?cs=srgb&dl=pexels-pixabay-36717.jpg&fm=jpg');            background-size: cover;        }        h1 {            color: #444444;            margin: 50px auto 30px;        }        h3 {            color: #444444;            margin-bottom: 50px;        }        .button {            display: block;            width: 80px;            background-color: #1abc9c;            border: none;            color: white;            padding: 13px 30px;            text-decoration: none;            font-size: 25px;            margin: 0px auto 35px;            cursor: pointer;            border-radius: 4px;        }        .button-on {            background-color: #1abc9c;        }        .button-on:active {            background-color: #16a085;        }        .button-off {            background-color: #ff0000;        }        .button-off:active {            background-color: #ff0000;        }        p {            font-size: 14px;            color: #888;            margin-bottom: 10px;        }        .mytext {    width: 300px;    margin: 0 auto;    margin-left: auto;    margin-right: auto;  }    </style></head><body> ";
  ptr+="<h1 style=\"color:rgb(255, 255, 255);\">Smart Plug Control Center</h1>";
  ptr+="<p style=\"color:rgb(255, 255, 255);\">" "Temperature: " + String(temp_val) +" °C" + "</p>";
  if(led1stat){
   ptr+="<p style=\"color:rgb(255, 255, 255);\">Smart Plug Status:</p> <a class=\"button button-off\" href=\"/led1off\">Off</a>\n";
  }  
  else{ptr +="<p style=\"color:rgb(255, 255, 255);\">Smart Plug Status:</p><a class=\"button button-on\" href=\"/led1on\">On</a>\n";}
  
  /*if(led2stat){
  ptr +="<p style=\"color:rgb(255, 255, 255);\">LED2 Durum: On</p><a class=\"button button-off\" href=\"/led2off\">Off</a>\n";
  }
  else{
  ptr +="<p>LED2 Durum: Off</p><a class=\"button button-on\" href=\"/led2on\">On</a>\n";
  }*/
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
