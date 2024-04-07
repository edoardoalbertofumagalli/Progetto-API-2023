#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEICOLI 512
#define LUNG_INPUT 20

// struttura BST stazioni nell'autostrada
typedef struct nodoAutostrada_
{
    int distanza;
    int stazione[MAX_VEICOLI];
    int numVeicoli;
    int step;
    struct nodoAutostrada_ *passaggio;
    struct nodoAutostrada_ *left;
    struct nodoAutostrada_ *right;
} nodoAutostrada_t;

// dichiaro le funzioni
nodoAutostrada_t *aggiungiStazione(nodoAutostrada_t *, int);
nodoAutostrada_t *creaStazione(int, int);

nodoAutostrada_t *demolisciStazione(nodoAutostrada_t *, int);
nodoAutostrada_t *cercaMinimo(nodoAutostrada_t *);

nodoAutostrada_t *aggiungiAuto(nodoAutostrada_t *, int, int);
int inserisciInOrdine(int[], int, int);

nodoAutostrada_t *rottamaAuto(nodoAutostrada_t *, int, int);
int binarySearch(int[], int, int, int);
int rimuoviElemento(int[], int, int);

nodoAutostrada_t *pianificaPercorso(nodoAutostrada_t *, int, int);
void percorsoMigliore(nodoAutostrada_t *, nodoAutostrada_t *, nodoAutostrada_t *);
void percorsoInversoMigliore(nodoAutostrada_t *, nodoAutostrada_t *, nodoAutostrada_t *);

nodoAutostrada_t *selezionaStazione(nodoAutostrada_t *, int);
nodoAutostrada_t *cercaStazioneSuccessiva(nodoAutostrada_t *, nodoAutostrada_t *);
nodoAutostrada_t *cercaStazionePrecedente(nodoAutostrada_t *, nodoAutostrada_t *);
nodoAutostrada_t *minValue(nodoAutostrada_t *);
nodoAutostrada_t *maxValue(nodoAutostrada_t *);

// funzione main
int main(int argc, char *argv[])
{
    char input[LUNG_INPUT];
    nodoAutostrada_t *radiceAutostrada = NULL;
    int distanza = 0, autonomia = 0, opzione = 0;
    int partenza = 0, arrivo = 0;
    int s = 0;

    opzione = 1;

    while (scanf("%s", input) != EOF)
    {

        opzione = strlen(input);

        if (input[0] == 'd')
            opzione++;

        switch (opzione)
        {
        case 17: // aggiungi-stazione
            s = scanf("%d", &distanza);
            radiceAutostrada = aggiungiStazione(radiceAutostrada, distanza);
            break;
        case 19: // demolisci-stazione
            s = scanf("%d", &distanza);
            radiceAutostrada = demolisciStazione(radiceAutostrada, distanza);
            break;
        case 13: // aggiungi-auto
            s = scanf("%d", &distanza);
            s = scanf("%d", &autonomia);
            radiceAutostrada = aggiungiAuto(radiceAutostrada, distanza, autonomia);
            break;
        case 12: // rottama-auto
            s = scanf("%d", &distanza);
            s = scanf("%d", &autonomia);
            radiceAutostrada = rottamaAuto(radiceAutostrada, distanza, autonomia);
            break;
        case 18: // pianifica-percorso
            s = scanf("%d", &partenza);
            s = scanf("%d", &arrivo);
            radiceAutostrada = pianificaPercorso(radiceAutostrada, partenza, arrivo);
            break;
        }
    }

    s = s + 1;

    return 0;
}

