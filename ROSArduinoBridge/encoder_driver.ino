/* *************************************************************
   Encoder definitions
   
   Add an "#ifdef" block to this file to include support for
   a particular encoder board or library. Then add the appropriate
   #define near the top of the main ROSArduinoBridge.ino file.
   
   ************************************************************ */
   
#ifdef USE_BASE

#ifdef ROBOGAIA
  /* The Robogaia Mega Encoder shield */
  #include "MegaEncoderCounter.h"

  /* Create the encoder shield object */
  MegaEncoderCounter encoders = MegaEncoderCounter(4); // Initializes the Mega Encoder Counter in the 4X Count mode
  
  /* Wrap the encoder reading function */
  long readEncoder(int i) {
    if (i == LEFT) return encoders.YAxisGetCount();
    else return encoders.XAxisGetCount();
  }

  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT) return encoders.YAxisReset();
    else return encoders.XAxisReset();
  }
#elif defined(ARDUINO_ENC_COUNTER)
  volatile long left_enc_pos = 0L;
  volatile long right_enc_pos = 0L;
    
  // D4 için PCINT2_vect kesmesi (PORTD)
  ISR (PCINT2_vect) {
    static uint8_t last_state_left = 0;
    uint8_t new_state_left = digitalRead(4);  // D4 pini (PCINT20)
    if (new_state_left != last_state_left) {
      left_enc_pos++;  // Sol encoder sayacını artır
      last_state_left = new_state_left;
    }
  }

  // A2 için PCINT1_vect kesmesi (PORTC)
  ISR (PCINT1_vect) {
    static uint8_t last_state_right = 0;
    uint8_t new_state_right = digitalRead(A2);  // A2 pini (PCINT10)
    if (new_state_right != last_state_right) {
      right_enc_pos++;  // Sağ encoder sayacını artır
      last_state_right = new_state_right;
    }
  }

  /* Define the readEncoder function for ARDUINO_ENC_COUNTER */
  long readEncoder(int i) {
    if (i == LEFT) return left_enc_pos;
    else return right_enc_pos;
  }
     
  /* Wrap the encoder reset function */
  void resetEncoder(int i) {
    if (i == LEFT){
      left_enc_pos=0L;
      return;
    } else { 
      right_enc_pos=0L;
      return;
    }
  }
#else
  #error A encoder driver must be selected!
#endif

/* Wrap the encoder reset function */
void resetEncoders() {
  resetEncoder(LEFT);
  resetEncoder(RIGHT);
}

#endif

