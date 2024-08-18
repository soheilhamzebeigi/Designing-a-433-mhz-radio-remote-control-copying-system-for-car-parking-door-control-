void buttonTwoEvents(AceButton* button, uint8_t eventType, uint8_t) {
  switch (eventType) {
    case AceButton::kEventReleased:
      parkingStateTopic.publish("0");
      if (button->getPin() == BUTTON_ONE_PIN) {
        Serial.println(" Button One Single Press !");
        sendSiginal(firstButtonCode);
        parkingStateTopic.publish("0");
      }
      if (button->getPin() == BUTTON_TWO_PIN) {
        Serial.println(" Button Two Single Press !");
        sendSiginal(secondButtonCode);
        parkingStateTopic.publish("1");
      }
      break;
    case AceButton::kEventLongReleased:
      globalButtonRecordingState = true;
      if (button->getPin() == BUTTON_ONE_PIN) {
        Serial.println(" Button One Long Press !");
        firstButtonRecordingState = true;
      }
      if (button->getPin() == BUTTON_TWO_PIN) {
        Serial.println(" Button Two Long Press !");
        secondButtonRecordingState = true;
      }
      break;
  }
}

void initButtons() {
  pinMode(BUTTON_ONE_PIN, INPUT_PULLUP);
  pinMode(BUTTON_TWO_PIN, INPUT_PULLUP);
  ButtonConfig* buttonOneConfigs = ButtonConfig::getSystemButtonConfig();
  buttonOneConfigs->setEventHandler(buttonTwoEvents);
  buttonOneConfigs->setFeature(ButtonConfig::kFeatureDoubleClick);
  buttonOneConfigs->setFeature(ButtonConfig::kFeatureSuppressClickBeforeDoubleClick);
  buttonOneConfigs->setFeature(ButtonConfig::kFeatureSuppressAfterClick);
  buttonOneConfigs->setFeature(ButtonConfig::kFeatureSuppressAfterDoubleClick);
  buttonOneConfigs->setFeature(ButtonConfig::kFeatureSuppressAfterLongPress);
  buttonOneConfigs->setFeature(ButtonConfig::kFeatureLongPress);
}