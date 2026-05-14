/*
limiti logici e strutturali del progetto: 
- il sangue intero non viene diviso nei suoi tre emocomponenti ma resta la dicitura "sangue intero"
- il trasporto porta una sola sacca di sangue o plasma per viaggio, non c'è un ddt che accorpa i colli
*/

CREATE TYPE gruppo_sanguigno_enum AS ENUM ('A', 'B', 'AB', '0');
CREATE TYPE fattore_rh_enum AS ENUM ('+', '-');
CREATE TYPE emocomponente_enum AS ENUM ('Sangue intero', 'Plasma');
CREATE TYPE reparto_enum AS ENUM ('Pronto soccorso', 'Chirurgia', 'Oncologia ed ematologia', 'Ostetricia', 'Terapia intensiva e rianimazione', 'Degenza ordinaria');
CREATE TYPE stato_richiesta_enum AS ENUM ('Non evasa', 'Evasa parzialmente', 'Soddisfatta', 'Annullata');
CREATE TYPE stato_sacca_enum AS ENUM ('Disponibile', 'In uso', 'Scartata', 'In transito', 'Esaurita');

CREATE TABLE OSPEDALE (
    id INT PRIMARY KEY,
    nome VARCHAR(128),
    civico VARCHAR(16),
    via VARCHAR(128),
    citta VARCHAR(128),
    provincia CHAR(2),
    cap CHAR(5)
);

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

CREATE TABLE DONATORE (
    cf CHAR(16) PRIMARY KEY,
    nome VARCHAR(128),
    cognome VARCHAR(128),
    gruppo_sanguigno gruppo_sanguigno_enum NOT NULL,
    fattore_rh fattore_rh_enum NOT NULL,
    numero_di_telefono VARCHAR(15) NOT NULL
);

CREATE TABLE PAZIENTE (
    cf CHAR(16) PRIMARY KEY,
    nome VARCHAR(128),
    cognome VARCHAR(128),
    gruppo_sanguigno gruppo_sanguigno_enum NOT NULL,
    fattore_rh fattore_rh_enum NOT NULL
);

CREATE TABLE PRELIEVO (
    id SERIAL PRIMARY KEY,
    cf_donatore CHAR(16), -- foreign key
    id_centro INT, -- foreign key
    id_ospedale INT, -- foreign key
    data_ora TIMESTAMP(0) NOT NULL, -- imposta la precisione al secondo netto
        FOREIGN KEY (cf_donatore) REFERENCES DONATORE(cf),
        FOREIGN KEY (id_ospedale, id_centro) REFERENCES CENTRO_PRELIEVI(id_ospedale, id_centro)
);

CREATE TABLE RICHIESTA_SANGUE (
    id SERIAL PRIMARY KEY,
    cf_medico_richiedente CHAR(16) NOT NULL,
    cf_paziente CHAR(16), -- foreign key
    id_ospedale_ricovero INT, -- foreign key
    data_richiesta TIMESTAMP(0),
    quantita_sacche INT NOT NULL,
    emocomponente_richiesto emocomponente_enum NOT NULL,
    reparto_destinazione reparto_enum NOT NULL,
    stato stato_richiesta_enum DEFAULT 'Non evasa',
        FOREIGN KEY (cf_paziente) REFERENCES PAZIENTE(cf),
        FOREIGN KEY (id_ospedale_ricovero) REFERENCES OSPEDALE(id)
);

CREATE TABLE SACCA (
    id SERIAL PRIMARY KEY,
    contenuto emocomponente_enum NOT NULL,
    id_prelievo INT, -- foreign key
    data_scadenza TIMESTAMP(0) NOT NULL, -- non ricavabile tramite operazioni matematiche per legge
    stato_sacca stato_sacca_enum DEFAULT 'Disponibile',
        FOREIGN KEY (id_prelievo) REFERENCES PRELIEVO(id),
);

CREATE TABLE TRASFERIMENTO (
    id SERIAL PRIMARY KEY,
    id_sacca INT, -- foreign key
    id_ospedale_mittente INT, -- foreign key
    id_ospedale_destinatario INT, -- foreign key
    data_spedizione TIMESTAMP(0),
    data_arrivo TIMESTAMP(0),
        FOREIGN KEY (id_sacca) REFERENCES SACCA(id),
        FOREIGN KEY (id_ospedale_mittente) REFERENCES OSPEDALE(id),
        FOREIGN KEY (id_ospedale_destinatario) REFERENCES OSPEDALE(id)
);

CREATE TABLE TRASFUSIONE (
    id SERIAL PRIMARY KEY,
    data_operazione TIMESTAMP(0) NOT NULL,
    id_sacca INT UNIQUE, -- foreign key, limite per non avere che più pazienti hanno la stessa sacca
    id_richiesta INT, -- foreign key 
        FOREIGN KEY(id_sacca) REFERENCES SACCA(id),
        FOREIGN KEY(id_richiesta) REFERENCES RICHIESTA_SANGUE(id)
);

-- Popolamento delle tabelle
\i data/popola_ospedale.sql
\i data/popola_centro_prelievi.sql
\i data/popola_donatore.sql
\i data/popola_paziente.sql
\i data/popola_prelievo.sql
\i data/popola_richiesta_sangue.sql
\i data/popola_sacca.sql
\i data/popola_trasferimento.sql
\i data/popola_trasfusione.sql