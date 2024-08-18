void handleGlobalStateLed() {

  if (globalButtonRecordingState) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
}


void blinkLed() {



  for (int i = 0; i < 5; i++) {
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(2, LOW);
    delay(100);
  }
}