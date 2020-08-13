unsigned int currentLEDMode;
unsigned int OLEDOn;


unsigned int darkLight;

String WiFi_SSID = "";
String WiFi_psk = "";
int ClockTimeOffsetToUTC = 0;

void resetPreferences()
{
  preferences.begin("SGS2init", false);
  preferences.clear();
  preferences.end();

  ClockTimeOffsetToUTC = DEFAULTCLOCKTIMEOFFSETTOUTC;
  Serial.println("----Clearing Preferences---");


}


void readPreferences();

void writePreferences()
{
  preferences.begin("SGS2init", false);




  preferences.putString("WiFi_SSID", WiFi_SSID);
  preferences.putString("WiFi_psk", WiFi_psk);
  preferences.putInt("OLEDOn", OLEDOn);

  preferences.putString("stationName", stationName);

  preferences.putInt("COffsetToUTC", ClockTimeOffsetToUTC);


  preferences.putInt("EnglishOrMetric", EnglishOrMetric);

  preferences.putString("adminPassword", adminPassword);

  preferences.putInt("SolarMAXLA", SolarMAXLA);
  preferences.putInt("SolarMAXLiPo", SolarMAXLiPo);
  preferences.putString("MQTT_IP", MQTT_IP);
  preferences.putInt("MQTT_PORT", MQTT_PORT);
  preferences.putInt("SENSORCYCLE", sensorCycle);

  preferences.end();



#ifdef SGS2EXTDEBUG
  Serial.println("----Writing Preferences---");


  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);

  Serial.print("OLEDOn="); Serial.println(OLEDOn);

  Serial.printf("stationName="); Serial.println(stationName);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);

  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("Station Name:");
  Serial.println(stationName);

  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");


  Serial.print("SolarMAXLA Enabled=");
  Serial.println(SolarMAXLA);
  Serial.print("SolarMAXLiPo Enabled=");
  Serial.println(SolarMAXLiPo);

  Serial.print("MQTT_IP=");
  Serial.println(MQTT_IP);
  Serial.print("MQTT_PORT=");
  Serial.println(MQTT_PORT);
  Serial.print("SensorCycle=");
  Serial.println(sensorCycle);
  Serial.println("--------------------------");

#endif


}

void readPreferences()
{

  Serial.print("preferencesfreeentries=");
  Serial.println(preferences.freeEntries());
  preferences.begin("SGS2init", false);
  OLEDOn = preferences.getInt("OLEDOn", 1);

  WiFi_SSID = preferences.getString("WiFi_SSID", "");
  WiFi_psk = preferences.getString("WiFi_psk", "");

  stationName =   preferences.getString("stationName", "");

  ClockTimeOffsetToUTC = preferences.getInt("COffsetToUTC", DEFAULTCLOCKTIMEOFFSETTOUTC);


  EnglishOrMetric = preferences.getInt("EnglishOrMetric");

  adminPassword = preferences.getString("adminPassword", "admin");


  SolarMAXLA = preferences.getInt("SolarMAXLA", 0);
  SolarMAXLiPo = preferences.getInt("SolarMAXLiPo", 0);
  MQTT_IP = preferences.getString("MQTT_IP", "");
  MQTT_PORT = preferences.getInt("MQTT_PORT", 1883);
  sensorCycle = preferences.getInt("SENSORCYCLE", 600);


  preferences.end();

#ifdef SGS2EXTDEBUG
  Serial.println("----Reading Preferences---");



  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);

  Serial.print("OLEDOn="); Serial.println(OLEDOn);

  Serial.printf("stationName="); Serial.println(stationName);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);

  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("Station Name:");
  Serial.println(stationName);

  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");


  Serial.print("SolarMAXLA Enabled=");
  Serial.println(SolarMAXLA);
  Serial.print("SolarMAXLiPo Enabled=");
  Serial.println(SolarMAXLiPo);
  Serial.print("MQTT_IP=");
  Serial.println(MQTT_IP);
  Serial.print("MQTT_PORT=");
  Serial.println(MQTT_PORT);
  Serial.print("SensorCycle=");
  Serial.println(sensorCycle);
  Serial.println("--------------------------");


#endif
}
