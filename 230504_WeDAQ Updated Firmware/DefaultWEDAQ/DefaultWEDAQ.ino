#include <DSPI.h>
#include <OBCI32_SD.h>
#include <EEPROM.h>
#include <OpenBCI_Wifi_Master_Definitions.h>
#include <OpenBCI_Wifi_Master.h>
#include <OpenBCI_32bit_Library_ISN.h>
#include <OpenBCI_32bit_Library_Definitions_ISN.h>

int wifiState = HIGH;           // initially use Wifi
boolean wifidown = false;             // value that indicates if wifi was set down

unsigned long nowTime;
boolean LEDindicate = true;

// Booleans Required for SD_Card_Stuff.ino
boolean addAccelToSD = false; // On writeDataToSDcard() call adds Accel data to SD card writeDataToSDcard
boolean addAuxToSD = false; // On writeDataToSDCard() call adds Aux data to SD card write
boolean SDfileOpen = false; // Set true by SD_Card_Stuff.ino on successful file open

void setup() {

  // Bring up the OpenBCI Board
  //wedaq.beginDebug();
  // Bring up the OpenBCI Board
  wedaq.begin();
  // Bring up wifi with rx/tx both true
  wifi.begin(true, true);

  wedaq.processChar('d'); // set all channels to default
}

void loop() {
  if (wedaq.streaming) {
      if (wedaq.channelDataAvailable) {
        // Read from the ADS(s), store data, set channelDataAvailable flag to false
        wedaq.updateChannelData();

        // Check to see if accel has new data
        if (wedaq.curAccelMode == wedaq.ACCEL_MODE_ON) {
          if(wedaq.accelHasNewData()) {
            // Get new accel data
            wedaq.accelUpdateAxisData();

            // Tell the SD_Card_Stuff.ino to add accel data in the next write to SD
            addAccelToSD = true; // Set false after writeDataToSDcard()
          }
        } else {
          addAuxToSD = true;
        }

        // Verify the SD file is open
        if(SDfileOpen) {
          // Write to the SD card, writes aux data
          writeDataToSDcard(wedaq.sampleCounter);
        }

        // Send the channel data
        //if (wifiState) {    //bring button controlled wifistate back into fold later
        if (wedaq.streamState || wedaq.unchain) {   //determine if data is to be sent to the wifi module
          wedaq.sendChannelData();
        }
      }
    }

    // Check serial 0 for new data
  if (wedaq.hasDataSerial0()) {
    // Read one char from the serial 0 port
    char newChar = wedaq.getCharSerial0();

    // Send to the sd library for processing
    sdProcessChar(newChar);

    // Send to the board library
    wedaq.processChar(newChar);
  }

  if (wedaq.hasDataSerial1()) {
    // Read one char from the serial 1 port
    char newChar = wedaq.getCharSerial1();

    // Send to the sd library for processing
    sdProcessChar(newChar);

    // Read one char and process it
    wedaq.processChar(newChar);
  }

  // Call the loop function on the board
  wedaq.loop();

  // Call to wifi loop
  //if (wifiState) {
  if (wedaq.streamState && !wedaq.unchain) {  //skip wifi loop if in "Dark" or "Unchain" mode
    if (wifidown == false) {
      wifi.loop();
      digitalWrite(11,HIGH);
    }
    else {
      //wifi.begin(true,true);
      wifidown = false;
    }

    if (wifi.hasData()) {

      // Read one char from the wifi shield
      char newChar = wifi.getChar();

      // Send to the sd library for processing
      sdProcessChar(newChar);

      // Send to the board library
      wedaq.processCharWifi(newChar);
    }

    if (!wifi.sentGains) {
      if(wifi.present && wifi.tx) {
        wifi.sendGains(wedaq.numChannels, wedaq.getGains());
      }
    }
  }
  else {
    //wifi.begin(false, false);
    digitalWrite(11,LOW);         // set D11 LED low when Dark
    wifidown = true;
  }
}