// alloco la nuova stazione nello heap
nodoAutostrada_t *creaStazione(int distanza, int numVeicoli)
{
    nodoAutostrada_t *nuovaStazione;
    int i = 0, j = 0, autonomia = 0;
    int s = 0;

    // alloco la memoria per la nuova stazione
    nuovaStazione = malloc(sizeof(nodoAutostrada_t));

    // inizializzo distanza e numero di veicoli
    nuovaStazione->distanza = distanza;
    nuovaStazione->numVeicoli = numVeicoli;
    nuovaStazione->step = -1;

    // inizializzo l'array tutto a 0
    for (i = 0; i < MAX_VEICOLI; i++)
        nuovaStazione->stazione[i] = 0;

    // inserisco i veicoli nell'array in ordine
    for (i = 0; i < numVeicoli; i++)
    {
        s = scanf("%d", &autonomia);
        for (j = i - 1; j >= 0 && nuovaStazione->stazione[j] > autonomia; j--)
            nuovaStazione->stazione[j + 1] = nuovaStazione->stazione[j];

        nuovaStazione->stazione[j + 1] = autonomia;
    }

    nuovaStazione->passaggio = NULL;

    // inizializzo i figli a NULL
    nuovaStazione->left = NULL;
    nuovaStazione->right = NULL;

    s = s + 1;

    return nuovaStazione;
}

// aggiungo la stazione al BST, ritorno la radice del BST
nodoAutostrada_t *aggiungiStazione(nodoAutostrada_t *radiceAutostrada, int distanza)
{
    int numVeicoli = 0;
    int s = 0;

    // controllo che non esista una stazione a questa distanza
    if (radiceAutostrada != NULL && radiceAutostrada->distanza == distanza)
    {
        printf("non aggiunta\n");
    }
    // se non la trovo chiamo creaStazione e la inserisco nel BST
    else if (radiceAutostrada == NULL)
    {
        // non ci sono stazioni nell'autostrada
        s = scanf("%d", &numVeicoli);
        radiceAutostrada = creaStazione(distanza, numVeicoli);
        printf("aggiunta\n");
    }
    else if (distanza <= radiceAutostrada->distanza)
    {
        // se la stazione da aggiungere si trova prima, vado nel sottoalbero sx
        radiceAutostrada->left = aggiungiStazione(radiceAutostrada->left, distanza);
    }
    else
    {
        // altrimenti vado nel sottoalberto dx
        radiceAutostrada->right = aggiungiStazione(radiceAutostrada->right, distanza);
    }

    s = s + 1;
    // ritorno la radice del BST
    return radiceAutostrada;
}

// trova la stazione più vicina al punto di partenza all'interno del sottoalbero
nodoAutostrada_t *cercaMinimo(nodoAutostrada_t *radiceAutostrada)
{
    while (radiceAutostrada->left != NULL)
    {
        radiceAutostrada = radiceAutostrada->left;
    }

    return radiceAutostrada;
}

// rimuovo la stazione dal BST, ritorno la radice del BST
nodoAutostrada_t *demolisciStazione(nodoAutostrada_t *radiceAutostrada, int distanza)
{
    int i = 0;

    // cerco la stazione da demolire
    if (radiceAutostrada == NULL)
    {
        printf("non demolita\n");
        return radiceAutostrada;
    }
    else if (distanza < radiceAutostrada->distanza)
    {
        radiceAutostrada->left = demolisciStazione(radiceAutostrada->left, distanza);
    }
    else if (distanza > radiceAutostrada->distanza)
    {
        radiceAutostrada->right = demolisciStazione(radiceAutostrada->right, distanza);
        // ho trovato la stazione da denolire
    }
    else
    {
        // caso 1: non ha figli
        if (radiceAutostrada->left == NULL && radiceAutostrada->right == NULL)
        {
            free(radiceAutostrada);
            radiceAutostrada = NULL;
            printf("demolita\n");
        }
        // caso 2: ha un figlio
        else if (radiceAutostrada->left == NULL)
        {
            nodoAutostrada_t *tmp = radiceAutostrada;
            radiceAutostrada = radiceAutostrada->right;
            free(tmp);
            printf("demolita\n");
        }
        else if (radiceAutostrada->right == NULL)
        {
            nodoAutostrada_t *tmp = radiceAutostrada;
            radiceAutostrada = radiceAutostrada->left;
            free(tmp);
            printf("demolita\n");
        }
        // caso 3: 2 figli
        else
        {
            nodoAutostrada_t *tmp = cercaMinimo(radiceAutostrada->right);
            radiceAutostrada->distanza = tmp->distanza;
            radiceAutostrada->numVeicoli = tmp->numVeicoli;
            for (i = 0; i < tmp->numVeicoli; i++)
                radiceAutostrada->stazione[i] = tmp->stazione[i];
            radiceAutostrada->right = demolisciStazione(radiceAutostrada->right, tmp->distanza);
        }
    }

    // ritorno la radice del BST
    return radiceAutostrada;
}

