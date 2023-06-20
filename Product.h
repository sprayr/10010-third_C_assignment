#pragma once

typedef enum {eFruitVegtable, eFridge, eFrozen, eShelf, eNofProductType} eProductType;
static const char* typeStr[eNofProductType] = { "Fruit Vegtable", "Fridge", "Frozen", "Shelf" };

#define NAME_LENGTH 20
#define BARCODE_LENGTH 7

typedef struct
{
	char			name[NAME_LENGTH+1];
	char			barcode[BARCODE_LENGTH + 1];
	eProductType	type;
	float			price;
	int				count;
}Product;

void	initProduct(Product* pProduct);
void	initProductNoBarcode(Product* pProduct);
void	initProductName(Product* pProduct);
void	printProduct(const void* pProduct);

void	getBorcdeCode(char* code);
eProductType getProductType();
const char*	getProductTypeStr(eProductType type);

void	updateProductCount(Product* pProduct);
void	freeProduct(Product* pProduct);
int		compareProduct(const void* v1, const void* v2);
int		saveProductToBinFile(Product* pProd, FILE* bFile);
int		readProductFromBinFile(Product* pProd, FILE* bFile);