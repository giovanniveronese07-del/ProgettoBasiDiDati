#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"
/*
#define PG_HOST "127.0.0.1"
#define PG_USER "postgres"
#define PG_DB "BancadelSangue"
#define PG_PASS "GiovaVero04."
#define PG_PORt 5432*/

#define COL_WIDTH 30


void checkResult(PGresult *res, const PGconn *conn);
void do_exit(PGconn *conn);
void printResult(PGresult *res);
void printMenu();

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
    
    PGconn *conn;
    conn = PQconnectdb(conninfo);

    //controllo della connessione
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

            case 0:
                PQfinish(conn);
                printf("Connessione chiusa\n");
                return 0;

            default:
                printf("scelta non valida\n");
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

void printMenu(){
    printf("\n====== MENU QUERY ======\n");
    printf("1.\n");
    printf("2.\n");
    printf("3.\n");
    printf("4.\n");
    printf("5.\n");
    printf("0. Esci\n");
}

void query1(PGconn *conn){}
void query2(PGconn *conn){}
void query3(PGconn *conn){}
void query4(PGconn *conn){}
void query5(PGconn *conn){}



void printResult(PGresult *res){
    //trovo il numero di tuple e campi selezinati
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