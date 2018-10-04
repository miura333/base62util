// test.cpp : ÉRÉìÉ\Å[Éã ÉAÉvÉäÉPÅ[ÉVÉáÉìÇÃÉGÉìÉgÉä É|ÉCÉìÉgÇíËã`ÇµÇ‹Ç∑ÅB
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "base62util.h"

#define MOVE_2_RIGHT	1
#define MOVE_2_LEFT		2
#define DATA_INCREMENT	3
#define DATA_DECREMENT	4

//const char code[] = {'a', 'b', 'c', 'd', 'e', 'J', 'K', 'L', 'M', 'N',
//                     'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
//                     'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
//                     'E', 'F', 'G', 'H', 'I', 'f', 'g', 'h', 'i', 'j',
//                     'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
//                     '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
//					 'Y', 'Z'};

base62util::base62util()
{
	srand((unsigned)time(NULL));
	
	memset(code, 0x00, CODE_SIZE);
}

base62util::~base62util()
{
}

char *base62util::getExpireDate(unsigned char *decryptedData, size_t dataSize)
{
	size_t dst_size = 9;
	char *dst = new char[dst_size];
	memset(dst, 0x00, dst_size);
	
	int i = 0, j = 0;
	for(i = 8; i > 0; i--){
		int a1 = getcode(decryptedData[dataSize - i]);
		char tmp1[2] = {0};
		sprintf(tmp1, "%d", a1);
		dst[j] = tmp1[0];
		j++;
	}
	
	return dst;
}

int base62util::getcode(char c)
{
	int size = sizeof(code) / sizeof(code[0]);

	int i = 0;
	for(i = 0; i < size; i++){
		if(code[i] == c)
			return i;
	}

	return -1;
}

unsigned char *base62util::incrementData2(unsigned char *src, int offset1, int offset2, int opt)
{
	int size = sizeof(code) / sizeof(code[0]);
	
	unsigned char *pBuf = new unsigned char[DATA_SIZE];
	memset(pBuf, 0x00, DATA_SIZE);
	
	int i = 0;
	for(i = 0; i < DATA_SIZE; i++){
		int index = 0;
		
		int offset = offset1;
		if((i % 2) != 0) offset = offset2;
		
		if(opt == DATA_INCREMENT){
			int tmp = getcode(src[i]) + offset;
			if(tmp >= size){
				index = tmp - size;
			}else{
				index = tmp;
			}
		}else{
			int tmp = getcode(src[i]) - offset;
			if(tmp < 0){
				index = size + tmp;
			}else{
				index = tmp;
			}
		}
		
		//if(index >= size){	//buffer overrun check
		//	int y = 1;
		//}
		
		pBuf[i] = code[index];
	}
	
	return pBuf;
}

unsigned char *base62util::incrementData(unsigned char *src, int offset, int opt)
{
	int size = sizeof(code) / sizeof(code[0]);

	unsigned char *pBuf = new unsigned char[DATA_SIZE];
	memset(pBuf, 0x00, DATA_SIZE);

	int i = 0;
	for(i = 0; i < DATA_SIZE; i++){
		int index = 0;

		if(opt == DATA_INCREMENT){
			int tmp = getcode(src[i]) + offset;
			if(tmp >= size){
				index = tmp - size;
			}else{
				index = tmp;
			}
		}else{
			int tmp = getcode(src[i]) - offset;
			if(tmp < 0){
				index = size + tmp;
			}else{
				index = tmp;
			}
		}

		//if(index >= size){	//buffer overrun check
		//	int y = 1;
		//}
		
		pBuf[i] = code[index];
	}

	return pBuf;
}

unsigned char *base62util::moveData(unsigned char *src, int offset, int opt)
{
	unsigned char *pBuf = new unsigned char[DATA_SIZE];
	memset(pBuf, 0x00, DATA_SIZE);
	int i = 0, curPos = offset;
	if(opt == MOVE_2_LEFT){
		curPos = DATA_SIZE - offset;
		if(offset == 0)
			curPos = 0;
	}

	for(i = 0; i < DATA_SIZE; i++){
		pBuf[i] = src[curPos];
		
		curPos++;

		if(curPos > DATA_SIZE){
			int x = 1;
		}

		if(curPos == DATA_SIZE)
			curPos = 0;
		
	}

	return pBuf;
}

unsigned char *base62util::encrypt(unsigned char *planeData, unsigned char *key)
{
	int encryptedSize = DATA_SIZE + KEY_SIZE;
	unsigned char *pBuf = new unsigned char[encryptedSize];
	memset(pBuf, 0x00, encryptedSize);

	unsigned char *b = incrementData2(planeData, getcode((char)key[0]), getcode((char)key[1]), DATA_INCREMENT);
	unsigned char *d = incrementData2(b, getcode((char)key[2]), getcode((char)key[3]), DATA_DECREMENT);
	//unsigned char *a = moveData(planeData, getcode((char)key[0]), MOVE_2_RIGHT);
	//unsigned char *b = incrementData(planeData, getcode((char)key[1]), DATA_INCREMENT);
	//unsigned char *c = moveData(b, getcode((char)key[2]), MOVE_2_LEFT);
	//unsigned char *d = incrementData(b, getcode((char)key[3]), DATA_DECREMENT);

	memcpy(pBuf, d, DATA_SIZE);
	memcpy(&pBuf[DATA_SIZE], key, KEY_SIZE);

	//delete [] a;
	delete [] b;
	//delete [] c;
	delete [] d;

	return pBuf;
}

