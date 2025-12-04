void _2desEncrypt(char* text, int* key1, int* key2, char* cipherText);
void _2desDecrypt(char* cipherText, int* key1, int* key2, char* decryptedText);


void _2DES_Encrypt(char* text, int* key1, int* key2, char* cipherText)
{      
    char cipher1[8];

    DES_Encrypt(text, key1, cipher1);

    DES_Encrypt(cipher1, key2, cipherText);
}

void _2DES_Decrypt(char* text, int* key1, int* key2, char* decipheredText)
{      
    char cipher1[8];

    DES_Decrypt(text, key2, cipher1);

    DES_Decrypt(cipher1, key1, decipheredText);
}