#pragma once


#define EXIT -1

typedef enum
{
	eShowSuperMarket, eAddProduct, eAddCustomer, eCustomerDoShopping,ePrintCart,eCustomerPay,
	ePrintProductByType,eSortCustomerArr,eSearchCustomer, eNofOptions
} eMenuOptions;

const char* menuStrings[eNofOptions];

int menu();