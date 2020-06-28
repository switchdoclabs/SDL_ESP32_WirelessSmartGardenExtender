//
//
//
// aRest Command functions
//
//

// parsing function
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


// Custom function accessible by the API

// general commands

int checkForID(String command) {
  Serial.println("---------->REST: checkForID");
  Serial.print("Command =");
  Serial.println(command);

  MQTT_IP = getValue(command, ',', 0);
  MQTT_PORT = (getValue(command, ',', 1)).toInt();

  writePreferences();

  RESTreturnString = myID + ",";
  RESTreturnString += String(GPIOExt_Present) + ",";
  RESTreturnString += String(OLED_Present) + ",";
  RESTreturnString += String(RELAY_Present) + ",";

  RESTreturnString +=  String(ADCExt_Present) + ",";
  RESTreturnString += SGSEXTENDERESP32VERSION;


  return 0;

}


int blinkPixelCommand(String command) {


  RESTreturnString = "";
  Serial.println("---------->REST: blinkPixelCommand");
  Serial.print("Command =");
  Serial.println(command);
  String password;
  password = getValue(command, ',', 0);
  if (password == adminPassword)
  {



    blinkPixel(0, 1, blue, 200);

    return 0;

  }
  return 1;




}

int checkSystem(String command) {


}


int getValveState(String command) {


  Serial.println("---------->REST: getValveState");
  Serial.print("Command =");
  Serial.println(command);
  RESTreturnString = "";


  int i;
  for (i = 0; i < 8; i++)
  {
    RESTreturnString += String(valveState[i]) + "," + String(valveTime[i]);
    if (i < 7)
    {
      RESTreturnString += ",";
    }

  }
  return 0;
}



// Water Commands

int enableMoistureSensors(String command) {

  // admin password, enableMoisture0, enableMoisture1, ....
  RESTreturnString = "";
  Serial.println("---------->REST: enableMoistureSensors");
  Serial.print("Command =");
  Serial.println(command);
  String password;
  password = getValue(command, ',', 0);
  if (password == adminPassword)
  {

    int i;
    for (i = 0; i < 4; i++)
    {

      if (getValue(command, ',', i + 1) != "")
        moistureSensorEnable[i] = getValue(command, ',', i + 1).toInt();

    }
    return 0;

  }
  return 1;

}

int readMoistureSensors(String command) {
  // admin password, returns 4 moisture sensors

  // command format:     adminpassworde
  RESTreturnString = "";
  Serial.println("---------->REST: readMoistureSensors");
  Serial.print("Command =");
  Serial.println(command);
  String password;
  password = getValue(command, ',', 0);
  if (password == adminPassword)
  {
    xSemaphoreTake( xSemaphoreSensorsBeingRead, 30000);
    int i;

    readSensors();
    for (i = 0; i < 4; i++)
    {
      if (moistureSensorEnable[i] == 0)
      {
        RESTreturnString += String(moistureSensors[i]) + ",";
      }
      RESTreturnString += String(moistureSensorEnable[i]) + ",";
      RESTreturnString += String(moistureSensors[i]) + ",";
    }
    String mySensorType = "C1,C1,C1,C1";
    RESTreturnString += mySensorType + ",";
    RESTreturnString = RESTreturnString.substring(0, RESTreturnString.length() - 1);
    xSemaphoreGive( xSemaphoreSensorsBeingRead);
    return 0;
  }
  return 1;
}



