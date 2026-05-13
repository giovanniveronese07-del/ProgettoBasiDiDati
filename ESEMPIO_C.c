#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dependencies/include/libpq-fe.h"

#define PG_HOST "127.0.0.1"
#define PG_USER "postgres"
#define PG_DB "RedGaming"
#define PG_PASS "postgres"
#define PG_PORT "5432"
#define TRUE 1
#define FALSE 0
//#define DEBUG

// Viene usato fgets in tutti i casi di ricezione in quanto presenta feature di gestione
// degli errori nettamente superiori rispetto al semplice scanf. Sono state dunque create
// funzioni apposite per la ricezione di dati in formato int e string. 

int read_int_from_stdin(const char* message) {

    char input[100];
    int result;
    char garbage;

    while (TRUE) {
        printf("%s", message);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Errore nella lettura dell' input.\n");
            exit(1);
        }
        input[strcspn(input, "\n")] = 0; // Rimuove '\n' che potrebbe sporcare input

        // Parse dell'input a int, con raccolta di caratteri non parsabili
        if (sscanf(input, " %d %c", &result, &garbage) == 1) {
            return result;
        } else {
            printf("Inserire solamente valori numerici.\n");
        }
    }

}

char* read_string_from_stdin(char* input, int size, const char* message) {

    printf("%s", message);

    if (fgets(input, size, stdin) == NULL) {
        fprintf(stderr, "Errore nella lettura dell' input.\n");
        exit(1);
    }

    // Controllo se sono stati forniti troppi caratteri
    if (input[strlen(input) - 1] != '\n') {
        printf("Input fornito troppo lungo, i caratteri in eccesso sono stati eliminati.\n"); 
    } else {
        input[strcspn(input, "\n")] = 0; // Rimuove '\n' che potrebbe sporcare input
    }

    return input;

}

// Connessione al database in base ai parametri in input
PGconn* connect_to_DB(const char* user, const char* password, const char* db_name, const char* host_name, const char* port) {

    char connection_info[250];

    // hostaddr richiede un ip numerico, quindi alias come 'localhost' non sono accettati
    sprintf(connection_info, 
            "user=%s password=%s dbname=%s hostaddr=%s port=%s",
             user, password, db_name, host_name, port);

    PGconn* connection = PQconnectdb(connection_info);

    if (PQstatus(connection) != CONNECTION_OK) {
        printf("\nErrore di connessione : %s \n ", PQerrorMessage(connection));
        PQfinish(connection);
        exit(1);
    }

    printf("Connessione al DB avvenuta con successo\n");

    return connection;

}

// Stampa la lista delle query disponibili
void print_query_list() {

    printf("\nMenu Query: \n");
    printf("1: Visionare l'elenco dei prodotti di un ordine effettuato da un cliente con il loro tipo e relativo prezzo.\n");
    printf("\tNota: Per visualizzare un esempio esplicativo, inserire il nome utente: daniele.bianchi e l'ID dell'ordine: 21.\n");
    printf("2: Visionare gli N (default: 5) utenti che hanno speso di piú, in media, ordinati per spesa.\n");
    printf("3: Lista dei venditori che hanno in vendita più di N (default: 10) prodotti.\n");
    printf("4: Elencare, per ogni corriere, il numero di spedizioni effettuate, il numero medio di prodotti gestiti e il valore di tutte le spedizioni.\n");
    printf("5: Top 5 utenti più attivi sul sito che hanno rilasciato recensioni, considerando media delle recensioni rilasciate.\n\n");
    
}

// Funzione per eseguire query non parametrizzate
PGresult* execute_query(PGconn* connection, const char* query) {

    PGresult* result = PQexec(connection, query);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        printf("\nErrore di visualizzazione dati: %s \n", PQerrorMessage(connection));
        PQclear(result);
        exit(1);
    }

    return result;

}

// Funzione per eseguire query parametrizzate
PGresult* execute_parametric_query(PGconn* connection, const char* query, int num_parameters, const char* const *parameters) {

    // Inferenza di tipo lasciata a PostgreSQL
    PGresult* result = PQexecParams(connection, query, num_parameters, NULL, parameters, NULL, NULL, 0);

    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        printf("\nErrore di visualizzazione dati: %s \n", PQerrorMessage(connection));
        PQclear(result);
        exit(1);
    }

    return result;

}

