#include "samples.h"

void SampleBuffer_init( SampleBuffer* sb)
{
  sb->in=0;
  sb->out=0;
}

int SampleBuffer_size( SampleBuffer* sb)
{
  // Wrap is automatically handled for 256 size buffer by using uint8_t indexes
  return (sb->in - sb->out);
}

bool SampleBuffer_empty( SampleBuffer* sb)
{
  return (sb->in == sb->out);
}

bool SampleBuffer_full( SampleBuffer* sb)
{
  return ((sb->in + 1) == sb->out);
}

void SampleBuffer_push( SampleBuffer* sb, uint16_t sample)
{
  sb->buff[sb->in++]=sample;
}

uint16_t SampleBuffer_pop( SampleBuffer* sb)
{
  uint16_t sample=sb->buff[sb->out++];
  return sample;
}
