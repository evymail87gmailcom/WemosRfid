#include"RfidFunctions.h"
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
void ServiceDesk::setup() {
    pinMode(D1, OUTPUT);
    Serial.begin(115200);
    Serial.println("hello, this is a RFID node");

    // Connect to WiFi
    Serial.print("Connecting to WiFi ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);


    while (WiFi.status() != WL_CONNECTED) {

        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("############################################################");
    Serial.print("Connected to wifi: ");
    Serial.println(ssid);
    Serial.print("My IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("send rfuid to host: ");
    Serial.print(serverName);
    Serial.print(":");
    Serial.println(httpPort);
    Serial.println("############################################################");
    Serial.println("");
    Serial.println("");
    Serial.println("ready, waiting for rfid card to hit me");
    Serial.println("");

    SPI.begin();
    rfid.PCD_Init();
};
void ServiceDesk::handleRFID() {
   // Serial.println("ready, waiting for rfid card to hit me");
    //Serial.println("");
     if (!rfid.PICC_IsNewCardPresent()) return;
        if (!rfid.PICC_ReadCardSerial()) return;

    card_uid = printHex(rfid.uid.uidByte, rfid.uid.size);


    if (WiFi.status() == WL_CONNECTED) {

        //Detta händer första gången
        dataReadings = httpGETRequest(serverName);


        //isolates the first substring

        int firstbracket = dataReadings.indexOf('{');
        int firstclosing = dataReadings.indexOf('}');
        String employeeone = dataReadings.substring(firstbracket, firstclosing + 1);
        isolateMembers(employeeone);

        //isolates the second substring

        int secondOpen = dataReadings.indexOf('{', firstbracket + 1);
        int secondClose = dataReadings.indexOf('}', firstclosing + 1);
        String employeetwo = dataReadings.substring(secondOpen, secondClose + 1);
        isolateMembers(employeetwo);


    }
  //  Uncomment for rfid
     Serial.print("sending card_uid to server: ");
     Serial.print(card_uid);
     Serial.println(" - ");
     Serial.println();
     Serial.println("[OK]");
  
    delay(2000);
};
String ServiceDesk::httpGETRequest(const char* serverName) {
    HTTPClient http;

    // Your IP address with path or Domain name with URL path 
    http.begin(serverName);

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    String payload = "{}";

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
};
String ServiceDesk::printHex(byte* buffer, byte bufferSize) {
    String id = "";
    for (byte i = 0; i < bufferSize; i++) {
        id += buffer[i] < 0x10 ? "0" : "";
        id += String(buffer[i], HEX);
    }
    return id;
};
void ServiceDesk::isolateMembers(String employee) {
    //isolating the "id" substring
    int first = employee.indexOf(',');
    id = employee.substring(1, first + 1);
    Serial.println(id);

    //isolates and typecasts the id to an integer
    int test = id.indexOf(':');
    int last = id.lastIndexOf('"');
    String idNumber = id.substring(test + 2, last);
    Serial.println(idNumber);
    int iD = idNumber.toInt();
    Serial.println(iD);

    //isolates the firstname
    int second = employee.indexOf(',', first + 1);
    firstName = employee.substring(id.lastIndexOf(',') + 2, second + 1);
    Serial.println(firstName);


    test = firstName.indexOf(':');
    last = firstName.lastIndexOf('"');
    String name = firstName.substring(test + 2, last);
    Serial.println(name);


    //isolates the lastname
    int third = employee.indexOf(',', second + 2);
    lastname = employee.substring(second + 1, third + 1);
    Serial.println(lastname);

    test = lastname.indexOf(':');
    last = lastname.lastIndexOf('"');
    String secondName = lastname.substring(test + 2, last);
    Serial.println(secondName);

    //isolates the role
    int four = employee.indexOf(',', third + 1);
    role = employee.substring(third + 1, four + 1);
    Serial.println(role);

    test = role.indexOf(':');
    last = role.lastIndexOf('"');
    String role2 = role.substring(test + 2, last);
    Serial.println(role2);

    //isolates the isactive
    int five = employee.indexOf(',', four + 1);
    isActive = employee.substring(four + 1, five + 1);
    Serial.println(isActive);

    test = isActive.indexOf(':');
    last = isActive.lastIndexOf(',');
    String isActive2 = isActive.substring(test + 1, last);
    Serial.println(isActive2);

    //isolates the gdpr
    int six = employee.indexOf(',', five + 1);
    gdpr = employee.substring(five + 1, six + 1);
    Serial.println(gdpr);

    test = gdpr.indexOf(':');
    last = gdpr.lastIndexOf(',');
    String gdpr2 = gdpr.substring(test - 1 + 2, last);
    //Put this in "checkemployee"
    if (gdpr2 == "true") {
        Serial.println(gdpr2);
    }

    else {
        Serial.println("Please read the license agreement before entering!   ");
        Serial.println(gdpr2);
    }
    //isolates the cardnumber
    cardNumber = employee.substring(six + 1, employee.lastIndexOf('"')+1);
    Serial.println(cardNumber);

    test = cardNumber.indexOf(':');
    String cardNumber2 = cardNumber.substring(test + 2, cardNumber.lastIndexOf('"'));
    Serial.println(cardNumber2);
    
    //Put this in "checkemployee"
    if (cardNumber2 == card_uid) {
        Serial.println("Welcome");
    }
    else {
        Serial.println("Contact frontdesk!");
    }

};