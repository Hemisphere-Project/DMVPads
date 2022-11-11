class VeloPad 
{       
  public:
    VeloPad(int _pin, int _minTrig, int _maxTrig, int _midiNote) 
    {
      pin = _pin;
      minTrig = _minTrig;
      maxTrig = _maxTrig;
      midiNote = _midiNote;
      
      isOn = true;
      counter = 0;    
      currentVelocity = 0;
    }

    void update() 
    {
      int lastVelocity = currentVelocity;
      int value = analogRead(pin);

      // Caclulate current velocity (0->127)
      currentVelocity = map(value, minTrig, maxTrig, 0, 127);
      currentVelocity = constrain(currentVelocity, 0, 127);

      // Dsiplay Raw value
      if ( currentVelocity != lastVelocity ) printRaw(value);

      // NoteON
      if (currentVelocity > 0 && !isOn) {
        isOn = true;
        noteOn();
      }
      
      // Send continuous CC value 
      if ( currentVelocity != lastVelocity ) CC();

      // NoteOFF
      if (currentVelocity == 0 && isOn) {
        isOn = false;
        noteOff();
      }
      
    }

    void noteOn() 
    {
      MIDI.sendNoteOn(midiNote, currentVelocity, MIDI_CHANNEL);
           
         Serial.print("[pad");
         Serial.print(pin);
         Serial.print("]: NOTEON ");
         Serial.print(midiNote);
         Serial.print(" @ ");
         Serial.println(currentVelocity);
    }

    void noteOff()
    {
      MIDI.sendNoteOff(midiNote, 0, MIDI_CHANNEL);
           
         Serial.print("[pad");
         Serial.print(pin);
         Serial.print("]: NOTEOFF ");
         Serial.println(midiNote);
    }

    void CC()
    {
      MIDI.sendControlChange(midiNote, currentVelocity, MIDI_CHANNEL); 
        
         Serial.print("[pad");
         Serial.print(pin);
         Serial.print("]: CC ");
         Serial.print(midiNote);
         Serial.print(" @ ");
         Serial.println(currentVelocity);
    }

    void printRaw(int raw_value)
    {
      MIDI.sendControlChange(midiNote, currentVelocity, MIDI_CHANNEL); 

         Serial.println("--------------");
         Serial.print("[pad");
         Serial.print(pin);
         Serial.print("]: raw value = ");
         Serial.println(raw_value);
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
