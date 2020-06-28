






int sendMQTT(int messageType, String argument)
{

  String AddString;

  String SendString;

  if (!MQTTclient.connected()) {
    MQTTreconnect();
  }
  MQTTclient.loop();


  char buffer[32];

  snprintf(buffer, sizeof(buffer), "%02d/%02d/%4d %02d:%02d:%02d", month(), day(), year(), hour(), minute(), second());

  //buffer now contains a string like 02/22/2013 05:03:06

  String myTime;
  myTime = String(buffer);


  switch (messageType)
  {

    case MQTTVALVECHANGE:

      {
        AddString = "\"id\": \"";
        AddString += myID;
        AddString += "\", \"messagetype\": \"";
        AddString += messageType;
        AddString += "\", \"timestamp\": \"";
        AddString += myTime;
        AddString += "\", \"valvestate\": \"";


        AddString += "V";
        int i;
        for (i = 0; i < 8; i++)
        {
          AddString += String(valveState[i]);
        }

        AddString += "\"";

        SendString = "{" + AddString +  "}"; //Send the request


        break;
      }
    case MQTTTESTMESSAGE:
      {


        AddString = "\"id\": \"";
        AddString += myID;
        AddString += "\", \"messagetype\": \"";
        AddString += messageType;
        AddString += "\", \"timestamp\": \"";
        AddString += myTime;
        SendString = "{" + AddString +  "\"}"; //Send the request

        break;
      }

    case MQTTDEBUG:
      {
        AddString = "\"id\": \"";
        AddString += myID;
        AddString += "\", \"messagetype\": \"";
        AddString += messageType;
        AddString += "\", \"timestamp\": \"";
        AddString += myTime;
        AddString += "\", \"value\": \"";
        AddString += argument;

        SendString = "{" + AddString +  "\"}"; //Send the request
        break;


      }
    case MQTTSENSORS:
      {

        AddString = "\"id\": \"";
        AddString += myID;
        AddString += "\", \"messagetype\": \"";
        AddString += messageType;
        AddString += "\", \"timestamp\": \"";
        AddString += myTime;
        AddString += "\", \"enableSensors\": \"";

        int i;
        for (i = 0; i < 4; i++)
        {

          AddString += String(moistureSensorEnable[i]) + ",";

        }
        AddString += "\", \"sensorValues\": \"";
        for (i = 0; i < 4; i++)
        {

          AddString += String(moistureSensors[i]) + ",";

        }
        AddString += "\", \"sensorType\": \"";
        String mySensorType = "C1,C1,C1,C1";
        AddString += mySensorType + "\"";


        SendString = "{" + AddString +  "}"; //Send the request


        break;
      }

    default:
      break;


  }


  // publish it

  Serial.println( "Sending MQTT Packet");
  Serial.println(SendString);
  int result;
  // Once connected, publish an announcement...
  String Topic = "SGS/" + myID;
  Serial.print("Topic=");
  Serial.println(Topic);
  result = MQTTclient.publish(Topic.c_str(), SendString.c_str());
  Serial.print("MQTT publish result=");
  Serial.println(result);
  return result;
}
