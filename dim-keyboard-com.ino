// Define pin numbers for clock and data lines
const int CLOCK_PIN = 2;  // Pin 8 for clock
const int DATA_PIN = 3;   // Pin 9 for data

// Timing for 10 kHz clock (50 microseconds per half cycle)
const int CLOCK_DELAY = 55;  // Try adjusting slightly for stability

bool self_test_done = false;

void setup() {
  pinMode(CLOCK_PIN, OUTPUT);  // Set clock pin as output
  pinMode(DATA_PIN, OUTPUT);   // Set data pin as output
  
  digitalWrite(CLOCK_PIN, HIGH);  // Idle state is high
  digitalWrite(DATA_PIN, HIGH);   // Idle state is high

  // Serial monitor setup for logging
  Serial.begin(9600);  
  Serial.println("Arduino Keyboard Emulation Initialized.");
}

// Function to toggle the clock line (one pulse cycle)
void clock_pulse() {
  digitalWrite(CLOCK_PIN, LOW);
  delayMicroseconds(CLOCK_DELAY);
  digitalWrite(CLOCK_PIN, HIGH);
  delayMicroseconds(CLOCK_DELAY);
}

// Function to calculate odd parity for a byte
int calculate_parity(byte data_byte) {
  return __builtin_popcount(data_byte) % 2;  // Built-in function to count 1's in a byte
}

// Function to send one byte of data according to the IBM AT protocol
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

  // Send the parity bit (odd parity)
  int parity_bit = calculate_parity(data_byte);
  digitalWrite(DATA_PIN, parity_bit == 0 ? HIGH : LOW);
  clock_pulse();

  // Send stop bit (1)
  digitalWrite(DATA_PIN, HIGH);
  clock_pulse();
}

// Function to send the self-test pass code after detecting PC power on
void send_self_test_pass_code() {
  delay(1000);
  Serial.println("Sending self-test pass code...");
  send_byte(0xAA);  // 0xAA is the self-test pass code
  Serial.println("Self-test pass code sent.");
}

// Function to send a key press scan code
void send_keypress(byte scan_code) {
  Serial.print("Key press: 0x");
  Serial.println(scan_code, HEX);
  send_byte(scan_code);  // Send the scan code
}

// Function to send a key release scan code
void send_keyrelease(byte scan_code) {
  Serial.print("Key release: 0x");
  Serial.println(scan_code, HEX);
  send_byte(0xF0);  // Send the key release identifier (0xF0)
  send_byte(scan_code);  // Followed by the scan code of the released key
}

// Function to wait for the PC to initiate communication (based on clock signal)
bool wait_for_pc_signal() {
  pinMode(CLOCK_PIN, INPUT);  // Set clock pin to input to detect PC signal
  Serial.println("Waiting for clock signal from PC (initially 0V)...");

  // Wait until the clock pin goes high, indicating the PC is ready to communicate
  while (digitalRead(CLOCK_PIN) == LOW) {
    delay(1);  // Small delay to avoid busy-waiting too much
  }

  if (digitalRead(CLOCK_PIN) == HIGH) {
    delay(1000);
  }

  Serial.println("Clock signal detected from PC (now high).");
  pinMode(CLOCK_PIN, OUTPUT);  // Set clock back to output for communication
  return true;
}

// Function to process and send a key based on Serial input
void process_serial_input() {
  // Check if data is available to read from the serial port
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');  // Read the input until newline character

    // Parse the input as a hexadecimal number (e.g., "0x5A")
    input.trim();  // Remove any whitespace
    long scan_code = strtol(input.c_str(), NULL, 16);  // Convert string to long, using base 16 (hexadecimal)

    if (scan_code >= 0 && scan_code <= 0xFF) {  // Ensure the scan code is within a valid byte range
      byte key_code = (byte)scan_code;
      
      // Send the keypress and key release
      send_keypress(key_code);
      delay(100);  // Simulate key press duration
      send_keyrelease(key_code);
    } else {
      Serial.println("Invalid scan code. Please enter a valid hex code (e.g., 0x5A).");
    }
  }
}

void loop() {
  // Wait for the PC to initiate communication
  if (!self_test_done && wait_for_pc_signal()) {
    send_self_test_pass_code();  // Send the self-test pass code once the PC is ready
    self_test_done = true;
  }
  
  
  /*
   Wait for 10 seconds before sending the '1' keypress
  delay(2000);

  send_keypress(0x5A);
  delay(100);
  send_keyrelease(0x5A);
  
  delay(25000);  // 10-second delay
  
  // Send the '1' keypress (scan code 0x16)
  //send_keypress(0x5A);  // Key press for '1' (scan code 0x16)
  //delay(100);  // Short delay to simulate the press duration
  //send_keyrelease(0x5A);  // Key release for '1'
  // The loop ends here; you can repeat or expand with more functionality
  while (true);  // Hold the program here, or implement further logic
  */

  process_serial_input();

}
