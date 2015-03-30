#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>


// Wielkosc bloku w bitach/32
int Nr=0;
// Wielkosc klucza w bitach/32
int Nk=0;
//Licza kolumn stanowi�cych stan w AES.
#define Nb = 4

// in - tablica danych wejsciowych
// out - tablica danych zaszyfrowanych
// state - szyfr po�redni zwany stanem
unsigned char in[16], out[16], stan[4][4];

// Tablica przechowuj�ca klucze rundy
unsigned char RoundKey[240];

// Klucz wejsciowy do szyfrowania
unsigned char Key[32];


int Sbox[256] = { 
		0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
		0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
		0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
		0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
		0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
		0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
		0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
		0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
		0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
		0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
		0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
		0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
		0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
		0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
		0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
		0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16};
int PobierzWartSBox(int num)
{
return Sbox[num];
}

//Tablica sta�ych s��w rundy zawiera warto�ci x do pot�gi i-1 b�d�cymi pot�gami x w ciele GF(28)
//i zaczyna si� od 1, nie od 0
int Rcon[255] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };

void GenerujKlucz()
{
	int i,j;
	unsigned char temp[4],k;

	//Pierwsza runda jest kluczem samym w sobie
	for(i=0; i<Nk; i++)
	{
		RoundKey[i*4] = Key[i*4];
		RoundKey[i*4+1] = Key[i*4+1];
		RoundKey[i*4+2] = Key[i*4+2];
		RoundKey[i*4+3] = Key[i*4+3];
	}

	//Wszystkie inne klucze rundy tworzy si� na podstawie poprzednich
	while (i < (4 * (Nr+1)))
	{
		for(j=0; j<4; j++)
		{
			temp[j] = RoundKey[(i-4) * 4 + j];
		}
		if(i % Nk == 0)
		{
			//Ta funkcja obraca 4 bajtowe s�owo w lewo
			//[a0,a1,a2,a3] do [a1,a2,a3,a0];
			{
				k = temp[0];
				temp[0] = temp[1];
				temp[1] = temp[2];
				temp[2] = temp[3];
				temp[3] = k;
			}
			//Konwersja wart�ci czterobajtowych na warto�ci z macierzy S-Box
			{
				temp[0] = PobierzWartSBox(temp[0]);
				temp[1] = PobierzWartSBox(temp[1]);
				temp[2] = PobierzWartSBox(temp[2]);
				temp[3] = PobierzWartSBox(temp[3]);
			}
			temp[0] = temp[0] ^ Rcon[i/Nk];
		}
		else if (Nk>6 && i%Nk == 4)
		{
			//Konwersja wart�ci czterobajtowych na warto�ci z macierzy S-Box
			{
				temp[0] = PobierzWartSBox(temp[0]);
				temp[1] = PobierzWartSBox(temp[1]);
				temp[2] = PobierzWartSBox(temp[2]);
				temp[3] = PobierzWartSBox(temp[3]);
			}
		}
		RoundKey[i*4+0] = RoundKey[(i-Nk)*4+0] ^temp[0];
		RoundKey[i*4+1] = RoundKey[(i-Nk)*4+1] ^temp[1];
		RoundKey[i*4+2] = RoundKey[(i-Nk)*4+2] ^temp[2];
		RoundKey[i*4+3] = RoundKey[(i-Nk)*4+3] ^temp[3];
		i++;
	}
}

void SubBytes(unsigned char stan[4][4])
{
	int i,j;
	for(i=0 ; i<4 ; i++)
	{
		for(j=0 ; j<4 ; j++)
		{
			stan[i][j] = Sbox[stan[i][j]];
		}
	}
}

void WyswietlStan()
{
	int i,j;
	for(i=0 ; i<4 ; i++)
	{
		printf("\n");
		for(j=0 ; j<4 ; j++)
		{
			printf("%x , ",stan[i][j]);
		}
	}
	system("pause");
}

void ShiftRows()
{	
	unsigned char temp;

	//w lewo o C1 (1 dla Ns = 4)
	temp = stan[1][0];
	stan[1][0] = stan[1][1];
	stan[1][1] = stan[1][2];
	stan[1][2] = stan[1][3];
	stan[1][3] = temp;

	//w lewo o C2 (2 dla Ns = 4)
	temp = stan[2][0];
	stan[2][0] = stan[2][2];
	stan[2][2] = temp;
	temp = stan[2][1];
	stan[2][1] = stan[2][3];
	stan[2][3] = temp;
	
	//w lewo o C3 (3 dla Ns = 4)
	temp = stan[3][3];
	stan[3][3] = stan[3][2];
	stan[3][2] = stan[3][1];
	stan[3][1] = stan[3][0];
	stan[3][0] = temp;
}


