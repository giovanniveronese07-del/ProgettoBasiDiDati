#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/libpq-fe.h"
/*
#define PG_HOST "127.0.0.1"
#define PG_USER "postgres"
#define PG_DB "BancadelSangue"
#define PG_PASS ""
#define PG_PORt 5432*/

#define COL_WIDTH 30


void checkResult(PGresult *res, const PGconn *conn);
void do_exit(PGconn *conn);
void printResult(PGresult *res); // per stampare i risultati
void printMenu(); // 0 esci, 1-5 esegui una query

void query1(PGconn *conn);
void query2(PGconn *conn);
void query3(PGconn *conn);
void query4(PGconn *conn);
void query5(PGconn *conn);


int main(int argc, char **argv){

    //connessione al server
    char PG_host[50];
    char PG_user[50];
    char PG_pw[100];
    char PG_db[50];
    int PG_port;

  // in caso l'utente voglia inserire i dati
    printf("Host: ");
    scanf("%49s", PG_host);

    printf("Database: ");
    scanf("%49s", PG_db);

    printf("Utente: ");
    scanf("%49s", PG_user);

    printf("password: ");
    scanf("%99s", PG_pw);
    
    printf("Porta: ");
    scanf("%d", PG_port);

    char conninfo[512]; 
    sprintf(conninfo,"user=%s password=%s dbname=%s hostaddr=%s port=%d", PG_user, PG_pw, PG_db, PG_host, PG_port);
    
    PGconn *conn; // esegue la stringa di connessione 
    conn = PQconnectdb(conninfo);

    if(PQstatus(conn) != CONNECTION_OK){
        printf("errore di connesione: %s\n", PQerrorMessage(conn));
        do_exit(conn);
    }

    // scelta query

    int scelta;

    while(1){
        printMenu();

        switch(scelta){

            case 1:
                query1(conn);
                break;

            case 2:
                query2(conn);
                break;

            case 3:
                query3(conn);
                break;

            case 4:
                query4(conn);
                break;

            case 5:
                query5(conn);
                break;

            case 0: // chiudi il programma
                PQfinish(conn);
                printf("Connessione chiusa\n");
                return 0;

            default:
                printf("scelta non valida\n"); // per qualunque altro dato
        }
    }
}

void checkResult(PGresult *res, const PGconn *conn){
    if(PQresultStatus(res) != PGRES_TUPLES_OK){
        printf("Risultati inconsistenti %s\n", PQerrorMessage(conn));
        PQclear(res);
        exit(1);
    }
}

void do_exit(PGconn *conn){
    PQfinish(conn);
    exit(1);
}
// essendo parametrica prima va preparata la query e mandata a postgre, poi si crea il parametro
void printMenu(){
    printf("\n====== MENU QUERY ======\n");
    printf("1. Gruppi sanguigni sotto soglia minima per ospedale\n");
    printf("2. Distribuzione delle scorte per provincia\n");
    printf("3. Sacche prossime alla scadenza\n");
    printf("4. Tipi di sangue più richiesti per città\n");
    printf("5. Ospedali con più richieste non evase\n");
    printf("0. Esci\n");
}

void query1(PGconn *conn) {
    PGresult *res;
    //creazione query
    char *query =
        "SELECT o.nome, s.gruppo_sanguigno, s.fattore_rh, COUNT(*) AS numero_sacche "
        "FROM sacca s JOIN ospedale o ON s.id_ospedale = o.id "
        "WHERE s.stato_sacca = 'Disponibile' "
        "GROUP BY o.nome, s.gruppo_sanguigno, s.fattore_rh "
        "HAVING COUNT(*) < $1 "
        "ORDER BY o.nome, s.gruppo_sanguigno, s.fattore_rh;";

    //preparo la query a Postgre
    res = PQprepare(conn, "query1", query, 1, NULL);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        printf("Errore nella preparazione della query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    PQclear(res);

    //creo il parametro
    int limite;
    printf("Inserire la soglia minima di sacche: ");
    scanf("%d", &limite);

    char limite_str[16];
    snprintf(limite_str, sizeof(limite_str), "%d", limite);

    const char *paramValues[1] = { limite_str };

    //eseguo la query
    res = PQexecPrepared(conn, "query1", 1, paramValues, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("Errore nell'esecuzione della query: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    //stampo il risultato
    printResult(res);
    PQclear(res);
}
// altre  query, prima si crerano, poi si eseguono, poi il risultato se c'è si stampa
void query2(PGconn *conn){
    PGresult *res;
    char *query = 
        "SELECT o.provincia, s.gruppo_sanguigno, fattore_rh, COUNT(*) AS num_scorte"
        "FROM ospedali o JOIN sacche s ON o.id = s.id_ospedale"
        "WHERE s.stato = 'disponibile'"
        "GROUP BY o.provincia, s.gruppo_sanguigno, fattore_rh"
        "ORDER BY o.provincia, s.gruppo_sanguigno, fattore_rh;";

    res = PQexec(conn,res);

    printResult(res);

    PQclear(res);
}

void query3(PGconn *conn){}
void query4(PGconn *conn){}
void query5(PGconn *conn){}



void printResult(PGresult *res){

    //trovo il numero di tuple e campi selezionati
    int tuple = PQntuples(res);
    int campi = PQnfields(res);

    //stampo le intestazioni delle colonne
    for(int i = 0; i < campi; i++){
        printf("%-*s",COL_WIDTH, PQfname(res, i));
    }

    printf("\n");

    // linea separatrice
    for (int i = 0; i < campi; i++) {
        printf("%-*s", COL_WIDTH, "-------------------");
    }
    printf("\n");

    //stampo i valori
    for(int i = 0; i < tuple; i++){
        for(int j = 0; j< campi; j++){
            printf("%-*s",COL_WIDTH, PQgetvalue(res, i, j));
        }
        printf("\n");
    }
}