
bool checkForI2CAddress(int myAddress)
{

  int error;
  // The i2c_scanner uses the return value of
  // the Write.endTransmisstion to see if
  // a device did acknowledge to the address.
  Wire.beginTransmission(myAddress);
  error = Wire.endTransmission();

  if (error == 0)
  {
    Serial.print("I2C device found at address 0x");
    if (myAddress < 16)
      Serial.print("0");
    Serial.print(myAddress, HEX);
    Serial.println("  !");
    return true;
  }
  return false;

}
