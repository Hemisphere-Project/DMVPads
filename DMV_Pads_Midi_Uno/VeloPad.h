class VeloPad 
{       
  public:
    VeloPad(int _pin, int _minTrig, int _maxTrig, int _midiNote) {
      pin = _pin;
      minTrig = _minTrig;
      maxTrig = _maxTrig;
      midiNote = _midiNote;
      
      thisIsOn = false;
      counter = 0;    
      currentVelocity = 0;
    }

    void update() {
      int value = analogRead(pin);

      // Not trigged
      if ( value < minTrig ) 
      {
        currentVelocity = 0;
        
        // Was on -> send OFF
        if ( thisIsOn ) {
           thisIsOn = false;
           MIDI.sendNoteOff(midiNote, 0, MIDI_CHANNEL);
             Serial.print("[pad");
             Serial.print(pin);
             Serial.print("]: NOTEOFF ");
             Serial.println(midiNote);
        }
      }

      // Triggered
      else {

        // Map value to velocity
        currentVelocity = map(value, minTrig, maxTrig, 0, 127);
        if ( currentVelocity > 127 ) currentVelocity = 127;

        // Was off -> send ON
        if ( !thisIsOn ) {
           thisIsOn = true;
           MIDI.sendNoteOn(midiNote, currentVelocity, MIDI_CHANNEL);
             Serial.print("[pad");
             Serial.print(pin);
             Serial.print("]: NOTEON ");
             Serial.print(midiNote);
             Serial.print(" @ ");
             Serial.println(currentVelocity);
        }
        
      }

      // Send continuous CC value
      MIDI.sendControlChange(midiNote, currentVelocity, MIDI_CHANNEL); 
         Serial.print("[pad");
         Serial.print(pin);
         Serial.print("]: CC ");
         Serial.print(midiNote);
         Serial.print(" @ ");
         Serial.print(currentVelocity);
         Serial.print(" -- raw =  ");
         Serial.println(value);
    }


    bool isOn() {
      return thisIsOn;
    }


    int velocity() {
      return currentVelocity;
    }
    

  private:
    int pin;
    int minTrig;
    int maxTrig;
    int midiNote;
    int currentVelocity;
    bool thisIsOn;
    int counter;
    
};

/*
digitalWrite(LED_BUILTIN, HIGH);
MIDI.sendNoteOn(42, 127, 1);    // Send a Note (pitch 42, velo 127 on channel 1)
delay(1000);                    // Wait for a second
MIDI.sendNoteOff(42, 0, 1);     // Stop the note
digitalWrite(LED_BUILTIN, LOW);
*/
