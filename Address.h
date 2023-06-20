#pragma once

typedef struct
{
	int		num;
	char*	street;
	char*	city;
}Address;

int		initAddress(Address* pAd);
void	printAddress(const Address* pAd);
void	freeAddress(Address* pAd);

int		checkElements(char**  elements, int count);
char*	fixAddressParam(char* param);
void	freeElements(char**  elements, int count);
int		saveAddressToBinFile(Address* pAd, FILE* bFile);
int		readAddressFromBinFile(Address* pAd, FILE* bFile);
