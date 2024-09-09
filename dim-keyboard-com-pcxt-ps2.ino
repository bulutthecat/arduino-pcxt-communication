#define xt_clk 2
#define xt_data 5

#define GROUP1_CNT 85
#define BREAK_GRP1 0xF0
#define MAKE_GRP2 0xE0
#define DEBUG

bool shift_pressed = false;

byte got_data = 0;
unsigned char value = 0;

struct ps2_struct_group {
  unsigned char character;
  unsigned char make;
  unsigned is_char;
  unsigned char xt_make;
  bool shifted;
};

typedef struct ps2_struct_group ps2_group1_type;
typedef struct ps2_struct_group ps2_group2_type;

// Add shift modifier make/break codes
#define SHIFT_MAKE 0x12
#define SHIFT_BREAK 0xF0

ps2_group1_type ps2_group1[] = {
  // Lowercase letters
  {'a', 0x1C, 1, 0x1E, false}, {'b', 0x32, 1, 0x30, false}, {'c', 0x21, 1, 0x2E, false},
  {'d', 0x23, 1, 0x20, false}, {'e', 0x24, 1, 0x12, false}, {'f', 0x2B, 1, 0x21, false},
  {'g', 0x34, 1, 0x22, false}, {'h', 0x33, 1, 0x23, false}, {'i', 0x43, 1, 0x17, false},
  {'j', 0x3B, 1, 0x24, false}, {'k', 0x42, 1, 0x25, false}, {'l', 0x4B, 1, 0x26, false},
  {'m', 0x3A, 1, 0x32, false}, {'n', 0x31, 1, 0x31, false}, {'o', 0x44, 1, 0x18, false},
  {'p', 0x4D, 1, 0x19, false}, {'q', 0x15, 1, 0x10, false}, {'r', 0x2D, 1, 0x13, false},
  {'s', 0x1B, 1, 0x1F, false}, {'t', 0x2C, 1, 0x14, false}, {'u', 0x3C, 1, 0x16, false},
  {'v', 0x2A, 1, 0x2F, false}, {'w', 0x1D, 1, 0x11, false}, {'x', 0x22, 1, 0x2D, false},
  {'y', 0x35, 1, 0x15, false}, {'z', 0x1A, 1, 0x2C, false},

  // Numbers (lowercase version)
  {'0', 0x45, 1, 0x0B, false}, {'1', 0x16, 1, 0x02, false}, {'2', 0x1E, 1, 0x03, false},
  {'3', 0x26, 1, 0x04, false}, {'4', 0x25, 1, 0x05, false}, {'5', 0x2E, 1, 0x06, false},
  {'6', 0x36, 1, 0x07, false}, {'7', 0x3D, 1, 0x08, false}, {'8', 0x3E, 1, 0x09, false},
  {'9', 0x46, 1, 0x0A, false},

  // Symbols requiring shift (uppercase version of the numbers)
  {'!', 0x16, 1, 0x02, true}, {'@', 0x1E, 1, 0x03, true}, {'#', 0x26, 1, 0x04, true},
  {'$', 0x25, 1, 0x05, true}, {'%', 0x2E, 1, 0x06, true}, {'^', 0x36, 1, 0x07, true},
  {'&', 0x3D, 1, 0x08, true}, {'*', 0x3E, 1, 0x09, true}, {'(', 0x46, 1, 0x0A, true},
  {')', 0x45, 1, 0x0B, true},

  // Punctuation and symbols
  {'-', 0x4E, 1, 0x0C, false}, {'_', 0x4E, 1, 0x0C, true},  // Dash and underscore
  {'=', 0x55, 1, 0x0D, false}, {'+', 0x55, 1, 0x0D, true},  // Equal and plus
  {'[', 0x54, 1, 0x1A, false}, {'{', 0x54, 1, 0x1A, true},  // Left bracket and left brace
  {']', 0x5B, 1, 0x1B, false}, {'}', 0x5B, 1, 0x1B, true},  // Right bracket and right brace
  {'\\', 0x5D, 1, 0x2B, false}, {'|', 0x5D, 1, 0x2B, true}, // Backslash and pipe
  {';', 0x4C, 1, 0x27, false}, {':', 0x4C, 1, 0x27, true},  // Semicolon and colon
  {'\'', 0x52, 1, 0x28, false}, {'"', 0x52, 1, 0x28, true},  // Single and double quote
  {',', 0x41, 1, 0x33, false}, {'<', 0x41, 1, 0x33, true},  // Comma and less than
  {'.', 0x49, 1, 0x34, false}, {'>', 0x49, 1, 0x34, true},  // Period and greater than
  {'/', 0x4A, 1, 0x35, false}, {'?', 0x4A, 1, 0x35, true},  // Slash and question mark

  // Control characters
  {'\b', 0x66, 0, 0x0E, false},  // Backspace
  {' ', 0x29, 1, 0x39, false},   // Space
  {'\t', 0x0D, 0, 0x0F, false},  // Tab
  {'\r', 0x5A, 1, 0x1C, false},  // Enter (Carriage return)

  // Function keys F1-F12
  {' ', 0x05, 0, 0x3B, false},  // F1
  {' ', 0x06, 0, 0x3C, false},  // F2
  {' ', 0x04, 0, 0x3D, false},  // F3
  {' ', 0x0C, 0, 0x3E, false},  // F4
  {' ', 0x03, 0, 0x3F, false},  // F5
  {' ', 0x0B, 0, 0x40, false},  // F6
  {' ', 0x83, 0, 0x41, false},  // F7
  {' ', 0x0A, 0, 0x42, false},  // F8
  {' ', 0x01, 0, 0x43, false},  // F9
  {' ', 0x09, 0, 0x44, false},  // F10
  {' ', 0x78, 0, 0x57, false},  // F11
  {' ', 0x07, 0, 0x58, false},  // F12

  // Additional keys (NumPad, etc.)
  {'*', 0x7C, 1, 0x37, false},  // NumPad *
  {'-', 0x7B, 1, 0x4A, false},  // NumPad -
  {'+', 0x79, 1, 0x4E, false},  // NumPad +
  {'.', 0x71, 1, 0x53, false},  // NumPad .
  {'0', 0x70, 1, 0x52, false},  // NumPad 0
  {'1', 0x69, 1, 0x4F, false},  // NumPad 1
  {'2', 0x72, 1, 0x50, false},  // NumPad 2
  {'3', 0x7A, 1, 0x51, false},  // NumPad 3
  {'4', 0x6B, 1, 0x4B, false},  // NumPad 4
  {'5', 0x73, 1, 0x4C, false},  // NumPad 5
  {'6', 0x74, 1, 0x4D, false},  // NumPad 6
  {'7', 0x6C, 1, 0x47, false},  // NumPad 7
  {'8', 0x75, 1, 0x48, false},  // NumPad 8
  {'9', 0x7D, 1, 0x49, false},  // NumPad 9
};

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pinMode(xt_clk, OUTPUT);
  pinMode(xt_data, OUTPUT);
  digitalWrite(xt_clk, HIGH);
  digitalWrite(xt_data, HIGH);
  
  Serial.begin(9600);  // Change baud rate if needed
}

