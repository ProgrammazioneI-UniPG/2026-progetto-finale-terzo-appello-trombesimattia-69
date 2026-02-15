#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "gamelib.h"
#include <time.h>
static struct Zona_mondoreale* prima_zona_mondoreale = NULL;
static struct Zona_soprasotto* prima_zona_soprasotto = NULL;
static int mappa_creata = 0; 
static struct Giocatore* giocatori[4]; 
static int numero_giocatori = 0;   
static char ultimi_vincitori[3][50] = {"", "", ""};   
static void pulisci_schermo() {
    system("clear"); 
}
 void stampa_logo_ufficiale() {
    
    const char* ROSSO_ACCESO = "\033[1;31m";
    const char* RESET = "\033[0m";

    system("clear");

    printf("%s", ROSSO_ACCESO);

    printf("         _____  _______ _____            _   _  _____ ______ _____\n");
    printf("        / ____||__   __|  __ \\     /\\   | \\ | |/ ____|  ____|  __ \\\n");
    printf("       | (___     | |  | |__) |   /  \\  |  \\| | |  __| |__  | |__) |\n");
    printf("        \\___ \\    | |  |  _  /   / /\\ \\ | . ` | | |_ |  __| |  _  /\n");
    printf("        ____) |   | |  | | \\ \\  / ____ \\| |\\  | |__| | |____| | \\ \\\n");
    printf("       |_____/    |_|  |_|  \\_\\/_/    \\_\\_| \\_|\\_____|______|_|  \\_\\\n");
 
    printf("        ___________________________________________________________\n");
    
    printf("                     _______ _    _ _____ _   _  _____  _____\n");
    printf("                    |__   __| |  | |_   _| \\ | |/ ____|/ ____|\n");
    printf("                       | |  | |__| | | | |  \\| | |  __| (___\n");
    printf("                       | |  |  __  | | | | . ` | | |_ |\\___ \\\n");
    printf("                       | |  | |  | |_| |_| |\\  | |__| |____) |\n");
    printf("                       |_|  |_|  |_|_____|_| \\_|\\_____|_____/\n");
    
    printf("          _____________________________________________________________\n");

    printf("%s", RESET);
    printf("\n\n\n"); 
    
}
 int leggi_pulito(int min, int max) {
    int valore;
    int check;
    while (1) {
        check = scanf("%d", &valore);
        while (getchar() != '\n'); 
        if (check != 1) {
            printf("ERRORE: Inserisci un numero valido: ");
        } else if (valore < min || valore > max) {
            printf("ERRORE: Inserisci un valore tra %d e %d: ", min, max);
        } else {
            return valore;
        }
    }
}
static void ins_zona(){
    //  Allocazione
    struct Zona_mondoreale* nuova_reale = (struct Zona_mondoreale*) malloc(sizeof(struct Zona_mondoreale));
    struct Zona_soprasotto* nuova_sopra = (struct Zona_soprasotto*) malloc(sizeof(struct Zona_soprasotto));

    if(nuova_reale == NULL || nuova_sopra == NULL){
        printf("ERRORE-MEMORIA ESAURITA");
        return;
    }

    //  Generazione dati casuali
    enum Tipo_zona tipo_casuale = (enum Tipo_zona)(rand() % 10);
    enum Tipo_oggetto oggetto_casuale = (enum Tipo_oggetto)(rand() % 5);

    //  Riempimento e Collegamento Verticale
    nuova_reale->link_soprasotto = nuova_sopra;
    nuova_sopra->link_mondoreale = nuova_reale;
    
    nuova_reale->tipo = tipo_casuale;
    nuova_sopra->tipo = tipo_casuale; 

    nuova_reale->oggetto = oggetto_casuale; 

    nuova_reale->nemico = (enum Tipo_nemico)(rand() % 3); 

    if (rand() % 2 == 0) {
        nuova_sopra->nemico = nessun_nemico; 
    } else {
        nuova_sopra->nemico = democane;   
    }

    //  Inizializzazione puntatori navigazione
    nuova_reale->avanti = NULL;
    nuova_reale->indietro = NULL;
    nuova_sopra->avanti = NULL;
    nuova_sopra->indietro = NULL;

    //  Inserimento in coda Mondo Reale
    if(prima_zona_mondoreale == NULL) {
        prima_zona_mondoreale = nuova_reale;
    } else {
        struct Zona_mondoreale* scorri = prima_zona_mondoreale;
        while(scorri->avanti != NULL){
            scorri = scorri->avanti;
        }
        scorri->avanti = nuova_reale;
        nuova_reale->indietro = scorri;
    }

    //  Inserimento in coda Soprasotto
    if(prima_zona_soprasotto == NULL) {
        prima_zona_soprasotto = nuova_sopra;
    } else {
        struct Zona_soprasotto* scorri = prima_zona_soprasotto;
        while(scorri->avanti != NULL){
            scorri = scorri->avanti;
        }
        scorri->avanti = nuova_sopra;
        nuova_sopra->indietro = scorri;
    }
   
    printf(">> Creata zona: Tipo %d\n", nuova_reale->tipo); 
}
static void distruggi_mappa(){
    struct Zona_mondoreale* scanner = prima_zona_mondoreale;
    while(scanner !=NULL){
       struct Zona_mondoreale* temp = scanner;
        scanner = scanner->avanti;
        free(temp->link_soprasotto);

        free(temp);
    }
    prima_zona_mondoreale = NULL;
    prima_zona_soprasotto = NULL;
    }
