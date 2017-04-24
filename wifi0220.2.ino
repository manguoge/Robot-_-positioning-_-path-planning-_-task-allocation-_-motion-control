/*Modified the pin configuration for cc3000
This example does a full test of core connectivity:
* Initialization
* SSID Scan
* AP connection
* DHCP printout
* DNS lookup
* Ping7
* Disconnect
*/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h> 
#include <string.h>
#include "utility/debug.h"
#include<SoftwareSerial.h>
// Config the interrupt and control pins on cc3000
#define ADAFRUIT_CC3000_IRQ   7
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
//与Leonardo通信的软串口
SoftwareSerial mySerial1(4,5);//RX,TX
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed but DI

//Please enter the SSID and password of the router you want to connect

#define WLAN_SSID       "robot_wifi"        // cannot be longer than 32 characters!
#define WLAN_PASS       "elab204d"	   // 

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define TIMEOUT_MS  2000

void setup(void)
{
  Serial.begin(115200);
  mySerial1.begin(115200);
   while (!Serial)
   {
    ; // wait for serial port to connect. Needed for Leonardo only
   }
  Serial.println(F("Hello, CC3000!\n")); 
  //displayDriverMode();
  
  /* Initialise the module */
  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while(1);
  }

  /* Optional: Update the Mac Address to a known value */
/*
  uint8_t macAddress[6] = { 0x08, 0x00, 0x28, 0x01, 0x79, 0xB7 };
   if (!cc3000.setMacAddress(macAddress))
   {
     Serial.println(F("Failed trying to update the MAC address"));
     while(1);
   }
*/
  
  uint16_t firmware = checkFirmwareVersion();
  if (firmware < 0x113) 
  {
    Serial.println(F("Wrong firmware version!"));
    for(;;);
  } 
    
  /* Optional: Get the SSID list (not available in 'tiny' mode) */
#ifndef CC3000_TINY_DRIVER
  //listSSIDResults();
#endif
  
  /* Delete any old connection data on the module */
  Serial.println(F("\nDeleting old connection profiles"));
  if (!cc3000.deleteProfiles()) 
  {
    Serial.println(F("Failed!"));
    while(1);
  }

  /* Optional: Set a static IP address instead of using DHCP.
     Note that the setStaticIPAddress function will save its state
     in the CC3000's internal non-volatile memory and the details
     will be used the next time the CC3000 connects to a network.
     This means you only need to call the function once and the
     CC3000 will remember the connection details.  To switch back
     to using DHCP, call the setDHCP() function (again only needs
     to be called once).
  */
  /*
  uint32_t ipAddress = cc3000.IP2U32(192, 168, 1, 20);
  uint32_t netMask = cc3000.IP2U32(255, 255, 255, 0);
  uint32_t defaultGateway = cc3000.IP2U32(192, 168, 1, 1);
  uint32_t dns = cc3000.IP2U32(8, 8, 4, 4);
  if (!cc3000.setStaticIPAddress(ipAddress, netMask, defaultGateway, dns))
  {
    Serial.println(F("Failed to set static IP!"));
    while(1);
  }
  */
  
  /* Optional: Revert back from static IP addres to use DHCP.
     See note for setStaticIPAddress above, this only needs to be
     called once and will be remembered afterwards by the CC3000.
  */
  
  if (!cc3000.setDHCP())
  {
    Serial.println(F("Failed to set DHCP!"));
    while(1);
  }
  

  /* Attempt to connect to an access point */
  char *ssid = WLAN_SSID;             /* Max 32 chars */
  Serial.print(F("\nAttempting to connect to ")); 
  Serial.println(ssid);
  
  /* NOTE: Secure connections are not available in 'Tiny' mode!
     By default connectToAP will retry indefinitely, however you can pass an
     optional maximum number of retries (greater than zero) as the fourth parameter.
  */
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) 
  {
    Serial.println(F("Router/AP Failed!"));
    while(1);
  }
   
  Serial.println(F("Router/AP Connected!"));
  
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  
  
  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails())
  {
    delay(1000);
  }
 
 /* Try looking up cn.bing.com  
#ifndef CC3000_TINY_DRIVER
  uint32_t ip = 0;
  Serial.print(F("www.baidu.com -> "));
  while  (ip  ==  0)  
  {
    if  (!  cc3000.getHostByName("www.baidu.com", &ip)) 
    {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }  
  cc3000.printIPdotsRev(ip);
  
  //Do a quick ping test on cn.bing.com
  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
  uint8_t replies = cc3000.ping(ip, 5);
  Serial.print(replies); Serial.println(F(" replies"));
  if (replies)
    Serial.println(F("Ping successful!"));
#endif
*/
}

