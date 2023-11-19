#include "main.h"
#include "OP_Table.h"

//연산테이블 TE, TD를 생성하는 함수

void gen_dec_table() {
	//inv_sbox -> *09 in GF(2^8)
	int i;
	byte TD09[256] = { 0, };
	for (i = 0; i < 256; i++) {
		byte x = isbox[i];
		byte x02 = GF_xtime(x);
		byte x04 = GF_xtime(x02);
		byte x08 = GF_xtime(x04);
		byte x09 = x08 ^ x;
		TD09[i] = x09;
	}
	//inv_sbox -> *0b in GF(2^8)
	byte TD0b[256] = { 0, };
	for (i = 0; i < 256; i++) {
		byte x = isbox[i];
		byte x02 = GF_xtime(x);
		byte x04 = GF_xtime(x02);
		byte x08 = GF_xtime(x04);
		byte x0b = x08 ^ x02 ^ x;
		TD0b[i] = x0b;
	}
	//inv_sbox -> *0d in GF(2^8)
	byte TD0d[256] = { 0, };
	for (i = 0; i < 256; i++) {
		byte x = isbox[i];
		byte x02 = GF_xtime(x);
		byte x04 = GF_xtime(x02);
		byte x08 = GF_xtime(x04);
		byte x0d = x08 ^ x04 ^ x;
		TD0d[i] = x0d;
	}
	//inv_sbox -> *0e in GF(2^8)
	byte TD0e[256] = { 0, };
	for (i = 0; i < 256; i++) {
		byte x = isbox[i];
		byte x02 = GF_xtime(x);
		byte x04 = GF_xtime(x02);
		byte x08 = GF_xtime(x04);
		byte x0e = x08 ^ x04 ^ x02;
		TD0e[i] = x0e;
	}
	//출력
	for (i = 0; i < 256; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", TD09[i]);
	}
	printf("\n");
	printf("\n");
	for (i = 0; i < 256; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", TD0b[i]);
	}
	printf("\n");
	printf("\n");
	for (i = 0; i < 256; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", TD0d[i]);
	}
	printf("\n");
	printf("\n");
	for (i = 0; i < 256; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", TD0e[i]);
	}
}

void gen_enc_table() {
	//sbox -> *02 in GF(2^8)
	int i;
	byte data1[256] = { 0, };
	for (i = 0; i < 256; i++) {
		byte x = sbox[i];
		x = GF_xtime(x);
		data1[i] = x;
	}
	//sbox -> *03 in GF(2^8)
	byte data2[256] = { 0, };
	for (i = 0; i < 256; i++) {
		byte x = sbox[i];
		x = GF_xtime(x) ^ x;
		data2[i] = x;
	}
	//출력
	for (i = 0; i < 256; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", data1[i]);
	}
	printf("\n");
	printf("\n");
	for (i = 0; i < 256; i++) {
		if (i != 0 && i % 16 == 0) {
			printf("\n");
		}
		printf("0x%02x, ", data2[i]);
	}
}

