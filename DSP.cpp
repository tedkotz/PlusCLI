/**
 * @file    [FileName.c]
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * [Description]
 *
 */
/* Includes ******************************************************************/
#include "DSP.h"
#include <avr/pgmspace.h>
/* Defines *******************************************************************/
/* Types *********************************************************************/
/* Interfaces ****************************************************************/
/* Data **********************************************************************/

/* Functions *****************************************************************/
//extern "C" int32_t Q15_mult ( int32_t a, int32_t b )
//{
//  return (a * b) >> 15;
//}


extern "C" CORDIC16_t cordic16( BAM16 angle )
{
  // Initialization of tables of constants used by CORDIC
  // need a table of arctangents of negative powers of two:
  // hex(arctan(2^(-index))*0x8000/pi+0.5)
//  static const int N = 16;
//  static const BAM16 arctanTable[N] =
//  {
//    0x2000, 0x12E4, 0x09FB, 0x0511, 
//    0x028B, 0x0146, 0x00A3, 0x0051,
//    0x0029, 0x0014, 0x000A, 0x0005,
//    0x0003, 0x0001, 0x0001, 0x0000,
//  };
  // make use of 32 bits basically BAM32
  // hex(arctan(2^(-index))*0x80000000/pi+0.5)
//  static const int N = 20;
//  static const int32_t arctanTable[N] =
//  {
//    0x20000000, 0x12E4051E, 0x09FB385B, 0x051111D4, 
//    0x028B0D43, 0x0145D7E1, 0x00A2F61E, 0x00517C55,
//    0x0028BE53, 0x00145F2F, 0x000A2F98, 0x000517CC,
//    0x00028BE6, 0x000145F3, 0x0000A2FA, 0x0000517D,
//    0x000028BE, 0x0000145F, 0x00000A30, 0x00000518,
//  };
  static const PROGMEM int N = 16;
  // Makes more effcient use of all 16 bits BAM32 >> 14
  // hex(arctan(2^(-index))*0x20000/pi+0.5)
  static const PROGMEM uint16_t arctanTable[N] =
  {
    0x8000, 0x4B90, 0x27ED, 0x1444, 
    0x0A2C, 0x0517, 0x028C, 0x0146,
    0x00A3, 0x0051, 0x0029, 0x0014,
    0x000A, 0x0005, 0x0003, 0x0001,
  };
  
  
  // hex(0.5+0x8000*1/sqrt(1+(2^(-2*(0))))*1/sqrt(1+(2^(-2*(1))))*1/sqrt(1+(2^(-2*(2))))*1/sqrt(1+(2^(-2*(3))))*1/sqrt(1+(2^(-2*(4))))*1/sqrt(1+(2^(-2*(5))))*1/sqrt(1+(2^(-2*(6))))*1/sqrt(1+(2^(-2*(7))))*1/sqrt(1+(2^(-2*(8))))*1/sqrt(1+(2^(-2*(9)))))
  // and a table of products of reciprocal lengths of vectors [1, 2^-2j]:
  // K[k] = 0x8000
  // for( i=0; i< k; ++i)
  // {
  //   K[k] *= 1/sqrt(1+(2^(-2*(i))));
  // }
  // K[k] = hex(K[k] +0.5)
//  static const Q_15 K[N] =
//  {
//    0x5a82, 0x50F4, 0x4E8A, 0x4DEE, 
//    0x4DC7, 0x4DBE, 0x4DBB, 0x4DBB, 
//    0x4DBB, 0x4DBA, 0x4DBA, 0x4DBA,
//    0x4DBA, 0x4DBA, 0x4DBA, 0x4DBA,
//  };
  // As long as N > ~14 0x4DBA76D4 is a great Q_31 approximation

  
  int32_t vsin = 0;
//  int32_t vcos = 0x4DBA76D4; // K[N-1] * 2^31
  int32_t vcos = 0x4DB9DB5F; // K[N-1] * 0x7FFF * 2^16
  int32_t vtmp;
  
  // Use Symmetry to get angle in quadrant 1 or 4.
  if( (angle + 0x4000) & 0x8000 )
  {
    angle += 0x8000;
    vcos = -vcos;
    vsin = -vsin;
  }

  int32_t angle32 = (int32_t)angle << 16;
    
  for(int i=0; i<N; ++i)
  {
    if(angle32 < 0)
    {
      angle32 += (int32_t)arctanTable[i] << 14;
      vtmp = vcos + (vsin >> i);
      vsin = vsin - (vcos >> i);
      vcos = vtmp;
    }
    else
    {
      angle32 -= (int32_t)arctanTable[i] << 14;
      vtmp = vcos - (vsin >> i);
      vsin = vsin + (vcos >> i);
      vcos = vtmp;
    }
  }
  
  CORDIC16_t v={ (Q_15)((vcos+0x8000) >> 16) , (Q_15)((vsin+0x8000) >> 16)};

  return v;
}


/**
 * [Description]
 *
 * @param
 * @return
 */
