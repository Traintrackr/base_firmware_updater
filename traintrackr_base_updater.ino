/* *************************
 *  traintrackr base updater
 *  Designed by Richard Hawthorn
 *  2021-01-15
 *  
 *  Allows firmware to be downloaded and installed from the traintrackr servers
 *  
 *  **** IMPORTANT PROGRAMMING STEPS **** 
 *  
 *  Install Arduino IDE - https://www.arduino.cc/en/Main/Software
 *  Install ESP8266 Board software - https://github.com/esp8266/Arduino
 *  
 *  If you haven't touched the file system on the esp8266 then your previous board settings should still work, set erase flash settings as 'only sketch'
 *  tools -> Erase flash -> only sketch
 *  
 *  If you have modified the file system, or are having problems getting the board working, then you can wipe the flash using 'all flash content'
 *  tools -> Erase flash ->  all flash content
 *  
 *  If you wipe the board settings you'll need to go through the alternative setup process on our website, clicking on the 'advanced settings' once you
 *  have access to the board's access point.  Speak to us to get the correct settings.
 *  
 *  Board Settings:
 *  
 *  Set the flash size to 4MB FS:1MB, OTA:~1019KB
 *  Set board type to Generic ESP8266 Module
 *  Set serial port to SLAB_USBtoUART (you may need to have the board plugged in to see this)
 *  
 *  Then set the WiFi and update_code variables below
 *  
 *  To program the board plug it into computer using the USB cable while pressing onboard 'FLASH' button 
 *  Wait 5 seconds
 *  Release 'FLASH' button.
 *  Wait 5 seconds
 *  Select the correct serial port if you haven't already
 *  Upload the code to the board.
 *  
 *  Once the board is programmed you can power cycle, and use the serial monitor (set to 115200) to view the update process
 *  
 *  You may not receive a success message through the serial port, but if you don't receive a "Update Failed" message, the update was a success. 
 *  
 *  If the filesystem was previously untouched your board may spring back to life, if it doesn't you should go through the alt setup process
 *  on the traintrackr website, confirming with us which advanced settings need updating.
 *  
 *  Note: The LEDs on the board may not work until you have gone through the above alternative setup process.
 *  
 *  Troubleshooting
 *  
 *  - If you receive a "Forbidden (403)" message, it is probably due to an incorrect update code.  
 *  These codes only work once, so you may need to request a second if you have already tried with this code
 *  
 *  - If you receive a "HTTP error: connection refused" message it means it didn't manage to connect to the WiFi
 *  It will try again until it connects succesfully, but you can also check the SSID and password to make sure they are correct
 *  
 *  - If you receive a "ets Jan  8 2013,rst cause:2, boot mode:(3,7)" message it means the board has reset itself.
 *  This probably means the update was a success
 *  
 *  -------------------
 *  
 *  
 *  Enter your WiFi SSID and Password 
 */
#define STASSID "your-ssid"
#define STAPSK  "your-password"
/* 
 *  
 * Enter your update code (speak to us to get this)
 */
char update_code[] = "your-update-code";
/* 
 *  
 * use the first line for boards purchased in UK/Europe.
 * use the second line for boards purchased in the US.
 */
char host[] = "api.traintrackr.co.uk/api/ota/download";
//char host[] = "api.traintrackrdata.com/api/ota/download";
/* 
 *  
 *  ----------------------------
 *  
 *  Don't touch the code below
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

int minuteNow = 0;
int minuteLast = 999;
const char* ssid     = STASSID;
const char* password = STAPSK;


void setup() {

  //start serial
  Serial.begin(115200);

  for (uint8_t t = 20; t > 0; t--) {
    Serial.printf("Wait %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  Serial.println("");
  Serial.println("traintrackr updater");
  Serial.println("");

  //start WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
}



void loop() {

  minuteNow = round(millis() / 1000 / 60);
  if (minuteNow != minuteLast){ 
    minuteLast = minuteNow;

    //check for updates
    Serial.println("Checking for ota update");

    WiFiClient ota_client;

    String url = "http://";
    url += host;
    url += "?code=";
    url += update_code;

    Serial.print("Update URL: ");
    Serial.println(url);

    Serial.println("Processing...");
    
    t_httpUpdate_return ret = ESPhttpUpdate.update(ota_client, url);

    switch (ret) {
      case HTTP_UPDATE_FAILED:

        Serial.println("Update Failed");
        Serial.println(ESPhttpUpdate.getLastErrorString());
        Serial.println("Trying again in 1 minute");
        break;

      case HTTP_UPDATE_NO_UPDATES:

        Serial.println("Nothing to update");
        break;

      case HTTP_UPDATE_OK:

        Serial.println("Uppdate success");
        break;
    }
    
  }
}