// aggiungo un veicolo alla stazione indicata con la relativa autonomia in ordine
nodoAutostrada_t *aggiungiAuto(nodoAutostrada_t *radiceAutostrada, int distanza, int autonomia)
{
    // cerco la stazione in cui aggiungere il veicolo

    // caso in cui la stazione non esiste
    if (radiceAutostrada == NULL)
    {
        printf("non aggiunta\n");
        return radiceAutostrada;
    }
    else if (distanza < radiceAutostrada->distanza)
    {
        radiceAutostrada->left = aggiungiAuto(radiceAutostrada->left, distanza, autonomia);
    }
    else if (distanza > radiceAutostrada->distanza)
    {
        radiceAutostrada->right = aggiungiAuto(radiceAutostrada->right, distanza, autonomia);
    }
    else // ho trovato la stazione
    {
        // aggiungo l'auto all'array della stazione scelta
        radiceAutostrada->numVeicoli = inserisciInOrdine(radiceAutostrada->stazione, radiceAutostrada->numVeicoli, autonomia);
    }

    return radiceAutostrada;
}

// quando aggiungo un veicolo, lo metto nella posizione corretta in ordine di autonomia
int inserisciInOrdine(int stazione[], int numVeicoli, int autonomia)
{
    int i = 0;

    // caso in cui eccedo il numero massimo di veicoli
    if (numVeicoli >= MAX_VEICOLI)
    {
        printf("non aggiunta\n");
        return numVeicoli;
    }

    // aggiungo l'auto nella posizione corretta
    for (i = numVeicoli - 1; i >= 0 && stazione[i] > autonomia; i--)
        stazione[i + 1] = stazione[i];

    stazione[i + 1] = autonomia;

    printf("aggiunta\n");

    return (numVeicoli + 1);
}

// faccio binary search per trovare il veicolo da demolire
int binarySearch(int stazione[], int low, int high, int autonomia)
{
    int mid = 0;

    mid = (low + high) / 2;

    if (high < low)
        return -1;

    if (autonomia == stazione[mid])
        return mid;

    if (autonomia > stazione[mid])
        return binarySearch(stazione, (mid + 1), high, autonomia);

    return binarySearch(stazione, low, (mid - 1), autonomia);
}

// funzione per rimuoevere un elemento da un array ordinato
int rimuoviElemento(int stazione[], int numVeicoli, int autonomia)
{
    int i = 0, posizione = 0;

    // cerco l'auto da demolire nella stazione
    posizione = binarySearch(stazione, 0, numVeicoli - 1, autonomia);

    // non ho trovato il veicolo
    if (posizione == -1)
    {

        printf("non rottamata\n");
        return numVeicoli;
    }

    if (numVeicoli == 1)
    {
        stazione[0] = 0;
        printf("rottamata\n");
        return (numVeicoli - 1);
    }

    // cancello l'elemento
    for (i = posizione; i < numVeicoli - 1; i++)
        stazione[i] = stazione[i + 1];

    printf("rottamata\n");

    return (numVeicoli - 1);
}

// funzione per demolire un'auto
nodoAutostrada_t *rottamaAuto(nodoAutostrada_t *radiceAutostrada, int distanza, int autonomia)
{

    // cerco la stazione in cui rottamare il veicolo

    // caso in cui la stazione non esiste
    if (radiceAutostrada == NULL)
    {
        printf("non rottamata\n");
        return radiceAutostrada;
    }
    else if (distanza < radiceAutostrada->distanza)
    {
        radiceAutostrada->left = rottamaAuto(radiceAutostrada->left, distanza, autonomia);
    }
    else if (distanza > radiceAutostrada->distanza)
    {
        radiceAutostrada->right = rottamaAuto(radiceAutostrada->right, distanza, autonomia);
    }
    else // ho trovato la stazione
    {
        // rimuovo il veicolo con l'autonomia cercata
        radiceAutostrada->numVeicoli = rimuoviElemento(radiceAutostrada->stazione, radiceAutostrada->numVeicoli, autonomia);
    }

    return radiceAutostrada;
}