int  setSingleValve(String command) {

  // command format:     adminpassword, valvenumber, valvelength
  RESTreturnString = "";
  Serial.println("---------->REST: setSingleValve");
  Serial.print("Command =");
  Serial.println(command);
  String password;
  password = getValue(command, ',', 0);
  Serial.print("adminPassword=");
  Serial.println(adminPassword);

  Serial.print("password=");
  Serial.println(password);
  if (password == adminPassword)
  {
    xSemaphoreTake( xSemaphoreEvaluatingValves, 1000);
    int i;


    int myValve;
    int myState;
    float myTime;



    myValve = getValue(command, ',', 1).toInt();
    myState = getValue(command, ',', 2).toInt();

    myTime = getValue(command, ',', 3).toFloat();

    if ((myValve < 1) || (myValve > 8))
    {
      return 1;
    }

    if (((myState == 0 ) || (myState == 1)) != true)
    {
      return 1;
    }

    valveState[myValve - 1] = myState;
    valveTime[myValve - 1] = myTime;


    turnOnAppropriateValves();

#ifdef SGS2EXTDEBUG

    Serial.println("____________________________________________");
    printValveState();
    Serial.println("____________________________________________");
#endif

    xSemaphoreGive( xSemaphoreEvaluatingValves);
    return 0;
  }
  return 1;
}


int setValves(String command) {

  // valves format:   17 items: adminpassword, valve0state, valve0length, valve1state, valve1state, .......


  RESTreturnString = "";
  Serial.println("---------->REST: setValves");
  Serial.print("Command =");
  Serial.println(command);
  String password;
  password = getValue(command, ',', 0);
  Serial.print("adminPassword=");
  Serial.println(adminPassword);

  Serial.print("password=");
  Serial.println(password);
  if (password == adminPassword)
  {

    xSemaphoreTake( xSemaphoreEvaluatingValves, 1000);
    int i;



    for (i = 0; i < 8; i++)
    {
      valveState[i] = getValue(command, ',', i * 2 + 1).toInt();
      valveTime[i] = getValue(command, ',', i * 2 + 1 + 1).toFloat();

    }

    turnOnAppropriateValves();

#ifdef SGS2EXTDEBUG

    printValveState();
#endif

    xSemaphoreGive( xSemaphoreEvaluatingValves);

    return 0;
  }
  return 1;
}



//General Commands

int ledControl(String command) {

  // Get state from command
  RESTreturnString = "";
  Serial.println("---------->REST: ledControl");
  Serial.print("Command =");
  Serial.println(command);
  int state = command.toInt();

  digitalWrite(6, state);
  return 1;
}

int setAdminPassword(String command)
{
  RESTreturnString = "";
  Serial.println("---------->REST: setAdminPassword");
  Serial.print("Command =");
  Serial.println(command);


  String oldPassword;
  String newPassword;

  oldPassword = getValue(command, ',', 0);
  newPassword = getValue(command, ',', 1);

  if (oldPassword == adminPassword)
  {
    adminPassword = newPassword;
    writePreferences();
    return 1;
  }
  else
    return 0;


  //http://192.168.1.134/setAdminPassword?params=oldpassword,newpassword
  return 1;

}


int setStationName(String command)
{
  RESTreturnString = "";
  Serial.println("---------->REST: setStationName");
  Serial.print("Command =");
  Serial.println(command);


  String Password;
  String newName;

  Password = getValue(command, ',', 0);
  newName = getValue(command, ',', 1);

  if (Password == adminPassword)
  {
    stationName = newName;
    writePreferences();
    return 1;
  }
  else
    return 0;

  return 1;

}

int setSensorCycle(String command)
{
  RESTreturnString = "";
  Serial.println("---------->REST: setSensorCycle");
  Serial.print("Command =");
  Serial.println(command);


  String Password;
  String newCycle;
  int newSensorCycle;

  Password = getValue(command, ',', 0);
  newCycle = getValue(command, ',', 1);

  if (Password == adminPassword)
  {
    newSensorCycle = newCycle.toInt();
    if (newSensorCycle < 5){
      return 0;
    }
    else
    {
      sensorCycle = newSensorCycle;
    }
      
    writePreferences();
    return 1;
  }
  else
    return 0;

  return 1;

}



