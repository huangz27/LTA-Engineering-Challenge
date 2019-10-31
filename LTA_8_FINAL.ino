#include <Servo.h>
#include <LiquidCrystal.h>

Servo servoleft;
Servo servoright;
Servo servocenter;

#define trigPinfrontleft 22
#define echoPinfrontleft 24
#define trigPinfrontright 50
#define echoPinfrontright 48
#define trigPinleft 26
#define echoPinleft 28
#define trigPinright 46
#define echoPinright 44
#define trigPinback 30
#define echoPinback 32

#define w 600
#define b 280
#define backblack 230
#define backwhite 500

#define YOUR_PIN_TO_SWITCH_MODE 2
#define Pin_out_to_button 3

 
int fsrAnalogPin = 3; // FSR is connected to analog 0
const int buzzerPin = 8;
const int songLength = 18;

char notes[] = "cdfda ag cdfdg gf "; // a space represents a rest

// Beats is an array of values for each note and rest.
// A "1" represents a quarter-note, 2 a half-note, etc.
// Don't forget that the rests (spaces) need a length as well.

int beats[] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};

// The tempo is how fast to play the song.
// To make the song play faster, decrease this value.

int tempo = 60;

int fsrReading;   

LiquidCrystal lcd(31, 33, 35, 37, 39, 41);
int input_signal;
float start, finish;
int distance;

void dontmove() {
  servoleft.write(92);
  servoright.write(94);
  servocenter.write(90);
}

void gostraight() {
  servoleft.write(50);
  servoright.write(128);
  servocenter.write(90);
}

void gostraightline() {
  servoleft.write(75);
  servoright.write(115);
  servocenter.write(90);
}

void reverse() {
  servoleft.write(130);
  servoright.write(50);
  servocenter.write(90);
}

void turnleft() {
  servoleft.write(100);
  servoright.write(134);
  servocenter.write(120);
}

void turnright() {
  servoleft.write(50);
  servoright.write(83);
  servocenter.write(60);
}

void pivotleft() {
  servoleft.write(130);
  servoright.write(128);
  servocenter.write(120);
}

void pivotright() {
  servoleft.write(50);
  servoright.write(50);
  servocenter.write(60);
}

void turnleftline() {
  servoleft.write(80);
  servoright.write(140);
  servocenter.write(120);
}

void turnrightline() {
  servoleft.write(50);
  servoright.write(105);
  servocenter.write(60);
}

void buzzer() 
{
  int i, duration;

  for (i = 0; i < songLength; i++) // step through the song arrays
  {
    duration = beats[i] * tempo;  // length of note/rest in ms

    if (notes[i] == ' ')          // is this a rest? 
    {
      delay(duration);            // then pause for a moment
    }
    else                          // otherwise, play the note
    {
      tone(buzzerPin, frequency(notes[i]), duration);
      delay(duration);            // wait for tone to finish
    }
    delay(tempo/10);              // brief pause between notes
  }

}


int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 8;  // number of notes we're storing

  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      return(frequencies[i]);     // Yes! Return the frequency
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}

