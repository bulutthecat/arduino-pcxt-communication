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
};

typedef struct ps2_struct_group ps2_group1_type;
typedef struct ps2_struct_group ps2_group2_type;

ps2_group1_type ps2_group1[] = {
  {'a', 0x1C, 1, 0x1E}, {'b', 0x32, 1, 0x30}, {'c', 0x21, 1, 0x2E},
  {'d', 0x23, 1, 0x20}, {'e', 0x24, 1, 0x12}, {'f', 0x2B, 1, 0x21},
  {'g', 0x34, 1, 0x22}, {'h', 0x33, 1, 0x23}, {'i', 0x43, 1, 0x17},
  {'j', 0x3B, 1, 0x24}, {'k', 0x42, 1, 0x25}, {'l', 0x4B, 1, 0x26},
  {'m', 0x3A, 1, 0x32}, {'n', 0x31, 1, 0x31}, {'o', 0x44, 1, 0x18},
  {'p', 0x4D, 1, 0x19}, {'q', 0x15, 1, 0x10}, {'r', 0x2D, 1, 0x13},
  {'s', 0x1B, 1, 0x1F}, {'t', 0x2C, 1, 0x14}, {'u', 0x3C, 1, 0x16},
  {'v', 0x2A, 1, 0x2F}, {'w', 0x1D, 1, 0x11}, {'x', 0x22, 1, 0x2D},
  {'y', 0x35, 1, 0x15}, {'z', 0x1A, 1, 0x2C}, {'0', 0x45, 1, 0x0B},
  {'1', 0x16, 1, 0x02}, {'2', 0x1E, 1, 0x03}, {'3', 0x26, 1, 0x04},
  {'4', 0x25, 1, 0x05}, {'5', 0x2E, 1, 0x06}, {'6', 0x36, 1, 0x07},
  {'7', 0x3D, 1, 0x08}, {'8', 0x3E, 1, 0x09}, {'9', 0x46, 1, 0x0A},
  {'`', 0x0E, 1, 0x29}, {'-', 0x4E, 1, 0x0C}, {'=', 0x55, 1, 0x0D},
  {'\\', 0x5D, 1, 0x2B}, {'\b', 0x66, 0, 0x0E}, {' ', 0x29, 1, 0x39},
  {'\t', 0x0D, 0, 0x0F}, {' ', 0x58, 0, 0x3A}, {' ', 0x12, 0, 0x2A},
  {' ', 0x14, 0, 0x1D}, {' ', 0x11, 0, 0x38}, {' ', 0x59, 0, 0x36},
  {'\r', 0x5A, 1, 0x1C}, {' ', 0x76, 0, 0x01}, {' ', 0x05, 0, 0x3B},
  {' ', 0x06, 0, 0x3C}, {' ', 0x04, 0, 0x3D}, {' ', 0x0C, 0, 0x3E},
  {' ', 0x03, 0, 0x3F}, {' ', 0x0B, 0, 0x40}, {' ', 0x83, 0, 0x41},
  {' ', 0x0A, 0, 0x42}, {' ', 0x01, 0, 0x43}, {' ', 0x09, 0, 0x44},
  {' ', 0x78, 0, 0x57}, {' ', 0x07, 0, 0x58}, {' ', 0x7E, 0, 0x46},
  {'[', 0x54, 1, 0x1A}, {' ', 0x77, 0, 0x45}, {'*', 0x7C, 1, 0x37},
  {'-', 0x7B, 1, 0x4A}, {'+', 0x79, 1, 0x4E}, {'.', 0x71, 1, 0x53},
  {'0', 0x70, 1, 0x52}, {'1', 0x69, 1, 0x4F}, {'2', 0x72, 1, 0x50},
  {'3', 0x7A, 1, 0x51}, {'4', 0x6B, 1, 0x4B}, {'5', 0x73, 1, 0x4C},
  {'6', 0x74, 1, 0x4D}, {'7', 0x6C, 1, 0x47}, {'8', 0x75, 1, 0x48},
  {'9', 0x7D, 1, 0x49}, {']', 0x5B, 1, 0x1B}, {';', 0x4C, 1, 0x27},
  {'\'', 0x52, 1, 0x28}, {',', 0x41, 1, 0x33}, {'.', 0x49, 1, 0x34},
  {'/', 0x4A, 1, 0x35}, {',', 0x41, 1, 0x33}, {'\'', 0x52, 1, 0x28},
  {':', 0x4C, 1, 0x27}, {'"', 0x52, 1, 0x28}, {'<', 0x41, 1, 0x33},
  {'>', 0x49, 1, 0x34}, {'{', 0x54, 1, 0x1A}, {'}', 0x5B, 1, 0x1B},
  {'(', 0x46, 1, 0x0A}, {')', 0x45, 1, 0x0B}, {'&', 0x3D, 1, 0x08},
  {'!', 0x16, 1, 0x02}, {'#', 0x26, 1, 0x04}, {'%', 0x2E, 1, 0x06},
  {'*', 0x7C, 1, 0x37}, {'^', 0x36, 1, 0x07}, {'~', 0x0E, 1, 0x29},
  {'`', 0x0E, 1, 0x29}, {' ', 0x7F, 0, 0x0E},

  // Function Keys F1-F12
  {' ', 0x05, 0, 0x3B}, // F1
  {' ', 0x06, 0, 0x3C}, // F2
  {' ', 0x04, 0, 0x3D}, // F3
  {' ', 0x0C, 0, 0x3E}, // F4
  {' ', 0x03, 0, 0x3F}, // F5
  {' ', 0x0B, 0, 0x40}, // F6
  {' ', 0x83, 0, 0x41}, // F7
  {' ', 0x0A, 0, 0x42}, // F8
  {' ', 0x01, 0, 0x43}, // F9
  {' ', 0x09, 0, 0x44}, // F10
  {' ', 0x78, 0, 0x57}, // F11
  {' ', 0x07, 0, 0x58}, // F12
};

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
#endif
  pinMode(xt_clk, OUTPUT);
  pinMode(xt_data, OUTPUT);
  digitalWrite(xt_clk, HIGH);
  digitalWrite(xt_data, HIGH);
  
  // Initialize the Serial interface to communicate via COM port
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

void loop() {
  unsigned char code = _read_serial();

#ifdef DEBUG
  if (code != 0) Serial.println(code, HEX);
#endif

  if (code != 0) {
    unsigned char i = 0;
    while (i < GROUP1_CNT) {
      if (ps2_group1[i].character == code) {
#ifdef DEBUG
        Serial.write(ps2_group1[i].character);
#endif
        _write(ps2_group1[i].xt_make);
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
