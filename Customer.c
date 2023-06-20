#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Customer.h"
#include "General.h"

int	initCustomer(Customer* pCustomer)
{

	do {
		pCustomer->name = getStrExactLength("Enter customer name\n");
		if (!pCustomer->name)
			return 0;
	} while (checkEmptyString(pCustomer->name));
	pCustomer->shopTimes = 0;
	pCustomer->totalSpend = 0;
	pCustomer->pCart = NULL;
	return 1;
}

void	printCustomer(const void* v)
{
	Customer* c = (Customer*)v;
	printf("Name: %s\t", c->name);
	printf("has shopped %d times ", c->shopTimes);
	printf("spent: %.2f ILS  ", c->totalSpend);
	if (c->pCart == NULL)
		printf("Shopping cart is empty!\n");
	else 
	{
		printf("Doing shopping now!!!\n");
		//printShoppingCart(pCustomer->pCart);
	}
}

void	pay(Customer* pCustomer)
{
	if (!pCustomer->pCart)
		return;
	printf("---------- Cart info and bill for %s ----------\n", pCustomer->name);
	printShoppingCart(pCustomer->pCart);
	printf("!!! --- Payment was recived!!!! --- \n");
	pCustomer->shopTimes++;
	pCustomer->totalSpend += getTotalPrice(pCustomer->pCart);
	freeShoppingCart(pCustomer->pCart);
	free(pCustomer->pCart);
	pCustomer->pCart = NULL;
}

int isCustomer(const Customer* pCust, const char* name)
{
	if (strcmp(pCust->name, name) == 0)
		return 1;
	return 0;
}

void	freeCustomer(void* v)
{
	Customer* c = (Customer*)v;
	if (c->pCart)
		pay(c); //will free every thing
	free(c->name);
}

int		compareCustomerByName(const void* v1,const void* v2) {
	Customer* c1 = (Customer*)v1;
	Customer* c2 = (Customer*)v2;
	return strcmp(c1->name, c2->name);
}

int		compareCustomerByTimesShopped(const void* v1, const void* v2) {
	Customer* c1 = (Customer*)v1;
	Customer* c2 = (Customer*)v2;
	return c1->shopTimes - c2->shopTimes;
}

int		compareCustomerByTotalSpendings(const void* v1, const void* v2) {
	Customer* c1 = (Customer*)v1;
	Customer* c2 = (Customer*)v2;
	if (c1->totalSpend > c2->totalSpend) {
		return 1;
	}
	else if (c1->totalSpend < c2->totalSpend) {
		return -1;
	}	
	else {
		return 0;
	}
	
}

int saveCustomerToTextFile(Customer* pCustomer, FILE* file) {
	if (!pCustomer || !file) return 0;
	fputs(pCustomer->name, file);
	fprintf(file, "\n%d %.2f\n",pCustomer->shopTimes, pCustomer->totalSpend);
	return 1;
}

int readCustomerFromTextFile(Customer* pCustomer, FILE* file) {
	if (!pCustomer || !file) return 0;
	char name[MAX_STR_LEN];
	fgetc(file);
	fgets(name, MAX_STR_LEN, file);
	int length = strlen(name);
	name[length-1] = '\0';
	pCustomer->name = _strdup(name); //saved zero terminated 
	fscanf(file, "%d %f", &pCustomer->shopTimes, &pCustomer->totalSpend);
	pCustomer->pCart = NULL;
	return 1;
}