// Logica per la scelta delle query e la ricezione di eventuali parametri di input
PGresult* choose_query(PGconn* connection) {
    
    int n = 0;

    while (n < -1 || n == 0 || n > 5) {
        char* message = "Digitare:\n"
                        "  [-1 ] >> Esci.\n"
                        "  [ 0 ] >> Query disponibili.\n"
                        "  [1-5] >> Seleziona Query.\n";
        n = read_int_from_stdin(message);
        if (n == 0) {
            print_query_list();
        } else if (n < -1 || n > 5) {
            printf("Input non valido. Riprovare.\n");
            n = 0;
        }
    }

    if (n == -1) {
        PQfinish(connection);
        exit(0);
    }

    //Query 1
    if (n == 1) {
        const char* parameters[2];
        const char* query =
            "SELECT u.Username AS Cliente, o.ID AS ID_Ordine, p.CodiceBarre AS Codice_Prodotto, "
                "p.Prezzo, p.Produttore, p.Modello, "
                "CASE "
                    "WHEN cpu.Prodotto IS NOT NULL THEN 'CPU' "
                    "WHEN gpu.Prodotto IS NOT NULL THEN 'GPU' "
                    "WHEN ram.Prodotto IS NOT NULL THEN 'RAM' "
                    "WHEN hdd.Prodotto IS NOT NULL THEN 'HDD' "
                    "WHEN ssd.Prodotto IS NOT NULL THEN 'SSD' "
                    "WHEN alimentatore.Prodotto IS NOT NULL THEN 'Alimentatore' "
                    "ELSE 'Altro' "
                "END AS Tipo_Prodotto "
            "FROM Utente u "
            "JOIN Ordine o ON u.Username = o.Cliente "
            "JOIN Contiene c ON o.ID = c.Ordine "
            "JOIN Prodotto p ON c.Prodotto = p.CodiceBarre "
            "LEFT JOIN CPU cpu ON p.CodiceBarre = cpu.Prodotto "
            "LEFT JOIN GPU gpu ON p.CodiceBarre = gpu.Prodotto "
            "LEFT JOIN RAM ram ON p.CodiceBarre = ram.Prodotto "
            "LEFT JOIN HDD hdd ON p.CodiceBarre = hdd.Prodotto "
            "LEFT JOIN SSD ssd ON p.CodiceBarre = ssd.Prodotto "
            "LEFT JOIN Alimentatore alimentatore ON p.CodiceBarre = alimentatore.Prodotto "
            "WHERE u.Username = $1 AND o.ID = $2;";
        char username[50];
        parameters[0] = read_string_from_stdin(username, sizeof(username), "Inserisci username: ");
        int codice_ordine = read_int_from_stdin("Inserisci il codice dell'ordine: ");
        // Conversione da int a char usa un char[16], sufficiente per un int_32
        char codice_ordine_char[16];
        sprintf(codice_ordine_char, "%d", codice_ordine);
        parameters[1] = codice_ordine_char;
        return execute_parametric_query(connection, query, 2, parameters);
    }

    //Query 2
    if (n == 2) {
        const char* parameters[1];
        const char* query =
            "SELECT u.Username AS Cliente, ROUND(AVG(f.Totale), 2) AS MediaSpesa "
            "FROM Utente u "
            "JOIN Ordine o ON u.Username = o.Cliente "
            "JOIN Fattura f ON o.ID = f.Ordine "
            "WHERE f.DataAcquisto >= CURRENT_DATE - INTERVAL '3 months' "
            "GROUP BY u.Username "
            "ORDER BY MediaSpesa DESC LIMIT $1;";
        int n_utenti;
        n_utenti = read_int_from_stdin("Inserisci il numero di utenti da visualizzare ([-1] >> default 5): ");
        if (n_utenti <= 0) {
            n_utenti = 5;  // Valore di default
        }
        // Conversione da int a char usa un char[16], sufficiente per un int_32
        char n_utenti_char[16];
        sprintf(n_utenti_char, "%d", n_utenti);
        parameters[0] = n_utenti_char;
        return execute_parametric_query(connection, query, 1, parameters);
    }

    //Query 3
    if (n == 3) {
        const char* parameters[1];
        const char* query = 
            "SELECT u.Username AS Venditore, COUNT(v.Prodotto) AS NumeroProdotti "
            "FROM Utente u "
            "JOIN Vende v ON u.Username = v.Venditore "
            "JOIN Prodotto p ON v.Prodotto = p.CodiceBarre "
            "WHERE u.Tipo = 'Venditore' "
            "GROUP BY u.Username "
            "HAVING COUNT(v.Prodotto) >= $1 "
            "ORDER BY NumeroProdotti DESC;";
        int minimo_prodotti;
        minimo_prodotti = read_int_from_stdin("Inserisci il numero minimo di prodotti da visualizzare ([-1] >> default 10): ");
        if (minimo_prodotti <= 0) {
            minimo_prodotti = 10; // Valore di default
        }
        // Conversione da int a char usa un char[16], sufficiente per un int_32
        char minimo_prodotti_char[16];
        sprintf(minimo_prodotti_char, "%d", minimo_prodotti);
        parameters[0] = minimo_prodotti_char;
        return execute_parametric_query(connection, query, 1, parameters);
    }

    //Query 4
    if (n == 4) {
        const char* query =
            "SELECT c.Nome AS Corriere, COUNT(DISTINCT s.ID) AS NumeroSpedizioni, "
            "ROUND(AVG(p.NumeroProdotti), 2) AS MediaProdottiGestiti, "
            "ROUND(SUM(f.Totale), 2) AS ValoreTotaleSpedizioni "
            "FROM Corriere c "
            "JOIN Spedizione s ON c.Nome = s.Corriere "
            "JOIN Ordine o ON o.Spedizione = s.ID "
            "JOIN Fattura f ON o.ID = f.Ordine "
            "JOIN ProdottiPerOrdine p ON o.ID = p.Ordine "
            "GROUP BY c.Nome "
            "ORDER BY NumeroSpedizioni DESC;";
        return execute_query(connection, query);
    }

    //Query 5
    if (n == 5) {
        const char* query =
            "SELECT r.Cliente AS Username, COUNT(r.ID) AS NumeroRecensioni, ROUND(AVG(r.Valutazione), 2) AS MediaValutazione "
            "FROM Recensione r "
            "GROUP BY r.Cliente "
            "ORDER BY NumeroRecensioni DESC, MediaValutazione DESC "
            "LIMIT 5;";
        return execute_query(connection, query);
    }

    // Caso fallback
    return NULL;

}

