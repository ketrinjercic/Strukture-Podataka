#define _CRT_SECURE_NO_WARNINGS //zbog upozorenja na funkcije scanf i fscanf
#include<stdio.h>
#include<stdlib.h>
#define LINE 30 //maksimalno 29 znakova i jedan za \0

typedef struct { //početak definicije strukture 
	char name[LINE];
	char lastName[LINE];
	int points;
} Student; //kraj definicije, dajemo naziv strukturi Student

int main() {


	FILE* fp = fopen("students.txt", "r"); //deklaracija pokazivača na datoteku i otvaranje te datoteke za čitanje
	if (!fp) { //ne može se otvoriti datoteka
		printf("Greska pri otvaranju datoteke.\n"); 
		return 1;
	}
	printf("Datoteka uspjesno otvorena!\n"); //provjera

	int numberOfRows = 0; //brojač uvijek postaviti na 0!
	char name[LINE], lastName[LINE];
	int points;

	while (fscanf(fp, "%29s %29s %d", name, lastName, &points) == 3) { //čita jedan red dok uspije pročitati 3 vrijednosti
		printf("Procitao: %s %s %d\n", name, lastName, points); //provjera je li uspješno pročitano
		numberOfRows++;
	}
	rewind(fp); //vraćanje pokazivača na početak datoteke

	Student* students = malloc(numberOfRows * sizeof(Student)); //dinamička alokacija memorije za niz struktura Student
	if (students == NULL) { //provjera
		printf("Greska! Alokacija memorije nije uspjela.\n");
		return 2;
	}

	for (int i = 0; i < numberOfRows; i++) { //čitanje podataka iz datoteke i spremanje u niz struktura red po red
		fscanf(fp, "%29s %29s %d",
			students[i].name,
			students[i].lastName,
			&students[i].points);
	}

	int maxPoints; //na početku nije definiran maksimalan broj bodova, pa ga korisnik unosi
	printf("Unesi maksimalan broj bodova na kolokviju: ");
	scanf("%d", &maxPoints);

	for (int i = 0; i < numberOfRows; i++) { //računanje apsolutnog i relativnog broja bodova
		float relative = (float)students[i].points / maxPoints * 100;

		printf("%s %s - apsolutni: %d, relativni: %.2f%%\n", //ispis podataka
			students[i].name,
			students[i].lastName,
			students[i].points,
			relative);
	}
	free(students); //oslobađanje memorije!!!
	fclose(fp); //zatvaranje datoteke!!!

	return 0;
}