void setup() {

  servoleft.attach (12);
  servoright.attach (52);
  servocenter.attach (11);
  Serial.begin (9600);
  pinMode (trigPinfrontleft, OUTPUT);
  pinMode (echoPinfrontleft, INPUT);
  pinMode (trigPinfrontright, OUTPUT);
  pinMode (echoPinfrontright, INPUT);
  pinMode (trigPinleft, OUTPUT);
  pinMode (echoPinleft, INPUT);
  pinMode (trigPinright, OUTPUT);
  pinMode (echoPinright, INPUT);
  pinMode (trigPinback, OUTPUT);
  pinMode (echoPinback, INPUT);


  pinMode(YOUR_PIN_TO_SWITCH_MODE, INPUT);
  pinMode(Pin_out_to_button, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  // Print a message to the LCD.
  digitalWrite(Pin_out_to_button, HIGH);
  lcd.display();
  lcd.print("Travelled:    m");
  lcd.setCursor(0, 1);
  lcd.print("Cost:    cents  ");
  distance = 0;
  userControl();
  lcd.noDisplay();
  lcd.clear();
  autoPilot();
}

void userControl() {

  dontmove();

  while (true) {
    if (digitalRead(YOUR_PIN_TO_SWITCH_MODE) == LOW) {
      break;
    }
    lcd.setCursor(11, 0);
    lcd.print(distance);
    lcd.setCursor(6, 1);
    lcd.print(distance / 5);


    while (Serial.available() > 0) {
      input_signal = Serial.parseInt();
      if (Serial.read() == '\n') {
        if (input_signal == 1) {
          gostraight();
          start = millis();
        }

        if (input_signal == 2) {
          finish = millis();
          distance = distance + (finish - start) / 100;
          dontmove();
        }

        else if (input_signal == 3) {
          turnright();
        }

        else if (input_signal == 4) {
          dontmove();
        }

        else if (input_signal == 5) {
          turnleft();
        }

        else if (input_signal == 6) {
          dontmove();
        }

        else if (input_signal == 7) {
          reverse();
        }

        else if (input_signal == 8) {
          dontmove();
        }
      }
    }
  }

}

void autoPilot() {
  while (true) {
    if (digitalRead(YOUR_PIN_TO_SWITCH_MODE) == HIGH) {
      break;
    }

    long durationfrontleft, frontleft, durationfrontright, frontright, durationleft, left, durationright, right, durationback, back;
    int lineleft, lineright, lineback;
    fsrReading = analogRead(fsrAnalogPin);

    lineleft = analogRead(A0);
    lineright = analogRead(A1);
    lineback = analogRead(A4);

    digitalWrite (trigPinfrontleft, LOW);
    delayMicroseconds(2);
    digitalWrite (trigPinfrontleft, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinfrontleft, LOW);
    durationfrontleft = pulseIn(echoPinfrontleft, HIGH);
    frontleft = (durationfrontleft / 2) * 0.0340;

    digitalWrite (trigPinfrontright, LOW);
    delayMicroseconds(4); // open to change
    digitalWrite (trigPinfrontright, HIGH);
    delayMicroseconds(12); // open to change
    digitalWrite(trigPinfrontright, LOW);
    durationfrontright = pulseIn(echoPinfrontright, HIGH);
    frontright = (durationfrontright / 2) * 0.0340;

    digitalWrite (trigPinright, LOW);
    delayMicroseconds(2);
    digitalWrite (trigPinright, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinright, LOW);
    durationright = pulseIn(echoPinright, HIGH);
    right = (durationright / 2) * 0.0340;

    digitalWrite (trigPinleft, LOW);
    delayMicroseconds(2);
    digitalWrite (trigPinleft, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinleft, LOW);
    durationleft = pulseIn(echoPinleft, HIGH);
    left = (durationleft / 2) * 0.0340;

    digitalWrite (trigPinback, LOW);
    delayMicroseconds(2);
    digitalWrite (trigPinback, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinback, LOW);
    durationback = pulseIn(echoPinback, HIGH);
    back = (durationback / 2) * 0.0340;

 Serial.print("\nleft: ");
    Serial.print(lineleft);
    Serial.print("right: ");
    Serial.print(lineright);
    Serial.print("Back: ");
    Serial.print(lineback);
    Serial.print("frontleft: ");
    Serial.print(frontleft);
    Serial.print("frontright: ");
    Serial.print(frontright);
    //black is outside, red is inside

    //frontleft = 30;
    //frontright = 30;

 if (fsrReading < 130){
    if (lineleft < b && lineright < b && lineback < backblack) {  

      if (frontleft > 5 && frontright > 5) {
        turnleftline();
      }
      else dontmove();
    }

    else if (lineleft < b && lineright < b && lineback > backwhite) {

      if (frontleft > 5 && frontright > 5) {
        turnright();
      }
      else dontmove();
    }

    else if (lineleft < b && lineright < b && lineback < backwhite && lineback > backblack) {

      if (frontleft > 5 && frontright > 5) {
        pivotleft();
        delay(50);
      }
      else dontmove();
    }

    else if (lineleft < b && lineright > w) {

      if (frontleft > 5 && frontright > 5) {
        turnleft();
      }
      else dontmove();
    }

    else if (lineleft > w && lineright < b && lineback < backwhite && lineback > backblack) {

      if (frontleft > 5 && frontright > 5) {
        turnrightline();
      }
      else dontmove();
    }

    else if (lineleft > w && lineright < b && lineback > backwhite) {

      if (frontleft > 5 && frontright > 5) {
        turnright();
      }
      else dontmove();
    }

    else if (lineleft > w && lineright > w && lineback < backblack) {

      if (frontleft > 5 && frontright > 5) {
        reverse();
        delay(500);
      }
      else dontmove();
    }


    else if (lineleft < w && lineleft > b && lineright < w && lineright > b && lineback < backwhite && lineback > backblack) {

      if (frontleft > 5 && frontright > 5) {
        turnrightline();
      }
      else dontmove();
    }

    else if (lineleft < w && lineleft > b && lineright < w && lineright > b && lineback > backwhite) {

      if (frontleft > 5 && frontright > 5) {
        turnleft();
      }
      else dontmove();
    }

    else if (lineleft < w && lineleft > b && lineright < w && lineright > b && lineback < backblack) {
      if (frontleft > 5 && frontright > 5) {
        pivotright();
        delay(30);
      }
      else dontmove();
    }


    else if (lineleft < w && lineleft > b && lineright > w && lineback < backwhite) {

      if (frontleft > 5 && frontright > 5) {
        turnleft();
        delay(50);  
      }
      else dontmove();
    }

    else if (lineleft < w && lineleft > b && lineright > w && lineback < backwhite && lineback > backblack) {
      if (frontleft > 5 && frontright > 5) {
        turnleft();
        delay(100);
      }
      else dontmove();
    }

  else if (lineleft > w && lineright < w && lineright > b) {
      if (frontleft > 5 && frontright > 5) {
        turnright();
      }
      else dontmove();
    }

    else if (lineleft > w && lineright > w && lineback < w && lineback > b) {

      if (frontleft > 5 && frontright > 5) {
        reverse();
        delay(300);
        turnright();
        delay(50);
      }
      else dontmove();
    }

    else if (lineleft < w && lineleft > b && lineright < b) {

      if (frontleft > 5 && frontright > 5) {
        gostraightline();
      }
      else dontmove();
    }

    else if (lineleft < b && lineright < w && lineright > b && lineback < backwhite && lineback > b) {
      if (frontleft > 5 && frontright > 5) {
        pivotleft();
        delay(130);
      }
      else dontmove();

    }

    
    else if (lineleft < b && lineright < w && lineright > b && lineback < b) {
      if (frontleft > 5 && frontright > 5) {
        pivotright();
        delay(130);
      }
      else dontmove();

    }
    else if (frontright > 16 && frontleft > 16) {
      gostraight();
    }

    else if (frontleft < 5 || frontright < 5) {
      if (back > 10) {
        reverse();
        delay(500); //open to change
      }
      else 
      {
       dontmove();
       buzzer();
      }
    }

    else if (left > 15) {
      if (left >= right)
      {
      if (right > 2) {
        pivotleft();
        delay(800);
      }
        else 
      {  turnleftline(); 
        delay(800);
      }
      }
    }

    else if (right > 15) {
      if (right > left)
      {
        if (left > 2) {
        pivotright();
        delay(800);
        }
        else {
        turnrightline(); 
        delay(800);
        }
        
      }

    }
 }
 else 
 {
 dontmove();
 buzzer();
  }
  }
}


