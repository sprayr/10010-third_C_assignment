#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Address.h"
#include "General.h"

#define ELEMENT_SEP "#"
#define WORD_SEP "  "

int	initAddress(Address* pAd)
{
	char allData[MAX_STR_LEN];
	char** elements=NULL;
	int count;
	int ok = 1;
	int totalLength;
	char msg[MAX_STR_LEN];
	sprintf(msg, "Enter address data\nFormat: street%shouse number%scity\nstreet and city can have spaces\n", 
			ELEMENT_SEP, ELEMENT_SEP);
	do {

		getsStrFixSize(allData, sizeof(allData), msg);
		int countSep = countCharInString(allData, ELEMENT_SEP[0]);
		if (countSep > 2)
		{
			printf("Too many separators in address\n");
			ok = 0;
		} else {
			elements = splitCharsToWords(allData, &count, ELEMENT_SEP, &totalLength);
			if (!elements)
				continue;
			ok = checkElements(elements, count);
			if (!ok)
			{
				printf("!!!incorrect address format!!!\n");
				freeElements(elements, count);
			}
		}
	} while (!ok);

	//get the house number
	sscanf(elements[1], "%d", &pAd->num);
	pAd->street = fixAddressParam(elements[0]);
	if (!pAd->street)
	{
		freeElements(elements, count);
		return 0;
	}

	pAd->city = fixAddressParam(elements[2]);
	if (!pAd->city)
	{
		free(pAd->street);
		freeElements(elements, count);
		return 0;
	}

	freeElements(elements, count);
	return 1;
}

char*	fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWords(param, &count," ", &totalLength);
	if (!wordsArray)
		return NULL;
	//add size for the WORD_SEP between words and for '\0'
	size_t length = (count - 1) * strlen(WORD_SEP) + 1;
	fixParamStr = (char*)calloc(totalLength + length, sizeof(char));
	if (!fixParamStr)
	{
		freeElements(wordsArray, count);
		return NULL;
	}

	if(count == 1)
		wordsArray[0][0] = toupper(wordsArray[0][0]); //upper
	else {
		for (int i = 0; i < count - 1; i++)
		{
			wordsArray[i][0] = toupper(wordsArray[i][0]); //upper
		}
		wordsArray[count - 1][0] = tolower(wordsArray[count - 1][0]); //lower
	}
	for (int i = 0; i < count - 1; i++)
	{
		strcat(fixParamStr, wordsArray[i]);
		strcat(fixParamStr, WORD_SEP);
	}
	strcat(fixParamStr, wordsArray[count - 1]);

	freeElements(wordsArray, count);
	return fixParamStr;
}


int checkElements(char**  elements, int count)
{
	if (count != 3)
		return 0;
	//check second word is a number;
	char* streetNum = elements[1];
	while (*streetNum)
	{
		if (!isdigit(*streetNum))
			return 0;
		streetNum++;
	}

	if (checkEmptyString(elements[0]))
		return 0;
	if (checkEmptyString(elements[2]))
		return 0;

	return 1;
}

void freeElements(char**  elements, int count)
{
	for (int i = 0; i < count; i++)
		free(elements[i]);
	free(elements);
}

void printAddress(const Address* pAd)
{
	printf("%s %d, %s\n", pAd->street, pAd->num, pAd->city);
}

void	freeAddress(Address* pAd)
{
	free(pAd->city);
	free(pAd->street);
}

int		saveAddressToBinFile(Address* pAd, FILE* bFile) {
	int streetLen = (int)strlen(pAd->street) + 1;
	int cityLen = (int) strlen(pAd->city) + 1;
	if(fwrite(&pAd->num, sizeof(int), 1 , bFile) != 1) return 0;
	if(fwrite(&streetLen,sizeof(int), 1, bFile) != 1) return 0;
	if (fwrite(pAd->street, sizeof(char), streetLen, bFile) != streetLen) return 0;
	if (fwrite(&cityLen, sizeof(int), 1, bFile) != 1) return 0;
	if (fwrite(pAd->city, sizeof(char), cityLen, bFile) != cityLen) return 0;
	return 1;
}


int readAddressFromBinFile(Address* pAd, FILE* bFile) {
	int streetLen, cityLen;
	if (fread(&pAd->num, sizeof(int), 1, bFile) != 1) return 0;
	if(fread(&streetLen, sizeof(int), 1, bFile) != 1) return 0;
	pAd->street = (char*)malloc(streetLen * sizeof(char));
	if (!pAd->street) return 0;
	if (fread(pAd->street, sizeof(char), streetLen, bFile) != streetLen) {
		free(pAd->street);
		return 0;
	}
	if (fread(&cityLen, sizeof(int), 1, bFile) != 1){
		free(pAd->street);
		return 0;
	}
	pAd->city = (char*)malloc(cityLen * sizeof(char));
	if (!pAd->city) { 
		free(pAd->street);
		return 0;
	}
	if (fread(pAd->city, sizeof(char), cityLen, bFile) != cityLen) {
		free(pAd->street);
		free(pAd->city);
		return 0;
	}
	return 1;
}	
	
	