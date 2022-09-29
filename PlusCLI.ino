/**
 * @file    PlusCLI.ino
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * [Description]
 *
 */


/* Includes ******************************************************************/
#include <Arduino.h>
#include "CLI.h"
#include "DSP.h"
#include "conio.h"
#include "mineswp.h"
#include "samples.h"
#include <limits.h>

/* Defines *******************************************************************/
/* Types *********************************************************************/
/* Interfaces ****************************************************************/
/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int monitor_main (int argc, char** argv);

/* Data **********************************************************************/
/* Functions *****************************************************************/

/**
 * Simple example starting place main callback. 
 * A good example to copy to start.
 * 
 * look for the commandEntryTable to see how this function is included.
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int hw_main (int argc, char** argv)
{
  while( !kbhit() )
  {
    printf(F("Hello World\n"));
  }
  
  return 0;
}

/**
 * Classic arduino blink sketch mofied to CLI command
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int blink (int argc, char** argv)
{
  // Setup 
  pinMode(LED_BUILTIN, OUTPUT);

  // Loop
  while( !kbhit() )
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);                       // wait for a second
  }

  // Cleanup
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW

  return SUCCESS;
}

/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int echo (int argc, char** argv)
{
  int x, y;
  if( argc >= 2 )
  {
    puts(argv[1]);
  }
  whereCursor( &x, &y);
  printf(F("\nx=%d y=%d\n"), x, y);
  return SUCCESS;
}

/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int bgcolor (int argc, char** argv)
{
  if( argc < 2 )
  {
    textbackground( 0 );

  }
  else
  {
    textbackground( atoi(argv[1]) );
  }
  clrscr();
  return SUCCESS;
}

/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int fgcolor (int argc, char** argv)
{
  if( argc < 2 )
  {
    textcolor( 0 );

  }
  else
  {
    textcolor( atoi(argv[1]) );
  }
  return SUCCESS;
}

/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int seedrand (int argc, char** argv)
{
  if( argc < 2 )
  {
    randomSeed(analogRead(0));

  }
  else
  {
    const char* ptr = argv[1];
    unsigned long sum =0;
    while (*ptr != '\0')
    {
      sum <<= 1;
      sum += *ptr++;
    }
    printf(F("Checksum = %lX\n"), sum);
    randomSeed(sum);
  }
  return SUCCESS;
}

/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int DSP_main (int argc, char** argv)
{
  SINCOS16_t tmp;
  Polar16 tmp2;
  Q_15 cosine;
  Q_15 sine;
  BAM16 deg = 0;
  for( int i=0; i<COSINE_TABLE_SIZE; ++i)
  {
    tmp = CORDIC16_sincos(deg);
    cosine = cosine_table(deg>>8);
    sine = cosine_table((deg+BAM16_270_DEGREES)>>8);
    tmp2 = CORDIC16_rect2polar( tmp);
    printf( F("Angle(%04hX) sin:% 6d - % 6d=% 6d\tcos:% 6d - % 6d=% 6d  0x%04hX 0x%04hX\n"), deg, tmp.sin, sine, tmp.sin-sine, tmp.cos, cosine, tmp.cos-cosine, tmp2.mag, tmp2.phase);
    deg += 0x0100;
  }

  if (argc > 1)
  {
    printf( F("%d\n"), cosine_table(atoi(argv[1])) );
  }
  
  return 2;
}

/**
 * [Description]
 *
 * @param
 * @return
 */
void bargraph( const Q_15* data, int N)
{
  while( N-- )
  {
    Q_15 val = *data++;
    printf(F("\n0x%04hx : "),val);
    unsigned int hashes = (((int)val+0x8200) >> 10) & 0x3F;
    while( hashes-- )
    {
      putch('#');
    }
  }
  putch('\n');
}

/**
 * [Description]
 *
 * @param argc number of arguments
 * @param argv a list of string arguments
 * @return Error Code, 0 means success
 */
