/**
 * @file    samples.h
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * Manages the collection of samples from the ADC at a set Sample Rate.
 *
 *  Circular Sample buffer
 *  putSample
 *  getSample
 *  putSamples
 *  getSamples
 *  size
 *  full
 *  contents
 *  
 */
#ifndef   SAMPLES_H
#define   SAMPLES_H

/* Includes ******************************************************************/
#include <inttypes.h>
#include "DSP.h"

/* Defines *******************************************************************/
// FIXED SIZE allows for a lot of optimizations vs paramaterized size.
#define SAMPLE_BUFFER_SIZE 256

#define PERIOD_US_8KHZ 125
#define PERIOD_US_6666HZ 150
#define PERIOD_US_5KHZ 200
#define PERIOD_US_4KHZ 250
#define PERIOD_US_3333HZ 300
#define PERIOD_US_2KHZ 500

/* Types *********************************************************************/
typedef struct SampleBuffer
{
  uint16_t buff[SAMPLE_BUFFER_SIZE];
  uint8_t in;
  uint8_t out;
} SampleBuffer;

/* Interfaces ****************************************************************/
/* Data **********************************************************************/
/* Functions *****************************************************************/

/**
 * [Description]
 *
 * @param
 * @return
 */
void SampleBuffer_init( SampleBuffer* sb);

/**
 * [Description]
 *
 * @param
 * @return
 */
int SampleBuffer_size( SampleBuffer* sb);

/**
 * [Description]
 *
 * @param
 * @return
 */
bool SampleBuffer_empty( SampleBuffer* sb);

/**
 * [Description]
 *
 * @param
 * @return
 */
bool SampleBuffer_full( SampleBuffer* sb);

/**
 * [Description]
 *
 * @param
 * @return
 */
void SampleBuffer_push( SampleBuffer* sb, uint16_t sample);

/**
 * [Description]
 *
 * @param
 * @return
 */
uint16_t SampleBuffer_pop( SampleBuffer* sb);


/**
 * An unbuffered blocking way to read a collection of sampled analog data from an analog input.
 * This is a attempt to limit the calls to only standard Arduino apis, avoiding hw specfic 
 * intterrupts and register accesses. It is perfect for getting a quick sample of an analog input
 * for future processing such as to update a spectrum display or detect inline tones.
 *
 * @param pin Arduino pin identifier for the analog pin to use
 * @param buf the buffer to write the samples to
 * @param count the number of samples to wait for
 * @param sampleTime_us time in microseconds between samples
 * @return Number of samples read
 */
int getSamples( int pin, Q_15* buf, int count, int sampleTime_us);

#endif // SAMPLES_H