unsigned char *base62util::decrypt(unsigned char *encryptedData)
{
	unsigned char key[KEY_SIZE] = {0};
	memcpy(key, &encryptedData[DATA_SIZE], KEY_SIZE);

	unsigned char buf[DATA_SIZE] = {0};
	memcpy(buf, encryptedData, DATA_SIZE);

	unsigned char *a = incrementData2(buf, getcode((char)key[2]), getcode((char)key[3]), DATA_INCREMENT);
	unsigned char *c = incrementData2(a, getcode((char)key[0]), getcode((char)key[1]), DATA_DECREMENT);
	//unsigned char *a = incrementData(buf, getcode((char)key[3]), DATA_INCREMENT);
	//unsigned char *b = moveData(a, getcode((char)key[2]), MOVE_2_RIGHT);
	//unsigned char *c = incrementData(a, getcode((char)key[1]), DATA_DECREMENT);
	//unsigned char *d = moveData(c, getcode((char)key[0]), MOVE_2_LEFT);

	delete [] a;
	//delete [] b;
	//delete [] c;

	return c;

}

char base62util::makeRandomData2()
{
	int i = 0;
	for(i = 0; i < 255; i++){
		unsigned char a = rand() % 127;
		if((0x30 <= a && a <= 0x39) || (0x41 <= a && a <= 0x5a) || (0x61 <= a && a <= 0x7a)){
			return a;
		}
	}

	return 0;
}

int base62util::makeRandomData(unsigned char *pBuf, size_t size)
{
	unsigned int i = 0;
	for(i = 0; i < size; i++){
		pBuf[i] = makeRandomData2();
	}

	return 1;
}

int base62util::makeRandomNumTable(unsigned char *pBuf)
{
	int curPos = 0;
	while(1){
		char c = makeRandomData2();

		//èdï°ÇµÇƒÇΩÇÁÇ‚ÇËíºÇµ
		int i = 0, hit = 0;
		for(i = 0; i < curPos; i++){
			if(pBuf[i] == c)
				hit = 1;
		}
		if(hit != 0)
			continue;

		pBuf[curPos] = c;
		curPos++;

		if(curPos == CODE_SIZE)
			break;
	}

	return 1;
}

int base62util::makeRandomNumTable()
{
	return makeRandomNumTable((unsigned char *)code);
}


/*
void test()
{
	char szDisplay[MAX_PATH] = {0};

	//ÉâÉìÉ_ÉÄílçÏê¨
	unsigned char buf[DATA_SIZE] = {0};
	makeRandomData(buf, DATA_SIZE - 8);


	//expire date
	char expireDate[8] = {0};
	expireDate[0] = code[2];
	expireDate[1] = code[0];
	expireDate[2] = code[0];
	expireDate[3] = code[8];
	expireDate[4] = code[0];
	expireDate[5] = code[3];
	expireDate[6] = code[0];
	expireDate[7] = code[5];
	//char expireDate[] = "20080305";
	memcpy(&buf[DATA_SIZE - 8], expireDate, 8);

	memset(szDisplay, 0x00, MAX_PATH);
	memcpy(szDisplay, buf, DATA_SIZE);
	printf("plane = %s\n", szDisplay);

	//make key
	unsigned char key[KEY_SIZE] = {0};
	makeRandomData(key, KEY_SIZE);

	memset(szDisplay, 0x00, MAX_PATH);
	memcpy(szDisplay, key, KEY_SIZE);
	printf("key = %s\n", szDisplay);

	//unsigned char *a = moveData(buf, getcode((char)key[0]), MOVE_2_RIGHT);
	//unsigned char *b = moveData(a, getcode((char)key[0]), MOVE_2_LEFT);

	//encrypt
	unsigned char *a = encrypt(buf, key);

	memset(szDisplay, 0x00, MAX_PATH);
	memcpy(szDisplay, a, DATA_SIZE + 4);
	printf("encrypted = %s\n", szDisplay);

	//decrypt
	unsigned char *b = decrypt(a);

	memset(szDisplay, 0x00, MAX_PATH);
	memcpy(szDisplay, b, DATA_SIZE);
	printf("decrypted = %s\n\n", szDisplay);

	delete [] a;
	delete [] b;
}
*/
/*
int _tmain(int argc, _TCHAR* argv[])
{
	_wsetlocale(LC_ALL, L"Japanese");

	srand((unsigned)time(NULL));

	//óêêîï\çÏê¨
	makeRandomNumTable((unsigned char *)code);

	char szDisplay[MAX_PATH] = {0};
	memcpy(szDisplay, code, DATA_SIZE);
	printf("code = %s\n", szDisplay);

	for(int i = 0; i < 5; i++){
		test();
	}
	
	
	return 0;
}
*/