static void genera_mappa(){
    distruggi_mappa(); 

    for(int i=0; i<15; i++){
        ins_zona();
    }
   
    int posizione_boss= rand() % 15;
    struct Zona_soprasotto* scorri= prima_zona_soprasotto;
    int j=0;
    
    while(scorri != NULL && j < posizione_boss){
        scorri = scorri-> avanti;
        j++;
    }

    if(scorri !=NULL){
        scorri->nemico= demotorzone;
    }

    mappa_creata = 1;

}
static int conta_zone() {
    int conta = 0;
    struct Zona_mondoreale* corr = prima_zona_mondoreale;
    while (corr != NULL) {
        conta++;
        corr = corr->avanti;
    }
    return conta;
}
static int controllo_validita_mappa() {
    int contatore_zone = 0;
    int contatore_boss = 0;
    struct Zona_soprasotto* scorri = prima_zona_soprasotto;

   
    while (scorri != NULL) {
        contatore_zone++;
        if (scorri->nemico == demotorzone) { 
            contatore_boss++;
        }
        scorri = scorri->avanti;
    }

    
    if (contatore_zone >= 15 && contatore_boss == 1) {
        return 1; 
    } else {
        printf("ERRORE MAPPA: Zone totali = %d (min 15), Boss trovati = %d (serve 1).\n", contatore_zone, contatore_boss);
        return 0; 
    }
}
static void inserisci_zona() {
    int max_pos = conta_zone();     
    printf("Inserisci la posizione (0-%d): ", max_pos);
    int posizione = leggi_pulito(0, max_pos);

    struct Zona_mondoreale* nuova_reale = (struct Zona_mondoreale*) malloc(sizeof(struct Zona_mondoreale));
    struct Zona_soprasotto* nuova_sopra = (struct Zona_soprasotto*) malloc(sizeof(struct Zona_soprasotto));

    if (nuova_reale == NULL || nuova_sopra == NULL) {
        printf("Errore di memoria!\n");
        return;
    }

    nuova_reale->tipo = (enum Tipo_zona)(rand() % 10);
    nuova_sopra->tipo = nuova_reale->tipo;

    printf("Scegli Nemico Mondo Reale (0:Nessuno, 1:Billi, 2:Democane): ");
    nuova_reale->nemico = (enum Tipo_nemico)leggi_pulito(0, 2);

    printf("Scegli Oggetto Mondo Reale (0:Nessuno, 1:Bici, 2:Maglietta, 3:Bussola, 4:Schitar.): ");
    nuova_reale->oggetto = (enum Tipo_oggetto)leggi_pulito(0, 4);

    int n_sopra;
    do {
        printf("Scegli Nemico Soprasotto (0:Nessuno, 2:Democane): ");
        n_sopra = leggi_pulito(0, 2);
        if(n_sopra == 1) printf("Errore: Billi non può stare nel Soprasotto!\n");
    } while(n_sopra == 1);
    nuova_sopra->nemico = (enum Tipo_nemico)n_sopra;

    nuova_reale->link_soprasotto = nuova_sopra;
    nuova_sopra->link_mondoreale = nuova_reale;

    if (posizione == 0 || prima_zona_mondoreale == NULL) {
        nuova_reale->avanti = prima_zona_mondoreale;
        nuova_reale->indietro = NULL;
        nuova_sopra->avanti = prima_zona_soprasotto;
        nuova_sopra->indietro = NULL;

        if (prima_zona_mondoreale != NULL) {
            prima_zona_mondoreale->indietro = nuova_reale;
            prima_zona_soprasotto->indietro = nuova_sopra;
        }
        prima_zona_mondoreale = nuova_reale;
        prima_zona_soprasotto = nuova_sopra;
    } else {
        struct Zona_mondoreale* corr_r = prima_zona_mondoreale;
        struct Zona_soprasotto* corr_s = prima_zona_soprasotto;
        int conta = 0;
        
        
        while (corr_r->avanti != NULL && conta < posizione - 1) {
            corr_r = corr_r->avanti;
            corr_s = corr_s->avanti;
            conta++;
        }
        
        // Colleghiamo il NUOVO al SUCCESSIVO 
        nuova_reale->avanti = corr_r->avanti;
        nuova_sopra->avanti = corr_s->avanti;
        
        if (corr_r->avanti != NULL) {
            corr_r->avanti->indietro = nuova_reale;
            corr_s->avanti->indietro = nuova_sopra;
        }
        
        // Colleghiamo il PRECEDENTE al NUOVO
        corr_r->avanti = nuova_reale;
        corr_s->avanti = nuova_sopra;
        
        // Colleghiamo il NUOVO al PRECEDENTE
        nuova_reale->indietro = corr_r;
        nuova_sopra->indietro = corr_s;
    }
    
    printf(">> Zona inserita in posizione %d!\n", posizione);
    printf("   conta zone: %d\n", conta_zone()); 
}
static void cancella_zona() {
    if (prima_zona_mondoreale == NULL) {
        printf("Mappa vuota!\n");
        return;
    }

    int n_zone = conta_zone(); 
    if (n_zone == 0) return;

    printf("Inserisci indice zona da cancellare (0-%d): ", n_zone - 1);
    int pos = leggi_pulito(0, n_zone - 1);

    struct Zona_mondoreale* temp_r = prima_zona_mondoreale;
    struct Zona_soprasotto* temp_s = prima_zona_soprasotto;
    int i = 0;

    
    while (temp_r != NULL && i < pos) {
        temp_r = temp_r->avanti;
        temp_s = temp_s->avanti;
        i++;
    }

    
    if (temp_r == NULL) {
        printf("Posizione non valida.\n");
        return;
    }

    //  Scollegamento Mondo Reale
    if (temp_r->indietro != NULL) {
        temp_r->indietro->avanti = temp_r->avanti;
    } else {
        
        prima_zona_mondoreale = temp_r->avanti; 
    }
    
    if (temp_r->avanti != NULL) {
        temp_r->avanti->indietro = temp_r->indietro;
    }

    //  Scollegamento Soprasotto 
    if (temp_s->indietro != NULL) {
        temp_s->indietro->avanti = temp_s->avanti;
    } else {
        prima_zona_soprasotto = temp_s->avanti;
    }

    if (temp_s->avanti != NULL) {
        temp_s->avanti->indietro = temp_s->indietro;
    }

    free(temp_r);
    free(temp_s);
    printf(">> Zona %d cancellata con successo.\n", pos);
}
static void stampa_mappa() {
    if(prima_zona_mondoreale == NULL) {
        printf("Mappa vuota.\n");
        return;
    }

    printf("\n--- MAPPA COMPLETA ---\n");
    struct Zona_mondoreale* r = prima_zona_mondoreale;
    struct Zona_soprasotto* s = prima_zona_soprasotto;
    int i = 0;

    while(r != NULL) {
        printf("[Zona %d]\n", i);
        printf("   REALE: Tipo=%d, Nemico=%d, Oggetto=%d\n", r->tipo, r->nemico, r->oggetto);
        printf("   SOPRA: Tipo=%d, Nemico=%d\n", s->tipo, s->nemico);
        printf("----------------------------------\n");
        
        r = r->avanti;
        s = s->avanti;
        i++;
    }
}
static void stampa_singola_zona() {
    if(prima_zona_mondoreale == NULL) {
        printf("Mappa vuota.\n");
        return;
    }

    int n = conta_zone();
    printf("Inserisci indice zona da visualizzare (0-%d): ", n - 1);
    int pos = leggi_pulito(0, n - 1); 

    struct Zona_mondoreale* r = prima_zona_mondoreale;
    struct Zona_soprasotto* s = prima_zona_soprasotto;
    int i = 0;

    
    while(r != NULL && i < pos) {
        r = r->avanti;
        s = s->avanti;
        i++;
    }

    if(r == NULL) {
        printf("Zona non trovata!\n");
    } else {
        printf("\n--- DETTAGLIO ZONA %d ---\n", pos);
        printf(">> MONDO REALE:\n");
        printf("   - Tipo: %d\n", r->tipo);
        printf("   - Nemico: %d\n", r->nemico);
        printf("   - Oggetto: %d\n", r->oggetto);
        printf(">> SOPRASOTTO:\n");
        printf("   - Tipo: %d\n", s->tipo);
        printf("   - Nemico: %d\n", s->nemico);
        printf("-------------------------\n");
    }
}
static int chiudi_mappa() {
    
    if (mappa_creata == 0 && prima_zona_mondoreale == NULL) {
        printf(">> ERRORE: Non hai ancora una mappa! Usa l'opzione 1.\n");
        return 0;
    }
    
    if (controllo_validita_mappa()) {
        mappa_creata = 1;
        printf(">> Mappa chiusa e confermata.\n");
        return 1; //ok
    } else {
        printf(">> Impossibile chiudere la mappa: non valida.\n");
        return 0; //ko
    }
}
static void stampa_stato_giocatore(int indice) {
    struct Giocatore* g = giocatori[indice];
    printf("\n--- STATO GIOCATORE: %s ---\n", g->nome);
    printf("HP: %d/100\n", g->hp);
    printf("Mondo: %s\n", (g->mondo == 0) ? "Reale" : "Soprasotto");
    printf("Attacco: %d | Difesa: %d | Fortuna: %d\n", g->attacco_psichico, g->difesa_psichica, g->fortuna);
    printf("Zaino: [%d] [%d] [%d]\n", g->zaino[0], g->zaino[1], g->zaino[2]);
    printf("-------------------------------\n");
}
static void stampa_info_zona(int indice) {
    struct Giocatore* g = giocatori[indice];
    printf("\n--- INFO ZONA ATTUALE ---\n");
    
    if (g->mondo == 0) {
       
        printf("Mondo: REALE\n");
        printf("Tipo Zona: %d\n", g->pos_mondoreale->tipo); 
        printf("Nemico presente: %d\n", g->pos_mondoreale->nemico);
        printf("Oggetto presente: %d\n", g->pos_mondoreale->oggetto);
    } else {
        
        printf("Mondo: SOPRASOTTO\n");
        printf("Tipo Zona: %d\n", g->pos_soprasotto->tipo);
        printf("Nemico presente: %d\n", g->pos_soprasotto->nemico);
    }
    printf("-------------------------\n");
}
void imposta_gioco(){

    if (numero_giocatori > 0) {
        for (int i = 0; i < 4; i++) {
            if (giocatori[i] != NULL) {
                free(giocatori[i]);
                giocatori[i] = NULL;
            }
        }
        numero_giocatori = 0;
        printf(">> Memoria giocatori precedenti liberata.\n");
    }

    int undici_virgola_cinque_preso=0;
    printf("|-----------------------------------|\n");
    printf("|         IMPOSTAZIONE GIOCO        |\n");
    printf("|-----------------------------------|\n");
    printf("Inserisci il numero di giocatori (1-4): ");
    numero_giocatori = leggi_pulito(1, 4);

    if(numero_giocatori < 1 || numero_giocatori > 4){
        printf("|----------------------------------------|\n");
        printf("| ERRORE: Numero di giocatori non valido!|\n");
        printf("|----------------------------------------|\n");
        numero_giocatori = 0; 
        return;
    }
    for(int i=0; i<numero_giocatori;i++){
        
        giocatori[i] = (struct Giocatore*) malloc (sizeof (struct Giocatore));

        printf("Inserisci il nome del giocatore %d: ",i+1);
        fgets(giocatori[i]->nome, sizeof(giocatori[i]->nome), stdin);
        giocatori[i]->nome[strcspn(giocatori[i]->nome, "\n")] = 0;
        giocatori[i]->attacco_psichico = rand() % 20 + 1;
        giocatori[i]->difesa_psichica = rand() % 20 + 1;
        giocatori[i]->fortuna = rand() % 20 + 1;
        
        printf("\n");
        printf("| Giocatore: \x1b[31m%-15s\x1b[0m\n", giocatori[i]->nome);
        printf("| - Attacco Psichico: \x1b[32m%-3d\x1b[0m\n", giocatori[i]->attacco_psichico);
        printf("| - Difesa Psichica: \x1b[32m%-3d\x1b[0m\n", giocatori[i]->difesa_psichica);
        printf("| - Fortuna: \x1b[32m%-3d\x1b[0m\n", giocatori[i]->fortuna);

        int scelta_valida = 0; 
        do {
            printf("|--------------------------------------------------------------|\n");
            printf("|                Vuoi modificare le statistiche?               |\n");
            printf("|--------------------------------------------------------------|\n");
            printf("| 1: Nessuna Modifica                                          |\n");
            printf("| 2: Modifica Attacco Psichico (+3 ATTACCO, -3 DIFESA)         |\n");
            printf("| 3: Modifica Difesa Psichica (+3 DIFESA, -3 ATTACCO)          |\n");
            
            if (undici_virgola_cinque_preso == 0) {
                printf("| 4: Diventa Undici Virgola Cinque (+4 ATT/ +4 DIF/ -7 FORTUNA)|\n");
            } else {
                printf("| 4: Diventa Undici Virgola Cinque (NON DISPONIBILE)           |\n");
            }
            printf("|--------------------------------------------------------------|\n");
            printf("| Inserisci la tua scelta: ");

            int scelta_statistiche = leggi_pulito(1, 4); 

            switch(scelta_statistiche) {
                case 1: scelta_valida = 1; break;
                case 2:
                    giocatori[i]->attacco_psichico += 3;
                    giocatori[i]->difesa_psichica -= 3;
                    scelta_valida = 1;
                    break;
                case 3:
                    giocatori[i]->difesa_psichica += 3;
                    giocatori[i]->attacco_psichico -= 3;
                    scelta_valida = 1;
                    break;
                case 4:
                    if (undici_virgola_cinque_preso == 0) {
                        giocatori[i]->attacco_psichico += 4;
                        giocatori[i]->difesa_psichica += 4;
                        giocatori[i]->fortuna -= 7;
                        undici_virgola_cinque_preso = 1;
                        strcat(giocatori[i]->nome, " (11.5)");
                        scelta_valida = 1;
                    } else {
                        printf("\033[1;31m| 11.5 già preso! Scegli un'altra opzione. |\033[0m\n");
                    }
                    break;
            }
        } while (!scelta_valida);
        giocatori[i]->pos_mondoreale = prima_zona_mondoreale;
        giocatori[i]->pos_soprasotto = prima_zona_soprasotto;
        giocatori[i]->mondo = 0; 
        giocatori[i]->stato = vivo;
        
    
        for(int z=0; z<3; z++) {
            giocatori[i]->zaino[z] = nessun_oggetto;
        }

    }
    int scelta_mappa = 0;
    do {
        pulisci_schermo();
        printf("|----------------MENU MAPPA------------------|\n");
        printf("| 1. Genera Mappa Casuale (OBBLIGATORIO)     |\n");
        printf("| 2. Inserisci Zona Manualmente              |\n");
        printf("| 3. Cancella Zona                           |\n");
        printf("| 4. Stampa Mappa (Debug)                    |\n");
        printf("| 5. Stampa Singola Zona                     |\n");
        printf("| 6. Chiudi Mappa e Gioca                    |\n");
        printf("|--------------------------------------------|\n");
        printf("Scelta: ");
        scelta_mappa = leggi_pulito(1, 6);
        

        switch(scelta_mappa) {
            case 1:
                genera_mappa(); 
                printf(">> Mappa generata con successo (15 Zone + Boss)!\n");
                printf("\n>> Premi INVIO per continuare...");
                getchar(); 
                break;

            case 2:
                if (mappa_creata == 0) {
                    printf(">> ERRORE: Devi prima generare la mappa base (Opzione 1)!\n");
                } else {
                    inserisci_zona();
                }
                printf("\n>> Premi INVIO per continuare...");
                getchar();
                break;

            case 3:
                 
                 cancella_zona();
                 printf("\n>> Premi INVIO per continuare...");
                 getchar();
                 break;

            case 4:
                
                stampa_mappa();
                printf("\n>> Premi INVIO per continuare...");
                getchar();
                break;

            case 5:   
                stampa_singola_zona();
                printf("\n>> Premi INVIO per continuare...");
                getchar();   
                break;
            case 6:
                if (chiudi_mappa() == 0) {
                    scelta_mappa = 0; 
                } 
                else {
                    printf(">> Si torna al menu principale.\n");
                }
                break;
                
            default:
                printf("Scelta non valida.\n");
        }
    } while(scelta_mappa != 6);
}
static void raccogli_oggetto(int indice_giocatore){
    struct Giocatore* g = giocatori[indice_giocatore];

    if(mappa_creata == 0 || numero_giocatori == 0){
         printf("ERRORE: Mappa o giocatori non impostati.\n");
         return;
    }

    
    if(g->mondo == 1){
        printf("|-------------------------------------------------|\n");
        printf("| ERRORE : Non puoi raccogliere oggetti nel Sottosopra!|\n");
        printf("|-------------------------------------------------|\n");
        return;
    }
    
     
    if(g->pos_mondoreale->oggetto == nessun_oggetto){
        printf("|-------------------------------------------------|\n");
        printf("| ERRORE : Non ci sono oggetti qui!               |\n");
        printf("|-------------------------------------------------|\n");
        return;
    }

   
    if(g->pos_mondoreale->nemico != nessun_nemico){
        printf("|-------------------------------------------------|\n");
        printf("| ERRORE : C'è un nemico! Devi sconfiggerlo prima!|\n");
        printf("|-------------------------------------------------|\n");
        return;
    }

    
    int posto_libero = -1;
    for(int i=0; i<3; i++){
        if(g->zaino[i] == nessun_oggetto){
            posto_libero = i;
            break;
        }
    }

    if(posto_libero == -1){
        printf("| ERRORE : Il tuo zaino è pieno! (Max 3 oggetti) |\n");
        return;
    }

    
    char nome_oggetto[50];
    enum Tipo_oggetto obj = g->pos_mondoreale->oggetto;

    switch(obj){
        case bicicletta: strcpy(nome_oggetto, "Bicicletta"); break;
        case maglietta_fuocoinferno: strcpy(nome_oggetto, "Maglietta Fuocoinferno"); break;
        case bussola: strcpy(nome_oggetto, "Bussola"); break;
        case schitarrata_metallica: strcpy(nome_oggetto, "Schitarrata Metallica"); break;
        default: strcpy(nome_oggetto, "Oggetto Misterioso"); break;
    }

    
    g->zaino[posto_libero] = obj;
    g->pos_mondoreale->oggetto = nessun_oggetto; 

    
    printf("| Hai raccolto [%s] e lo hai messo nello zaino!|\n", nome_oggetto);
}
static void utilizza_oggetto(int indice_giocatore){
    struct Giocatore* g = giocatori[indice_giocatore];

    printf("|---------------------------|\n");
    printf("|      Oggetti nello zaino: |\n");
    for(int i=0; i<3;i++){
        printf("| %d: [%d]                 |\n", i+1, g->zaino[i]);
    }
    printf("|---------------------------|\n");
    printf("Scegli l'oggetto da usare (1-3): ");
    int scelta_oggetto = leggi_pulito(1, 3); 

    if(scelta_oggetto < 1 || scelta_oggetto > 3){
        printf("|-------------------------------------------------|\n");
        printf("| ERRORE : Scelta oggetto non valida!             |\n");
        printf("|-------------------------------------------------|\n");
        return;
    }

    int indice = scelta_oggetto - 1;
    if(g->zaino[indice] == nessun_oggetto){
        printf("|-------------------------------------------------|\n");
        printf("| ERRORE : Non c'è nessun oggetto in questo slot! |\n");
        printf("|-------------------------------------------------|\n");
        return;
    }
    switch(g->zaino[indice]) {
        case maglietta_fuocoinferno:
            printf(">> Indossi la Maglietta Fuocoinferno! Ti senti rinvigorito.\n");
            g->hp += 50; 
            if(g->hp > 100) g->hp = 100; 
            printf("| HP attuali: %d/100\n", g->hp);
            break;

        case schitarrata_metallica:
            printf("| Suoni una Schitarrata Metallica pazzesca! Ti senti potente.\n");
            g->attacco_psichico += 5; 
            printf("| Attacco Psichico aumentato a: %d\n", g->attacco_psichico);
            break;

        case bicicletta:
            printf("| Sali sulla Bicicletta! Sei più veloce a schivare.\n");
            g->difesa_psichica += 5; 
            printf("| Difesa Psichica aumentata a: %d\n", g->difesa_psichica);
            break;

        case bussola:
            printf("| Usi la Bussola! La fortuna ti sorride.\n");
            g->fortuna += 5; 
            if(g->fortuna > 20) g->fortuna = 20; 
            printf("| Fortuna aumentata a: %d\n", g->fortuna);
            break;
            
        default:
            printf("| L'oggetto non ha alcun effetto apparente.\n");
            break;
    }

    printf("| Hai usato l'oggetto [%d]!)|\n", g->zaino[indice]);
    g->zaino[indice] = nessun_oggetto; 
}
static void riposa(int indice_giocatore) {
    struct Giocatore* g = giocatori[indice_giocatore];
    int cura = 10; 
    
    printf("| Decidi di accamparti per riposare un po'... |\n");
    g->hp += cura;
    if (g->hp > 100) g->hp = 100; 
    
    printf("| Ti senti meglio! Hai recuperato %d HP. (HP Attuali: %d) |\n", cura, g->hp);
}
static int combattimento(int indice_giocatore){ 
    struct Giocatore* giocatore = giocatori[indice_giocatore];
    int hp_nemico=0;
    int attacco_nemico=0;
    char nome_nemico[30];
    int tipo_nemico_trovato=-1;

    
    if(giocatore->mondo == 0){
        tipo_nemico_trovato = giocatore->pos_mondoreale->nemico;
    }
    else{
        tipo_nemico_trovato = giocatore->pos_soprasotto->nemico;
    }

    if(tipo_nemico_trovato == nessun_nemico){
        printf("|-------------------------------------------------|\n");
        printf("| ERRORE : Non c'è nessun nemico in questa zona! |\n");
        printf("|-------------------------------------------------|\n");
        return 0;
    }

    switch(tipo_nemico_trovato){
        case 1: strcpy(nome_nemico, "Billi"); hp_nemico = 20; attacco_nemico = 10; break;
        case 2: strcpy(nome_nemico, "Democane"); hp_nemico = 40; attacco_nemico = 20; break;
        case 3: strcpy(nome_nemico, "Demotorzone"); hp_nemico = 70; attacco_nemico = 30; break;  
    }

    printf("\n!!!! UN %s APPARE !!!!\n", nome_nemico);
    
    while(hp_nemico > 0 && giocatore->hp > 0){
        printf("|----------------------|\n");
        printf("| HP Nemico: %d         \n", hp_nemico);
        printf("| HP Giocatore: %d      \n", giocatore->hp);
        printf("|----------------------|\n");
        printf("| 1: Attacca           | \n");
        printf("| 2: Usa Oggetto       | \n");
        printf("| 3: Fuggi.            | \n");
        printf("|----------------------|\n");
       printf("| Scelta: ");
        int scelta = leggi_pulito(1, 3); 

        switch(scelta){
            case 1:
                printf("|Hai sferrato un attacco psichico!|\n");
                int danno_inflitto = giocatore->attacco_psichico; 
                hp_nemico -= danno_inflitto;
                printf("|Hai inflitto %d danni al %s!|\n", danno_inflitto, nome_nemico);
                break;
            case 2:
                printf("|Hai scelto di usare un oggetto|\n");
                utilizza_oggetto(indice_giocatore);
                break;
            case 3:
                printf("|Hai scelto di fuggire!|\n");
               
                return 0; 
            default:
                printf("|Scelta non valida!|\n");
                break;
        }

        if (hp_nemico <= 0) {
            printf("\n|=========================================|\n");
            printf("| VITTORIA! Il %s è stato sconfitto! |\n", nome_nemico);
            printf("|=========================================|\n");
            
            int probabilita = rand() % 2;

            if(probabilita == 0) {
                printf("Il corpo del %s si dissolve nell'aria (Zona liberata).\n", nome_nemico);
                if(giocatore->mondo == 0) 
                    giocatore->pos_mondoreale->nemico = nessun_nemico; 
                else 
                    giocatore->pos_soprasotto->nemico = nessun_nemico;
            } 
            else {
                printf("Il corpo del %s rimane a terra... ma sembra muoversi ancora!\n", nome_nemico);
                printf("(Il nemico NON è scomparso: chi passerà di qui dovrà combatterlo di nuovo!)\n");
                
            }
                
            if(tipo_nemico_trovato == 3){
                printf("\n|=========================================|\n");
                printf("| COMPLIMENTI! HAI SCONFITTO DEMOTORZONE! |\n");
                printf("|=========================================|\n");         
                return 1;
            }
            return 0; 
        }

        printf("\n| Il %s contrattacca ferocemente!|\n", nome_nemico);
        int danno_subito = attacco_nemico - giocatore->difesa_psichica;
        
        if(danno_subito < 0) danno_subito = 0; 
        if(danno_subito == 0 && attacco_nemico > 0) danno_subito = 1; 

        giocatore->hp -= danno_subito;
        printf("|Subisci %d danni!|\n", danno_subito);

        if(giocatore->hp <= 0){
            printf("\n|=========================================|\n");
            printf("| SCONFITTA! %s è stato sconfitto! |\n", giocatore->nome);
            printf("|=========================================|\n"); 
              
            free(giocatori[indice_giocatore]);
            giocatori[indice_giocatore] = NULL; 
                 
            return 0;
        }
    } 
    return 0;
}
static void aggiungi_vincitore(char* nome) {
    strcpy(ultimi_vincitori[2], ultimi_vincitori[1]);
    strcpy(ultimi_vincitori[1], ultimi_vincitori[0]);
    strcpy(ultimi_vincitori[0], nome);
}
static int avanza(int indice_giocatore) {
    struct Giocatore* g = giocatori[indice_giocatore];
    
    
    int nemico_presente = 0;
    if(g->mondo == 0) {
        if(g->pos_mondoreale->nemico != nessun_nemico) nemico_presente = 1;
    } else {
        if(g->pos_soprasotto->nemico != nessun_nemico) nemico_presente = 1;
    }

    if(nemico_presente) {
        printf("C'è un nemico! Devi combattere prima di avanzare.\n");
        return 0; 
    }

    printf("|Hai scelto di muoverti avanti!|\n");
    if(g->mondo == 0) {
        if(g->pos_mondoreale->avanti != NULL) {
            g->pos_mondoreale = g->pos_mondoreale->avanti;
            g->pos_soprasotto = g->pos_mondoreale->link_soprasotto; 
            printf("|Sei ora nella zona successiva (Reale)!|\n");
            return 1;
        }
    } else {
        if(g->pos_soprasotto->avanti != NULL) {
            g->pos_soprasotto = g->pos_soprasotto->avanti;
            g->pos_mondoreale = g->pos_soprasotto->link_mondoreale;
            printf("|Sei ora nella zona successiva (Soprasotto)!|\n");
            return 1;
        }
    }
    printf("|Non puoi muoverti avanti, sei alla fine della mappa!|\n");
    return 1; 
}
static int indietreggia(int indice_giocatore) {
    struct Giocatore* g = giocatori[indice_giocatore];

    
    int nemico_presente = 0;
    if(g->mondo == 0) {
        if(g->pos_mondoreale->nemico != nessun_nemico) nemico_presente = 1;
    } else {
        if(g->pos_soprasotto->nemico != nessun_nemico) nemico_presente = 1;
    }

    if(nemico_presente) {
        printf("C'è un nemico! Devi combattere prima di indietreggiare.\n");
        return 0;
    }

    printf("|Hai scelto di muoverti indietro!|\n");
    if(g->mondo == 0) {
        if(g->pos_mondoreale->indietro != NULL) {
            g->pos_mondoreale = g->pos_mondoreale->indietro;
            g->pos_soprasotto = g->pos_mondoreale->link_soprasotto;
            printf("|Sei tornato indietro (Reale)!|\n");
            return 1;
        }
    } else {
        if(g->pos_soprasotto->indietro != NULL) {
            g->pos_soprasotto = g->pos_soprasotto->indietro;
            g->pos_mondoreale = g->pos_soprasotto->link_mondoreale;
            printf("|Sei tornato indietro (Soprasotto)!|\n");
            return 1;
        }
    }
    printf("|Sei all'inizio della mappa, non puoi indietreggiare!|\n");
    return 1;
}
static int cambia_mondo(int indice_giocatore) {
    struct Giocatore* g = giocatori[indice_giocatore];

    if(g->mondo == 0) { 
        
        if(g->pos_mondoreale->nemico != nessun_nemico) {
            printf("Non puoi passare al Soprasotto con un nemico qui!\n");
            return 0;
        }
        g->mondo = 1;
        
        printf("|Sei stato catapultato nel SOPRASOTTO!|\n");
        return 1; 

    } else { 
        
        int dado = rand() % 20 + 1;
        printf(">> Tiri il dado: %d (La tua fortuna: %d)\n", dado, g->fortuna);
        
        if(dado < g->fortuna) {
            g->mondo = 0;
            printf("|SUCCESSO! Sei tornato nel Mondo Reale!|\n");
            return 1;
        } else {
            printf("|FALLIMENTO! Resti nel Soprasotto.\n");
            return 1; 
        }
    }
}
 void gioca(){
    if(mappa_creata == 0){
        printf("ERRORE: Mappa non creata.\n");
        return;
    }
    if(numero_giocatori == 0){
        printf("ERRORE: Giocatori non impostati.\n");
        return;
    }

   
    for(int k=0; k<numero_giocatori; k++){
        giocatori[k]->stato = vivo;
        giocatori[k]->hp = 100;
        giocatori[k]->pos_mondoreale = prima_zona_mondoreale;
        giocatori[k]->pos_soprasotto = prima_zona_soprasotto;
        giocatori[k]->mondo = 0;
    }

    int fine_gioco = 0;

    while(fine_gioco == 0){
       
        
        int ordine[4] = {0, 1, 2, 3};
        for(int j = 0; j < numero_giocatori; j++){
            int r = rand() % numero_giocatori;
            int temp = ordine[j];
            ordine[j] = ordine[r];
            ordine[r] = temp;
        }

        for(int k=0; k<numero_giocatori; k++){
            int i = ordine[k];
            
            
            if(giocatori[i] == NULL) continue;

            printf("\n>> Tocca a %s! Premi INVIO...", giocatori[i]->nome);
            getchar();
            pulisci_schermo();
            
            if(giocatori[i]->mondo == 0) 
                printf("Sei nel Mondo Reale (Zona %d).\n", giocatori[i]->pos_mondoreale->tipo);
            else 
                printf("Sei nel Soprasotto (Zona %d).\n", giocatori[i]->pos_soprasotto->tipo);

            int turno_finito = 0;
            int azione_movimento_usata = 0; 

            while(turno_finito == 0 && giocatori[i] != NULL && fine_gioco == 0){
                
                
                printf("\033[1;31m"); 
                printf("\n");
                printf("|-----------------------------------|\n");
                printf("|           MENU DI GIOCO           |\n");
                printf("|-----------------------------------|\n");
                printf("\033[0m"); 
                printf("| 1: AVANZA                         |\n");
                printf("| 2: INDIETREGGIA                   |\n");
                printf("| 3: CAMBIA MONDO                   |\n");
                printf("| 4: RACCOGLI OGGETTO               |\n");
                printf("| 5: USA OGGETTO                    |\n");
                printf("| 6: COMBATTI                       |\n");
                printf("| 7: PASSA TURNO                    |\n");
                printf("| 8: INFO GIOCATORE/ZONA            |\n");
                printf("| 9: RIPOSA                         |\n");
                printf("|-----------------------------------|\n");
                printf("INSERISCI LA TUA SCELTA: ");
                

                                            
                int scelta = leggi_pulito(1, 9); // Pulisce buffer

                switch(scelta){
                    case 1: 
                        if(azione_movimento_usata) {
                            printf(">> Hai già usato un movimento in questo turno! Usa 'Passa Turno'.\n");
                        } else {
                            
                            if(avanza(i)) {
                                azione_movimento_usata = 1;
                            }
                        }
                        break;

                    case 2: 
                         if(azione_movimento_usata) {
                            printf(">> Hai già usato un movimento in questo turno! Usa 'Passa Turno'.\n");
                        } else {
                            if(indietreggia(i)) {
                                azione_movimento_usata = 1;
                            }
                        }
                        break;

                    case 3: 
                        if(giocatori[i]->mondo == 0) {
                            
                            if(azione_movimento_usata) {
                                printf(">> Non puoi cambiare mondo se hai già avanzato/indietreggiato!\n");
                            } else {
                                if(cambia_mondo(i)) azione_movimento_usata = 1;
                            }
                        } else {
                            
                            cambia_mondo(i);
                            azione_movimento_usata = 1;
                        }
                        break;

                    case 4: 
                        raccogli_oggetto(i);
                        break;
                    case 5:
                        utilizza_oggetto(i);
                        break;
                    case 6: 
                        if(combattimento(i)) {
                            aggiungi_vincitore(giocatori[i]->nome); 
                            fine_gioco = 1; 
                        }
                        break;

                    case 7: 
                        printf(">> Passi il turno al giocatore successivo.\n");
                        turno_finito = 1; 
                        break;

                    case 8:
                        stampa_stato_giocatore(i);
                        stampa_info_zona(i);
                        break;
                    case 9:
                        riposa(i);
                        turno_finito = 1; 
                        break;
                    default:
                        printf("Scelta non valida, DEVI INSERIRE UN NUMERO!!!!!\n");
                }
            } 

            if(fine_gioco) break;
        } 
        
       
        int tutti_morti = 1;
        for(int c=0; c<numero_giocatori; c++) {
            if(giocatori[c] != NULL) tutti_morti = 0;
        }
        if(tutti_morti && numero_giocatori > 0) { 
            printf("TUTTI I GIOCATORI SONO MORTI. GAME OVER.\n");
            fine_gioco = 1;
        }
    } 
}
void termina_gioco(){
    printf("\n--- FINE PARTITA ---\n");
    distruggi_mappa();
    for(int i=0; i<4; i++) if(giocatori[i]) free(giocatori[i]);
}
void crediti(){
    pulisci_schermo();
  printf("\n##################################\n");
    printf("#      PROGETTO COSESTRANE       #\n");
    printf("#      Creato da Mattia          #\n");
    printf("##################################\n");
    
    
    printf("\n--- (Ultimi Vincitori) ---\n");
    for(int i=0; i<3; i++){
        if(strlen(ultimi_vincitori[i]) > 0)
            printf("%d. %s\n", i+1, ultimi_vincitori[i]);
        else
            printf("%d. - \n", i+1);
    }
    printf("\n");
}