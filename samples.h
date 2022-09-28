
#include <inttypes.h>

// Circular Sample buffer
//  putSample
//  getSample
//  putSamples
//  getSamples
//  size
//  full
//  contents


// FIXED SIZE allows for a lot of optimizations vs paramaterized size.
#define SAMPLE_BUFFER_SIZE 256

typedef struct SampleBuffer
{
  uint16_t buff[SAMPLE_BUFFER_SIZE];
  uint8_t in;
  uint8_t out;
} SampleBuffer;

void SampleBuffer_init( SampleBuffer* sb);
int SampleBuffer_size( SampleBuffer* sb);
bool SampleBuffer_empty( SampleBuffer* sb);
bool SampleBuffer_full( SampleBuffer* sb);
void SampleBuffer_push( SampleBuffer* sb, uint16_t sample);
uint16_t SampleBuffer_pop( SampleBuffer* sb);
