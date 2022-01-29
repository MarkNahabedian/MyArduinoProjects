/*

 My first Arduinoproject: blink a message in Morse Code

 Based on the "SparkFun Inventor’s Kit: Circuit 1A-"

*/


#define DEBUG_PRINT (1==0)


#include <ctype.h>

typedef struct {
  char c;
  char *morse;
} Morse;

Morse morse_code[] = {
  { 'A',	".-"       },
  { 'B',	"-..."     },
  { 'C',	"-.-."     },
  { 'D',	"-.."      },
  { 'E',	"."        },
  { 'F',	"..-."     },
  { 'G',	"--."      },
  { 'H',	"...."     },
  { 'I',	".."       },
  { 'J',	".---"     },
  { 'K',	" -.-"     },
  { 'L',	".-.."     },
  { 'M',	"--"       },
  { 'N',	"-."       },
  { 'O',	"---"      },
  { 'P',	".--."     },
  { 'Q',	"--.-"     },
  { 'R',	".-."      },
  { 'S',	"..."      },
  { 'T',	"-"        },
  { 'U',	"..-"      },
  { 'V',	"...-"     },
  { 'W',	".--"      },
  { 'X',	"-..-"     },
  { 'Y',	"-.--"     },
  { 'Z',	"--.."     },
  { '0',	"-----"    },
  { '1',	".----"    },
  { '2',	"..---"    },
  { '3',	"...--"    },
  { '4',	"....-"    },
  { '5',	"....."    },
  { '6',	"-...."    },
  { '7',	"--..."    },
  { '8',	"---.."    },
  { '9',	"----."    },
  { '.',	".-.-.-"   },
  { ',',	"--..--"   },
  { '?',	"..--.."   },
  { '=',	"-...-"    },
  { '1',	".----"    },
  { '2',	"..---"    },
  { '3',	"...--"    },
  { '4',	"....-"    },
  { '5',	"....."    },
  { '6',	"-...."    },
  { '7',	"--..."    },
  { '8',	"---.."    },
  { '9',	"----."    },
  { '0',	"-----"    },
  { ' ',	""         }
};


char* char2morse(char c) {
  for (int i = 0; morse_code[i].morse != ""; i++) {
    if (toupper(c) == morse_code[i].c) {
      return morse_code[i].morse;
    }
  }
  return "";
}


/* Timing, from https://morsecode.world/international/timing.html

   Dit: 1 unit
   Dah: 3 units
   Intra-character space (the gap between dits and dahs within a character): 1 unit
   Inter-character space (the gap between the characters of a word): 3 units
   Word space (the gap between two words): 7 units

 This page describes the standard word for a words per minute calculation is 50 units long.

 */

/* How long is 1 unit (in milliseconds) for a specified word rate */
int wpm2ms(float wpm) {
  // If 1000 isn't a float then trhe mapth gets fucked up
  return (int)floor(0.5 + 1000.0 * (60 / (50 * wpm)));
}

const int WORDS_PER_MINUTE = 8.0;

int UNIT_TIME = 150;  // multiplier for all times below

int DIT_TIME = 1;        // How long the LED is on for dot
int DAH_TIME = 3;        // How long the LED is on for dash
int OFF_TIME = 1;        // How long the LED is off for after a dot or dash
int BETWEEN_TIME = 1;    // Gap between characters
int WORDGAP_TIME = 7;    // delay between words

void init_morse_timing(int wpm) {
  UNIT_TIME = wpm2ms(wpm);
}


char * MESSAGE = "And on that farm he had some henchmen e i e i o";


void dit() {
  digitalWrite(13, HIGH);   // Turn on the LED
  delay(UNIT_TIME * DIT_TIME);
  digitalWrite(13, LOW);    // Turn off the LED
  /* Should this be included AFTER every character or only BETWEEN
     characters? */
  delay(UNIT_TIME * OFF_TIME);
}

void dah() {
  digitalWrite(13, HIGH);   // Turn on the LED
  delay(UNIT_TIME * DAH_TIME);
  digitalWrite(13, LOW);    // Turn off the LED
  /* Should this be included AFTER every character or only BETWEEN
     characters? */
  delay(UNIT_TIME * OFF_TIME);
}

void quiet(int ms) {
  delay(ms);
}



void do_message(void (*charfun)(char), char *message) {
  for (int i = 0; message[i] != '\0'; i++) {
    charfun(message[i]);
  }
}


void flash_morse(char c) {
  char * m = char2morse(c);
  if (DEBUG_PRINT) {
    Serial.println(m);
  }
  if (m == "") {
    quiet(UNIT_TIME * WORDGAP_TIME);
  } else {
    for (int j = 0; m[j] != '\0'; j++) {
      switch(m[j]) {
      case '.':
        dit();
        break;
      case '-':
        dah();
        break;
      }
      // Turn on 12 between characters
      digitalWrite(12, HIGH);
      quiet(UNIT_TIME * BETWEEN_TIME);
      digitalWrite(12, LOW);
    }
  }
}

void send_message(char * message) {
  for (int i = 0; message[i] != '\0'; i++) {
    char c = message[i];
    flash_morse(c);
  }
}


void setup() {
  init_morse_timing(WORDS_PER_MINUTE);
  if (DEBUG_PRINT) {
    Serial.begin(9600);
    Serial.println(UNIT_TIME);
  }
  pinMode(13, OUTPUT);      // Set pin 13 to output
  // Extra lights for debugging:
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  // Test lights
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);    // On between characters
  digitalWrite(11, HIGH);    // On between repetitions of the message
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW);
  delay(3000);
  if (DEBUG_PRINT) {
    Serial.println("begin");
  }
}

void loop() {
  if (DEBUG_PRINT) {
    Serial.println("loop");
  }
  send_message(MESSAGE);
  // Use a second LED to show we're between messages
  digitalWrite(11, HIGH);
  delay(3000);
  digitalWrite(11, LOW);
  delay(1000);
}

