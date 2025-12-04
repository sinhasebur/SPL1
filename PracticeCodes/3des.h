void _3DES_Encrypt(char* text, int* key1, int* key2, int* key3, char* cipherText);
void _3DES_Decrypt(char* cipherText, int* key1, int* key2,int* key3, char* decryptedText);


void _3DES_Encrypt(char* text, int* key1, int* key2,int* key3, char* cipherText)
{      
    char cipher1[8], cipher[8];

    DES_Encrypt(text, key1, cipher);
    DES_Decrypt(cipher, key2,cipher1 );
    DES_Encrypt(cipher1, key3, cipherText);
}

void _3DES_Decrypt(char* text, int* key1, int* key2, int* key3, char* decipheredText)
{      
    char cipher1[8], cipher[8];
    DES_Decrypt(text, key3, cipher1);
    DES_Encrypt(cipher1, key2, cipher);
    DES_Decrypt(cipher, key1, decipheredText);
}