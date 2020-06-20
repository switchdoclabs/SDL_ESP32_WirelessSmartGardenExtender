//
// do realtime evaluation of valves
//


void evaluateValves(int timeDecrementMS)
{
  // Loop Around Valves

#ifdef SGS2EXTDEBUG
  //printValveState();
#endif
  int i;
  bool valveChange;
  valveChange = false;

  for (i = 0; i < 8; i++)
  {


    if (valveState[i] == 1)
    {
      valveTime[i] = valveTime[i] - (float)timeDecrementMS / 1000;

      if (valveTime[i] <= 0.0)
      {
        valveTime[i] = 0.0;
        valveState[i] = 0;
        writeValve(i + 1, 0);
        Serial.print("valve #:");
        Serial.print(i + 1);
        Serial.println(" Turned Off");
        valveChange = true;
      }

    }

  }

  if (valveChange == true)
  {
    sendMQTT(MQTTVALVECHANGE, "");
  }



}
