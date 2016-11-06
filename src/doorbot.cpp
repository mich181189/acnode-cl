#include "doorbot.h"

Doorbot::Doorbot(Door &d, Watchdog &w, PN532 &n, RGB &l) :
  Role(n),
  door(d),
  wdog(w),
  led(l)
  {
    this->announcer = NULL;
};

void Doorbot::enableAnnouncer(uint16_t port) {
  if (this->announcer) {
    delete this->announcer;
  }
  this->announcer = new Announcer(port);
  this->announcer->START();
}

void Doorbot::run() {
  wdog.feed();
  door.maybeClose();
  led.run();
  cardPresent((void (Role::*)(Card c))&Doorbot::handleCardPresent);
  if (door.isOpen()) {
    led.solid(GREEN);
  } else {
    led.solid(BLUE);
  }
};

void Doorbot::handleCardPresent(Card c) {
  int status = networking::querycard(c);
  announceCard(c);

  switch (status) {
    case -1:
        denyAccess();
      break;
    default:
      grantAccess();
  }
}

void Doorbot::announceCard(Card c) {
  // Debouncing.
  if (this->lastScanned != c || millis() - this->lastScannedTime > 5000) {
    char buffer[14];
    c.str(buffer);
    this->announcer->RFID(buffer);
    this->lastScanned = c;
    this->lastScannedTime = millis();
  }
}

void Doorbot::denyAccess() {
  led.solid(RED);
  delay(ACCESS_DENIED_LED_ON_TIME);
}

void Doorbot::grantAccess() {
  door.open();
}
