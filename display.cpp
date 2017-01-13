#include "display.h"

Display::Display()
{
}

// Initialize the display.
void Display::init()
{
  DDRB |= (SR_DATA | SR_LATCH | SR_CLOCK | SR_RESET); // Set shift register pins as outputs
  SHIFT_PORT &= ~(SR_DATA | SR_LATCH | SR_CLOCK);     // Set shift register control pins low
  SHIFT_PORT |= SR_RESET;                             // Set reset pin high
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << DORD);      // Start SPI as Master, transfer with LSBFIRST
}

void Display::set_pixel(byte x, byte y, byte value)
{
  _framebuffer[y][x] = value;
}
  
void Display::clear_pixel(byte x, byte y)
{
  _framebuffer[y][x] = 0;
}

byte Display::get_pixel(byte x, byte y)
{
  return _framebuffer[y][x];
}

// Update the display according to the data in the framebuffer.
void Display::update(unsigned long t)
{
  /*if (t - _last_update < (1000 / FPS))
  {
    // Only update FPS times per second
    return;
  }*/
  
  for (int row = 0; row < 20; row++)
  {
    unsigned long row_data = 0; // 32 bits that are all disabled.
                                // The first 20 bits of row_data are used for the state of the row outputs (top to bottom).

    row_data |= 1UL << (32 - row - 1);  // Set enabled bit for current row
                                        // Example: if row == 2 (which is the 3rd row) then row_data = 00100000000000000000000000000000

    if (DEBUG_DISPLAY)
    {
      Serial.print("Row: ");
      Serial.print(row);
      Serial.print(": ");
      Serial.println(row_data);
    }
    
    for (int col = 0; col < 10; col++)
    {
      if (_framebuffer[row][col] == 0)
      {
        clearscreen();  // Clear all the outputs.
        continue;       // Continue processing next column.
      }

      uint32_t column_data = (uint32_t)-1;  // 32 bits that are all enabled.
                                            // The first 30 bits of column_data are used for the state of the column outputs (left to right).
                                            // 0 - 9 = RED, 10 - 19 = GREEN, 20 - 29 = BLUE.

      // Get RGB value for current pixel in framebuffer.
      byte r = _palette[_framebuffer[row][col]][0];
      byte g = _palette[_framebuffer[row][col]][1];
      byte b = _palette[_framebuffer[row][col]][2];

      if (r == 1)
      {
        column_data &= ~(1UL << (32 - col - 1));
      }

      if (g == 1)
      {
        column_data &= ~(1UL << (32 - col - 10 - 1));
      }

      if (b == 1)
      {
        column_data &= ~(1UL << (32 - col - 20 - 1));
      }
      
      // Split row_data and column_data to seperate bytes.
      _data[0] = (byte)((row_data >> 24) & 0xFF);
      _data[1] = (byte)((row_data >> 16) & 0xFF);
      _data[2] = (byte)((row_data >> 8) & 0xFF);
      _data[3] = (byte)((column_data >> 24) & 0xFF);
      _data[4] = (byte)((column_data >> 16) & 0xFF);
      _data[5] = (byte)((column_data >> 8) & 0xFF);
      _data[6] = (byte)(column_data & 0xFF);

      // Output debug data to serial monitor.
      if (DEBUG_DISPLAY)
      {
        Serial.print(row);
        Serial.print(":\t");
        
        for (int i = 0; i < 7; i++)
        {
          Serial.print("(");
          Serial.print(_data[i]);
          Serial.print(") ");
          Serial.print("\t");
        }
  
        Serial.print(column_data);
        Serial.print("\r\n");
      }
      
      // Shift out the data to the shift registers.
      // The first byte needs to be shift out as last (rows are at the top of the shift registers).
      // The last bit if each byte needs to be shift out first (LSBFIRST).
      shiftout(_data);
    }
  }

  //_last_update = millis();
}

// Clear the shift registers (clears screen)
void Display::clearscreen()
{
  SHIFT_PORT &= ~SR_RESET;  // Reset low
  SHIFT_PORT |= SR_RESET;   // Reset high
}

// Shift out data to shift register and latch.
void Display::shiftout(byte* data)
{
  SHIFT_PORT &= ~SR_LATCH; // Latch low
  
  for (int i = 6; i >= 0; i--)
  {
    spi_send(data[i]);
  }
  
  SHIFT_PORT |= SR_LATCH;  // Toggle latch
  SHIFT_PORT &= ~SR_LATCH; // Latch low
}

// Send bytes using SPI.
void Display::spi_send(byte data)
{
  SPDR = data; //Shift out the data.
  
  loop_until_bit_is_set(SPSR, SPIF); // Wait for SPI to finish
}
