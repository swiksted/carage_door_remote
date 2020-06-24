#include <ESP8266WiFi.h>
#include <secrets.h>


int timeout = 5000;

int ledPin = D5;
WiFiServer server(80);

void send_pulse() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(10);


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (WiFi.status() != WL_CONNECTED) {
  WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  // Wait until the client sends some data
  client.setTimeout(1000);
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  client.setTimeout(1000);
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Match the request

  int value = LOW;
  if (request.indexOf("/DoorActivation") != -1) {
    //digitalWrite(ledPin, HIGH);
    //value = HIGH;
    send_pulse();
  }

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Garace door activator: ");

  client.println("<br><br>");
  //client.println("Click <a href=\"/DoorActivation\">here</a> Open/Close the door<br>");
  client.println("<a href=\"DoorActivation\" style=\"font-size: 200px; text-decoration: none\">OPEN/CLOSE DOOR</a>");
  client.println("</html>");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

}