// Funzione per stampare una riga di separazione
void print_separator(int* col_widths, int cols) {
    
    printf("+");
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < col_widths[i] + 2; j++) {
            printf("-");
        }
        printf("+");
    }
    printf("\n");
    
}

// Funzione principale per stampare i risultati della query
void print_query_result(PGresult* result) {
    
    int rows = PQntuples(result);
    int cols = PQnfields(result);

    // Calcola la larghezza massima per ogni colonna
    int* col_widths = malloc(cols * sizeof(int));
    if (!col_widths) {
        fprintf(stderr, "Errore di allocazione memoria.\n");
        PQclear(result);
        return;
    }

    for (int i = 0; i < cols; i++) {
        col_widths[i] = strlen(PQfname(result, i));
        for (int j = 0; j < rows; j++) {
            int len = strlen(PQgetvalue(result, j, i));
            if (len > col_widths[i]) {
                col_widths[i] = len;
            }
        }
    }

    // Stampa intestazione
    print_separator(col_widths, cols);
    printf("|");
    for (int i = 0; i < cols; i++) {
        printf(" %-*s |", col_widths[i], PQfname(result, i));
    }
    printf("\n");
    print_separator(col_widths, cols);

    // Stampa i dati
    for (int i = 0; i < rows; i++) {
        printf("|");
        for (int j = 0; j < cols; j++) {
            printf(" %-*s |", col_widths[j], PQgetvalue(result, i, j));
        }
        printf("\n");
    }
    print_separator(col_widths, cols);

    free(col_widths);
    PQclear(result);

}

// ENTRY POINT
int main(int argc, char** argv) {

    #ifdef DEBUG 
        printf("Compilazione in modalità DEBUG [Commentare #define DEBUG per entrare in modalità RELEASE]\n");
        PGconn* connection = connect_to_DB(PG_USER, PG_PASS, PG_DB, PG_HOST, PG_PORT);
    #else 
        printf("Compilazione in modalità RELEASE\n");
        char user[50]; char pswd[50]; char db[50]; char ip[50]; char port[50];

        char* username = read_string_from_stdin(user, sizeof(user), "Inserire username: ");
        char* password = read_string_from_stdin(pswd, sizeof(pswd), "Inserire password: ");
        char* db_name = read_string_from_stdin(db, sizeof(db), "Inserire nome database: ");
        char* host_ip = read_string_from_stdin(ip, sizeof(ip), "Inserire ip host: ");
        char* port_number = read_string_from_stdin(port, sizeof(port), "Inserire porta: ");

        PGconn* connection = connect_to_DB(username, password, db_name, host_ip, port_number);
    #endif

    while (TRUE) {
        PGresult* result = choose_query(connection);
        
        if (result == NULL) {
            printf("Query non valida. Riprova.\n");
        } else {
            print_query_result(result);
        }
    }

    PQfinish(connection);

    return 0;
    
}