// funzione per determinare il percorso migliore da partenza ad arrivo
nodoAutostrada_t *pianificaPercorso(nodoAutostrada_t *radiceAutostrada, int partenza, int arrivo)
{

    // trovo la stazione di partenza e quella di arrivo
    nodoAutostrada_t *stazionePartenza, *stazioneArrivo;
    stazionePartenza = NULL;
    stazioneArrivo = NULL;

    stazionePartenza = selezionaStazione(radiceAutostrada, partenza);
    stazioneArrivo = selezionaStazione(radiceAutostrada, arrivo);

    // controllo che la stazione di partenza e di arrivo esistano
    if (stazionePartenza != NULL && stazioneArrivo != NULL)
    {

        // caso in cui la stazione di partenza si trova prima di quella di arrivo
        if (partenza < arrivo)
        {
            percorsoMigliore(radiceAutostrada, stazionePartenza, stazioneArrivo);
        }

        // caso in cui la stazione di partenza si trova dopo quella di arrivo
        else if (partenza > arrivo)
        {
            percorsoInversoMigliore(radiceAutostrada, stazionePartenza, stazioneArrivo);
        }
    }
    else
        printf("nessun percorso\n");

    return radiceAutostrada;
}

void percorsoMigliore(nodoAutostrada_t *radiceAutostrada, nodoAutostrada_t *stazionePartenza, nodoAutostrada_t *stazioneArrivo)
{
    int i = 0, j = 0;
    nodoAutostrada_t *stazioneSuccessiva;
    nodoAutostrada_t *stazioneCorrente;
    nodoAutostrada_t *sequenza[50000];

    stazioneSuccessiva = NULL;
    stazioneCorrente = NULL;

    for (i = 0; i < 50000; i++)
        *(sequenza + i) = NULL;

    i = 0;

    stazioneCorrente = stazionePartenza;
    while (stazioneCorrente->distanza != stazioneArrivo->distanza)
    {
        *(sequenza + i) = stazioneCorrente;
        stazioneCorrente = cercaStazioneSuccessiva(radiceAutostrada, stazioneCorrente);
        i++;
    }

    *(sequenza + i) = stazioneCorrente;

    i = 0;
    j = i + 1;

    stazioneCorrente = *(sequenza);
    stazioneSuccessiva = *(sequenza + j);

    // ciclo fino a che la stazione corrente non corrisponde a quella antecedente l'arrivo
    while (stazioneCorrente->distanza != stazioneArrivo->distanza)
    {
        int arrivato = 0;

        // ciclo fino a che raggiungo la stazione successiva
        while (stazioneCorrente->numVeicoli > 0 && stazioneCorrente->stazione[stazioneCorrente->numVeicoli - 1] >= stazioneSuccessiva->distanza - stazioneCorrente->distanza)
        {
            // se non ho ancora assegnato passaggio, lo aggiorno
            if (stazioneSuccessiva->passaggio == NULL)
                stazioneSuccessiva->passaggio = stazioneCorrente;

            // se sono arrivato in fondo esco
            if (stazioneSuccessiva == stazioneArrivo)
            {
                stazioneCorrente = stazioneSuccessiva;
                arrivato = 1;
                break;
            }

            // aggiorno successiva
            j++;
            stazioneSuccessiva = *(sequenza + j);
        }

        // se non sono arrivato aggiorno corrente e successiva
        if (!arrivato)
        {
            i++;
            j = i + 1;
            stazioneCorrente = *(sequenza + i);
            stazioneSuccessiva = *(sequenza + j);
        }
        else if (arrivato || stazioneCorrente->passaggio == NULL)
            break;
    }

    // se sono arrivato a scorrere fino alla stazione di arrivo, verifico se esiste il percorso e nel caso lo stampo
    if (stazioneCorrente->distanza == stazioneArrivo->distanza)
    {
        int stazioniPassaggio[100];
        int i = 0, j = 0, numStazioniPassaggio = 0;

        // ciclo fino a tornare alla stazione di partenza o fino a che non posso più retrocedere
        while (stazioneCorrente != NULL && stazioneCorrente->passaggio != NULL)
        {
            stazioniPassaggio[i] = stazioneCorrente->distanza;
            stazioneCorrente = selezionaStazione(radiceAutostrada, stazioneCorrente->passaggio->distanza);
            i++;
        }

        stazioniPassaggio[i] = stazioneCorrente->distanza;

        if (stazioneCorrente->distanza != stazionePartenza->distanza)
        {
            printf("nessun percorso\n");
        }
        else
        {
            numStazioniPassaggio = i;

            for (j = 0; j <= numStazioniPassaggio; j++)
            {
                printf("%d ", stazioniPassaggio[i]);
                i--;
            }
            printf("\n");
        }
    }
    // altrimenti non esiste nessun percorso
    else
    {
        printf("nessun percorso\n");
    }

    // pulisco tutti i puntatori a passaggio DOVREI PULIRE SOLO QUELLI CHE HO USATO
    while (stazionePartenza != stazioneArrivo)
    {
        stazionePartenza->passaggio = NULL;
        stazionePartenza = cercaStazioneSuccessiva(radiceAutostrada, stazionePartenza);
    }

    stazionePartenza->passaggio = NULL;
}

