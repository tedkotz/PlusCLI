/**
 * @file    [FileName.c]
 * @author  Ted Kotz <ted@kotz.us>
 * @version 0.1
 *
 * A coolection of tools for Fixed Point arithmetic.
 *
 */
#ifndef   DSP_H
#define   DSP_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ******************************************************************/
#include <inttypes.h>
#include <stdlib.h>

/* Defines *******************************************************************/
#define Q15_ZERO 0x0000
#define Q15_ONE  0x7FFF


/* Types *********************************************************************/

/**
 * Binary Angle Measurement(BAM) is a system to use all the bits in a number
 * to encode the angle around the unit circle as an unsigned fraction of turn. It
 * takes advantage of the fact that binary numbers modulas 2^N arithmetic wrap at
 * overflow circular to strictly enforce the fact that angles also wrap.
 *  i.e. 0 degrees = 360 degrees = -360 degrees
 *
 *  BAM16 is the 16 bit variant of this encoding
 *
 *  BAM16 = DEG * 0x8000 / 180
 *  BAM16 = RAD * 0x8000 / PI
 *  BAM16 = TURNS * 0x8000 / 2
 *
 *  |Degrees |Radians |Turns |BAM16    |
 *  |--------|--------|------|---------|
 *  |-360    |-2PI    |-1    |0x0000   |
 *  |-270    |-3PI/2  |-3/4  |0x4000   |
 *  |-180    |-PI     |-1/2  |0x8000   |
 *  |-90     |-PI/2   |-1/4  |0xC000   |
 *  |0       |0       |0     |0x0000   |
 *  |30      |PI/6    |1/12  |0x4000/3 |
 *  |45      |PI/4    |1/8   |0x2000   |
 *  |60      |PI/3    |1/6   |0x8000/3 |
 *  |90      |PI/2    |1/4   |0x4000   |
 *  |180     |PI      |1/2   |0x8000   |
 *  |270     |3PI/2   |3/4   |0xC000   |
 *  |360     |2PI     |1     |0x0000   |
 *
 *  https://en.wikipedia.org/wiki/Binary_angular_measurement
 *  
 *  This type is unsigned so as to take advantage of the defined wrapping behaviour of unsinged integer types in C.
 *  
 *  Real frequencies are represented in ratios as BAM16 degrees per sample. For example a 250 Hz @ 1000 samp/sec would be 1/4 of a stune per sample so 0x40. 
 *  The FREQUENCY_BAM16_PER_SAMPLE macros is available to make these conversions at run or compile time.
 */
 
typedef uint8_t BAM8;
typedef uint16_t BAM16;
//typedef uint32_t BAM32;


/*
 *  Fixed Point is an encoding that allows a fractional value to be specified without using
 *  a floating point number. It can be generalized to signed types of the form Qm_n, where it
 *  has 1 sign bit, m bits before the radix point and n bits after.  It is the same as always
 *  representing the number divided by 2**n. This conversion can easily be accomplished with a shift.
 *  Regular integers are just a special case of fixed point where n=0;
 *
 *  Fixed Point numbers with the same n can be directly cast between with the rules and dangers as
 *  tradition  integers, they can also be added/subtracted with the same logic. Mutliplecation
 *  and division require some additional manipulation to remove the effects of the implied denominator.
 *
 *  As with integers wrap is bad so saturating addition and subtraction are often used (chacks for roll over)
 *
 */
/**
 *  Q_15 is a 16 bit fixed point quantity representing numbers on the range [-1..1] at a step of 1/32767.
 */
typedef int16_t   Q_15;

/**
 *  UQ1_15 is a 16 bit fixed point quantity representing numbers on the range [0..2] at a step of 1/32768.
 */
typedef uint16_t UQ1_15;

/**
 *  Q16_15 is a 16 bit fixed point quantity representing numbers on the range [-65536..65536] at a step of 1/32768.
 */
typedef int32_t   Q16_15;

/**
 *  UQ15_15 is a 16 bit fixed point quantity representing numbers on the range [0..131072] at a step of 1/32768.
 */
typedef uint32_t UQ17_15;

/**
 *  UQ_16 is a 16 bit fixed point quantity representing numbers on the range [0..1] at a step of 1/65536.
 */
typedef uint16_t UQ_16;

/**
 *  UQ16_16 is a 16 bit fixed point quantity representing numbers on the range [0..65536] at a step of 1/65536.
 */
typedef uint32_t UQ16_16;

typedef struct Q15_DIVMOD_t
{
  Q16_15 quot;
  Q_15 rem;
} Q15_DIVMOD_t;


