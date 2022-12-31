#include <stdio.h>
#include <string.h>

#define MAX_BUF 256

#define IV 0b10110001
#define KEY 0b11001011
#define CTR 0b00110101

void encode(unsigned char *, unsigned char *, int);
void decode(unsigned char *, unsigned char *, int);

unsigned char processCtr(unsigned char, unsigned char);
unsigned char encryptByte(unsigned char, unsigned char, unsigned char);
unsigned char decryptByte(unsigned char, unsigned char, unsigned char);

unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);

int counter = 0;

int main()
{

    unsigned char ct[MAX_BUF];
    unsigned char pt[MAX_BUF];

    char input[MAX_BUF];
    int size = 0;

    int output[MAX_BUF];
    int count = 0;

    char str[8];
    int choice;

    printf("\nYou may:\n");
    printf("  (1) Encrypt a message \n");
    printf("  (2) Decrypt a message \n");
    printf("  (0) Exit\n");
    printf("\n  what is your selection: ");
    fgets(str, sizeof(str), stdin);
    sscanf(str, "%d", &choice);

    if (choice == 0)
        return 0;

    switch (choice)
    {

    case 1:

        printf("\n What is your input that you want to encrypt: ");
        fgets(input, sizeof(input), stdin);

        for (int z = 0; input[z] != '\0'; z++)
        {
            size = z;
        }

        for (int j = 0; j < size; j++)
        {
            pt[j] = input[j];
        }

        encode(pt, ct, size);

        for (int i = 0; i < size; i++)
        {
            printf("%d ", ct[i]);
        }

        break;

    case 2:
        printf("Enter the decrypted code: ");

        for (int i = 0; i < MAX_BUF; i++)
        {
            scanf("%d", &output[i]);

            if (output[i] == -1)
            {
                break;
            }

            ct[i] = output[i];
            count++;
        }

        decode(pt, ct, count);

        for (int i = 0; i < count; i++)
        {
            printf("%c", pt[i]);
        }
        break;
    }

    return (0);
}

/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n)
{
    int bit;
    bit = (c >> n) & 1;
    return bit;
}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n)
{
    c = c | (1 << n);
    return c;
}

/*  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n)
{
    c = c & (~(1 << n));
    return c;
}

/*  Function:  processCtr
  Purpose:   Process the counter with the key to get the correct encryption algorithm
       in:   the counter; either the CTR defined above or incremented counter
       in:   the key; used to encrypt/decrypt the code
   return:   it returns the processed counter
*/

unsigned char processCtr(unsigned char ctr, unsigned char key)
{
    int tempCtr = ctr;
    int j;

    if (getBit(tempCtr, 0) == 1)
    {
        j = 1;
    }
    else
    {
        j = 0;
    }

    for (int i = j; i < 8; i += 2)
    {
        int spot = (getBit(ctr, i) ^ getBit(key, i));

        if (spot == 0)
        {
            tempCtr = clearBit(tempCtr, i);
        };

        if (spot == 1)
        {
            tempCtr = setBit(tempCtr, i);
        };
    };

    return tempCtr;
}

/*  Function:  encryptByte
  Purpose:   Encrypts each byte
       in:   the plain text byte you are encrypting
       in:   the counter of this specific plain text byte
       in:   the prev cyphered byte that will be used in the encryption process;
   return:   the encrypted plain text byte
*/

unsigned char encryptByte(unsigned char pt, unsigned char ctr, unsigned char prev)
{
    unsigned char tempByte = 0b00000000;

    unsigned char spot;

    for (int i = 0; i < 8; i++)
    {
        if ((getBit(ctr, i)) == 1)
        {
            spot = ((getBit(pt, i)) ^ (getBit(prev, i)));

            if (spot == 1)
            {
                tempByte = setBit(tempByte, i);
            }
            else
            {
                tempByte = clearBit(tempByte, i);
            }
        }
        else
        {
            spot = ((getBit(pt, i)) ^ (getBit(prev, 7 - i)));

            if (spot == 1)
            {
                tempByte = setBit(tempByte, i);
            }
            else
            {
                tempByte = clearBit(tempByte, i);
            }
        }
    }

    return tempByte;
}

/*  Function:  encode
  Purpose:   it takes all the line/paragraph you want to encrypt
       in:   pointer to the array holding the plain text
       in:   pointer to the array holding the cyphered text
       in:   size of plain text you want to encrypt
   return:   void no return value
*/

void encode(unsigned char *pt, unsigned char *ct, int numBytes)
{
    unsigned char ctr = processCtr(CTR, KEY);
    unsigned char prev = IV;

    for (int i = 0; i < numBytes; i++)
    {

        unsigned char spy = encryptByte(pt[i], ctr, prev);

        ct[i] = spy;

        prev = spy;
        ctr++;
        ctr = processCtr(ctr, KEY);
    }
}

/*  Function:  decryptByte
  Purpose:   decrypts each byte
       in:   the plain text byte you are decrypting
       in:   the counter of this specific plain text byte
       in:   the prev cyphered byte that will be used in the encryption process;
   return:   the decrypted plain text byte
*/

unsigned char decryptByte(unsigned char ct, unsigned char ctr, unsigned char prev)
{
    unsigned char tempByte = 0b00000000;

    unsigned char spot;

    for (int i = 0; i < 8; i++)
    {
        if ((getBit(ctr, i)) == 1)
        {
            spot = ((getBit(ct, i)) ^ (getBit(prev, i)));

            if (spot == 1)
            {
                tempByte = setBit(tempByte, i);
            }
            else
            {
                tempByte = clearBit(tempByte, i);
            }
        }
        else
        {
            spot = ((getBit(ct, i)) ^ (getBit(prev, 7 - i)));

            if (spot == 1)
            {
                tempByte = setBit(tempByte, i);
            }
            else
            {
                tempByte = clearBit(tempByte, i);
            }
        }
    }

    return tempByte;
}

/*  Function:  decode
  Purpose:   it takes all the line/paragraph you want to decrypt
       in:   pointer to the array holding the plain text
       in:   pointer to the array holding the cyphered text
       in:   size of plain text you want to decrypt
   return:   void no return value
*/

void decode(unsigned char *pt, unsigned char *ct, int numBytes)
{

    unsigned char ctr = processCtr(CTR, KEY);
    unsigned char prev = IV;

    for (int i = 0; i < numBytes; i++)
    {

        unsigned char spy = decryptByte(ct[i], ctr, prev);

        pt[i] = spy;

        prev = ct[i];
        ctr++;
        ctr = processCtr(ctr, KEY);
    }
}
