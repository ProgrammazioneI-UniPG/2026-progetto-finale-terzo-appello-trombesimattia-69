#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include "gamelib.h"
#include <time.h>
int main() {
   stampa_logo_ufficiale();
   
    printf("\033[1;31m"); 
    int scelta;
        printf("|-----------------------------------------------------------|\n");
        printf("|         BENVENUTO NEL GIOCO DI STRANGER THINGS            |\n");
        printf("|-----------------------------------------------------------|\n");
    printf("\033[0m"); 
 
    do{
                        printf("\n");
                        printf("|-----------------------|\n");
                        printf("|      MENU GIOCO.      |\n");
                        printf("|-----------------------|\n");
                        printf("| 1: IMPOSTA GIOCO      |\n");
                        printf("| 2: AVVIA GIOCO        |\n");
                        printf("| 3: ESCI               |\n");
                        printf("| 4: VISUALIZZA CREDITI |\n");
                        printf("|-----------------------|\n");
                        printf("INSERISCI LA TUA SCELTA: ");
                        scelta = leggi_pulito(1, 4);
                        printf("\n");
        
        

        switch(scelta){
            case 1:
                imposta_gioco();
                printf("\n");
                break;
            case 2:
                gioca();
                printf("\n");
                break;
            case 3:
                termina_gioco();
                printf("|-------------------|\n");  
                printf("|  GIOCO TERMINATO  |\n");
                printf("|-------------------|\n");
                printf("GRAZIE PER AVER GIOCATO!\n");
                break;
            case 4:
                crediti();
                printf("\n");
                break;
            default:
                printf("Scelta non valida!!\n");
        }

    }while(scelta != 3);
   
}
