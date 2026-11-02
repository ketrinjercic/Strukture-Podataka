#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// struktura jednog člana polinoma
typedef struct Poly {
    int coeff;// koeficijent člana
    int power;// eksponent člana
    struct Poly* next;// pokazivač na sljedeći član u listi
} Poly;

Poly* createHead() { // funkcija stvara head čvor
    Poly* head = malloc(sizeof(Poly)); // alociramo memoriju za head
    head->coeff = 0; 
    head->power = 0; 
    head->next = NULL;// lista je prazna
    return head;
}

//stvaranje novog čvora u listi
Poly* createElement(int coeff, int power) { // funkcija prima koeficijent i eksponent
    Poly* element = malloc(sizeof(Poly)); // alociramo memoriju za novi čvor
    element->coeff = coeff; // spremamo koeficijent
    element->power = power; // spremamo eksponent
    element->next = NULL; // novi čvor još ne pokazuje ni na što
    return element;
}

//sortirani unos
Poly* insertSorted(Poly* head, int coeff, int power) { // vraća head nakon umetanja
    Poly* newElement = createElement(coeff, power); // stvorimo novi čvor

    Poly* curr = head; // krećemo od head-a

    while (curr->next != NULL && curr->next->power > power) {
        curr = curr->next; // pomičemo se kroz listu
    }

    if (curr->next != NULL && curr->next->power == power) {
        curr->next->coeff += coeff; // zbrojimo koeficijente

        if (curr->next->coeff == 0) { // ako koeficijent postane 0
            Poly* temp = curr->next; // spremimo čvor koji brišemo
            curr->next = temp->next; // preskačemo ga
            free(temp); // oslobađamo memoriju
        }

        free(newElement); // novi čvor nam ne treba
        return head;
    }

    newElement->next = curr->next; // novi čvor pokazuje na sljedeći
    curr->next = newElement; // curr sada pokazuje na novi čvor

    return head;
}

//čitanje polinoma iz datoteke
Poly* readPoly(const char* filename, Poly* head) { 
    FILE* f = fopen(filename, "r"); // otvaramo datoteku

    if (!f) { // ako se ne može otvoriti
        printf("Greska pri otvaranju datoteke.\n"); 
        return head;
    }

    int c, p; // koeficijent i eksponent

    while (fscanf(f, "%d %d", &c, &p) == 2) { // čitamo parove koef i eksp
        head = insertSorted(head, c, p); // ubacujemo sortirano
    }

    fclose(f); // zatvaramo datoteku
    return head;
}


//ispis polinoma
Poly* printPoly(Poly* head) {
    Poly* curr = head->next;// prvi pravi član

    if (!curr) { // ako je lista prazna
        printf("0\n"); // ispiši 0
        return head;
    }

    while (curr) { // prolazimo kroz listu
        printf("%d*x^%d", curr->coeff, curr->power); // ispis člana

        if (curr->next != NULL) // ako postoji sljedeći član
            printf(" + "); // dodaj plus

        curr = curr->next; // idi dalje
    }

    printf("\n");
    return head;
}

//zbrajanje polinoma
Poly* addPoly(Poly* A, Poly* B) {
    Poly* result = createHead(); // rezultat počinje praznim head-om

    Poly* p = A->next; // prvi član A
    Poly* q = B->next; // prvi član B

    while (p && q) { // dok oba imaju članove
        if (p->power > q->power) { // A ima veći eksponent
            result = insertSorted(result, p->coeff, p->power);
            p = p->next;
        }
        else if (p->power < q->power) { // B ima veći eksponent
            result = insertSorted(result, q->coeff, q->power);
            q = q->next;
        }
        else { // isti eksponent
            result = insertSorted(result, p->coeff + q->coeff, p->power);
            p = p->next;
            q = q->next;
        }
    }

    while (p) { // ostatak A
        result = insertSorted(result, p->coeff, p->power);
        p = p->next;
    }

    while (q) { // ostatak B
        result = insertSorted(result, q->coeff, q->power);
        q = q->next;
    }

    return result; // vraćamo rezultat
}

//množenje polinoma
Poly* multiplyPoly(Poly* A, Poly* B) {
    Poly* result = createHead();// rezultat počinje praznim head-om

    Poly* p = A->next; // prvi član A

    while (p) { // za svaki član A
        Poly* q = B->next; // prolazimo kroz B

        while (q) { // za svaki član B
            int newCoeff = p->coeff * q->coeff;   // množenje koeficijenata
            int newPower = p->power + q->power;   // zbrajanje eksponenata

            result = insertSorted(result, newCoeff, newPower); // ubacujemo rezultat

            q = q->next; // idemo na sljedeći član B
        }

        p = p->next; // idemo na sljedeći član A
    }

    return result; // vraćamo rezultat
}

//oslobađanje memorije
Poly* freePoly(Poly* head) {
    Poly* curr = head;// krećemo od head-a

    while (curr) { // prolazimo kroz sve čvorove
        Poly* temp = curr; // spremimo trenutni čvor
        curr = curr->next;// idemo na sljedeći
        free(temp);// oslobađamo trenutni
    }

    return NULL; // vraćamo NULL jer je lista obrisana
}

int main() {
    Poly* A = createHead();// stvaramo polinom A
    Poly* B = createHead();// stvaramo polinom B

    A = readPoly("poly1.txt", A);// čitamo A iz datoteke
    B = readPoly("poly2.txt", B);// čitamo B iz datoteke

    printf("Polinom A: ");
    printPoly(A);

    printf("Polinom B: ");
    printPoly(B);

    Poly* Sum = addPoly(A, B);// zbrajanje
    Poly* Prod = multiplyPoly(A, B);// množenje

    printf("Zbroj: ");
    printPoly(Sum);

    printf("Umnozak: ");
    printPoly(Prod);

    A = freePoly(A);// oslobađanje memorije
    B = freePoly(B);
    Sum = freePoly(Sum);
    Prod = freePoly(Prod);

    return 0;
}
