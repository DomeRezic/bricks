#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define MAX_SIZE 100
#define FILE_DIDNT_OPEN_ERROR -1
#define EXIT_SUCCESS 0
#define MAX_FILE_NAME (256)
#define ALLOCATION_ERROR -2
#define COUNTRY_DOSE_NOT_EXIST -3
#define HASH_TABLE_SIZE (11)

typedef struct City* Tree;
struct City
{
	char CityName[MAX_SIZE];
	int Population;
	Tree Left;
	Tree Right;
}_city;

typedef struct State* Position;
struct State
{
	char CountryName[MAX_SIZE];
	char FileName[MAX_FILE_NAME];
	Position next;
	Tree root;
}_state;

int readCountryFromFile(Position, char*);
Position InsertState(char*, char*);
int PrintState(Position Head);
int NewStateInList(Position, Position);
Position FindState(Position, char*);

int readCityFromFile(Position, char*);
Tree MakeTree(char*, int);
int Inorder(Tree);
int Preorder(Tree);
Tree CompletelyTree(Tree, Tree);


int main()
{
	char nameOfFile[MAX_FILE_NAME] = "\0";
	Position s = NULL;
	char CountryName[MAX_SIZE] = "";
	Position head = NULL;
	head = (Position)malloc(sizeof(_state) * HASH_TABLE_SIZE);

	if (head == NULL) {
		perror("Nesto");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		strcpy(head[i].CountryName, "");
		head[i].next = NULL;
		head[i].root = NULL;
	}

	printf("Enter file name:\n");
	scanf("%s", nameOfFile);
	if (readCountryFromFile(head, nameOfFile))
		return FILE_DIDNT_OPEN_ERROR;

	PrintState(head);
	printf("\t\t\nCHOOSE THE COUNTRY YOU WANT:  ");
	scanf("%s", CountryName);
	s = FindState(head, CountryName);
	if (s == NULL)
	{
		printf("The selected country does not exist in our list!");
		return COUNTRY_DOSE_NOT_EXIST;
	}

	printf("\n\t\t SELECTED COUNTRY: %s\n\n", s->CountryName);
	printf("\t\t CITIES:\n\n");
	Preorder(s->root);

	return EXIT_SUCCESS;
}


int readCountryFromFile(Position headNiz, char* filename)
{
	char CountryName[MAX_SIZE] = "";
	char CountryFileName[MAX_SIZE] = "";
	Position NewState = NULL;

	FILE* fp = fopen(filename, "r");

	if (fp == NULL)
	{
		printf("File didn't open! %s\n", strerror(errno));
		return FILE_DIDNT_OPEN_ERROR;
	}

	while (!feof(fp))
	{
		Position p = NULL;
		fscanf(fp, " %s %s", CountryName, CountryFileName);
		NewState = InsertState(CountryName, CountryFileName);

		p = &headNiz[calculateIndex(CountryName)];

		NewStateInList(p, NewState);
		readCityFromFile(NewState, CountryFileName);
	}

	fclose(fp);
	return 0;
}

int NewStateInList(Position head, Position NewState)
{
	Position p = head;
	while (p->next != NULL && strcmp(p->next->CountryName, NewState->CountryName) < 0) {
		p = p->next;
	}
	NewState->next = p->next;
	p->next = NewState;
	return EXIT_SUCCESS;
}

Position InsertState(char* CountryName, char* FileName)
{
	Position NewState = (Position)malloc(sizeof(_state));
	if (NewState == NULL) {
		return NULL;
	}
	strcpy(NewState->CountryName, CountryName);
	strcpy(NewState->FileName, FileName);
	NewState->next = NULL;
	NewState->root = NULL;
	return NewState;
}


int PrintState(Position headNiz)
{
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		Position p = headNiz[i].next;
		while (p)
		{
			printf("\t\t ~ %s \n", p->CountryName);
			p = p->next;
		}
	}

	return EXIT_SUCCESS;
}

Position FindState(Position headNiz, char* CountryName)
{
	Position p = &headNiz[calculateIndex(CountryName)];
	while (p->next != NULL && strcmp(CountryName, p->CountryName) != 0)
	{
		p = p->next;
	}
	return p;
}

Tree MakeTree(char* CityName, int Population)
{
	Tree c;
	c = (Tree)malloc(sizeof(_city));

	if (c == NULL)
	{
		printf("Memory allocation error!\n");
		return NULL;
	}

	strcpy(c->CityName, CityName);
	c->Population = Population;
	c->Left = NULL;
	c->Right = NULL;

	return c;
}

Tree CompletelyTree(Tree root, Tree newCity)
{
	Tree temp = NULL;
	int result = 0;

	if (!root)
	{
		root = newCity;
	}

	else if (newCity->Population > root->Population)
	{
		root->Right = CompletelyTree(root->Right, newCity);
	}

	else if (newCity->Population < root->Population)
	{
		root->Left = CompletelyTree(root->Left, newCity);
	}
	else
	{
		if (strcmp(root->CityName, newCity->CityName) < 0)
		{
			root->Right = CompletelyTree(root->Right, newCity);
		}

		else if (strcmp(root->CityName, newCity->CityName) > 0)
		{
			root->Left = CompletelyTree(root->Left, newCity);
		}
		else {
			free(newCity);
		}
	}

	return root;
}

int Inorder(Tree root)
{
	if (root == NULL)
		return 0;

	Inorder(root->Right);
	printf("%s %d", root->CityName, root->Population);
	Inorder(root->Left);

	return 0;
}
int readCityFromFile(Position country, char* CountryFileName)
{
	Position CountryName = NULL;
	char CityName[MAX_SIZE] = "";
	int Population;
	Tree newCity = NULL;

	FILE* fp = fopen(CountryFileName, "r+");

	if (fp == NULL)
	{
		printf("File didn't open!\n");
		return FILE_DIDNT_OPEN_ERROR;
	}

	while (!feof(fp))
	{
		fscanf(fp, " %s  %d\n", CityName, &Population);
		newCity = MakeTree(CityName, Population);
		country->root = CompletelyTree(country->root, newCity);
	}

	fclose(fp);
	return 0;
}

int Preorder(Tree root) {
	if (root == NULL)
	{
		return 0;
	}
	Preorder(root->Left);
	printf("\t\t-> %s\n", root->CityName);
	printf("\t\tPOPULATION: %d\n", root->Population);
	Preorder(root->Right);

	return EXIT_SUCCESS;
}

int calculateIndex(char* CountryName)
{
	int len = 0;
	int sum = 0;
	int suma = 0;
	int i = 0;

	len = strlen(CountryName);

	if (CountryName == NULL) {
		return 0;
	}

	if (len < 5)
	{
		len = len;
	}
	else
	{
		len = 5;
	}

	for (i = 0; i < len; i++)
	{
		sum += CountryName[i];
	}

	return sum % HASH_TABLE_SIZE;
}