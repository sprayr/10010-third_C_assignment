#pragma once
#include "Address.h"
#include "Customer.h"
#include "Product.h"
#include "ShoppingCart.h"
#include "listGen.h"

typedef enum {eNotSorted,eNameSort,eNumOfTimesShoppedSort,eTotalSpent, eNumOfSortingOptions} sortingOptions;

typedef struct
{
	sortingOptions		sortedBy;
	char*		name;
	Address		location;
	Customer*	customerArr;
	int			customerCount;
	LIST productList;
}SuperMarket;


int		initSuperMarket(SuperMarket* pMarket);
void	printSuperMarket(const SuperMarket* pMarket);
int		addProduct(SuperMarket* pMarket);
int		addNewProduct(SuperMarket* pMarket, const char* barcode);
int		addCustomer(SuperMarket* pMarket);
int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust);
int		doShopping(SuperMarket* pMarket);
Customer* doPrintCart(SuperMarket* pMarket);
int		doPayment(SuperMarket* pMarket);
Customer*		getCustomerShopPay(SuperMarket* pMarket);
void fillCart(SuperMarket* pMarket, ShoppingCart* pCart);

void	printProductByType(SuperMarket* pMarket);
void	printAllProducts(const SuperMarket* pMarket);
void	printAllCustomers(const SuperMarket* pMarket);



Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode);
Product* getProductFromUser(SuperMarket* pMarket, char* barcode);
void	getUniquBarcode(char* barcode, SuperMarket* pMarket);
Customer* FindCustomerByName(SuperMarket* pMarket, const char* name);

Customer* getCustomerWhoShop(SuperMarket* pMarket);
Product* getProductAndCount(SuperMarket* pMarket, int* pCount);

void	freeMarket(SuperMarket* pMarket);
int		countProducts(const SuperMarket* pMarket);
int		sortCustomersArr(SuperMarket* pMarket);
int		sortMenu();
void	searchCustomer(SuperMarket* pMarket);
int		saveSuperMarketToBinFile(SuperMarket* pMarket);
int		saveProductsListToBinFile(LIST pList, FILE* bFile, int count);
int		saveCustomersToTextFile(SuperMarket* pMarket);
int		readSuperMarketFromBinFile(SuperMarket* pMarket);
int		readSuperNameFromBinFile(SuperMarket* pMarket, FILE* bFile);
int		readProductLinkedListFromBinFile(SuperMarket* pMarket, FILE* bFile);
int		readCustomersFromTextFile(SuperMarket* pMarket);
void	everyCustomerNeedsToPay(SuperMarket* pMarket);