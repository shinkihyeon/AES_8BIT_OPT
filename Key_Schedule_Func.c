#include "main.h"
#include "OP_Table.h"

//키스케줄 과정에서 쓰이는 상수값
const word Rcon[10] = { 0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000,0x1b000000,0x36000000 };

word RotWord(word input) {
	return ((input << 8) | (input >> 24));
}

word SubWord(word input) {
	return (((sbox[input >> 24]) << 24) ^ ((sbox[(input >> 16) & 0xff]) << 16) ^ ((sbox[(input >> 8) & 0xff]) << 8) ^ (sbox[input & 0xff]));
}

word KeySchedule_F(word input, int rn) {
	return ((SubWord(RotWord(input))) ^ (Rcon[rn]));
}

//Enc과정에서 쓰이는 KeySchedule 함수
void AES_KeySchedule(byte* key, word rk[11][4]) {
	int cnt_i, cnt_j;
	rk[0][0] = GETU32(key + (4 * 0));
	rk[0][1] = GETU32(key + (4 * 1));
	rk[0][2] = GETU32(key + (4 * 2));
	rk[0][3] = GETU32(key + (4 * 3));
	//1ROUND~10ROUND
	for (cnt_i = 1; cnt_i < 11; cnt_i++) {
		for (cnt_j = 0; cnt_j < 4; cnt_j++) {
			if (cnt_j == 0) {
				rk[cnt_i][cnt_j] = (KeySchedule_F(rk[cnt_i - 1][3], (cnt_i - 1)) ^ rk[cnt_i - 1][0]);
			}
			else {
				rk[cnt_i][cnt_j] = (rk[cnt_i][cnt_j - 1] ^ rk[cnt_i - 1][cnt_j]);
			}
		}
	}
}

//EQ_DEC
//AdRoundKey, MixColumns과정의 순서를 바꾸기 위해 새로운 RoundKey를 기존의 RoundKey를 활용해서 
//새롭게 생성하는 과정
void dec_key_schedule(byte* mk, byte rk[11][16]) {
	word word_rk[11][4];
	AES_KeySchedule(mk, word_rk);

	//rk data형변환 -> 4word * 11 -> (1byte*16) * 11
	byte dec_rk[11][16] = { 0, };
	int2byte(word_rk[0], dec_rk[0]);
	int2byte(word_rk[1], dec_rk[1]);
	int2byte(word_rk[2], dec_rk[2]);
	int2byte(word_rk[3], dec_rk[3]);
	int2byte(word_rk[4], dec_rk[4]);
	int2byte(word_rk[5], dec_rk[5]);
	int2byte(word_rk[6], dec_rk[6]);
	int2byte(word_rk[7], dec_rk[7]);
	int2byte(word_rk[8], dec_rk[8]);
	int2byte(word_rk[9], dec_rk[9]);
	int2byte(word_rk[10], dec_rk[10]);

	//0R,10R 새로운 RoundKey는 기존의 RoundKey와 동일
	memcpy(rk[0], dec_rk[0], 16 * sizeof(byte));
	memcpy(rk[10], dec_rk[10], 16 * sizeof(byte));

	//1R~9R RK -> RK'(새로운 Round Key)
	//복호화를 위해 사전계산한 TD 0e, 0b, 0d, 0e를 가지고 생성
	int i;
	for (i = 1; i < 10; i++) {
		rk[i][0] = TD0e[sbox[dec_rk[i][0]]] ^ TD0b[sbox[dec_rk[i][1]]] ^ TD0d[sbox[dec_rk[i][2]]] ^ TD09[sbox[dec_rk[i][3]]];
		rk[i][1] = TD09[sbox[dec_rk[i][0]]] ^ TD0e[sbox[dec_rk[i][1]]] ^ TD0b[sbox[dec_rk[i][2]]] ^ TD0d[sbox[dec_rk[i][3]]];
		rk[i][2] = TD0d[sbox[dec_rk[i][0]]] ^ TD09[sbox[dec_rk[i][1]]] ^ TD0e[sbox[dec_rk[i][2]]] ^ TD0b[sbox[dec_rk[i][3]]];
		rk[i][3] = TD0b[sbox[dec_rk[i][0]]] ^ TD0d[sbox[dec_rk[i][1]]] ^ TD09[sbox[dec_rk[i][2]]] ^ TD0e[sbox[dec_rk[i][3]]];

		rk[i][4] = TD0e[sbox[dec_rk[i][4]]] ^ TD0b[sbox[dec_rk[i][5]]] ^ TD0d[sbox[dec_rk[i][6]]] ^ TD09[sbox[dec_rk[i][7]]];
		rk[i][5] = TD09[sbox[dec_rk[i][4]]] ^ TD0e[sbox[dec_rk[i][5]]] ^ TD0b[sbox[dec_rk[i][6]]] ^ TD0d[sbox[dec_rk[i][7]]];
		rk[i][6] = TD0d[sbox[dec_rk[i][4]]] ^ TD09[sbox[dec_rk[i][5]]] ^ TD0e[sbox[dec_rk[i][6]]] ^ TD0b[sbox[dec_rk[i][7]]];
		rk[i][7] = TD0b[sbox[dec_rk[i][4]]] ^ TD0d[sbox[dec_rk[i][5]]] ^ TD09[sbox[dec_rk[i][6]]] ^ TD0e[sbox[dec_rk[i][7]]];

		rk[i][8] = TD0e[sbox[dec_rk[i][8]]] ^ TD0b[sbox[dec_rk[i][9]]] ^ TD0d[sbox[dec_rk[i][10]]] ^ TD09[sbox[dec_rk[i][11]]];
		rk[i][9] = TD09[sbox[dec_rk[i][8]]] ^ TD0e[sbox[dec_rk[i][9]]] ^ TD0b[sbox[dec_rk[i][10]]] ^ TD0d[sbox[dec_rk[i][11]]];
		rk[i][10] = TD0d[sbox[dec_rk[i][8]]] ^ TD09[sbox[dec_rk[i][9]]] ^ TD0e[sbox[dec_rk[i][10]]] ^ TD0b[sbox[dec_rk[i][11]]];
		rk[i][11] = TD0b[sbox[dec_rk[i][8]]] ^ TD0d[sbox[dec_rk[i][9]]] ^ TD09[sbox[dec_rk[i][10]]] ^ TD0e[sbox[dec_rk[i][11]]];

		rk[i][12] = TD0e[sbox[dec_rk[i][12]]] ^ TD0b[sbox[dec_rk[i][13]]] ^ TD0d[sbox[dec_rk[i][14]]] ^ TD09[sbox[dec_rk[i][15]]];
		rk[i][13] = TD09[sbox[dec_rk[i][12]]] ^ TD0e[sbox[dec_rk[i][13]]] ^ TD0b[sbox[dec_rk[i][14]]] ^ TD0d[sbox[dec_rk[i][15]]];
		rk[i][14] = TD0d[sbox[dec_rk[i][12]]] ^ TD09[sbox[dec_rk[i][13]]] ^ TD0e[sbox[dec_rk[i][14]]] ^ TD0b[sbox[dec_rk[i][15]]];
		rk[i][15] = TD0b[sbox[dec_rk[i][12]]] ^ TD0d[sbox[dec_rk[i][13]]] ^ TD09[sbox[dec_rk[i][14]]] ^ TD0e[sbox[dec_rk[i][15]]];
	}
}
