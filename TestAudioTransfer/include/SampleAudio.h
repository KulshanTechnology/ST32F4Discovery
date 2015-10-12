/* This file contains sample audio to be transfered to a PC
 * signal processing script.
 *
 * SampleAudio is a 2 kHz sin wave sampled at 16 kHz at 16 bits for 50 msec.
 */

#define SAMPLEAUDIO_SIZE 800
#define SAMPLEAUDIO_SAMPLERATE 16000
#define SAMPLEAUDIO_RESOLUTION 16
#define SAMPLEAUDIO_INTERVAL_MSEC 50

extern const short SampleAudio[SAMPLEAUDIO_SIZE];

/* Helper Macros for splitting up the Samples */

#define Lo(X) (unsigned char)(X&0x00ff)
#define Hi(X) (unsigned char)((X>>8)&0x00ff)


