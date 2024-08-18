void recordSiginal() {
  if (receiverRF.available()) {
    preferences.begin("rf-code", false);
    int recordingCode = 0;
    Serial.print("Received ");
    recordingCode = receiverRF.getReceivedValue();
    if (firstButtonRecordingState) {
      firstButtonCode = recordingCode;
      preferences.putInt("firstButtonCode", recordingCode);
    }
    if (secondButtonRecordingState) {
      secondButtonCode = recordingCode;
      preferences.putInt("secondButtonCode", recordingCode);
    }
    receiverRF.resetAvailable();
    resetStates();
    Serial.println("Recorded Code -> " + String(recordingCode));
    preferences.end();


    globalButtonRecordingState = false;
  }
}

void resetStates() {
  firstButtonRecordingState = false;
  secondButtonRecordingState = false;
  globalButtonRecordingState = false;
}