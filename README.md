# base62util
簡単換字式暗号エンコード・デコードクラス

# How to use

### int base62util::makeRandomNumTable()

換字表作成
換字表がメンバ変数codeに格納される
このcodeを共有することで複数PC間で暗号化、復号化が可能

### unsigned char *base62util::encrypt(unsigned char *planeData, unsigned char *key)

暗号化

### unsigned char *base62util::decrypt(unsigned char *encryptedData)

復号化
