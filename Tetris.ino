#define SR_LATCH 2
#define SR_CLOCK 3
#define SR_DATA 4
#define SR_RESET 5

byte colors[][3] = {
  {  0,  0,  0 },
  { 10,  0,  0 },
  {  0, 10,  0 },
  {  0,  0, 10 },
  { 10, 10,  0 },
  {  0, 10, 10 },
  { 10,  0, 10 },
  { 10, 10, 10 }
};

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

byte data[7]; // SR data

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

  //Serial.begin(9600);
  //Serial.println("Setup done");
}

void loop() 
{
  for (int row = 0; row < 20; row++)
  {
    //Serial.print("Row ");
    //Serial.println(row);
    
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;

    int index = 0;
    int shift = 0;

    if (row < 8)
      index = 0;
    else if (row < 16)
      index = 1;
    else
      index = 2;

    if (row > 0)
    {
      shift = row - (8 * (row / 8));
    }

    // set row bit
    data[index] = 1 << shift;
    
    for (int col = 0; col < 10; col++)
    {
      if (framebuffer[row][col] == 0)
      {
        // display nothing
        continue;
      }

      uint32_t column_data2 = 4294967295;

      // Get RGB value for current pixel in framebuffer
      byte r = colors[framebuffer[row][col]][0];
      byte g = colors[framebuffer[row][col]][1];
      byte b = colors[framebuffer[row][col]][2];

      if (r == 10)
      {
        column_data2 &= ~(1UL << (col + 20 + 2));
      }

      if (g == 10)
      {
        column_data2 &= ~(1UL << (col + 10 + 2));
      }

      if (b == 10)
      {
        column_data2 &= ~(1UL << (col + 2));
      }

      data[3] = (byte)((column_data2 >> 24) & 0xFF) ;
      data[4] = (byte)((column_data2 >> 16) & 0xFF) ;
      data[5] = (byte)((column_data2 >> 8) & 0XFF);
      data[6] = (byte)((column_data2 & 0XFF));

/*
      Serial.print("ROW_");
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

      //print_bin(column_data2);
      Serial.print("\t");
      Serial.print(column_data2);

      //for (int i = 0; i < 30; i++)
      //{
      //  Serial.print(column_data[i]);
      //}

      Serial.print("\r\n");
      //Serial.println(column_data);
      */
      
      // shift out data BACKWARDS! (data[7] to data[0])
      // ...

      digitalWrite(SR_LATCH, LOW);

      for (int i = 7; i >= 0; i--)
      {
        // Serial.println(i);

        if (i > 2)
        {
          shiftOut(SR_DATA, SR_CLOCK, LSBFIRST, data[i]);
        }
        else
        {
          shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, data[i]);
        }
      }
      
      digitalWrite(SR_LATCH, HIGH);
    }
  }

  /*
  for (int cycle = 0; cycle < 10; cycle++)
  {
    for (int i = 0; i < 20; i++)
    {
      byte data = 0;
      byte shift = 0;
      
      if (i > 0)
      {
        shift = i - (8 * (i / 8));
      }
      
      data = 1 << shift;

      // Test PWM
      if (cycle > 3)
      {
        if (i == 4 || i == 8 || i == 12)
        {
          data = 0;
        }
      }

      if (cycle > 6)
      {
        if (i == 6 || i == 10 || i == 14)
        {
          data = 0;
        }
      }
  
      digitalWrite(SR_LATCH, LOW);
  
      shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, data);
  
      if (i > 0)
      {
        // Add empty bits in upper SR's
        if (i / 8 == 1)
        {
          shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, 0);
        }
        
        if (i / 8 == 2)
        {
          shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, 0);
          shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, 0);
        }
      }
      
      //Serial.print(shift);
      //Serial.print("\t");
      //Serial.println(data);
  
      digitalWrite(SR_LATCH, HIGH);
  
      // Reset
      digitalWrite(SR_LATCH, LOW);
      shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, 0);
      shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, 0);
      shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, 0);
      digitalWrite(SR_LATCH, HIGH);
  
      //delay(300);
    }
  }
  */
  
  //digitalWrite(SR_LATCH, LOW);
  //shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, B00000000);
  //shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, B00000000);
  //shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, B00000000);
  //digitalWrite(SR_LATCH, HIGH);

  //delay(5000);
}

void print_bin(byte val)
{
    if(val!=0) print_bin(val/2);
    if(val!=0) Serial.print(val%2);
}
