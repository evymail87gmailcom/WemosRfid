
#include"RfidFunctions.h";


ServiceDesk myFrontDesk;


void setup()
{
    myFrontDesk.setup();
    
  
}

void loop(void) {
    delay(150);
    myFrontDesk.handleRFID();
  
}
