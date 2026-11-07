#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Stack* Position; //pokazivač na strukturu stack

//struktura čvora stoga
typedef struct Stack {
	int broj;  //sprema vrijednost broja u čvoru
	Position next;
}Stack;

//funkcija za dodavanje elemenata na vrh stoga / ubacuje se na početak iza head-a
int Push(Position head, int broj) {
	Position newP = NULL; //inicijaliziramo pokazivač na null

	newP = (Position)malloc(sizeof(Stack)); //alokacija memorije za novi čvor
	if (newP == NULL) { //provjera
		printf("Greska pri alokaciji memorije!\n");
		return -1;
	}

	newP->broj = broj;  //unos podataka u novi čvor

	//umetanje na početak liste
	newP->next = head->next; 
	head->next = newP;

	return 0;
}

//funkcija za uklanjanje elemenatra sa vrha stoga / uklanja se prvi element iza head-a
int Pop(Position head) {
	Position temp = NULL;  //pomoćni pokazivač za privremeno čuvanje čvora koji brišemo
	int rez = 0; //sprema broj koji se uklanja 

	//provjera je li stoh prazan
	if (head->next == NULL) {
		printf("Greska, stog je prazan!\n");
		return -1;
	}

	//brisanje
	temp = head->next;
	rez = temp->broj;
	
	head->next = temp->next;
	free(temp);

	return rez;
}

//funkcija za oslobađanje memorije stoga
int FreeStack(Position head) {
	Position temp = NULL;

	//vrti se sve dok ima elemenata u stogu
	while (head->next != NULL) {
		temp = head->next; //prvi element
		head->next = temp->next; //pomičemo pokazivač
		free(temp); //brišemo trenutni element iz memorije
	}

	return 0;
}

int main() {
	//incijalizacija head-a
	Stack head = { .broj = 0, .next = NULL };

	FILE* fp = NULL; //pokazivač na datoteku
	char buffer[100]; //string u koji čitamo dio po dio iz datoteke
	int rez = 0; //rezultat
	int op1 = 0, op2 = 0; //operandi skinuti sa stoga

	//otvaramo datoteku za čitanje
	fp = fopen("postfiks.txt", "r");
	if (fp == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		return 1;
	}
	//čitamo iz datoteke znak po znak
	while (fscanf(fp, "%s", buffer) == 1) {
		//provjera je li pročitani znak operator
		if (strcmp(buffer, "+") == 0 || strcmp(buffer, "-") == 0 ||
			strcmp(buffer, "*") == 0 || strcmp(buffer, "/") == 0) {

			//skidamo dva broja sa stoga
			op2 = Pop(&head);  //desni
			op1 = Pop(&head);  //lijevi

			//izvršavanje operacije ovisno o operatoru
			if (strcmp(buffer, "+") == 0) rez = op1 + op2;
			else if (strcmp(buffer, "-") == 0) rez = op1 - op2;
			else if (strcmp(buffer, "*") == 0) rez = op1 * op2;
			else if (strcmp(buffer, "/") == 0) {
				if (op2 == 0) { //provjera dijeljenja s nulom
					printf("Greska: Dijeljenje s nulom!\n");
					FreeStack(&head); //oslobađanje memorije
					fclose(fp); //zatvaranje datoteke
					return 1;
				}
				rez = op1 / op2;
			}
			
			Push(&head, rez); //vraćamo izračunati rezultat natrag na stog

		}
		else {
			//ako nije operator, pročitamo ga kao broj
			int broj = 0;
			if (sscanf(buffer, "%d", &broj) == 1) { //pretvaranje u broj

				Push(&head, broj);  //ako je uspješno, stavljamo broj na stog
			}
			else {
				//ako znak nije ni operator ni valjani broj
				printf("Greska: Krivi unos u datoteci (%s)!\n", buffer);
				FreeStack(&head); //oslobađanje memorije
				fclose(fp); //zatvaranje datoteke
				return 1;
			}
		}
	}

	fclose(fp); //zatvramo datoteku

	if (head.next != NULL) {  //konačni rezultat spremljen na samom vrhu stoga (iza head-a)
		printf("Konacni rezultat postfiks iznosa je: %d\n", head.next->broj);
	}
	else {
		printf("Greska, nema rezultata na stogu.\n");
	}

	FreeStack(&head); //čišćenje memorije stoga

	return 0;
}