typedef struct Complex16
{
  union
  {
    Q_15 real;
    Q_15 I;
    Q_15 x;
    Q_15 cos;
  };
  union
  {
    Q_15 imag;
    Q_15 Q;
    Q_15 y;
    Q_15 sin;
  };
} Complex16;

typedef Complex16 SINCOS16_t;

typedef struct Polar16
{
  Q_15  mag;
  BAM16 phase;
} Polar16;

/* Interfaces ****************************************************************/
/* Data **********************************************************************/
static const BAM16 BAM16_PI_RADIANS  = 0x8000;
static const BAM16 BAM16_0_DEGREES   = 0x0000;
static const BAM16 BAM16_270_DEGREES = 0xC000;
static const BAM16 BAM16_180_DEGREES = BAM16_PI_RADIANS;
static const BAM16 BAM16_90_DEGREES  = 0x4000;
static const BAM16 BAM16_60_DEGREES  = 0x10003/6; //BAM16_180_DEGREES/3+0.5
static const BAM16 BAM16_45_DEGREES  = BAM16_90_DEGREES/2;
static const BAM16 BAM16_30_DEGREES  = 0x8003/6;  //BAM16_90_DEGREES/3+0.5

#define COSINE_TABLE_SIZE 256

/* Functions *****************************************************************/
#define DEG2BAM16(X)     ((BAM16)(((X)*0x10000)/360))
#define BAM8toBAM16(X)   ((BAM16)((X)<<8))
#define BAM16toBAM8(X)   ((BAM8)((X)>>8))
//#define BAM16toBAM32(X)  ((BAM32)((X)<<16))
//#define BAM32toBAM16(X)  ((BAM16)((X)>>16))
#define Q15_mult(A,B) ((((int32_t)(A))*((int32_t)(B)))>>15)

#define FREQUENCY_BAM16_PER_SAMPLE( HZ, SAMPLE_RATE ) ((BAM16)(((uint32_t)(HZ) << 16)/(SAMPLE_RATE)))

// BAM16 Quadrant tests
#define BAM16_Quad4(X) (0xC000U==((X)&0xC000U))
#define BAM16_Quad3(X) (0x8000U==((X)&0xC000U))
#define BAM16_Quad2(X) (0x4000U==((X)&0xC000U))
#define BAM16_Quad1(X) (0x0000U==((X)&0xC000U))

#define BAM16_Quad34(X) ((X)&0x8000U)
#define BAM16_Quad24(X) ((X)&0x4000U)
#define BAM16_Quad23(X) BAM16_Quad34((X)+0x4000U)
#define BAM16_Quad14(X) BAM16_Quad34((X)-0x4000U)
#define BAM16_Quad13(X) (!BAM16_Quad24(X))
#define BAM16_Quad12(X) (!BAM16_Quad34(X))


/**
 * [Description]
 *
 * @param
 * @return
 */
int16_t add_sat ( int16_t a, int16_t b );
int16_t sub_sat ( int16_t a, int16_t b );
//int32_t Q15_mult ( int32_t a, int32_t b );
int32_t UQ16_mult ( int32_t a, int32_t b );
Q16_15 Q15_mac ( Q_15* a, Q_15* b , size_t count);
UQ16_16 UQ16_mac ( UQ_16* a, UQ_16* b , size_t count);
Q15_DIVMOD_t Q15_divmod ( Q16_15 a, Q_15 b);


Q_15 cosine_table( BAM8 x );


// CORDIC
Complex16 CORDIC16_rotate( BAM16 angle, Complex16 vector );
Complex16 CORDIC16_polar2rect( Polar16 vector );
Polar16 CORDIC16_rect2polar( Complex16 vector );

//SINCOS16_t CORDIC16_sincos( BAM16 angle );
#define CORDIC16_sincos(A) (CORDIC16_rotate( A, {Q15_ONE, 0}))

// Frequenct Detection and Fourier Transforms
Q16_15 powerMeasurement_inphase( const Q_15* src, BAM16 freq, BAM16 phase, int N);
Q16_15 powerMeasurement_magnitude( const Q_15* src, BAM16 freq, int N);

void FFT_inphase( Q_15* dst, const Q_15* src, int order, BAM8 phase );
void FFT_magnitude( Q_15* dst, const Q_15* src, int order );

void Complex_FFT( Complex16* dst, Complex16* src, int order );
void Complex_IFT( Complex16* dst, Complex16* src, int order );

void Real2Complex_FFT( Complex16* dst, Q_15* src, int order );
void Complex2Real_IFT( Q_15* dst, Complex16* src, int order );


// Filters
// LPF
// HPF
// BPF

// Define Filter
// Apply Filter 




/*****************************************************************************/
#ifdef  __cplusplus
}
#endif

#endif // DSP_H
