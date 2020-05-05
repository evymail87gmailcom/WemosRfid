#pragma once
#include "Arduino.h"
#include <Arduino_JSON.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#include <SPI.h>
#include <MFRC522.h>
class ServiceDesk {
public:


#define SS_PIN D8 //Pin on WeMos D1 Mini
#define RST_PIN D3 //Pin on WeMos D1 Mini
	

	String card_uid;
	String testcard_uid = "12";
	// network settings
	const char* ssid = "#Telia-5BB008";
	const char* password = "A()Zc14yN1#uZw8)";

	HTTPClient http;

	const char* serverName = "http://5eac5fba4bf71e00166a0821.mockapi.io/api/v1/Employees";

	const char* folderLocation = "/api/v1/Employees";
	const char* folderLocation2 = "/3";

	//String serverPath = serverName + folderLocation;
	String dataReadings;
	String id, firstName, lastname, role, isActive, gdpr, status, cardNumber;
	char dataReadinsgArr[50];

	const int httpPort = 80;
	WiFiClient client;

	ServiceDesk() {};
	void setup();
	void handleRFID();
	String httpGETRequest(const char* serverName);
	String printHex(byte* buffer, byte bufferSize);
	void isolateMembers(String employee) ;
	~ServiceDesk() {};
};
