void initMQTT() {
  client.setCACert(adafruitio_root_ca);
  mqtt.subscribe(&parkingOpenSub);
  mqtt.subscribe(&parkingCloseSub);
}

void MQTT_connect() {
  int8_t ret;


  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {  // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {

      while (1)
        ;
    }
  }

  Serial.println("MQTT Connected!");
}

void handleSubscribers() {
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription())) {
    // Check if Button is one
    if (subscription == &parkingOpenSub) {
      String value = (char *)parkingOpenSub.lastread;
      Serial.println(value);
      if (value == "OPEN") {
        sendSiginal(firstButtonCode);
        parkingStateTopic.publish("1");
      }
    }
    // Check if Button is Two
    if (subscription == &parkingCloseSub) {
      String value = (char *)parkingCloseSub.lastread;
      Serial.println(value);
      if (value == "CLOSE") {
        sendSiginal(secondButtonCode);
        parkingStateTopic.publish("0");
      }
    }

  
  }
}