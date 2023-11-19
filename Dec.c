#include "main.h"
#include "OP_Table.h"

//기존 AddRoundKey는 rk의 형태가 word * 4 라서 따로 만들어줌
void Dec_AddRoundKey(byte* input, byte* rk) {
	*(input + 0) ^= *(rk + 0);
	*(input + 1) ^= *(rk + 1);
	*(input + 2) ^= *(rk + 2);
	*(input + 3) ^= *(rk + 3);

	*(input + 4) ^= *(rk + 4);
	*(input + 5) ^= *(rk + 5);
	*(input + 6) ^= *(rk + 6);
	*(input + 7) ^= *(rk + 7);

	*(input + 8) ^= *(rk + 8);
	*(input + 9) ^= *(rk + 9);
	*(input + 10) ^= *(rk + 10);
	*(input + 11) ^= *(rk + 11);

	*(input + 12) ^= *(rk + 12);
	*(input + 13) ^= *(rk + 13);
	*(input + 14) ^= *(rk + 14);
	*(input + 15) ^= *(rk + 15);
}

//1R~9R 복호화 과정을 TD 테이블값들을 이용해서 연산 진행
void DEC_ROUND(byte* input, byte* rk) {
	byte temp[16] = { 0, };
	memcpy(temp, input, 16 * sizeof(byte));
	*(input + 0) = TD0e[*(temp + 0)] ^ TD0b[*(temp + 13)] ^ TD0d[*(temp + 10)] ^ TD09[*(temp + 7)] ^ *(rk + 0);
	*(input + 1) = TD09[*(temp + 0)] ^ TD0e[*(temp + 13)] ^ TD0b[*(temp + 10)] ^ TD0d[*(temp + 7)] ^ *(rk + 1);
	*(input + 2) = TD0d[*(temp + 0)] ^ TD09[*(temp + 13)] ^ TD0e[*(temp + 10)] ^ TD0b[*(temp + 7)] ^ *(rk + 2);
	*(input + 3) = TD0b[*(temp + 0)] ^ TD0d[*(temp + 13)] ^ TD09[*(temp + 10)] ^ TD0e[*(temp + 7)] ^ *(rk + 3);

	*(input + 4) = TD0e[*(temp + 4)] ^ TD0b[*(temp + 1)] ^ TD0d[*(temp + 14)] ^ TD09[*(temp + 11)] ^ *(rk + 4);
	*(input + 5) = TD09[*(temp + 4)] ^ TD0e[*(temp + 1)] ^ TD0b[*(temp + 14)] ^ TD0d[*(temp + 11)] ^ *(rk + 5);
	*(input + 6) = TD0d[*(temp + 4)] ^ TD09[*(temp + 1)] ^ TD0e[*(temp + 14)] ^ TD0b[*(temp + 11)] ^ *(rk + 6);
	*(input + 7) = TD0b[*(temp + 4)] ^ TD0d[*(temp + 1)] ^ TD09[*(temp + 14)] ^ TD0e[*(temp + 11)] ^ *(rk + 7);

	*(input + 8) = TD0e[*(temp + 8)] ^ TD0b[*(temp + 5)] ^ TD0d[*(temp + 2)] ^ TD09[*(temp + 15)] ^ *(rk + 8);
	*(input + 9) = TD09[*(temp + 8)] ^ TD0e[*(temp + 5)] ^ TD0b[*(temp + 2)] ^ TD0d[*(temp + 15)] ^ *(rk + 9);
	*(input + 10) = TD0d[*(temp + 8)] ^ TD09[*(temp + 5)] ^ TD0e[*(temp + 2)] ^ TD0b[*(temp + 15)] ^ *(rk + 10);
	*(input + 11) = TD0b[*(temp + 8)] ^ TD0d[*(temp + 5)] ^ TD09[*(temp + 2)] ^ TD0e[*(temp + 15)] ^ *(rk + 11);

	*(input + 12) = TD0e[*(temp + 12)] ^ TD0b[*(temp + 9)] ^ TD0d[*(temp + 6)] ^ TD09[*(temp + 3)] ^ *(rk + 12);
	*(input + 13) = TD09[*(temp + 12)] ^ TD0e[*(temp + 9)] ^ TD0b[*(temp + 6)] ^ TD0d[*(temp + 3)] ^ *(rk + 13);
	*(input + 14) = TD0d[*(temp + 12)] ^ TD09[*(temp + 9)] ^ TD0e[*(temp + 6)] ^ TD0b[*(temp + 3)] ^ *(rk + 14);
	*(input + 15) = TD0b[*(temp + 12)] ^ TD0d[*(temp + 9)] ^ TD09[*(temp + 6)] ^ TD0e[*(temp + 3)] ^ *(rk + 15);
}

//AES 마지막 라운드에는 MixColumns 과정이 없어서 따로 처리
void Last_Dec_Round(byte* input, byte* rk) {
	byte temp[16] = { 0 };
	memcpy(temp, input, 16 * sizeof(byte));
	//inv_SB -> inv_SR -> AR
	*(input + 0) = isbox[*(temp + 0)] ^ *(rk + 0);
	*(input + 4) = isbox[*(temp + 4)] ^ *(rk + 4);
	*(input + 8) = isbox[*(temp + 8)] ^ *(rk + 8);
	*(input + 12) = isbox[*(temp + 12)] ^ *(rk + 12);

	*(input + 1) = isbox[*(temp + 13)] ^ *(rk + 1);
	*(input + 5) = isbox[*(temp + 1)] ^ *(rk + 5);
	*(input + 9) = isbox[*(temp + 5)] ^ *(rk + 9);
	*(input + 13) = isbox[*(temp + 9)] ^ *(rk + 13);

	*(input + 2) = isbox[*(temp + 10)] ^ *(rk + 2);
	*(input + 6) = isbox[*(temp + 14)] ^ *(rk + 6);
	*(input + 10) = isbox[*(temp + 2)] ^ *(rk + 10);
	*(input + 14) = isbox[*(temp + 6)] ^ *(rk + 14);

	*(input + 3) = isbox[*(temp + 7)] ^ *(rk + 3);
	*(input + 7) = isbox[*(temp + 11)] ^ *(rk + 7);
	*(input + 11) = isbox[*(temp + 15)] ^ *(rk + 11);
	*(input + 15) = isbox[*(temp + 3)] ^ *(rk + 15);
}

////AES-8bit ver 복호화 함수
void AES8_DEC(byte* ct, byte* mk, byte* pt) {
	byte cp_ct[16] = { 0, };
	memcpy(cp_ct, ct, 16 * sizeof(byte));
	byte rk[11][16];
	dec_key_schedule(mk, rk);
	//0R
	Dec_AddRoundKey(cp_ct, rk[10]);
	//1R~9R
	DEC_ROUND(cp_ct, rk[9]);
	DEC_ROUND(cp_ct, rk[8]);
	DEC_ROUND(cp_ct, rk[7]);
	DEC_ROUND(cp_ct, rk[6]);
	DEC_ROUND(cp_ct, rk[5]);
	DEC_ROUND(cp_ct, rk[4]);
	DEC_ROUND(cp_ct, rk[3]);
	DEC_ROUND(cp_ct, rk[2]);
	DEC_ROUND(cp_ct, rk[1]);
	//10R
	Last_Dec_Round(cp_ct, rk[0]);
	//last section
	memcpy(pt, cp_ct, 16 * sizeof(byte));
}