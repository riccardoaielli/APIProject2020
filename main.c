#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *leggi_riga();
void change(int ind1, int ind2);
void print(int ind1, int ind2);
void inserisci(char **arraydains, int lungarraydains, int ind1, int ind2, char lc);
void undo(int nundonow);
void redo(int nundonow);
void costruisciarray();
char **changeundo(int ind1, int ind2, char **arrayundo, char **arraychangeundo);
void svuotalista();
void stampalista();

typedef struct node{
    char **array;
    int lungarraydains;
    char lc;
    int ind1;
    int ind2;
    struct node *prev;
    struct node *next;
} t_node;

struct node *head=NULL;
struct node *attuale=NULL;
struct node *headhead=NULL;

char **array;
int lungarray=0;

int main() {

    char *com;
    int lungcom, ind1, ind2, lungarrayvecchio, i, k;
    int lungarraydains;
    char **arrayvecchio, **arraydains;

    array=NULL;
    arrayvecchio=NULL;
    arraydains=NULL;

    array=calloc(1, sizeof(char**));

    headhead = malloc(sizeof(t_node));

    headhead->array = NULL;
    headhead->lungarraydains = 0;
    headhead->ind1 = 0;
    headhead->ind2 = 0;
    headhead->lc = 't';
    headhead->next = NULL;
    headhead->prev = NULL;

    head = malloc(sizeof(t_node));

    head->array = array;
    head->lungarraydains = 0;
    head->ind1 = 0;
    head->ind2 = 0;
    head->lc = 'i';
    head->next = NULL;
    head->prev = headhead;

    headhead->next = head;

    attuale = headhead;

    while(1!=0) {

        com = leggi_riga();
        lungcom = strlen(com);

        //OK
        //Gestisco Q
        if (com[lungcom - 1] == 'q') {

            return 0;
        }

        if (com[lungcom - 1] == 'f') {
            stampalista();
        }

        //Gestisco C
        if (com[lungcom - 1] == 'c') {
            sscanf(com, "%d,%d%*s", &ind1, &ind2);

            svuotalista();

            if(ind2>lungarray) {
                lungarray=ind2;
                array=realloc(array, ind2*sizeof(char**));
            }

            change(ind1, ind2);

            //inserisco dati in struttura lista l'array da inserire è quello letto da stdin
            lungarraydains = ind2 - ind1 + 1;

            arraydains = malloc(sizeof(char **) * lungarraydains);
            for (i = ind1 - 1, k = 0; i < ind2; k++, i++) {
                arraydains[k] = array[i];
            }
            inserisci(arraydains, lungarraydains, ind1, ind2, 'c');
            //Fine inserimento struttura dati
        }

        //OK
        //Gestisco P
        if (com[lungcom - 1] == 'p') {
            sscanf(com, "%d,%d%*s", &ind1, &ind2);

            print(ind1, ind2);
        }

        //Gestisco D
        if (com[lungcom - 1] == 'd') {
            sscanf(com, "%d,%d%*s", &ind1, &ind2);

            svuotalista();

            if ((ind1==0&&ind2==0) || (ind1>lungarray&&ind2>lungarray)){
                arrayvecchio = array;
                lungarrayvecchio=lungarray;
                array=NULL;
                array=malloc(lungarray*sizeof(char**));
                for(i=0;i<lungarray;i++)
                    array[i]=arrayvecchio[i];
            }
            else {
                if(ind2>lungarray)
                    ind2=lungarray;
                if(ind1==0)
                    ind1=1;

                arrayvecchio = array;
                lungarrayvecchio = lungarray;
                lungarray = lungarrayvecchio - ind2 + ind1 - 1;
                array=NULL;
                array=malloc(lungarray*sizeof(char**));

                for(i=0;i<ind1-1;i++)
                    array[i]=arrayvecchio[i];
                for(k=i,i=ind2;i<lungarrayvecchio;i++,k++)
                    array[k]=arrayvecchio[i];

            }
            inserisci(arrayvecchio, lungarrayvecchio, ind1, ind2, 'd');
        }

        //OK
        //Gestisco U e R
        if ((com[lungcom - 1] == 'u') || (com[lungcom - 1] == 'r')) {
            sscanf(com, "%d%*s", &ind1);

            if (com[lungcom - 1] == 'u')
                undo(ind1);
            else if (com[lungcom - 1] == 'r')
                redo(ind1);

            //da verificare fare free array le faccio gia in costruisci array
            costruisciarray();
        }
    }
}

//OK
char *leggi_riga(){
    char *buffer, *riga;
    buffer=NULL;
    riga=NULL;
    buffer=malloc(sizeof(char)*1025);
    gets(buffer);
    riga=malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(riga,buffer);
    free(buffer);
    buffer=NULL;
    return(riga);
}

//OK
void change(int ind1, int ind2){
    int i;
    for(i=ind1-1; i<ind2; i++){
        array[i]=leggi_riga();
    }
    leggi_riga();
}

//OK
void print(int ind1, int ind2){
    int i;
    if(ind1==0){
        ind1++;
        puts(".");
    }
    for(i=ind1-1; i<ind2 ; i++){
        if(i<lungarray&&i<ind2)
            puts(array[i]);
        else puts(".");
    }
}

//OK
//inserisco in testa
void inserisci(char **arraydains, int lungarraydains, int ind1, int ind2, char lc){

    // creo il nuovo nodo e alloco uno spazio di memoria
    t_node *new_node;
    new_node = malloc(sizeof(t_node));

    // inizializzo il nuovo nodo e inserisco i nuovi dati
    new_node->array = arraydains;
    new_node->lungarraydains = lungarraydains;
    new_node->ind1 = ind1;
    new_node->ind2 = ind2;
    new_node->lc = lc;

    // CASO 2: la lista contiene già almeno un elemento
    new_node->prev = headhead;
    new_node->next = head;
    head->prev= new_node;
    head = new_node;
    headhead->next = head;

    attuale = headhead;
}

