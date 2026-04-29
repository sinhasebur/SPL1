CRYPTOOL is a cryptography tool based on openSSL. This is an educational project where modes of operation is the focus. 

TO USE THIS
1. Download the code
2. "make" to compile everything

FEATURES 
1. ENCRYPTION AND DECRYPTION
This uses modes of operations like ECB (Electronic Cookbook) , CBC (Cipher Block Chaining), CFB (Cipher Feedback), OFB (Output Feedback) and Counter. It also has AES-GCM (Galois Counter Mode) for authentication.
2. Authentication
AES-GCM returns tag which can be used for authentication
3. ATTACKS
It can simlulate Pattern Leak of ECB, weakness of 2DES by Meet-in-the-Middle Attack and weakness of AES by Known-Plaintext Attack 

and much more like S-BOX generation of AES, randomizing key 


USING THE CLI
The CLI mirror openSSL cli style 

./cryptool (type) -flag value

Types are 
1. enc (encrypt)
2. dec (decrypt)
3. break (any attack)
4. inspect
5. bench (benchmark to see times of all combinations in a csv file)

Flags are 
1. -in (input file)
2. -out (output filename)
3. -algo (algorithm used) This may be DES, 2DES, 3DES or AES
4. -mode (modes of operation) This may be ecb, cbc, cfb, ofb, counter or gcm 
5. -key (key for encryption or decryption) A hex key can be directly given, "random" for random key generation or random20 for random key of 20 keyspace
6. -iv (Initialization vector) A hex key can be directly given or "random" for random key generation
7. -show (used to show internal data) AES-sboxes to generate sbox and inverse sbox , firstBlock to see the first few bytes of a file
8. -knowPT (known plaintext) a hex data is sent for known plaintext attacks 


References:
1. FIPS 197 Specifications for the Advanced Encryption Standard by National Institute of Standards and Technology
2. The Galois/Counter Mode of Operation (GCM) by David A. McGrew and John Viega. 
