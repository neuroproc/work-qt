#include <stdio.h>
#include <iostream>
#include <string.h>

/* VARIANT 3 */

using namespace std;

u_long LSFSR(FILE *C, FILE *M, FILE *Kf);
u_long GEFFE(FILE *C, FILE *M, FILE *Kf);
u_long RC4(FILE *C, FILE *M, FILE *Kf);
void swapChar(char &, char &);

int main(int argc, char *argv[])
{

    if( argc < 4 )
    {
        cout << "incorrect data" << endl;
        return -1;
    }

    FILE *C, *M, *K;
    M = fopen(argv[1], "r");
    C = fopen(argv[2], "w");
    K = fopen(argv[3], "w");


    /* TEST SECTION */
    /*
    FILE *C, *M, *K;
    M = fopen("input.txt", "r");
    C = fopen("output", "w");
    K = fopen("key.txt", "w");

    cout << system("pwd") << endl;
    */
    /* END */

    int numb;
    std::bitset<sizeof(u_long) * 8> endState;

    cout << "1 - LSFSR" << endl << "2 - GEFFE" << endl << "3 - RC4"  << endl <<  "Enter crypto algorithm: ";
    cin >> numb;

    /* TEST SECTION */
    //numb = 1;
    /* END */

    switch (numb)
    {
    case 1:
        endState = LSFSR(C, M, K);
        break;
    case 2:
        endState = GEFFE(C, M, K);
        break;
    case 3:
        endState = RC4(C, M, K);
        break;
    default:
        break;
    }

    //std::bitset<sizeof(u_long) * 8> key(LSFSR(C, M));
    fprintf(K, "%s\n", endState.to_string().data());

    fclose(C);
    fclose(M);
    fclose(K);

    return 0;
}

u_long LSFSR(FILE *C, FILE *M, FILE *Kf)
{
    char K;
    u_long x25, x3, x0;
    // x - члены многочлена для вставки первого чимвола
    x25 = ((u_long)1) << 24;
    x3 = ((u_long)1) << 2;
    x0 = (u_long) 1;
    u_long longK;
    u_long mask25 = 0;//25 - битовая маска (cдвиговый регистр, длина которого равна степени многочлена)
    u_long bit25 = ((u_long) 1) << 25;//маска для извлечения 25 бита
    for( int i = 0; i < 25; i++ )
    {
        mask25 = mask25 | x0;
        x0 = x0 << 1;
    }
    x0 = (u_long) 1;

    std::bitset<25> key;
    cout << "Enter start register: ";
    cin >> key;

    /* TEST SECTION */
    //key = 1;
    /* END */

    cout << "U KEY: " << key << endl;

    K = (char)key.to_ulong();

    longK = (u_long) K;     //

    char inputsymbol = fgetc(M);
    while( !feof(M) )
    {
        char outputsymbol = K ^ inputsymbol;
        fputc(outputsymbol, C );
        u_long temp = 0;

        if( longK & x25 )
            temp = temp ^ (u_long)1;
        if( longK & x3 )
            temp = temp ^ (u_long)1;

        longK = (longK << 1) & mask25;
        longK = longK | temp;

        K = K << 1;
        if( temp == 1 )
            K = K | (char)1;

        inputsymbol = fgetc(M);
    }
    return (u_long)K;
}