//OK
void undo(int nundonow){
    struct node *scorri;
    int i=0;
    scorri = attuale;
    while (i<nundonow){
        if(scorri->next->next == NULL) {
            attuale = scorri;
            return;
        }
        scorri = scorri->next;
        i++;
    }
    attuale = scorri;
}

//OK
void redo(int nundonow){
    struct node *scorri;
    int i=0;
    scorri = attuale;
    while (i<nundonow){
        if(scorri->prev == NULL) {
            attuale = scorri;
            return;
        }
        scorri = scorri->prev;
        i++;
    }
    attuale = scorri;
}

void costruisciarray() {

    struct node *scorri;
    int lungarrayundo, nuovoind1, nuovoind2, i, k, lungarrayvecchio;
    char **arrayundo=NULL;

    scorri = attuale;

    if (attuale->lc == 'd') {

        lungarrayundo = attuale->lungarraydains;
        arrayundo = malloc(lungarrayundo * sizeof(char **));
        for (i = 0; i < lungarrayundo; i++)
            arrayundo[i] = attuale->array[i];

        //salvo i valori corretti
        free(array);
        array=NULL;
        lungarray = lungarrayundo;
        array = arrayundo;
        return;
    }

    while ((scorri->lc != 'd') && (scorri->next != NULL)) {
        scorri = scorri->next;
    }
    //faccio la delete di arrayundo
    if (scorri->lc == 'd') {
        lungarrayundo = scorri->lungarraydains;

        if (!((scorri->ind1 == 0 && scorri->ind2 == 0) || (scorri->ind1 > lungarrayundo && scorri->ind2 > lungarrayundo))){

            nuovoind2 = scorri->ind2;
            nuovoind1 = scorri->ind1;

            if (nuovoind2 > lungarrayundo)
                nuovoind2 = lungarrayundo;
            if (nuovoind1 == 0)
                nuovoind1 = 1;

            lungarrayvecchio = lungarrayundo;
            lungarrayundo = lungarrayvecchio - nuovoind2 + nuovoind1 - 1;
            if(lungarrayundo==0){
                arrayundo = malloc(1 * sizeof(char **));
            }
            else {
                arrayundo = malloc(lungarrayundo * sizeof(char **));

                for (i = 0; i < nuovoind1 - 1; i++)
                    arrayundo[i] = scorri->array[i];
                for (k = i, i = nuovoind2; i < lungarrayvecchio; i++, k++)
                    arrayundo[k] = scorri->array[i];
            }
        }
        else {
            arrayundo = malloc(lungarrayundo * sizeof(char **));
            for (i = 0; i < lungarrayundo; i++)
                arrayundo[i] = scorri->array[i];
        }
    }
    else if(scorri->lungarraydains==0) {
            lungarrayundo = scorri->lungarraydains;
            arrayundo = malloc(sizeof(char **) * 1);
        }

    scorri = scorri->prev;
    while (scorri != attuale) {
        //faccio realloc se la change inserisce piu elementi della lunghezza del mio array
        if (scorri->ind2 > lungarrayundo) {
            lungarrayundo = scorri->ind2;
            arrayundo = realloc(arrayundo, lungarrayundo * sizeof(char **));
        }
        arrayundo = changeundo(scorri->ind1, scorri->ind2, arrayundo, scorri->array);
        scorri = scorri->prev;
    }
    //salvo i valori corretti
    free(array);
    array=NULL;
    lungarray = lungarrayundo;
    array = arrayundo;
}

char **changeundo (int ind1, int ind2, char **arrayundo, char **arraychangeundo) {
    int i, k;
    for (i = ind1 - 1, k = 0; i < ind2; k++, i++) {
        arrayundo[i] = arraychangeundo[k];
    }
    return arrayundo;
}

void svuotalista(){
    struct node *cancella=NULL;

    //printf("cancellazione -1");
    while(attuale->prev!=NULL){
        //printf("cancellazione 0");
        cancella = attuale->prev;
        //printf("cancellazione 1");
        attuale->prev = attuale->prev->prev;
        //printf("cancellazione 2");
        attuale->prev->next = attuale;
        //printf("cancellazione 3");
        free(cancella);
        //printf("cancellazione 4");
    }
    //printf("cancellazione lista effettuata");
    //stampalista();
    //if(attuale != headhead)
        //printf("errore cancellazione lista");

    //attuale = scorri;

}


void stampalista(){
    struct node *scorri=NULL;

    scorri = headhead;
    while(scorri->next!=NULL) {
        for (int i = 0; i < scorri->lungarraydains; i++)
            puts(scorri->array[i]);
        printf("%d\n", scorri->lungarraydains);
        printf("%d\n", scorri->ind1);
        printf("%d\n", scorri->ind2);
        printf("%c\n", scorri->lc);
        scorri = scorri->next;
    }
    scorri=NULL;

    scorri = headhead;
    while(scorri->next!=NULL) {
        for (int i = 0; i < scorri->lungarraydains; i++)
            puts(scorri->array[i]);
        printf("%d\n", scorri->lungarraydains);
        printf("%d\n", scorri->ind1);
        printf("%d\n", scorri->ind2);
        printf("%c\n", scorri->lc);
        scorri = scorri->next;
    }
    scorri=NULL;
}