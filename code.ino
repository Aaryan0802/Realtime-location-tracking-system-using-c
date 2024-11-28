//Rx D1  TX D2 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>


TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5); 


const char* ssid = "Aaryan";
const char* password = "nahidenar";


float Latitude , Longitude;
int year , month , date, hour , minute , second;
String DateString , TimeString , LatitudeString , LongitudeString;




WiFiServer server(80);
void setup()
{
  Serial.begin(9600);
  SerialGPS.begin(9600);
  Serial.println();
  Serial.print("Connecting");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");


  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}


void loop()
{
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);
      }


      if (gps.date.isValid())
      {
        DateString = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();


        if (date < 10)
        DateString = '0';
        DateString += String(date);


        DateString += " / ";


        if (month < 10)
        DateString += '0';
        DateString += String(month);
        DateString += " / ";


        if (year < 10)
        DateString += '0';
        DateString += String(year);
      }


      if (gps.time.isValid())
      {
        TimeString = "";
        hour = gps.time.hour()+ 5; //adjust UTC
        minute = gps.time.minute();
        second = gps.time.second();
    
        if (hour < 10)
        TimeString = '0';
        TimeString += String(hour);
        TimeString += " : ";


        if (minute < 10)
        TimeString += '0';
        TimeString += String(minute);
        TimeString += " : ";


        if (second < 10)
        TimeString += '0';
        TimeString += String(second);
      }


    }
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }


  //Response
    String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE html><html><head>";
  s += "<title>NEO-6M GPS Readings</title>";
  s += "<style>";
  s += "body { font-family: 'Arial', sans-serif; margin: 0; padding: 0; background: linear-gradient(135deg, #74ebd5, #ACB6E5); color: #333; }";
  s += "header { background: linear-gradient(90deg, #4CAF50, #2E7D32); color: white; padding: 20px 0; text-align: center; font-size: 3em; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2); }";
  s += "table { margin: 30px auto; width: 80%; border-collapse: collapse; background: white; border-radius: 10px; overflow: hidden; box-shadow: 0 6px 12px rgba(0, 0, 0, 0.1); }";
  s += "table th, table td { padding: 20px; text-align: center; border: 1px solid #ddd; font-size: 1.2em; }";
  s += "table th { background: #4CAF50; color: white; font-size: 1.3em; }";
  s += "table tr:nth-child(even) { background: #f9f9f9; }";
  s += "table tr:hover { background: #e0f7fa; transform: scale(1.01); transition: all 0.2s ease-in-out; }";
  s += ".button { display: inline-block; margin: 30px auto; padding: 15px 30px; font-size: 1.5em; color: white; background: linear-gradient(90deg, #FF5722, #E91E63); text-decoration: none; border-radius: 30px; box-shadow: 0 4px 10px rgba(0, 0, 0, 0.3); transition: background 0.3s, box-shadow 0.3s; }";
  s += ".button:hover { background: linear-gradient(90deg, #E91E63, #FF5722); box-shadow: 0 6px 14px rgba(0, 0, 0, 0.4); }";
  s += ".footer { text-align: center; font-size: 1em; color: #666; margin-top: 50px; }";
  s += "</style>";
  s += "</head><body>";
  s += "<header>NEO-6M GPS Readings</header>";
  s += "<p style=\"text-align: center; font-size: 1.5em; margin-top: 20px; color: #333;\"><strong>Location Details</strong></p>";
  s += "<table>";
  s += "<tr><th>Latitude</th><td>" + LatitudeString + "</td></tr>";
  s += "<tr><th>Longitude</th><td>" + LongitudeString + "</td></tr>";
  s += "<tr><th>Date</th><td>" + DateString + "</td></tr>";
  s += "<tr><th>Time</th><td>" + TimeString + "</td></tr>";
  s += "</table>";

  if (gps.location.isValid())
  {
    s += "<div style=\"text-align: center;\">";
    s += "<a class=\"button\" href=\"http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=" + LatitudeString + "+" + LongitudeString + "\" target=\"_top\">";
    s += "Open Location in Google Maps</a>";
    s += "</div>";
  }

  s += "<div class=\"footer\">&copy; 2024 GPS Tracker | Powered by NEO-6M</div>";
  s += "</body></html>\n";



  client.print(s);
  delay(100);


}