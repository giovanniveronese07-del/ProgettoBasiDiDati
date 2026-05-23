# abstract

1. il sangue intero non viene diviso nei suoi tre emocomponenti ma resta la dicitura "sangue intero"
2. il trasporto porta una sola sacca di sangue o plasma per viaggio, non c'è un ddt che accorpa i colli
3. il tempo di attesa dopo plasmaferesi e sangue intero è lo stesso nel progetto, ma nella realtà è differente

# popolazione delle tabelle
## OSPEDALE

ci sono 77 ospedali tra privati e pubblici all'interno della regione Veneto, che è la dimensione del territorio di simulazione. nel nostro progetto sono 42, il numero degli ospedali pubblici  

CREATE TABLE OSPEDALE (
    id INT PRIMARY KEY,
    nome VARCHAR(128),
    civico VARCHAR(16),
    via VARCHAR(128),
    citta VARCHAR(128),
    provincia CHAR(2),
    cap CHAR(5)
);


id: valore int di 4 cifre, univoco che identifica l'ospedale

nome: stringa di caratteri a lunghezza variabile, nomi fittizi ma ispirati alla realtà

civico: stringa di caratteri a lunghezza variabile (per ammettere anche input come 4int2, 5B, ...)

via: stringa di caratteri a lunghezza variabile ("via" non è una parola preimpostata, va specificato se si tratta di una via, viale, corso, piazza, ...)

citta: stringa di caratteri a lunghezza variabile (senza accento). le città principali (più popolose) della regione Veneto hanno un ospedale, le provincie possono averne più di uno

provincia: stringa di caratteri a lunghezza predefinita a 2 caratteri

cap: stringa di caratteri a lunghezza predefinita a 5 caratteri

## CENTRO_PRELIEVI

ce ne sono 3-400 nella regione Veneto, ma in questo progetto sono 150

CREATE TABLE CENTRO_PRELIEVI (
    id_centro INT,
    nome VARCHAR(128),
    civico VARCHAR(16),
    via VARCHAR(128),
    citta VARCHAR(128),
    provincia CHAR(2),
    cap CHAR(5),
    id_ospedale INT, -- foreign key
        PRIMARY KEY (id_ospedale, id_centro),
        FOREIGN KEY (id_ospedale) REFERENCES OSPEDALE(id)
);

id_centro: numero a 3 cifre che fa riferimento a un solo ospedale. devono essere univoci solo in riferimento a un singolo ospedale. due centri che hanno lo stesso id_centro sono necessariamente collegati a due ospedali diversi

nome: stringa di caratteri a lunghezza variabile, nomi fittizi ma ispirati alla realtà, quasi tutti sono i nomi del Comune

civico: stringa di caratteri a lunghezza variabile (per ammettere anche input come 4int2, 5B, ...)

via: stringa di caratteri a lunghezza variabile ("via" non è una parola preimpostata, va specificato se si tratta di una via, viale, corso, piazza, ...)

citta: stringa di caratteri a lunghezza variabile (senza accento). anche comuni non densamente popolati possono avere un centro

provincia: stringa di caratteri a lunghezza predefinita a 2 caratteri

cap: stringa di caratteri a lunghezza predefinita a 5 caratteri

## DONATORE

ci sono circa 140 000 donatori attivi nella regione veneto, ai fini del porogetto per avere una distribuzione statistica accettabile ne abbiamo inseriti 470 circa

CREATE TABLE DONATORE (
    cf CHAR(16) PRIMARY KEY,
    nome VARCHAR(128),
    cognome VARCHAR(128),
    gruppo_sanguigno gruppo_sanguigno_enum NOT NULL,
    fattore_rh fattore_rh_enum NOT NULL,
    numero_di_telefono VARCHAR(15) NOT NULL
);

cf: i codici fiscali pseudo-realistici, sono coerenti nel possibile con il nome e il cognome, con le seguenti regole
primi 3 caratteri: consonanti cognome
caratteri 4-6: consonanti nome
caratteri 7-8: anno di nascita (generati con distribuzione gaussiana della popolazione donatrice dal 1961 al 2008)
caratteri 9: mese di nascita, varia (A,B,C,D,E,H,L,M,P,R,S,T sono le opzioni)
caratteri 10-11: giorno di nascita, per gli uomini da 01 a 31, per le donne da 41 a 71
caratteri 12-13-14-15: lettera + 3 cifre casuali
carattere 16: randomico 

è necessario che i caratteri dl 7 al 11 siano realistici, per calcolare quando è possibile donare (varia da uomo a donna) e quando si deve smettere la propria carriera di donatore a causa del raggungimento dell'età massima: 65 anni

nome: stringa di caratteri a lunghezza variabile, nomi fittizi ma ispirati alla realtà, italiani per la maggioranza e alcuni internazionali

cognome: stringa di caratteri a lunghezza variabile, nomi fittizi ma ispirati alla realtà, italiani per la maggioranza e alcuni internazionali

gruppo sanguigno / fattore rh: generato seguendo una distribuzione statistica realistica (40% 0+, 36% A+, 7.5% B+, 7% 0-, 6% A-, 2.5% AB+, 0.7% B-, 0.3% AB-)

numero di telefono: Formato standard E.164: prefisso internazionale (+ seguito da 1 a 3 cifre) e numero del Paese locale (fino a 11 cifre). ma considerato l'errore umano di compilazione de campi, per cui alcuni hanno il prefisso, alcuni no

## PAZIENTE

CREATE TABLE PAZIENTE (
    cf CHAR(16) PRIMARY KEY,
    nome VARCHAR(128),
    cognome VARCHAR(128),
    gruppo_sanguigno gruppo_sanguigno_enum NOT NULL,
    fattore_rh fattore_rh_enum NOT NULL
);

cf: generato cercando una coerenza con solo il nome e il cognome, visto che non è neessario estrapolare dati dal codice fiscale

nome: stringa di caratteri a lunghezza variabile, nomi fittizi ma ispirati alla realtà, italiani per la maggioranza e alcuni internazionali

cognome: stringa di caratteri a lunghezza variabile, nomi fittizi ma ispirati alla realtà, italiani per la maggioranza e alcuni internazionali

gruppo sanguigno / fattore rh: generato seguendo una distribuzione statistica realistica (40% 0+, 36% A+, 7.5% B+, 7% 0-, 6% A-, 2.5% AB+, 0.7% B-, 0.3% AB-)

## PRELIEVO

-- Inserisce 500 righe con id, cf_donatore e data su giorni lavorativi, da giugno 2025 a fine maggio 2026.
-- Weekend e festività non sono presenti, con più giorni mancanti e date ripetute secondo una distribuzione gaussiana 1-10.
-- CF donatore preso da 250 selezionati casualmente da popola_donatore.sql, con distanze minime: 90 giorni maschi, 180 giorni femmine.


CREATE TABLE PRELIEVO (
    id SERIAL PRIMARY KEY,
    cf_donatore CHAR(16), -- foreign key
    id_centro INT, -- foreign key
    id_ospedale INT, -- foreign key
    data_ora TIMESTAMP(0) NOT NULL, -- imposta la precisione al secondo netto
        FOREIGN KEY (cf_donatore) REFERENCES DONATORE(cf),
        FOREIGN KEY (id_ospedale, id_centro) REFERENCES CENTRO_PRELIEVI(id_ospedale, id_centro)
);

id: 

cf_donatore: 

id_centro: 

id_ospedale: 

data_ora: 

## RICHIESTA SANGUE