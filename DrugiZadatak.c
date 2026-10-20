#define _CRT_SECURE_NO_WARNINGS   // omogućuje korištenje scanf bez upozorenja u Visual Studiu
#include<stdio.h>    // uključuje funkcije za ispis i unos (printf, scanf)
#include<string.h>   // uključuje funkcije za rad sa stringovima (strcmp, strcpy)
#include<stdlib.h>   // uključuje malloc, free za dinamičku memoriju
#define LINE 40       // maksimalna duljina stringa name i surname

// struktura koja predstavlja jednu osobu
typedef struct {
    char name[LINE];   
    char surname[LINE];  
    int year;            
} Person;

// struktura koja predstavlja jedan čvor u povezanoj listi
typedef struct Element {
    Person person;       // podaci o osobi
    struct Element* next; // pokazivač na sljedeći element u listi
} Element;

// funkcija koja dodaje osobu u listu bez unosa (automatski)
Element* addPerson(Element* head, Person p) {
    Element* new = malloc(sizeof(Element)); // alocira novi čvor
    if (!new) return head;                  // ako nema memorije, vrati staru listu

    new->person = p;        // kopira podatke osobe u novi čvor
    new->next = head;       // novi čvor pokazuje na dosadašnju glavu
    return new;             // vraća novi čvor kao novu glavu liste
}

// funkcija koja dodaje osobu na početak liste uz unos korisnika
Element* addToBegg(Element* head) {
    Element* new = malloc(sizeof(Element)); // alocira novi čvor
    if (!new) return head;                  // provjera memorije

    printf("Ime: ");                         // traži unos imena
    scanf("%s", new->person.name);

    printf("Prezime: ");                     // traži unos prezimena
    scanf("%s", new->person.surname);

    printf("Godina rodenja: ");              // traži unos godine rođenja
    scanf("%d", &new->person.year);

    if (head == NULL) {                      // ako je lista prazna
        new->next = NULL;                    // novi čvor nema sljedeći
        return new;                          // novi čvor postaje glava
    }

    new->next = head;                        // novi čvor pokazuje na staru glavu
    return new;                              // vraća novi čvor kao glavu
}

// funkcija koja ispisuje sve elemente liste
int printList(Element* head) {
    int count = 0;            // brojač elemenata
    Element* p = head;        // pokazivač koji prolazi kroz listu

    while (p != NULL) {       // prolazi sve dok ne dođe do kraja liste
        printf("%s %s (%d)\n", p->person.name, p->person.surname, p->person.year);
        // ispisuje podatke o osobi
        p = p->next;           // prelazi na sljedeći čvor
        count++;               // povećava brojač
    }
    return count;              // vraća broj elemenata
}

// funkcija koja dodaje osobu na kraj liste uz unos korisnika
Element* addToEnd(Element* head) {
    Element* new = malloc(sizeof(Element)); // alocira novi čvor
    if (!new) return head;                  // provjera memorije

    printf("Ime: ");                         // unos imena
    scanf("%s", new->person.name);

    printf("Prezime: ");                     // unos prezimena
    scanf("%s", new->person.surname);

    printf("Godina rodenja: ");              // unos godine rođenja
    scanf("%d", &new->person.year);

    new->next = NULL;                        // novi čvor je zadnji u listi

    if (head == NULL)                       // ako je lista prazna
        return new;                         // novi čvor postaje glava

    Element* p = head;                      // pokazivač za prolazak liste
    while (p->next != NULL)                 // ide do zadnjeg čvora
        p = p->next;

    p->next = new;                          // zadnji čvor pokazuje na novi
    return head;                            // glava ostaje ista
}

// funkcija koja traži osobu po prezimenu
Element* findBySurname(Element* head, char* surname) {
    Element* p = head;                      // pokazivač za prolazak liste

    while (p) {                             // prolazi kroz listu
        if (!strcmp(p->person.surname, surname)) { // uspoređuje prezime
            printf("Pronadena osoba: %s %s (%d)\n",
                p->person.name, p->person.surname, p->person.year);
            return p;                       // vraća pronađeni čvor
        }
        p = p->next;                        // ide na sljedeći čvor
    }

    printf("Osoba nije pronadena.\n");      // ako nije pronađena
    return NULL;
}

// funkcija koja briše osobu po prezimenu
Element* deleteBySurname(Element* head, char* surname) {
    if (!head) return NULL;                 // ako je lista prazna

    if (strcmp(head->person.surname, surname) == 0) { // ako je prva osoba ta
        Element* temp = head;               // privremeni pokazivač
        head = head->next;                  // glava postaje sljedeći čvor
        free(temp);                         // briše stari prvi čvor
        return head;                        // vraća novu glavu
    }

    Element* p = head;                      // pokazivač za prolazak liste
    while (p->next != NULL && strcmp(p->next->person.surname, surname) != 0)
        p = p->next;                        // traži čvor prije onog koji treba obrisati

    if (p->next != NULL) {                  // ako je pronađen čvor za brisanje
        Element* temp = p->next;            // čvor koji se briše
        p->next = temp->next;               // preskače se taj čvor
        free(temp);                         // briše se iz memorije
    }

    return head;                            // vraća glavu liste
}

int main() {
    Element* head = NULL;                   // lista je prazna na početku

    // automatski dodani članovi
    Person p1 = { "Julia", "Martinic", 2006 };
    Person p2 = { "Ketrin", "Jercic", 2007 };
    Person p3 = { "Eva", "Capkovic", 2008 };

    head = addPerson(head, p1);             
    head = addPerson(head, p2);             
    head = addPerson(head, p3);             

    printf("Dodavanje na pocetak:\n");
    head = addToBegg(head);                 // dodaje novu osobu na početak

    printf("Dodavanje na kraj:\n");
    head = addToEnd(head);                  // dodaje novu osobu na kraj

    printf("Ispis liste:\n");
    printList(head);                        // ispisuje cijelu listu

    char surname[LINE];                     // buffer za prezime

    printf("Unesi prezime za pretragu: ");
    scanf("%s", surname);                   // unos prezimena za pretragu
    findBySurname(head, surname);           // traži osobu

    printf("Unesi prezime za brisanje:");
    scanf("%s", surname);                   // unos prezimena za brisanje
    head = deleteBySurname(head, surname);  // briše osobu i vraća novu glavu

    printf("Lista nakon brisanja:\n");
    printList(head);                        // ispisuje listu nakon brisanja

    return 0;                               // kraj programa
}