void percorsoInversoMigliore(nodoAutostrada_t *radiceAutostrada, nodoAutostrada_t *stazionePartenza, nodoAutostrada_t *stazioneArrivo)
{
    int i = 0, j = 0;
    nodoAutostrada_t *stazioneCorrente;
    nodoAutostrada_t *stazionePrecedente;
    nodoAutostrada_t *sequenza[50000];

    stazioneCorrente = NULL;
    stazionePrecedente = NULL;

    for (i = 0; i < 50000; i++)
        *(sequenza + i) = NULL;

    i = 0;

    stazioneCorrente = stazionePartenza;
    while (stazioneCorrente->distanza != stazioneArrivo->distanza)
    {
        *(sequenza + i) = stazioneCorrente;
        stazioneCorrente = cercaStazionePrecedente(radiceAutostrada, stazioneCorrente);
        i++;
    }

    *(sequenza + i) = stazioneCorrente;

    i = 0;
    j = i + 1;

    stazioneCorrente = *(sequenza);
    stazionePrecedente = *(sequenza + j);

    stazioneCorrente->step = 0;

    // ciclo fino a che la stazione corrente non corrisponde a quella di arrivo
    while (stazioneCorrente->distanza != stazioneArrivo->distanza)
    {
        while (stazioneCorrente->numVeicoli > 0 && stazioneCorrente->stazione[stazioneCorrente->numVeicoli - 1] >= stazioneCorrente->distanza - stazionePrecedente->distanza)
        {
            if (stazionePrecedente->step == -1)
            {
                stazionePrecedente->passaggio = stazioneCorrente;
                stazionePrecedente->step = stazioneCorrente->step + 1;
            }
            else
            {
                if (stazioneCorrente->step + 1 == stazionePrecedente->step && stazioneCorrente->distanza < stazionePrecedente->passaggio->distanza)
                {
                    stazionePrecedente->passaggio = stazioneCorrente;
                }
            }

            if (stazionePrecedente == stazioneArrivo)
                break;

            j++;
            stazionePrecedente = *(sequenza + j);
        }

        i++;
        j = i + 1;
        stazioneCorrente = *(sequenza + i);
        stazionePrecedente = *(sequenza + j);

        if (stazioneCorrente->passaggio == NULL)
            break;
    }

    if (stazioneCorrente->distanza == stazioneArrivo->distanza)
    {
        int stazioniPassaggio[100];
        int i = 0, j = 0, numStazioniPassaggio = 0;

        while (stazioneCorrente != NULL && stazioneCorrente->passaggio != NULL)
        {
            stazioniPassaggio[i] = stazioneCorrente->distanza;
            stazioneCorrente = selezionaStazione(radiceAutostrada, stazioneCorrente->passaggio->distanza);
            i++;
        }

        stazioniPassaggio[i] = stazioneCorrente->distanza;

        if (stazioneCorrente->distanza != stazionePartenza->distanza)
        {
            printf("nessun percorso\n");
        }
        else
        {
            numStazioniPassaggio = i;

            for (j = 0; j <= numStazioniPassaggio; j++)
            {
                printf("%d ", stazioniPassaggio[i]);
                i--;
            }
            printf("\n");
        }
    }
    else
    {
        printf("nessun percorso\n");
    }

    // pulisco tutti i puntatori a passaggio DOVREI PULIRE SOLO QUELLI CHE HO USATO
    while (stazionePartenza != stazioneArrivo)
    {
        stazionePartenza->passaggio = NULL;
        stazionePartenza->step = -1;
        stazionePartenza = cercaStazionePrecedente(radiceAutostrada, stazionePartenza);
    }

    stazionePartenza->passaggio = NULL;
    stazionePartenza->step = -1;
}

