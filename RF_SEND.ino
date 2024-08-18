void sendSiginal(int code) {
  transmiterRF.send(code,24);
  Serial.println("Siginal -> " + String(code) + " Sended !");
  blinkLed();
}

