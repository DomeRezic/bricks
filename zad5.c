#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS (0)
#define NAME_MAX (100)
#define PROGRAM_EXIT (-1)
#define ALOCC_POZ_FAIL (NULL)
#define ALOCC_FAIL (-1)
#define FILE_FAIL (-1)
#define BUFFER_MAX (200)
#define ZERODIVIDE (-1)

typedef struct account* Position;

struct account {
	float br;
	Position next;
};

typedef struct account account;

int readfile(char[NAME_MAX], Position);
Position createHead();
int addtoheap(int, Position);
int takefromheap(char, Position);
int elementwipe(Position);
int printout(Position);
int wipe(Position);

int main()
{
	int status = 0;
	Position head = NULL;
	head = createHead();

	if (head == NULL)
	{
		return PROGRAM_EXIT;
	}

	char nameDat[NAME_MAX] = { 0 };

	printf("\nUnesite ime datoteke: ");
	scanf(" %s", nameDat);

	status = readfile(nameDat, head);
	if (status != 0)
	{
		return PROGRAM_EXIT;
	}

	status = printout(head);
	if (status != 0)
	{
		return PROGRAM_EXIT;
	}

	status = wipe(head);
	if (status != 0)
	{
		return PROGRAM_EXIT;
	}

	return SUCCESS;
}

int readfile(char nameDat[NAME_MAX], Position p)
{
	int status = 0, n = 0;
	int tmp = 0, br = 0;
	char buffer[BUFFER_MAX] = { 0 };
	char* buf;
	char operacija = '\0';
	FILE* fp = NULL;
	fp = fopen(nameDat, "r");

	if (fp == NULL)
	{
		printf("\nGreska pri otvaranju datoteke!");
		return FILE_FAIL;
	}

	fgets(buffer, BUFFER_MAX, fp);
	buf = buffer;

	printf("\nPostfix izraz iscitan iz datoteke:\n%s", buffer);

	while (strlen(buf) != 0)
	{
		if (sscanf(buf, "%d", &br) == 1)
		{
			sscanf(buf, "%d %n", &tmp, &n);
			status = addtoheap(br, p);
			if (status != 0)
			{
				return FILE_FAIL;
			}
		}
		else
		{
			sscanf(buf, "%c %n", &operacija, &n);
			status = takefromheap(operacija, p);
			if (status != 0)
			{
				return FILE_FAIL;
			}
		}
		buf += n;
	}

	return SUCCESS;
}

Position createHead()
{
	Position head = NULL;
	head = (Position)malloc(sizeof(account));

	if (head == NULL)
	{
		printf("\nGreska pri alokaciji!");
		return ALOCC_POZ_FAIL;
	}

	head->next = NULL;

	return head;
}

int addtoheap(int br, Position p)
{
	Position q = NULL;
	q = (Position)malloc(sizeof(account));
	if (q == NULL)
	{
		printf("\nGreska pri alokaciji!");
		return ALOCC_FAIL;
	}

	q->next = p->next;
	p->next = q;

	q->br = (float)br;

	return SUCCESS;
}

int takefromheap(char operacija, Position p)
{
	float rezultat = 0;
	Position head = NULL;
	head = p;

	Position pret = NULL;
	p = p->next;
	pret = p->next;

	switch (operacija)
	{
	case '+':
	{
		rezultat = (pret->br) + (p->br);
		break;
	}
	case '*':
	{
		rezultat = (pret->br) * (p->br);
		break;
	}
	case '/':
	{
		if (p->br == 0) {
			printf("\nDijeljenje s nulom nije dozvoljeno!");
			return ZERODIVIDE;
		}
		else {
			rezultat = (pret->br) / (p->br);
			break;
		}
	}
	case '-':
	{
		rezultat = (pret->br) - (p->br);
		break;
	}
	}

	pret->br = rezultat;

	elementwipe(head);

	return SUCCESS;
}

int elementwipe(Position p)
{
	Position tmp = NULL;
	tmp = p->next;

	p->next = tmp->next;

	free(tmp);

	return SUCCESS;
}

int printout(Position p)
{
	printf("\nRezultat je %.2f", p->next->br);

	return SUCCESS;
}

int wipe(Position p)
{
	Position temp = NULL;

	while (p != NULL)
	{
		temp = p;
		p = p->next;
		free(temp);
	}

	return SUCCESS;
}