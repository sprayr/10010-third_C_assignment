#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "General.h"
#include "Supermarket.h"






int menu();



const char* menuStrings[eNofOptions] = { "Show SuperMarket", "Add Product",
								"Add Customer", "Customer Shopping","Print Shopping Cart","Customer Pay",
								"Print Product By Type", "Sort Customers", "Search Customer"};

int main()
{

	
	SuperMarket	market;

	if (readSuperMarketFromBinFile(&market)&& readCustomersFromTextFile(&market)){
		
		printf("Market loaded successfully\n\n");
	}
	else if(!initSuperMarket(&market)) {
		printf("error init  Super Market");
		return 0;
	}

	int option;
	int stop = 0;
	
	do
	{
		option = menu();
		switch (option)
		{
		case eShowSuperMarket:
			printSuperMarket(&market);
			break;


		case eAddProduct:
			if (!addProduct(&market))
				printf("Error adding product\n");
			break;

		case eAddCustomer:
			if (!addCustomer(&market))
				printf("Error adding customer\n");
			break;

		case eCustomerDoShopping:
			if(!doShopping(&market))
				printf("Error in shopping\n");
			break;

		case ePrintCart:
			doPrintCart(&market);
			break;

		case eCustomerPay:
			if(!doPayment(&market))
				printf("Error in payment\n");
			break;

		case ePrintProductByType:
			printProductByType(&market);
			break;

		case eSortCustomerArr:
			sortCustomersArr(&market);
			break;

		case eSearchCustomer:
			searchCustomer(&market);
			break;

		case EXIT:
			everyCustomerNeedsToPay(&market);
			printf("Bye bye\n");
			saveSuperMarketToBinFile(&market);
			saveCustomersToTextFile(&market);
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);


	freeMarket(&market);
	
	system("pause");
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i, menuStrings[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

