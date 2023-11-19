#include "main.h"

//(1byte * 4) 배열 -> word 
word GETU32(byte* b) {
	return (((*(b + 0)) << 24) ^ ((*(b + 1)) << 16) ^ ((*(b + 2)) << 8) ^ (*(b + 3)));
}

//4 * word 형태의 배열 -> 16 * byte 형태의 배열
void int2byte(word* input, byte* output) {
	*(output + 0) = *(input + 0) >> 24;
	*(output + 1) = ((*(input + 0)) >> 16) & 0xff;
	*(output + 2) = ((*(input + 0)) >> 8) & 0xff;
	*(output + 3) = (*(input + 0)) & 0xff;

	*(output + 4) = (*(input + 1)) >> 24;
	*(output + 5) = ((*(input + 1)) >> 16) & 0xff;
	*(output + 6) = ((*(input + 1)) >> 8) & 0xff;
	*(output + 7) = (*(input + 1)) & 0xff;

	*(output + 8) = (*(input + 2)) >> 24;
	*(output + 9) = ((*(input + 2)) >> 16) & 0xff;
	*(output + 10) = ((*(input + 2)) >> 8) & 0xff;
	*(output + 11) = (*(input + 2)) & 0xff;

	*(output + 12) = (*(input + 3)) >> 24;
	*(output + 13) = ((*(input + 3)) >> 16) & 0xff;
	*(output + 14) = ((*(input + 3)) >> 8) & 0xff;
	*(output + 15) = (*(input + 3)) & 0xff;
}