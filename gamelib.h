#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum Stato_giocatore { morto, vivo };

enum Tipo_zona { 
    bosco, scuola, laboratorio, caverna, strada, giardino, 
    supermercato, centrale_elettrica, deposito_abbandonato, stazione_polizia 
};

enum Tipo_nemico { 
    nessun_nemico, billi, democane, demotorzone 
};

enum Tipo_oggetto { 
    nessun_oggetto, bicicletta, maglietta_fuocoinferno, bussola, schitarrata_metallica 
};

struct Zona_soprasotto; 

struct Zona_mondoreale {
    enum Tipo_zona tipo;       
    enum Tipo_nemico nemico;  
    enum Tipo_oggetto oggetto; 
    struct Zona_mondoreale* avanti;
    struct Zona_mondoreale* indietro;
    struct Zona_soprasotto* link_soprasotto;
};

struct Zona_soprasotto {
    enum Tipo_zona tipo;
    enum Tipo_nemico nemico;
    struct Zona_soprasotto* avanti;           
    struct Zona_soprasotto* indietro;         
    struct Zona_mondoreale* link_mondoreale;  
};

struct Giocatore {
    char nome[50]; 
    int hp;
    int attacco_psichico; 
    int difesa_psichica;
    int fortuna;
    enum Stato_giocatore stato; 
    
    
    struct Zona_mondoreale* pos_mondoreale; 
    struct Zona_soprasotto* pos_soprasotto; 
    
    int mondo; 
    enum Tipo_oggetto zaino[3];
};

void imposta_gioco();
void gioca();          
void termina_gioco();
void crediti();
void stampa_logo_ufficiale();
int leggi_pulito(int min, int max);
#endif