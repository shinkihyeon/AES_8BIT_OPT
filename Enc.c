#include "main.h"
#include "OP_Table.h"

void AddRoundKey(byte* input, word* rk) {
	byte temp[16] = { 0x00, };
	int2byte(rk, temp);

	*(input + 0) ^= *(temp + 0);
	*(input + 1) ^= *(temp + 1);
	*(input + 2) ^= *(temp + 2);
	*(input + 3) ^= *(temp + 3);

	*(input + 4) ^= *(temp + 4);
	*(input + 5) ^= *(temp + 5);
	*(input + 6) ^= *(temp + 6);
	*(input + 7) ^= *(temp + 7);

	*(input + 8) ^= *(temp + 8);
	*(input + 9) ^= *(temp + 9);
	*(input + 10) ^= *(temp + 10);
	*(input + 11) ^= *(temp + 11);

	*(input + 12) ^= *(temp + 12);
	*(input + 13) ^= *(temp + 13);
	*(input + 14) ^= *(temp + 14);
	*(input + 15) ^= *(temp + 15);
}

//1R~9R SB -> SR -> MC 과정을 사전계산한 TABLE(TE02, TE03)을 이용해서 구현
void ENC_ROUND(byte* input, word* rk) {
	byte temp[16] = { 0, };
	memcpy(temp, input, 16 * sizeof(byte));
	byte byte_rk[16] = { 0, };
	int2byte(rk, byte_rk);

	*(input + 0) = TE02[*(temp + 0)] ^ TE03[*(temp + 5)] ^ sbox[*(temp + 10)] ^ sbox[*(temp + 15)] ^ *(byte_rk + 0);
	*(input + 1) = sbox[*(temp + 0)] ^ TE02[*(temp + 5)] ^ TE03[*(temp + 10)] ^ sbox[*(temp + 15)] ^ *(byte_rk + 1);
	*(input + 2) = sbox[*(temp + 0)] ^ sbox[*(temp + 5)] ^ TE02[*(temp + 10)] ^ TE03[*(temp + 15)] ^ *(byte_rk + 2);
	*(input + 3) = TE03[*(temp + 0)] ^ sbox[*(temp + 5)] ^ sbox[*(temp + 10)] ^ TE02[*(temp + 15)] ^ *(byte_rk + 3);

	*(input + 4) = TE02[*(temp + 4)] ^ TE03[*(temp + 9)] ^ sbox[*(temp + 14)] ^ sbox[*(temp + 3)] ^ *(byte_rk + 4);
	*(input + 5) = sbox[*(temp + 4)] ^ TE02[*(temp + 9)] ^ TE03[*(temp + 14)] ^ sbox[*(temp + 3)] ^ *(byte_rk + 5);
	*(input + 6) = sbox[*(temp + 4)] ^ sbox[*(temp + 9)] ^ TE02[*(temp + 14)] ^ TE03[*(temp + 3)] ^ *(byte_rk + 6);
	*(input + 7) = TE03[*(temp + 4)] ^ sbox[*(temp + 9)] ^ sbox[*(temp + 14)] ^ TE02[*(temp + 3)] ^ *(byte_rk + 7);

	*(input + 8) = TE02[*(temp + 8)] ^ TE03[*(temp + 13)] ^ sbox[*(temp + 2)] ^ sbox[*(temp + 7)] ^ *(byte_rk + 8);
	*(input + 9) = sbox[*(temp + 8)] ^ TE02[*(temp + 13)] ^ TE03[*(temp + 2)] ^ sbox[*(temp + 7)] ^ *(byte_rk + 9);
	*(input + 10) = sbox[*(temp + 8)] ^ sbox[*(temp + 13)] ^ TE02[*(temp + 2)] ^ TE03[*(temp + 7)] ^ *(byte_rk + 10);
	*(input + 11) = TE03[*(temp + 8)] ^ sbox[*(temp + 13)] ^ sbox[*(temp + 2)] ^ TE02[*(temp + 7)] ^ *(byte_rk + 11);

	*(input + 12) = TE02[*(temp + 12)] ^ TE03[*(temp + 1)] ^ sbox[*(temp + 6)] ^ sbox[*(temp + 11)] ^ *(byte_rk + 12);
	*(input + 13) = sbox[*(temp + 12)] ^ TE02[*(temp + 1)] ^ TE03[*(temp + 6)] ^ sbox[*(temp + 11)] ^ *(byte_rk + 13);
	*(input + 14) = sbox[*(temp + 12)] ^ sbox[*(temp + 1)] ^ TE02[*(temp + 6)] ^ TE03[*(temp + 11)] ^ *(byte_rk + 14);
	*(input + 15) = TE03[*(temp + 12)] ^ sbox[*(temp + 1)] ^ sbox[*(temp + 6)] ^ TE02[*(temp + 11)] ^ *(byte_rk + 15);
}

