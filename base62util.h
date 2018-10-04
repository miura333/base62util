// test.cpp : ÉRÉìÉ\Å[Éã ÉAÉvÉäÉPÅ[ÉVÉáÉìÇÃÉGÉìÉgÉä É|ÉCÉìÉgÇíËã`ÇµÇ‹Ç∑ÅB
//

#ifndef _BASE62_UTIL_H_
#define _BASE62_UTIL_H_

#define DATA_SIZE	20
#define CODE_SIZE	62
#define KEY_SIZE	4

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

class base62util
{
public:
	char code[CODE_SIZE];
	
	base62util();
	~base62util();
	
	char *getExpireDate(unsigned char *decryptedData, size_t dataSize);
	
	int makeRandomNumTable();
	int makeRandomData(unsigned char *pBuf, size_t size);
	char makeRandomData2();
	unsigned char *decrypt(unsigned char *encryptedData);
	unsigned char *encrypt(unsigned char *planeData, unsigned char *key);
	unsigned char *moveData(unsigned char *src, int offset, int opt);
	unsigned char *incrementData(unsigned char *src, int offset, int opt);
	int getcode(char c);
	
	unsigned char *incrementData2(unsigned char *src, int offset1, int offset2, int opt);

	int makeRandomNumTable(unsigned char *pBuf);
protected:
private:
};

//void test();
#endif //_BASE62_UTIL_H_