// Define pin numbers for clock and data lines
const int CLOCK_PIN = 2;  // Pin 8 for clock
const int DATA_PIN = 3;   // Pin 9 for data

// Timing for 10 kHz clock (50 microseconds per half cycle)
const int CLOCK_DELAY = 55;  // Adjust for stability

void setup() {
  pinMode(CLOCK_PIN, OUTPUT);  // Set clock pin as output
  pinMode(DATA_PIN, OUTPUT);   // Set data pin as output
  
  digitalWrite(CLOCK_PIN, HIGH);  // Idle state is high
  digitalWrite(DATA_PIN, HIGH);   // Idle state is high

  // Serial monitor setup for logging
  Serial.begin(9600);  
  Serial.println("PC XT Keyboard Emulation Initialized.");
}

// Function to toggle the clock line (one pulse cycle)
void clock_pulse() {
  digitalWrite(CLOCK_PIN, LOW);
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(CLOCK_PIN, HIGH);
  delayMicroseconds(CLOCK_DELAY);
}

// Function to send one byte of data in PC XT protocol (without parity bit)
void send_byte(byte data_byte) {
  Serial.print("Sending byte: 0x");
  Serial.println(data_byte, HEX);

  // Send start bit (0)
  digitalWrite(DATA_PIN, LOW);
  clock_pulse();

  // Send each of the 8 data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    int bit = (data_byte >> i) & 0x01;
    digitalWrite(DATA_PIN, bit ? HIGH : LOW);
    clock_pulse();
  }

  // Send stop bit (1)
  digitalWrite(DATA_PIN, HIGH);
  clock_pulse();
}

// Function to send a key press scan code (PC XT Scan Code Set 1)
void send_keypress(byte scan_code) {
  Serial.print("Key press: 0x");
  Serial.println(scan_code, HEX);
  send_byte(scan_code);  // Send the scan code for key press
}

// Function to send a key release scan code (PC XT Scan Code Set 1)
void send_keyrelease(byte scan_code) {
  Serial.print("Key release: 0x");
  Serial.println(scan_code, HEX);
  send_byte(scan_code | 0x80);  // Send the scan code with the high bit set for key release
}

// Function to process and send a key based on Serial input
void process_serial_input() {
  // Check if data is available to read from the serial port
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read the input until newline character

    // Parse the input as a hexadecimal number (e.g., "0x1C")
    input.trim();  // Remove any whitespace
    long scan_code = strtol(input.c_str(), NULL, 16);  // Convert string to long, using base 16 (hexadecimal)

    if (scan_code >= 0 && scan_code <= 0xFF) {  // Ensure the scan code is within a valid byte range
      byte key_code = (byte)scan_code;
      
      // Send the keypress and key release
      send_keypress(key_code);
      delay(100);  // Simulate key press duration
      send_keyrelease(key_code);
    } else {
      Serial.println("Invalid scan code. Please enter a valid hex code (e.g., 0x1C).");
    }
  }
}

void loop() {
  // After setup, continuously check for Serial input and send keypresses
  process_serial_input();  // Check if a keypress is entered via Serial
}