int DSP2_main (int argc, char** argv)
{
  SINCOS16_t tmp;
  Q_15 input[32];
  Q_15 output[32];
  BAM16 angle;
  BAM16 dAngle;
  

  puts(F("building signal\n"));
  angle = 0;
  dAngle = FREQUENCY_BAM16_PER_SAMPLE(128,1024);
  for( int i=0; i<32; ++i)
  {
    tmp = CORDIC16_sincos(angle);
    input[i]= tmp.cos >> 3;
    angle += dAngle;
  }

  angle = BAM16_45_DEGREES;
  dAngle = FREQUENCY_BAM16_PER_SAMPLE(192,1024);
  for( int i=0; i<32; ++i)
  {
    tmp = CORDIC16_sincos(angle);
    input[i]+= (tmp.cos >> 2);
    angle += dAngle;
  }

  angle = 0;
  dAngle = FREQUENCY_BAM16_PER_SAMPLE(288,1024);
  for( int i=0; i<32; ++i)
  {
    tmp = CORDIC16_sincos(angle);
    input[i]+= (tmp.cos >> 2);
    angle += dAngle;
  }

  bargraph( input, 32);

  puts(F("Inphase\n"));
  FFT_inphase( output, input, 5, 0);
  bargraph( output, 32);

  puts(F("Quadrature\n"));
  FFT_inphase( output, input, 5, BAM16toBAM8(BAM16_90_DEGREES));
  bargraph( output, 32);

  puts(F("Magnitude\n"));
  FFT_magnitude( output, input, 5);
  bargraph( output, 32);
  
  return 0;  
}

#define ADC_PIN A0
#define TONE_OUT 8
#define ADC_NUM_SAMPLES_ORDER 6
#define ADC_NUM_SAMPLES (1 << ADC_NUM_SAMPLES_ORDER)
SampleBuffer adcSB;

void adc_intterupt (void)
{
  SampleBuffer_push(&adcSB, (analogRead(ADC_PIN) - 0x200) << 6);
}

int adc_main (int argc, char** argv)
{
  Q_15 input[ADC_NUM_SAMPLES];
  Q_15 output[ADC_NUM_SAMPLES];

  int freqT=440;

  if( argc > 1 )
  {
    freqT=atoi(argv[1]);
  }

  freqT = constrain(freqT, 200, 4000);

  //setup 
  pinMode( ADC_PIN, INPUT ); 
  //SampleBuffer_init( &adcSB ); 

  tone(TONE_OUT, freqT);

  //loop
  while( !kbhit() )
  {
    // Get enough samples to do forier analysis
    getSamples(ADC_PIN, input, ADC_NUM_SAMPLES, PERIOD_US_5KHZ );

    // Forward Fourier Transform
    FFT_magnitude( output, input, ADC_NUM_SAMPLES_ORDER);

    // Display results
    clrscr();
    bargraph( output, ADC_NUM_SAMPLES);
  }
  
  noTone(TONE_OUT);
  
  return 0;
}

/**
 * [Description]
 *
 * @param
 * @return
 */
const char* getPrompt (void)
{
  static char prompt[32];
  static int count = 0;
  snprintf(prompt, 31, "\n%d,%d> ", count++, CLI_getLastReturnCode() );
  return prompt;
}

/**
 * [Description]
 *
 * @param
 * @return
 */
void setup()
{
  const CLI_CommandEntry commandEntryTable[] =
  {
    { "BLINK"   , blink           },
    { "ECHO"    , echo            },
    { "BGCOLOR" , bgcolor         },
    { "FGCOLOR" , fgcolor         },
    { "SEED"    , seedrand        },
    { "GAME"    , minesweep_main  },
    { "DSP"     , DSP_main        },
    { "FFT"     , DSP2_main       },
    { "MONITOR" , monitor_main    },
    { "ADC"     , adc_main        },
    { "HW"      , hw_main         },
  };

  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(LONG_MAX);
  randomSeed(analogRead(0));
  pinMode(LED_BUILTIN, OUTPUT);
  CLI_registerCommandEntryTable( commandEntryTable, sizeof(commandEntryTable)/sizeof(commandEntryTable[0]));
  CLI_getPrompt = getPrompt;
}

/**
 * [Description]
 *
 * @param
 * @return
 */
void loop()
{
  CLI_loop();
}
