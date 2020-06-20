






int sendMQTT(int messageType, String argument)
{

  String AddString;

  String SendString;

  if (!MQTTclient.connected()) {
    MQTTreconnect();
  }
  MQTTclient.loop();


  switch (messageType)
  {

    case MQTTVALVECHANGE:

      {
        AddString = "\"id\": \"";
        AddString += myID;
        AddString += "\", \"messagetype\": \"";
        AddString += messageType;
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

        SendString = "{" + AddString +  "}"; //Send the request

        break;
      }

    case MQTTDEBUG:
      {
        AddString = "\"id\": \"";
        AddString += myID;
        AddString += "\", \"messagetype\": \"";
        AddString += messageType;
        AddString += "\", \"value\": \"";
        AddString += argument;

        SendString = "{" + AddString +  "}"; //Send the request



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
