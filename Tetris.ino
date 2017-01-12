//#include <avr/io.h>

#define SR_LATCH 2
#define SR_CLOCK 3
#define SR_DATA 4
#define SR_RESET 5
#define DEBUG false

/*
#define SHIFT_REGISTER DDRB
#define SHIFT_PORT PORTB
#define DATA (1<<PB3)    //MOSI (SI)
#define LATCH (1<<PB2)    //SS   (RCK)
#define CLOCK (1<<PB5)    //SCK  (SCK)

// http://jumptuck.com/2011/11/03/how-to-drive-595-shift-registers-with-avr-hardware-spi/
*/

// Color data.
// Always on = 10, always off = 0, otherwise on for number of cycles.
byte palette[][3] = {
  {  0,  0,  0 },
  { 10,  0,  0 },
  {  0, 10,  0 },
  {  0,  0, 10 },
  { 10, 10,  0 },
  {  0, 10, 10 },
  { 10,  0, 10 },
  { 10, 10, 10 }
};

// The frame buffer (pixel data).
byte framebuffer[20][10] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 7, 7 },
  { 0, 1, 2, 3, 4, 5, 6, 0, 7, 7 },
  { 0, 1, 0, 0, 0, 0, 6, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 6, 0, 0, 0 },
  { 0, 6, 5, 4, 3, 2, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 2, 0, 0, 0 },
  { 0, 0, 0, 0, 3, 0, 0, 4, 0, 0 },
  { 0, 0, 0, 2, 0, 0, 0, 0, 5, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 6, 0 },
  { 0, 0, 0, 0, 2, 0, 0, 5, 0, 0 },
  { 0, 0, 0, 0, 0, 3, 4, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 6, 0, 0, 0 },
  { 1, 0, 1, 0, 0, 5, 0, 5, 0, 0 },
  { 2, 0, 2, 0, 0, 4, 0, 4, 0, 0 },
  { 3, 6, 3, 0, 3, 0, 6, 0, 3, 0 },
  { 4, 0, 4, 0, 2, 0, 0, 0, 2, 0 },
  { 5, 0, 5, 0, 1, 0, 0, 0, 1, 0 }
};

/*
byte framebuffer[20][10] = {
  { 7, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};
*/

byte data[7]; // Shift register data

void setup() 
{
  pinMode(SR_LATCH, OUTPUT);
  pinMode(SR_CLOCK, OUTPUT);
  pinMode(SR_DATA, OUTPUT);
  pinMode(SR_RESET, OUTPUT);

  digitalWrite(SR_CLOCK, LOW);
  digitalWrite(SR_LATCH, LOW);
  digitalWrite(SR_DATA, LOW);
  digitalWrite(SR_RESET, HIGH);

  /*
  SHIFT_REGISTER |= (DATA | LATCH | CLOCK);  //Set control pins as outputs
  SHIFT_PORT &= ~(DATA | LATCH | CLOCK);    //Set control pins low

  SPCR = (1<<SPE) | (1<<MSTR);  //Start SPI as Master
  */
  
  Serial.begin(9600);
  Serial.println("Setup done");
}

void loop() 
{
  unsigned long start_time = micros();
  
  for (int row = 0; row < 20; row++)
  {
    unsigned long row_data = 0; // 32 bits that are all disabled.
                                // The first 20 bits of row_data are used for the state of the row outputs (top to bottom).

    row_data |= 1UL << (32 - row - 1);  // Set enabled bit for current row
                                      // Example: if row == 2 (which is the 3rd row) then row_data = 00100000000000000000000000000000

    if (DEBUG)
    {
      Serial.print("Row: ");
      Serial.print(row);
      Serial.print(": ");
      Serial.println(row_data);
    }
    
    for (int col = 0; col < 10; col++)
    {
      if (framebuffer[row][col] == 0)
      {
        clear_sr(); // Clear all the outputs.
        continue;   // Continue processing next column.
      }

      uint32_t column_data = (uint32_t)-1;  // 32 bits that are all enabled.
                                            // The first 30 bits of column_data are used for the state of the column outputs (left to right).
                                            // 0 - 9 = RED, 10 - 19 = GREEN, 20 - 29 = BLUE.

      // Get RGB value for current pixel in framebuffer.
      byte r = palette[framebuffer[row][col]][0];
      byte g = palette[framebuffer[row][col]][1];
      byte b = palette[framebuffer[row][col]][2];

      if (r == 10)
      {
        column_data &= ~(1UL << (32 - col - 1));
      }

      if (g == 10)
      {
        column_data &= ~(1UL << (32 - col - 10 - 1));
      }

      if (b == 10)
      {
        column_data &= ~(1UL << (32 - col - 20 - 1));
      }
      
      // Split row_data and column_data to seperate bytes.
      data[0] = (byte)((row_data >> 24) & 0xFF);
      data[1] = (byte)((row_data >> 16) & 0xFF);
      data[2] = (byte)((row_data >> 8) & 0xFF);
      data[3] = (byte)((column_data >> 24) & 0xFF); //255 & ~(1 << 7);
      data[4] = (byte)((column_data >> 16) & 0xFF);
      data[5] = (byte)((column_data >> 8) & 0xFF);
      data[6] = (byte)(column_data & 0xFF);

      // Output debug data to serial monitor.
      if (DEBUG)
      {
        Serial.print(row);
        Serial.print(":\t");
        
        for (int i = 0; i < 7; i++)
        {
          Serial.print("(");
          Serial.print(data[i]);
          Serial.print(") ");
          print_bin(data[i]);
          Serial.print("\t");
        }
  
        Serial.print(column_data);
        Serial.print("\r\n");
      }
      
      // Shift out the data to the shift registers.
      // The first byte needs to be shift out as last (rows are at the top of the shift registers).
      // The last bit if each byte needs to be shift out first (LSBFIRST).
      shiftout_sr(data);
    }
  }

  if (DEBUG)
  {
    Serial.print("Duration: ");
    Serial.println(micros() - start_time);
  
    delay(100000);
  }
}

// Clear the shift registers.
void clear_sr()
{
  digitalWrite(SR_RESET, LOW);
  digitalWrite(SR_RESET, HIGH);
}

// Shift out data to shift register (including latch).
void shiftout_sr(byte* d)
{
  //SHIFT_PORT &= ~LATCH;
  digitalWrite(SR_LATCH, LOW);

  for (int i = 6; i >= 0; i--)
  {
    shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, d[i]);
    //spi_send(d[i]);
  }
  
  digitalWrite(SR_LATCH, HIGH);
  //SHIFT_PORT |= LATCH;
  //SHIFT_PORT &= ~LATCH;
}

void spi_send(byte d)
{
  SPDR = d; //Shift out the data.
  
  //while(!(SPSR & (1<<SPIF))); //Wait for SPI process to finish
  loop_until_bit_is_set(SPSR, SPIF); 
}

// Serial print binary value of a byte.
void print_bin(byte val)
{
    if(val!=0) print_bin(val/2);
    if(val!=0) Serial.print(val%2);
}
