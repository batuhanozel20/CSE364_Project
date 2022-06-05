#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/*Ağ adınızı ve Şifrenizi Giriniz*/
const char* ssid = "EddaNetwork23";  // Ağ adınızı girin
const char* password = "Zeynepbora2";  //Ağ şifrenizi girin

ESP8266WebServer server(80); //80 portunu kullanarak bir webserver nesnesi oluşturduk

uint8_t LED1pin = D7;
bool LED1status = LOW;

uint8_t LED2pin = D6;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  
  Serial.println(ssid);
  Serial.println(" Ağına Bağlanılıyor");

  //yerel ağınıza bağlanmanız için gerekli komut
  WiFi.begin(ssid, password);

  //bağlantının gerçekleştiğini kontrol ediyoruz
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print("."); //gerçekleşene kadar loading gibi işaret yaptırıyoruz
  }
  Serial.println("");
  Serial.println("Ağ Bağlantısı Sağlandı..!");
  Serial.print("IP Adresiniz: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Sunucusu Başlatıldı");
}
float temp_val;
void loop() {
  server.handleClient();
  int temp_adc_val;
  temp_adc_val = analogRead(A0);  /* Read Temperature */
  temp_val = (temp_adc_val / 1023.0)*5000; /* Convert adc value to equivalent voltage */
  temp_val = (temp_val/10); /* LM35 gives output of 10mv/°C */
  Serial.print("Temperature = ");
  Serial.print(temp_val);
  Serial.print(" Degree Celsius\n");
  delay(1000);
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}
  
  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Durumu: OFF | GPIO6 Durumu: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO7 Durumu: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 Durumu: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 Durumu: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 Durumu: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Sayfa Bulunamadı");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
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
