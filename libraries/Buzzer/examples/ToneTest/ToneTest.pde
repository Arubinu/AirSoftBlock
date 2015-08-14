// Duelling Tones - Simultaneous tone generation.
// To mix the output of the signals to output to a small speaker (i.e. 8 Ohms or higher),
// simply use 1K Ohm resistors from each output pin and tie them together at the speaker.
// Don't forget to connect the other side of the speaker to ground!

// This example plays notes 'a' through 'g' sent over the Serial Monitor.
// 's' stops the current playing tone.  Use uppercase letters for the second.

#include <Buzzer.h>

int notes[] = { NOTE_A3,
                NOTE_B3,
                NOTE_C4,
                NOTE_D4,
                NOTE_E4,
                NOTE_F4,
                NOTE_G4 };

// You can declare the tones as an array
Buzzer notePlayer;

void setup(void)
{
  Serial.begin(9600);
  notePlayer.begin(A4);
}

void loop(void)
{
  char c;

  if(Serial.available())
  {
    c = Serial.read();

    switch(c)
    {
      case 'a'...'g':
        notePlayer.play(notes[c - 'a']);
        Serial.println(notes[c - 'a']);
        break;
      case 's':
        notePlayer.stop();
        break;

      case 'A'...'G':
        //notePlayer[1].play(notes[c - 'A']);
        Serial.println(notes[c - 'A']);
        break;
      case 'S':
        //notePlayer[1].stop();
        break;

      default:
        //notePlayer[1].stop();
        notePlayer.play(NOTE_B2);
        delay(300);
        notePlayer.stop();
        delay(100);
        //notePlayer[1].play(NOTE_B2);
        delay(300);
        //notePlayer[1].stop();
        break;
    }
  }
}