void MixColumns()
{
	int i;
    unsigned char temp,x,t;

    for(i=0;i<4;i++)
    {    
		t =stan[0][i];
		//Suma modulo wszystkich elementow w kolumnie
        temp = stan[0][i] ^ stan[1][i] ^ stan[2][i] ^ stan[3][i];
	
        x = stan[0][i] ^ stan[1][i];
		x = ((x<<1) ^ (((x>>7) & 1) * 0x1b)); 
		stan[0][i] = stan[0][i] ^ ( x ^ temp );

        x = stan[1][i] ^ stan[2][i] ;
		x = ((x<<1) ^ (((x>>7) & 1) * 0x1b));
		stan[1][i] = stan[1][i] ^ ( x ^ temp );

        x = stan[2][i] ^ stan[3][i];
		x = ((x<<1) ^ (((x>>7) & 1) * 0x1b));
		stan[2][i] = stan[2][i] ^ ( x ^ temp );

        x = stan[3][i] ^ t;
		x = ((x<<1) ^ (((x>>7) & 1) * 0x1b));
		stan[3][i] = stan[3][i] ^ ( x ^ temp );

		// Mnozenie przez wartosc(np 02) liczby kt�rej najstarszy bit=1
		// moze byc zaimplementowane jako przesuniecie o 1 w lewo 
		// oraz operacje XOR z wartoscia 00011011 (czyli 0x1b).
    }
}

void AddRoundKey(int runda)
{
	int i,j;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			stan[j][i] ^=RoundKey[runda * 4 * 4 + i * 4 +j];
		}
	}
}

// Funkcja rozpoczynaj�ca szyfrowanie tablicy znak�w in[]
void Szyfruj()
{
    int i,j,runda=0;

    //Transformacja tablicy znak�w w macierz stanu
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            stan[j][i] = in[i*4 + j];
        }
    }

    // Dodanie pierwszego klucza rundy do stanu
    AddRoundKey(0); 
    
    //Pierwsze Nr-1 rund
    for(runda=1 ; runda<Nr ; runda++)
    {
        SubBytes(stan);
        ShiftRows();
        MixColumns();
        AddRoundKey(runda);
    }
    

	//Ostatnia runda
    SubBytes(stan);
    ShiftRows();
    AddRoundKey(Nr);

    //Kopiowanie zaszyfrowanego tekstu na wyjscie
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            out[i*4+j]=stan[j][i];
        }
    }
}


int main()
{
	int i;
	// Wiadomosc "Wesolych_swiat!!"
	unsigned char wiadomosc[16] = {0x57, 0x65, 0x73, 0x6F, 0x6C, 0x79, 0x63, 0x68, 0x20, 0x73, 0x77, 0x69, 0x61, 0x74, 0x21, 0x21}; 
   //unsigned char wiadomosc[16] = {0xd4, 0xbf, 0x5d, 0x30, 0xe0, 0xb4, 0x52, 0xae, 0xb8, 0x41, 0x11, 0xf1, 0x1e, 0x27, 0x98, 0xe5};
	//temp - klucz
	unsigned char temp[16] = {0x00  ,0x01  ,0x02  ,0x03  ,0x04  ,0x05  ,0x06  ,0x07  ,0x08  ,0x09  ,0x0a  ,0x0b  ,0x0c  ,0x0d  ,0x0e  ,0x0f};
   
    while(Nr!=128 && Nr!=192 && Nr!=256)
    {
        printf("Wybierz dlugosc klucza:(128, 192 lub 256): ");
        scanf("%d",&Nr);
    }
    
    // obliczenie Nk i Nr
    Nk = Nr / 32;
    Nr = Nk + 6;

	for(i=0 ; i < Nk*4 ; i++)
	{
		in[i] = wiadomosc[i];
		Key[i] = temp[i];
	}
	GenerujKlucz();
	Szyfruj();
	printf("Szyfrujemy: Wesolych Swiat!!\n");
	printf("Tekst po szyfryzacji: \n");
	for(i=0; i<Nk*4; i++)
	{
		printf("%02x",out[i]);
	}
	getch();
}