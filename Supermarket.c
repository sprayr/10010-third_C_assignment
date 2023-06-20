#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Supermarket.h"
#include "Product.h"
#include "Customer.h"
#include "Address.h"
#include "General.h"
#include "ShoppingCart.h"

#define  BIN_FILE_NAME "SuperMarket.bin"
#define  TXT_FILE_NAME "Customers.txt"

static const char* sortStr[eNumOfSortingOptions] = { "Not sorted", "Name", "Times shopped", "Total spendings" };

int		initSuperMarket(SuperMarket* pMarket)
{
	pMarket->customerCount = 0;
	pMarket->customerArr = NULL;
	pMarket->sortedBy = eNotSorted;
	if (!L_init(&pMarket->productList)) {
		printf("sorry, Couldn't initialize the supermarket.");
		return 0;
	 }
	pMarket->name = getStrExactLength("Enter market name");
	return initAddress(&pMarket->location);
}

void	printSuperMarket(const SuperMarket* pMarket)
{
	printf("Super Market Name: %s\t", pMarket->name);
	printf("Address: ");
	printAddress(&pMarket->location);
	printf("\n");
	printAllProducts(pMarket);
	printf("\n");
	printAllCustomers(pMarket);
}

int		addProduct(SuperMarket* pMarket)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket,barcode);
	if (pProd != NULL) //This barcode exist in stock
		updateProductCount(pProd);
	else { //new product to stock
		if(!addNewProduct(pMarket, barcode))
			return 0;
	}

	return 1;
}

int		addNewProduct(SuperMarket* pMarket,const char* barcode)
{
	Product* pProd = (Product*)calloc(1, sizeof(Product));
	if (!pProd)
	{
		L_free(&pMarket->productList,freeProduct);
		return 0;
	}

	strcpy(pProd->barcode,barcode);
	initProductNoBarcode(pProd);
	NODE* ptr = &pMarket->productList.head;
	if (countProducts(pMarket)!=0) {
		while (compareProduct(pProd, ptr->next->key) > 0) {
			if (ptr->next->next) {
				ptr = ptr->next;
			}
			else {
				if (compareProduct(pProd, ptr->next->key) > 0) {
					ptr = ptr->next;
				}
				break;
			}
		}
	}
	L_insert(ptr,pProd);

	return 1;
}

int		addCustomer(SuperMarket* pMarket)
{
	Customer cust;
	if (!initCustomer(&cust))
		return 0;

	if (isCustomerInMarket(pMarket, &cust))
	{
		printf("This customer already in market\n");
		free(cust.name);
		return 0;
	}

	pMarket->customerArr = (Customer*)realloc(pMarket->customerArr, (pMarket->customerCount + 1) * sizeof(Customer));
	if (!pMarket->customerArr)
	{
		free(cust.name);
		return 0;
	}

	pMarket->customerArr[pMarket->customerCount] = cust;
	pMarket->customerCount++;
	pMarket->sortedBy = eNotSorted;
	return 1;
}

int		isCustomerInMarket(SuperMarket* pMarket, Customer* pCust)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (strcmp(pMarket->customerArr[i].name, pCust->name) == 0)
			return 1;
	}
	return 0;
}

int	doShopping(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return 0;

	if (pCustomer->pCart == NULL)
	{
		pCustomer->pCart = (ShoppingCart*)malloc(sizeof(ShoppingCart));
		if (!pCustomer->pCart)
			return 0;
		initCart(pCustomer->pCart);
	}
	fillCart(pMarket, pCustomer->pCart);
	printf("---------- Shopping ended ----------\n");
	return 1;
}

Customer*	doPrintCart(SuperMarket* pMarket)
{
	Customer* pCustomer = getCustomerShopPay(pMarket);
	if (!pCustomer)
		return NULL;
	if (pCustomer->pCart == NULL)
	{
		printf("Customer cart is empty\n");
		return NULL;
	}
	printShoppingCart(pCustomer->pCart);
	return pCustomer;
}

int	doPayment(SuperMarket* pMarket)
{
	Customer* pCustomer = doPrintCart(pMarket);
	if(!pCustomer)
		return 0;
	pay(pCustomer);
	return 1;
}