u_long GEFFE(FILE *C, FILE *M, FILE *Kf)
{
    char K1, K2, K3, Kg;
    u_long x25, x3, x0, x33, x13, x23, x5;
    // x - члены многочлена для вставки первого чимвола
    x25 = ((u_long)1) << 24;
    x3 = ((u_long)1) << 2;

    x33 = ((u_long)1) << 32;
    x13= ((u_long)1) << 12;

    x23 = ((u_long)1) << 22;
    x5 = ((u_long)1) << 4;

    x0 = (u_long) 1;


    u_long longK1, longK2, longK3, longKg;
    u_long mask25 = 0, mask33 = 0, mask23 = 0;//23, 31, 39 - битовая маска
    u_long bit25 = ((u_long) 1) << 24, bit33 = ((u_long) 1) << 32, bit23 = ((u_long) 1) << 22;//маски для извлечения 24, 32  и 40 битов
    for( int i = 0; i < 24; i++ )
    {
     mask25 = mask25 | x0;
     x0 = x0 << 1;
    }
    mask33 = mask25;
    for( int i = 26; i < 32; i++ )
    {
     mask33 = mask33 | x0;
     x0 = x0 << 1;
    }
    for( int i = 0; i < 22; i++ )
    {
     mask23 = mask23 | x0;
     x0 = x0 << 1;
    }
    x0 = (u_long) 1;


    std::bitset<25> key1;
    std::bitset<33> key2;
    std::bitset<23> key3;

    cout << "Enter start register N1: ";
    cin >> key1;

    cout << "Enter start register N2: ";
    cin >> key2;

    cout << "Enter start register N3: ";
    cin >> key3;

    cout << "U KEY1: " << key1 << endl;
    cout << "U KEY2: " << key2 << endl;
    cout << "U KEY3: " << key3 << endl;

    K1 = (char)key1.to_ulong();
    K2 = (char)key2.to_ulong();
    K3 = (char)key3.to_ulong();

    char inputsymbol = fgetc(M);
    Kg = (K1 & K2) | !(K1 & K3);
    longKg = (longK1 & longK2) | !( longK1 & longK3 );
    while( !feof(M) )
    {
     fputc( (Kg ^ inputsymbol), C );

     u_long temp = 0;
     if( longK1 & x25 )
         temp = temp ^ (u_long)1;
     if( longK1 & x3 )
         temp = temp ^ (u_long)1;
     longK1 = (longK1 << 1) & mask25;
     longK1 = longK1 | temp;
     K1 = K1 << 1;
     if( temp == 1 )
         K1 = K1 | (char)1;

     temp = 0;
     if( longK2 & x33 )
         temp = temp ^ (u_long)1;
     if( longK2 & x13 )
         temp = temp ^ (u_long)1;
     longK2 = (longK2 << 1) & mask33;
     longK2 = longK2 | temp;
     K2 = K2 << 1;
     if( temp == 1 )
         K2 = K2 | (char)1;

     temp = 0;
     if( longK3 & x23 )
         temp = temp ^ (u_long)1;
     if( longK3 & x5 )
         temp = temp ^ (u_long)1;
     longK3 = (longK3 << 1) & mask23;
     longK3 = longK3 | temp;
     K3 = K3 << 1;
     if( temp == 1 )
         K3 = K3 | (char)1;

     Kg = (K1 & K2) | !(K1 & K3);
     longKg = (longK1 & longK2) | !( longK1 & longK3 );
     inputsymbol = fgetc(M);
    }
    return (u_long)Kg;
}

u_long RC4(FILE *C, FILE *M, FILE *Kf)
{
    char userkey[100];
    char Sbox[256];
    int count = 0;

    cout << "Enter key: ";
    cin >> userkey;

    for( int i= 0; i < 256; i++ )
         Sbox[i] = i;
    int j = 0;
    for( int i = 0; i < 256; i++)
    {
         j = (j + (unsigned char)Sbox[i] + userkey[i % strlen(userkey)]) % 256;
         swapChar( Sbox[i], Sbox[j] );
    }

    int i = 0;
    j = 0;
    char currentKey;


    //char *outputText =  (char*)calloc(size, 1);
/*
    fseek(M, 0, SEEK_END);
    u_long size = ftell(M);
    fseek(M, 0, SEEK_SET);
*/
    char symbol = fgetc(M);
    while (!feof(M))
    {
        count++;

        i = (i+1) % 256;
        j = (j+(unsigned char)Sbox[i]) % 256;
        swapChar(Sbox[i], Sbox[j]);
        currentKey = Sbox[ ( (unsigned char)Sbox[i] + (unsigned char)Sbox[j] ) % 256 ];

        char newsym = (currentKey ^ symbol);

        fputc(newsym, C);

        symbol = fgetc(M);
    }
    cout << count << endl;

    return count;

}

void swapChar( char &a, char &b )
{
     char h = a;
     a = b;
     b = h;
}

