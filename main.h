#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char byte;
typedef unsigned int word;



void CT_printstate(byte* data);
void DEPT_printstate(byte* data);

void gen_dec_table();
void gen_enc_table();

byte GF_xtime(byte gf);
byte GF_mul(byte op1, byte op2);
byte GF_add(byte op1, byte op2);

word RotWord(word input);
word SubWord(word input);
word KeySchedule_F(word input, int rn);
void AES_KeySchedule(byte* key, word rk[11][4]);
void dec_key_schedule(byte* mk, byte rk[11][16]);

void AddRoundKey(byte* input, word* rk);
void ENC_ROUND(byte* input, word* rk);
void Last_Round(byte* input, word* rk);
void AES8_ENC(byte* pt, byte* mk, byte* ct);

void Dec_AddRoundKey(byte* input, byte* rk);
void DEC_ROUND(byte* input, byte* rk);
void Last_Dec_Round(byte* input, byte* rk);
void AES8_DEC(byte* ct, byte* mk, byte* pt);

word GETU32(byte* b);
void int2byte(word* input, byte* output);