int setClockOffset(String command)
{
  RESTreturnString = "";
  Serial.println("---------->REST: setClockOffset");
  Serial.print("Command =");
  Serial.println(command);


  String Password;
  String newOffset;

  Password = getValue(command, ',', 0);
  newOffset = getValue(command, ',', 1);

  if (Password == adminPassword)
  {
    ClockTimeOffsetToUTC = newOffset.toInt();
    timeClient.setTimeOffset(ClockTimeOffsetToUTC);
    writePreferences();
    return 1;
  }
  else
    return 0;

  return 1;

}

int resetSGS2Ext(String command) {
  RESTreturnString = "";
  Serial.println("---------->REST: resetSGS2Ext - settings invalidated");
  Serial.print("Command =");
  Serial.println(command);
  if (command == adminPassword)
  {

    resetPreferences();

    system_restart();

    // qdelay(10000);

    return 1;
  }
  return 0;
}



// FOTA update commands


int updateSGS(String command)
{

  WiFiClient client;
  Serial.println("---------->REST: updateSGS");
  Serial.print("Command =");
  Serial.println(command);
  RESTreturnString = "";
  // grab the semaphore to stop reading and display
  xSemaphoreTake( xSemaphoreReadSensor, 60);

  String newFWVersion = "XX";

  if (command == adminPassword)
  {
    updateDisplay(DISPLAY_UPDATING);
    delay(5000);
    String fwVersionURL;
    fwVersionURL = "http://www.switchdoc.com/binSGS/SGSWExtCurrentFirmware.html";
    // Get the values for the update software

    HTTPClient httpClient;
    httpClient.begin( fwVersionURL );
    int httpCode = httpClient.GET();
    if ( httpCode == 200 ) {
      newFWVersion = httpClient.getString();
      newFWVersion.trim();

      Serial.print( "Current firmware version: " );
      Serial.println( SGSEXTENDERESP32VERSION );
      Serial.print( "Available firmware version: " );
      Serial.println( newFWVersion );



      if ( newFWVersion != SGSEXTENDERESP32VERSION ) {
        Serial.println( "Preparing to update" );
        // shut down Pixel LED

        xSemaphoreTake( xSemaphorePixelPulse, 10000); // Stop the flashing light


      }
      else
      {
        Serial.println("You have the current Software Version");
        RESTreturnString = "You have the current Software Version";
        xSemaphoreGive( xSemaphoreRESTCommand);
        return 2;
      }
    }




    String fwImageURL;
    newFWVersion.trim();
    fwImageURL = "http://www.switchdoc.com/binSGS/SGS2-" + newFWVersion + ".bin";
    Serial.println(fwImageURL);
    t_httpUpdate_return ret = httpUpdate.update(client, fwImageURL);
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.println("[update] Update failed.");
        RESTreturnString = "[update] Update failed.";
        updateDisplay(DISPLAY_NO_UPDATE_FAILED);
        delay(5000);
        xSemaphoreGive( xSemaphoreRESTCommand);
        xSemaphoreGive( xSemaphorePixelPulse); //Restart the flashing
        return 1;

        break;
      case HTTP_UPDATE_NO_UPDATES:

        Serial.println("[update] Update no Updates.");
        RESTreturnString = "[update] Update no Updates.";
        updateDisplay(DISPLAY_NO_UPDATE_AVAILABLE);

        delay(5000);
        xSemaphoreGive( xSemaphoreRESTCommand);
        xSemaphoreGive( xSemaphorePixelPulse); //Restart the flashing
        return 2;
        break;
      case HTTP_UPDATE_OK:

        Serial.println("[update] Update ok."); // may not called we reboot the ESP
        updateDisplay(DISPLAY_UPDATE_FINISHED);
        xSemaphoreGive( xSemaphoreRESTCommand);
        xSemaphoreGive( xSemaphorePixelPulse); //Restart the flashing
        return 3;
        break;
    }



  }
  xSemaphoreGive( xSemaphorePixelPulse); //Restart the flashing
  xSemaphoreGive( xSemaphoreRESTCommand);
  return 0;


}