Customer*	getCustomerShopPay(SuperMarket* pMarket)
{
	if (pMarket->customerCount == 0)
	{
		printf("No customer listed to market\n");
		return NULL;
	}

	if (pMarket->productList.head.next == NULL)
	{
		printf("No products in market - cannot shop\n");
		return NULL;
	}

	Customer* pCustomer = getCustomerWhoShop(pMarket);
	if (!pCustomer)
	{
		printf("this customer not listed\n");
		return NULL;
	}

	return pCustomer;
}

void	printAllProducts(const SuperMarket* pMarket)
{
	int productCount = countProducts(pMarket);
	if (productCount == -1) {
		return;
	}
	printf("There are %d products\n", productCount);
	printf("%-20s %-10s\t", "Name", "Barcode");
	printf("%-20s %-10s %s\n", "Type", "Price", "Count In Stoke");
	printf("--------------------------------------------------------------------------------\n");

	L_print(&(pMarket->productList),printProduct);
}

void	printAllCustomers(const SuperMarket* pMarket)
{
	printf("There are %d listed customers\n", pMarket->customerCount);
	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), printCustomer);
}


Customer* getCustomerWhoShop(SuperMarket* pMarket)
{
	printAllCustomers(pMarket);
	char name[MAX_STR_LEN];
	getsStrFixSize(name, sizeof(name), "Who is shopping? Enter cutomer name\n");

	Customer* pCustomer = FindCustomerByName(pMarket, name);
	
	return pCustomer;
}


void fillCart(SuperMarket* pMarket,ShoppingCart* pCart)
{
	printAllProducts(pMarket);
	char op;
	while (1)
	{
		printf("Do you want to shop for a product? y/Y, anything else to exit!!\t");
		do {
			scanf("%c", &op);
		} while (isspace(op));
		getchar(); //clean the enter
		if (op != 'y' && op != 'Y')
			break;
		int count;
		Product* pProd = getProductAndCount(pMarket, &count);
		if(pProd)
		{
			if (!addItemToCart(pCart, pProd->barcode, pProd->price, count))
			{
				printf("Error adding item\n");
				return;
			}
			pProd->count -= count; //item bought!!!
		}
	}
}

Product* getProductAndCount(SuperMarket* pMarket, int* pCount)
{
	char barcode[BARCODE_LENGTH + 1];
	Product* pProd = getProductFromUser(pMarket, barcode);
	if (pProd == NULL)
	{
		printf("No such product\n");
		return NULL;
	} 
	
	if (pProd->count == 0)
	{
		printf("This product out of stock\n");
		return NULL;
	}
	
	int count;
	do {
		printf("How many items do you want? max %d\n", pProd->count);
		scanf("%d", &count);
	} while (count <= 0 || count > pProd->count);
	*pCount = count;
	return pProd;
}

void	printProductByType(SuperMarket* pMarket)
{
	if (!pMarket->productList.head.next)
	{
		printf("No products in market\n");
		return;
	}
	eProductType	type = getProductType();
	int count = 0;
	
	NODE* ptr = pMarket->productList.head.next;
	while (ptr) {
		if (((Product*)ptr->key)->type == type) {
			count++;
			printProduct(ptr->key);
		}
		ptr = ptr->next;
	}

	if (count == 0)
		printf("There are no product of type %s in market %s\n", getProductTypeStr(type), pMarket->name);
}

Product* getProductFromUser(SuperMarket* pMarket, char* barcode)
{
	getBorcdeCode(barcode);
	if (countProducts(pMarket)==0) return NULL;
	return getProductByBarcode(pMarket, barcode);
}

void	freeMarket(SuperMarket* pMarket)
{
	free(pMarket->name);
	freeAddress(&pMarket->location);
	L_free(&pMarket->productList,freeProduct);	
	generalArrayFunction(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), freeCustomer);
	free(pMarket->customerArr);
}

void	getUniquBarcode(char* barcode, SuperMarket* pMarket)
{
	int cont = 1;
	while (cont)
	{
		getBorcdeCode(barcode);
		Product prod;
		strcpy(prod.barcode, barcode);
		NODE* ptr = L_find(&pMarket->productList.head,&prod,compareProduct);
		if (ptr)
			cont = 0;
		else
			printf("This product already in market\n");
	}
}



