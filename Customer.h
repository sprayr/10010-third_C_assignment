#pragma once
#include "ShoppingCart.h"

typedef struct
{
	char*			name;
	int				shopTimes;
	float			totalSpend;
	ShoppingCart*	pCart;
}Customer;

int		initCustomer(Customer* pCustomer);
void	printCustomer(const void* v);

int		isCustomer(const Customer* pCust, const char* name);

void	pay(Customer* pCustomer);

void	freeCustomer(void* v);

int		compareCustomerByName(const void* v1, const void* v2);
int		compareCustomerByTimesShopped(const void* v1, const void* v2);
int		compareCustomerByTotalSpendings(const void* v1, const void* v2);
int		saveCustomerToTextFile(Customer* pCustomer, FILE* file);
int		readCustomerFromTextFile(Customer* pCustomer, FILE* file);