unsigned char _read_serial() {
  if (Serial.available() > 0) {
    unsigned char code = Serial.read();
    if (code == 0x7F) return '\b';  // Map 0x7F to Backspace
    return code;
  }
  return 0;
}

void _write(unsigned char value) {
  while (digitalRead(xt_clk) != HIGH);
  unsigned char bits[8];
  byte p = 0, j = 0;

  for (j = 0; j < 8; j++) {
    bits[j] = (value & 1) ? 1 : 0;
    value = value >> 1;
  }

  digitalWrite(xt_clk, LOW);
  digitalWrite(xt_data, HIGH);
  delayMicroseconds(120);
  digitalWrite(xt_clk, HIGH);
  delayMicroseconds(66);
  digitalWrite(xt_clk, LOW);
  delayMicroseconds(30);
  digitalWrite(xt_clk, HIGH);

  for (p = 0; p < 8; p++) {
    digitalWrite(xt_clk, HIGH);
    digitalWrite(xt_data, bits[p]);
    delayMicroseconds(95);
    digitalWrite(xt_clk, LOW);
    digitalWrite(xt_data, LOW);
  }
  digitalWrite(xt_clk, HIGH);
  digitalWrite(xt_data, LOW);
  delay(1);
}

void _write_shifted(unsigned char xt_make_code) {
  // Write shift make code
  _write(SHIFT_MAKE);
  // Write the XT make code
  _write(xt_make_code);
  // Write shift break code
  _write(SHIFT_BREAK);
}

void loop() {
  unsigned char code = _read_serial();

#ifdef DEBUG
  if (code != 0) Serial.println(code, HEX);
#endif

  if (code != 0) {
    unsigned char i = 0;
    while (i < GROUP1_CNT) {
      if (ps2_group1[i].character == code) {
        // Check if this character requires shifting
        if (ps2_group1[i].shifted) {
          // Send PCXT left shift make code
          _write(0x2A);  // Left Shift make code for PCXT
        }

        // Send the actual character's XT make code
#ifdef DEBUG
        Serial.write(ps2_group1[i].character);
#endif
        _write(ps2_group1[i].xt_make);

        // If shifted, unshift by sending the shift break code
        if (ps2_group1[i].shifted) {
          // Send PCXT left shift break code
          _write(0xAA);  // Left Shift break code for PCXT
        }

        break;
      }
      i++;
    }
  }

  if (digitalRead(xt_clk) == LOW) {
    delay(10);
    _write(0xAA); // Power-on self-test success code
  }
}