Product* getProductByBarcode(SuperMarket* pMarket, const char* barcode)
{
	Product* pProd = (Product*)malloc(sizeof(Product));
	if (!pProd) {
		return NULL;
	}

	strcpy(pProd->barcode, barcode);
	NODE* tempNode =  L_find(&pMarket->productList.head, pProd, compareProduct);	
	if (!tempNode) {
		freeProduct(pProd);
		free(pProd);
		return NULL;
	}
	pProd = tempNode->key;
	return pProd;

}

Customer* FindCustomerByName(SuperMarket* pMarket, const char* name)
{
	for (int i = 0; i < pMarket->customerCount; i++)
	{
		if (isCustomer(&pMarket->customerArr[i], name))
			return &pMarket->customerArr[i];
	}
	return  NULL;
}


int countProducts(const SuperMarket* pMarket) {
	if (!pMarket) return -1;
	
	int productCount = 0;
	NODE* ptr = pMarket->productList.head.next;
	while (ptr) {
		productCount++;
		ptr = ptr->next;
	}

	return productCount;
	
}


int sortCustomersArr(SuperMarket* pMarket) {
	if (!pMarket->customerCount) {
		printf("There are no customers registered\n\n");
		return 0;
	}
	int choice = sortMenu();
	switch (choice){
	case eNameSort: 
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustomerByName);
		pMarket->sortedBy = eNameSort;
		break;
	case eNumOfTimesShoppedSort: 
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustomerByTimesShopped);
		pMarket->sortedBy = eNumOfTimesShoppedSort;
		break;
	case eTotalSpent: 
		qsort(pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustomerByTotalSpendings);
		pMarket->sortedBy = eTotalSpent;
		break;
	}
	return 1;
}

int sortMenu() {
	printf("How would you like to sort the customers?\n");
	for (int i = 1; i < eNumOfSortingOptions; i++) {
		printf("%d - %s\n", i, sortStr[i]);
	}
	int choice;
	do {
		scanf("%d", &choice);
		getchar();
		if (choice <= 0 || choice >= eNumOfSortingOptions) printf("wrong input, try again\n");

	} while (choice <= 0 || choice >= eNumOfSortingOptions);
	return choice;
}

void		searchCustomer(SuperMarket* pMarket) {
	Customer tempCustomer;
	Customer* pCust = NULL;
	switch (pMarket->sortedBy)
	{
	case eNotSorted:
		printf("Cannot search customers, please sort customers first!");
		return;
		break;
	case eNameSort:
		initCustomer(&tempCustomer);
		pCust = bsearch(&tempCustomer, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustomerByName);
		break;
	case eNumOfTimesShoppedSort:
		printf("Enter number of times shopped:\n");
		scanf("%d", &(tempCustomer.shopTimes));
		pCust = bsearch(&tempCustomer, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustomerByTimesShopped);
		break;
	case eTotalSpent:
		printf("Enter total amount of money spent:\n");
		scanf("%f", &(tempCustomer.totalSpend));
		pCust = bsearch(&tempCustomer, pMarket->customerArr, pMarket->customerCount, sizeof(Customer), compareCustomerByName);
		break;
	}
	if (!pCust) {
		printf("\nCustomer not found\n\n");
		return;
	}
	printf("\nCustomer found:\n");
	printCustomer(pCust);
}

int		saveSuperMarketToBinFile(SuperMarket* pMarket) {
	FILE* bFile = fopen(BIN_FILE_NAME, "wb");
	if (!bFile) return 0;

	int nameSize = (int)strlen(pMarket->name) + 1;
	if (fwrite(&nameSize, sizeof(int), 1, bFile) != 1) {
		fclose(bFile);
		return 0;
	}
	if (fwrite(pMarket->name, sizeof(char), nameSize, bFile) != nameSize) {
		fclose(bFile);
		return 0;
	}
	if (!saveAddressToBinFile(&pMarket->location, bFile)){
		fclose(bFile);
		return 0;
	}
	int count = countProducts(pMarket);
	if (!saveProductsListToBinFile(pMarket->productList, bFile, count )) {
		fclose(bFile);
		return 0;
	}
	fclose(bFile);
	
	return 1;
}


