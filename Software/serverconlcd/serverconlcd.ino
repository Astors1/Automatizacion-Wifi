#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD I2C (dirección 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Credenciales Wi-Fi
const char* ssid = "ZTE";
const char* password = "mesi1234";

// Servidor web en el puerto 80
WiFiServer server(80);

// Estados de pines
String output16State = "off";
String output17State = "off";
String output18State = "off";
String output19State = "off";

// Pines asignados a relés
const int output16 = 16;
const int output17 = 17;
const int output18 = 18;
const int output19 = 19;

void setup() {
  Serial.begin(115200);

  // Inicializar I2C para LCD
  Wire.begin(21, 22); // SDA, SCL
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");

  // Configurar GPIO como salida
  pinMode(output16, OUTPUT);
  pinMode(output17, OUTPUT);
  pinMode(output18, OUTPUT);
  pinMode(output19, OUTPUT);
  digitalWrite(output16, LOW);
  digitalWrite(output17, LOW);
  digitalWrite(output18, LOW);
  digitalWrite(output19, LOW);

  // Conectar a Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Wi-Fi conectado
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  Serial.println();
  Serial.println("WiFi conectado.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // Iniciar servidor
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Nuevo cliente.");
    String request = "";
    int newlines = 0;
    unsigned long timeout = millis() + 3000;

    while (client.connected() && millis() < timeout) {
      if (client.available()) {
        char c = client.read();
        request += c;
        Serial.write(c);
        if (c == '\n') {
          newlines++;
          if (newlines == 2) break;
        } else if (c != '\r') {
          newlines = 0;
        }
      }
    }

    Serial.println("\nPetición completa:");
    Serial.println(request);

    // Control de relés por petición
    if (request.indexOf("GET /16/on") >= 0) {
      digitalWrite(output16, HIGH); output16State = "on";
    } else if (request.indexOf("GET /16/off") >= 0) {
      digitalWrite(output16, LOW); output16State = "off";
    }

    if (request.indexOf("GET /17/on") >= 0) {
      digitalWrite(output17, HIGH); output17State = "on";
    } else if (request.indexOf("GET /17/off") >= 0) {
      digitalWrite(output17, LOW); output17State = "off";
    }

    if (request.indexOf("GET /18/on") >= 0) {
      digitalWrite(output18, HIGH); output18State = "on";
    } else if (request.indexOf("GET /18/off") >= 0) {
      digitalWrite(output18, LOW); output18State = "off";
    }

    if (request.indexOf("GET /19/on") >= 0) {
      digitalWrite(output19, HIGH); output19State = "on";
    } else if (request.indexOf("GET /19/off") >= 0) {
      digitalWrite(output19, LOW); output19State = "off";
    }

    // Respuesta HTML al cliente
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();

    client.println("<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<style>html { font-family: Helvetica; text-align: center;} .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px; font-size: 30px; margin: 2px; cursor: pointer;} .button2 {background-color: #555555;}</style>");
    client.println("</head><body><h1>ESP32 Web Server</h1>");

    // Botones para cada pin
    client.println("<p>GPIO 16 - Estado: " + output16State + "</p>");
    client.println("<p><a href=\"/16/" + String(output16State == "off" ? "on" : "off") + "\"><button class=\"button" + String(output16State == "on" ? " button2" : "") + "\">" + String(output16State == "off" ? "ON" : "OFF") + "</button></a></p>");

    client.println("<p>GPIO 17 - Estado: " + output17State + "</p>");
    client.println("<p><a href=\"/17/" + String(output17State == "off" ? "on" : "off") + "\"><button class=\"button" + String(output17State == "on" ? " button2" : "") + "\">" + String(output17State == "off" ? "ON" : "OFF") + "</button></a></p>");

    client.println("<p>GPIO 18 - Estado: " + output18State + "</p>");
    client.println("<p><a href=\"/18/" + String(output18State == "off" ? "on" : "off") + "\"><button class=\"button" + String(output18State == "on" ? " button2" : "") + "\">" + String(output18State == "off" ? "ON" : "OFF") + "</button></a></p>");

    client.println("<p>GPIO 19 - Estado: " + output19State + "</p>");
    client.println("<p><a href=\"/19/" + String(output19State == "off" ? "on" : "off") + "\"><button class=\"button" + String(output19State == "on" ? " button2" : "") + "\">" + String(output19State == "off" ? "ON" : "OFF") + "</button></a></p>");

    client.println("</body></html>");
    client.println();
    client.stop();
    Serial.println("Cliente desconectado.");
  }
}