void loop(void)
{
  static Adafruit_CC3000_Client tcpClient;
  static unsigned long heartRate = millis();
  
  if(!tcpClient.connected())
  {
    Serial.println("Try to connect the Server...");
    tcpClient.close();
    uint32_t ip = cc3000.IP2U32(219,223,240,87);
    tcpClient = cc3000.connectTCP(ip, 8090);    
    
    if(!tcpClient.connected())
    {
      Serial.println(F("Couldn't connect to server!"));
      while(1);
    }
  }
  //connection is sucessed!!then sends and recieves data
  //TIMEOUT_MS time to update the posiotion
  else if(millis() - heartRate > 1000)
  {
    heartRate = millis();  // Update time stamp of the microcontroller system
    Serial.println(F("Connected to server!"));
    // Update time stamp of the microcontroller system
    char posiotionString[30];
    int x=0,y=0;
//    while(!mySerial1.available());
//    while(mySerial1.available())
//      x=mySerial1.read()+10*x;
//      sprintf(posiotionString,"x %d",x);
//      tcpClient.fastrprintln(posiotionString);
//    while(!mySerial1.available());
//    while(mySerial1.available())
//    {
//      y=mySerial1.read()+10*y;
//      sprintf(posiotionString,"y %d",y);
//      tcpClient.fastrprintln(posiotionString);
//    }
    sprintf(posiotionString,"p %d %d",x,y);
    Serial.println(posiotionString);
    Serial.println("sending robot's current position...");
    //tcpClient.fastrprintln(posiotionString);
    
    tcpClient.fastrprint("p 20 100");
  }
  
  /* Read data until either the connection is closed, or the timeout is reached. */ 
  Serial.println("receiving this robot's result of allocation...");
  unsigned long lastRead = millis();
  while (tcpClient.connected() && (millis() - lastRead < TIMEOUT_MS)) 
  {
    while (tcpClient.available()) 
    {
      //Serial.println("available...");
      char c = tcpClient.read();
      Serial.print(c);
      lastRead = millis();
      
      // Disable sending message for a moment
      heartRate = millis();
    }
  }
}

/**************************************************************************/
/*!
    @brief  Displays the driver mode (tiny of normal), and the buffer
            size if tiny mode is not being used
    @note   The buffer size and driver mode are defined in cc3000_common.h
*/
/**************************************************************************/
void displayDriverMode(void)
{
  #ifdef CC3000_TINY_DRIVER
    Serial.println(F("CC3000 is configure in 'Tiny' mode"));
  #else
    Serial.print(F("RX Buffer : "));
    Serial.print(CC3000_RX_BUFFER_SIZE);
    Serial.println(F(" bytes"));
    Serial.print(F("TX Buffer : "));
    Serial.print(CC3000_TX_BUFFER_SIZE);
    Serial.println(F(" bytes"));
  #endif
}

/**************************************************************************/
/*!
    @brief  Tries to read the CC3000's internal firmware patch ID
*/
/**************************************************************************/
uint16_t checkFirmwareVersion(void)
{
  uint8_t major, minor;
  uint16_t version;
  
#ifndef CC3000_TINY_DRIVER  
  if(!cc3000.getFirmwareVersion(&major, &minor))
  {
    Serial.println(F("Unable to retrieve the firmware version!\r\n"));
    version = 0;
  }
  else
  {
    Serial.print(F("Firmware V. : "));
    Serial.print(major); Serial.print(F(".")); Serial.println(minor);
    version = major; version <<= 8; version |= minor;
  }
#endif
  return version;
}


/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    //Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

/**************************************************************************/
/*!
    @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);
    
    Serial.print(F("SSID Name    : ")); Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}