int saveProductsListToBinFile(LIST pList, FILE* bFile, int count) {
	if (fwrite(&count, sizeof(int), 1, bFile) != 1) return 0;
	if (!count) return 1; //no products to save
	
	NODE* temp = pList.head.next;
	Product* pProd; 
	while (temp != NULL) {
		pProd = (Product*)(temp->key);
		if(!saveProductToBinFile(pProd, bFile)) return 0;
		temp = temp->next;
	}

	return 1;
}

int saveCustomersToTextFile(SuperMarket* pMarket) {
	FILE* file = fopen(TXT_FILE_NAME, "w");
	if (!file) return 0;
	fprintf(file, "%d\n", pMarket->customerCount);
	for (int i = 0; i < pMarket->customerCount; i++) {
		saveCustomerToTextFile(&(pMarket->customerArr[i]), file);
	}

	fclose(file);
	return 1;
}

int readSuperMarketFromBinFile(SuperMarket* pMarket) {
	if (!pMarket) return 0;
	FILE* bFile = fopen(BIN_FILE_NAME, "rb");
	if (!bFile) return 0;
	if (!readSuperNameFromBinFile(pMarket, bFile)) {
		fclose(bFile);
		return 0;
	}
	if (!readAddressFromBinFile(&pMarket->location, bFile)) {
		free(pMarket->name);
		fclose(bFile);
		return 0;
	}
	if (!readProductLinkedListFromBinFile(pMarket, bFile)) {
		free(pMarket->name);
		fclose(bFile);
		return 0;
	}
	pMarket->sortedBy = eNotSorted;
	fclose(bFile);
	return 1;
}


int readSuperNameFromBinFile(SuperMarket* pMarket, FILE* bFile) {
	if (!pMarket || !bFile) return 0;
	int nameLen;
	if (fread(&nameLen, sizeof(int), 1, bFile) != 1) {
		return 0;
	}
	pMarket->name = (char*)malloc(nameLen * sizeof(char));
	if (!pMarket->name) {
		return 0;
	}
	if (fread(pMarket->name, sizeof(char), nameLen, bFile) != nameLen) {
		free(pMarket->name);
		return 0;
	}
	return 1;
}

int readProductLinkedListFromBinFile(SuperMarket* pMarket, FILE* bFile) {	
	if (!pMarket || !bFile) return 0;
	if (!L_init(&pMarket->productList)) return 0;
	int prodCount;
	if (fread(&prodCount, sizeof(int), 1, bFile) != 1) {
		L_free(&pMarket->productList, freeProduct);
		return 0;
	}
	Product* pProd;
	NODE* ptr = &pMarket->productList.head;
	for (int i = 0; i < prodCount; i++) {
		pProd = (Product*)malloc(sizeof(Product));
		if (!pProd) {
			L_free(&pMarket->productList, freeProduct);
			free(pProd);
			return 0;
		}
		if (!readProductFromBinFile(pProd, bFile)) {
			L_free(&pMarket->productList, freeProduct);
			free(pProd);
			return 0;
		}
		L_insert(ptr, pProd);
		ptr = ptr->next;
	}
	return 1;	
}

int readCustomersFromTextFile(SuperMarket* pMarket) {
	if (!pMarket) return 0;
	FILE* file = fopen(TXT_FILE_NAME, "r");
	if (!file) return 0;
	fscanf(file, "%d", &pMarket->customerCount);
	if (pMarket->customerCount < 0) return 0;
	pMarket->customerArr = (Customer*)calloc(pMarket->customerCount, sizeof(Customer));
	if (!pMarket->customerArr) return 0;
	for (int i = 0; i < pMarket->customerCount; i++) {
		if (!readCustomerFromTextFile(&(pMarket->customerArr[i]), file)) {
			for (int j = 0; j <= i; j++) {
				freeCustomer(&pMarket->customerArr[i]);
				free(pMarket->customerArr);
				return 0;
			}
		}
	}
	fclose(file);
	return 1;
}

void everyCustomerNeedsToPay(SuperMarket* pMarket) {
	for (int i = 0; i < pMarket->customerCount; i++) {
		if (pMarket->customerArr[i].pCart) {
			pay(&pMarket->customerArr[i]);
		}
	}
}