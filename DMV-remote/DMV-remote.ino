
#include "MIDIUSB.h"

// MID channel
#define MIDI_CHANNEL 13

int pinA = 4;
int pinB = 2;

int lastA = 1;
int lastB = 1;

// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  Serial.println("CC"+String(control)+" "+String(value));
}


void setup() 
{
  Serial.begin(115200);

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
}


void loop() 
{
  int rpin = 0;
  
  rpin = digitalRead(pinA);
  if (rpin != lastA) {
    controlChange(MIDI_CHANNEL-1, 100, (rpin == 0) ? 127 : 0);
    lastA = rpin;
  }

  rpin = digitalRead(pinB);
  if (rpin != lastB) {
    controlChange(MIDI_CHANNEL-1, 101, (rpin == 0) ? 127 : 0);
    lastB = rpin;
  }

  MidiUSB.flush();
  
  /*
  Serial.println("Sending note on");
  noteOn(0, 48, 64);   // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(500);
  Serial.println("Sending note off");
  noteOff(0, 48, 64);  // Channel 0, middle C, normal velocity
  MidiUSB.flush();
  delay(1500);
  */
}