// funzione per selezionare la stazione di partenza e quella di arrivo
nodoAutostrada_t *selezionaStazione(nodoAutostrada_t *radiceAutostrada, int distanza)
{
    if (radiceAutostrada == NULL)
    {
        return NULL;
    }
    else if (radiceAutostrada->distanza == distanza)
    {
        return radiceAutostrada;
    }
    else if (distanza <= radiceAutostrada->distanza)
    {
        return selezionaStazione(radiceAutostrada->left, distanza);
    }
    else
    {
        return selezionaStazione(radiceAutostrada->right, distanza);
    }
}

// cerco la stazione successiva a stazioneCorrente e la ritorno
nodoAutostrada_t *cercaStazioneSuccessiva(nodoAutostrada_t *radiceAutostrada, nodoAutostrada_t *stazioneCorrente)
{
    nodoAutostrada_t *stazioneSuccessiva;

    stazioneSuccessiva = NULL;

    // se il sottoalbero dx della stazione corrente non è NULL allora il succ sta nel sottoalbero dx
    if (stazioneCorrente->right != NULL)
        return minValue(stazioneCorrente->right);

    // parto dalla radice e cerco la stazione successiva
    while (radiceAutostrada != NULL)
    {

        if (stazioneCorrente->distanza < radiceAutostrada->distanza)
        {
            stazioneSuccessiva = radiceAutostrada;
            radiceAutostrada = radiceAutostrada->left;
        }
        else if (stazioneCorrente->distanza > radiceAutostrada->distanza)
        {
            radiceAutostrada = radiceAutostrada->right;
        }
        else
            break;
    }

    return stazioneSuccessiva;
}

// cerco la stazione precedente e la ritorno
nodoAutostrada_t *cercaStazionePrecedente(nodoAutostrada_t *radiceAutostrada, nodoAutostrada_t *stazioneCorrente)
{
    nodoAutostrada_t *stazionePrecedente;

    stazionePrecedente = NULL;

    // se il sottoalbero sx della stazione corrente non è NULL allora il prec sta nel sottoalbero sx
    if (stazioneCorrente->left != NULL)
        return maxValue(stazioneCorrente->left);

    // parto dalla radice e cerco la stazione successiva
    while (radiceAutostrada != NULL)
    {

        if (stazioneCorrente->distanza > radiceAutostrada->distanza)
        {
            stazionePrecedente = radiceAutostrada;
            radiceAutostrada = radiceAutostrada->right;
        }
        else if (stazioneCorrente->distanza < radiceAutostrada->distanza)
        {
            radiceAutostrada = radiceAutostrada->left;
        }
        else
            break;
    }

    return stazionePrecedente;
}

// cerco la stazione minore più vicina allo zero nel sottoalbero senza perdere la radice
nodoAutostrada_t *minValue(nodoAutostrada_t *radiceAutostrada)
{
    nodoAutostrada_t *current = NULL;

    current = radiceAutostrada;

    while (current->left != NULL)
    {
        current = current->left;
    }

    return current;
}

// cerco la stazione minore più vicina alla stazione in ingresso nel sottoalbero senza perdere la radice
nodoAutostrada_t *maxValue(nodoAutostrada_t *radiceAutostrada)
{
    nodoAutostrada_t *current = NULL;

    current = radiceAutostrada;

    while (current->right != NULL)
    {
        current = current->right;
    }

    return current;
}