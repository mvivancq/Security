#include <stdio.h>
#include "openssl/evp.h"
#include <string.h>

#define MAX_LENGTH 16 // A global variable t define max-length
// compile: gcc project-1.c -lssl -lcrypto
void print_hex(int len, unsigned char *text)
{
	for(int j = 0; j < len; j++)
	{
		/* Use a loop to print out the buffer */
		printf("%x",text[j] );
	}
}
int main (int argc, const char *argv[])
{
	/* This variable is used to store the ciphertext provided to you. Make sure to use the proper format, eg. the hex 'd23a' would be written as {0xd2,0x3a} in C */
	unsigned char ciphertxt[] = {0x20,0x75,0x38,0x6b,0x75,0xee,0xd8,0xb4,0xf2,0xb4,0xa9,0xc9,0xb7,0x69,0x67,0xd0,0x57,0xb4,0xa4,0x41,0xd3,0x49,0xc1,0x5d,0xd4,0xb8,0xbf,0x4b,0x87,0x44,0x5a,0x9e};

	unsigned char plaintext[] = "This is a top secret";

	// Pointer fp used to point to the English words file provided along.
	FILE *fp;

	// The initialization Vector is set to 0
	unsigned char iv[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	// Output buffer set to 1024 size
	unsigned char outbuf [1024];

	// Some other variables that are used in the program
	int outlen = 0, tmplen = 0, flag = 0;
	/* You may want to declare some additional variables to be used as flags*/

	EVP_CIPHER_CTX *ctx;
	//ctx = EVP_CIPHER_CTX_new();
	if ((fp=fopen("words.txt", "r")) == NULL)
	{
		printf("File not found\n");
		return 0;
	}

	unsigned char key [MAX_LENGTH]; //array to store the key getting from the dictionary

	while (fgets(key, MAX_LENGTH, fp) != NULL)
	{
		for(int i = 0; i < MAX_LENGTH; i++)
		{
			if (key[i] == '\n' || key[i] == 0 ) //when key is less than 16 characters, the extra left part is filled with " "
			{
				for (int j = i; j < strlen(key); j++)
				{
					key[j] = ' ';
				}
				break;
			}
		}
		//printf("%s\n", key);
		// Use the EVP library to initialize cipher
		ctx = EVP_CIPHER_CTX_new();

		// Use the EVP library to encrypt the cipher
		EVP_EncryptInit_ex (ctx, EVP_aes_128_cbc(), NULL, key, iv);

		// Checking to see if EVP_EncryptUpdate is valid or not
		if (!EVP_EncryptUpdate (ctx, outbuf, &outlen, plaintext, strlen(plaintext)))
		{
			/*Print Out a relevant Error Messege*/
			printf("Error de no se que pedo!!!\n");
			return 0;
		}

		// Buffer passed to EVP_EncryptFinal() must be after data just encrypted to avoid overwriting it

		// Checking to see if !EVP_EncryptFinal_ex is valid or not
		if (!EVP_EncryptFinal_ex (ctx, outbuf + outlen, &tmplen))
		{
			/*Print Out a relevant Error Messege*/
			printf("Error de no se que pedo 2!!!\n");
			return 0;
		}

		outlen += tmplen;

		EVP_CIPHER_CTX_free(ctx);

		//print curret key and its corresponding ciphertext
		printf ("The key is : %s       The Corresponding Cipher Text Is:  ", key);

		for(int j = 0; j < outlen; j++)
		{
			/* Use a loop to print out the buffer */
			printf("%x",outbuf[j] );
		}
		printf ("\n");
		int q;
		for(q = 0; q < outlen; q++)
		{
		/* Judge whether the cipher text generated by this key is match for the provoded one */
		/* As the whole ciphertext cannot be matched at once, use this loop to match it bit by bit */
			if (outbuf[q] != ciphertxt[q]) {
				q = outlen;
			}
		}

		if (q==outlen)/* If the generated ciphertext matched with the one provided*/
		{
			printf ("\n*****************************************************\n");

			/* Print the key used */
			printf("Key: %s\n", key);
			/* Print the text in the buffer */
			printf("Current text hex: ");
			print_hex(outlen, ciphertxt);
			/* Print the provided ciphertext*/
			printf("\nProvided text hex: ");
			print_hex(outlen, outbuf);
			/* Print the length of the ciphertext */
			printf("\nLength of ciphertext: %i\n", outlen);
			printf ("\n*****************************************************\n");
			return 0;
		}

	}
	fclose (fp);
	return 0;
}
