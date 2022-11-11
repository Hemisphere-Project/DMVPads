// MID channel
#define MIDI_CHANNEL 13

// MIDI on SoftwareSerial
//
#include <MIDI.h>           // MIDI Library (Library Manager): https://github.com/FortySevenEffects/arduino_midi_library
#include <SoftwareSerial.h>
using Transport = MIDI_NAMESPACE::SerialMIDI<SoftwareSerial>;
int rxPin = 8;  // MIDI IN (not used)
int txPin = 9;  // MIDI OUT
SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);
Transport serialMIDI(mySerial);
MIDI_NAMESPACE::MidiInterface<Transport> MIDI((Transport&)serialMIDI);

// PADS
//
#include "VeloPad.h"  
VeloPad* pads[8];

// SETUP
//
void setup()
{
    // USB Serial (logs)
    //
    Serial.begin(115200);
    delay(300);
    Serial.println(".:: DMVPads ::.");

    // Internal LED
    //
    pinMode(LED_BUILTIN, OUTPUT);

    // Clean pad array
    //
    for (int i = 0; i < 8; i++) pads[i] = NULL;


    // Init Pads: new VeloPad(analogPin, minTrig, maxTrig, midiNote/CC )
    //
    pads[0] = new VeloPad(A0, 300, 1024, 60 );   
    //pads[1] = new VeloPad(A1, 200, 1024, 61 );
    //pads[2] = new VeloPad(A2, 200, 1024, 62 );
    //pads[3] = new VeloPad(A3, 200, 1024, 63 );

    // MIDI 
    //
    MIDI.begin(MIDI_CHANNEL); 
    
}

// LOOP
//
void loop()
{
    // Update each pad
    bool oneIsOn = false;
    for (int i = 0; i < 8; i++) 
      if (pads[i] != NULL) {
        pads[i]->update();
        oneIsOn = oneIsOn || pads[i]->isOn();
      }

    // LED (on if at least one pad is on)
    if (oneIsOn) digitalWrite(LED_BUILTIN, HIGH);
    else digitalWrite(LED_BUILTIN, LOW);
      
}