//AES의 마지막 라운드는 나머지 라운드들과 다르게 MixColumns과정이 없어서 따로 만듬
//SubBytes -> ShiftRows -> AddRoundKey
void Last_Round(byte* input, word* rk) {
	byte temp[16] = { 0 };
	memcpy(temp, input, 16 * sizeof(byte));
	byte byte_rk[16] = { 0, };
	int2byte(rk, byte_rk);

	//SB -> SR -> AR
	*(input + 0) = sbox[*(temp + 0)] ^ *(byte_rk + 0);
	*(input + 4) = sbox[*(temp + 4)] ^ *(byte_rk + 4);
	*(input + 8) = sbox[*(temp + 8)] ^ *(byte_rk + 8);
	*(input + 12) = sbox[*(temp + 12)] ^ *(byte_rk + 12);

	*(input + 1) = sbox[*(temp + 5)] ^ *(byte_rk + 1);
	*(input + 5) = sbox[*(temp + 9)] ^ *(byte_rk + 5);
	*(input + 9) = sbox[*(temp + 13)] ^ *(byte_rk + 9);
	*(input + 13) = sbox[*(temp + 1)] ^ *(byte_rk + 13);

	*(input + 2) = sbox[*(temp + 10)] ^ *(byte_rk + 2);
	*(input + 6) = sbox[*(temp + 14)] ^ *(byte_rk + 6);
	*(input + 10) = sbox[*(temp + 2)] ^ *(byte_rk + 10);
	*(input + 14) = sbox[*(temp + 6)] ^ *(byte_rk + 14);

	*(input + 3) = sbox[*(temp + 15)] ^ *(byte_rk + 3);
	*(input + 7) = sbox[*(temp + 3)] ^ *(byte_rk + 7);
	*(input + 11) = sbox[*(temp + 7)] ^ *(byte_rk + 11);
	*(input + 15) = sbox[*(temp + 11)] ^ *(byte_rk + 15);
}

//AES-8bit ver 암호화 함수
void AES8_ENC(byte* pt, byte* mk, byte* ct) {
	byte cp_pt[16] = { 0, };
	memcpy(cp_pt, pt, 16 * sizeof(byte));
	word rk[11][4];
	AES_KeySchedule(mk, rk);
	//0R
	AddRoundKey(cp_pt, rk[0]);
	//1R~9R
	ENC_ROUND(cp_pt, rk[1]);
	ENC_ROUND(cp_pt, rk[2]);
	ENC_ROUND(cp_pt, rk[3]);
	ENC_ROUND(cp_pt, rk[4]);
	ENC_ROUND(cp_pt, rk[5]);
	ENC_ROUND(cp_pt, rk[6]);
	ENC_ROUND(cp_pt, rk[7]);
	ENC_ROUND(cp_pt, rk[8]);
	ENC_ROUND(cp_pt, rk[9]);
	//10R
	Last_Round(cp_pt, rk[10]);
	//last section
	memcpy(ct, cp_pt, 16 * sizeof(byte));
}