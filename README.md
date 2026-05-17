\# Base di Dati di un Sistema Informativo per la Gestione della Donazione del Sangue



\## Indice



1\. Abstract  

2\. Analisi dei Requisiti  

3\. Progettazione Concettuale  



\---



\# 1 Abstract



Questo progetto sviluppa una base di dati progettata per gestire in modo strutturato e coerente le informazioni relative ai donatori di sangue, ai prelievi effettuati, alle sacche prodotte, ai trasferimenti tra ospedali e alle trasfusioni effettuate sui pazienti.



L’obiettivo principale è fornire un sistema che supporti le attività di raccolta, conservazione, trasporto e utilizzo del sangue e del plasma, garantendo la tracciabilità completa delle sacche durante tutto il loro ciclo di vita. La base di dati consente inoltre di monitorare le richieste di sangue effettuate dagli ospedali e la gestione dei pazienti sottoposti a trasfusione.



Nel contesto sanitario, il sistema distingue i centri prelievi dagli ospedali, permettendo di registrare i prelievi effettuati dai donatori, la creazione delle sacche, il loro stato di disponibilità e il loro eventuale trasferimento tra strutture ospedaliere. Ogni sacca può essere utilizzata in una trasfusione, collegata a una specifica richiesta di sangue e a un determinato paziente.



Questa base di dati è progettata per garantire un’archiviazione efficiente e organizzata delle informazioni sanitarie, migliorando il recupero dei dati e il controllo delle operazioni di gestione del sangue all’interno delle strutture ospedaliere.



\---



\# 2 Analisi dei Requisiti



Questa sezione riassume i requisiti a cui deve sottostare la base di dati.



\## Donatori



Ogni donatore è identificato tramite:

\- Codice fiscale.

\- Nominativo.

\- Numero di telefono.

\- Gruppo sanguigno.

\- Fattore Rh.



Un donatore può effettuare più prelievi in momenti differenti.



\---



\## Prelievi



Ogni prelievo è identificato da:

\- ID\_prelievo.

\- Data del prelievo.



Ogni prelievo:

\- è effettuato da un solo donatore;

\- viene eseguito presso un centro prelievi;

\- genera una sola sacca.



\---



\## Centri Prelievi



Ogni centro prelievi è identificato da:

\- ID\_centro.

\- Nome.

\- Indirizzo.



L’indirizzo contiene:

\- Via.

\- Civico.

\- CAP.

\- Città.

\- Provincia.



Ogni centro prelievi può:

\- eseguire più prelievi;

\- rifornire un solo ospedale.



\---



\## Sacche



Ogni sacca è identificata da:

\- ID\_sacca.

\- Gruppo sanguigno.

\- Fattore Rh.

\- Stato della sacca.

\- Data di scadenza.



Lo stato della sacca può assumere valori differenti, come:

\- in attesa;

\- disponibile;

\- prenotata;

\- utilizzata.



Ogni sacca viene generata da un solo prelievo.



Le sacche si distinguono in:

\- sangue;

\- plasma.



Una sacca può:

\- essere immagazzinata in un ospedale;

\- essere contenuta in un trasferimento;

\- essere utilizzata in una trasfusione.



\---



\## Ospedali



Ogni ospedale è identificato da:

\- ID\_ospedale.

\- Nome.

\- Indirizzo.



L’indirizzo contiene:

\- Via.

\- Civico.

\- CAP.

\- Città.

\- Provincia.



Ogni ospedale può:

\- ricevere trasferimenti;

\- spedire trasferimenti;

\- immagazzinare sacche;

\- ricoverare pazienti.



\---



\## Trasferimenti



Ogni trasferimento è identificato da:

\- ID\_spedizione.

\- Data di spedizione.

\- Data di arrivo.

\- Stato della spedizione.



Ogni trasferimento:

\- contiene una o più sacche;

\- viene spedito da un ospedale;

\- viene ricevuto da un ospedale.



Un ospedale può spedire più trasferimenti e può riceverne più di uno.



\---



\## Pazienti



Ogni paziente è identificato tramite:

\- Codice fiscale.

\- Nominativo.

\- Gruppo sanguigno.

\- Fattore Rh.



Un paziente può essere ricoverato in più ospedali nel tempo.



\---



\## Richieste di sangue



Ogni richiesta di sangue è identificata da:

\- ID\_richiesta.

\- Quantità richiesta.

\- Emocomponente.

\- Tipo operazione.

\- Stato.

\- CF del medico.



Ogni richiesta:

\- appartiene a un paziente;

\- può portare a una trasfusione.



\---



\## Trasfusioni



Ogni trasfusione è identificata da:

\- ID\_trasfusione.

\- Data della trasfusione.



Ogni trasfusione:

\- utilizza una sola sacca;

\- deriva da una richiesta di sangue.



\---



\# 3 Progettazione Concettuale



Il diagramma Entità-Relazione (E-R) riassume i requisiti descritti nella sezione precedente.



Ogni donatore può effettuare uno o più prelievi, mentre ogni prelievo appartiene a un solo donatore. I prelievi vengono eseguiti presso i centri prelievi, che rappresentano le strutture incaricate della raccolta del sangue e del plasma.



Ogni prelievo genera una sola sacca. La sacca rappresenta l’elemento centrale del sistema ed è caratterizzata dal gruppo sanguigno, dal fattore Rh, dalla data di scadenza e dallo stato corrente. Le sacche possono essere di sangue oppure di plasma; per questo motivo il modello include una generalizzazione dell’entità Sacca nelle entità Sangue e Plasma.



I centri prelievi possono rifornire più ospedali, mentre ogni ospedale può ricevere sacche da più centri differenti. Gli ospedali costituiscono le strutture principali di gestione delle sacche disponibili e possono immagazzinare il materiale ricevuto.



Il trasferimento delle sacche tra ospedali viene rappresentato tramite l’entità Trasferimento, che contiene le informazioni relative alla spedizione e all’arrivo delle sacche. Ogni trasferimento può contenere più sacche ed è associato a un ospedale mittente e a un ospedale destinatario, distinti rispettivamente tramite le relazioni spedisce e riceve.



Il sistema tiene inoltre traccia dei pazienti e delle richieste di sangue effettuate per essi. Ogni richiesta può essere associata a una trasfusione, che utilizza una specifica sacca.



La relazione tra paziente e ospedale è di tipo molti-a-molti, poiché un paziente può essere ricoverato in più ospedali nel tempo e un ospedale può ricoverare più pazienti.



Il modello E-R contiene quindi le entità Donatore, Prelievo, Centro Prelievi, Sacca, Sangue, Plasma, Ospedale, Trasferimento, Paziente, Richiesta sangue e Trasfusione, collegate tramite le relazioni dona, esegue, crea, rifornisce, immagazzina, contiene, spedisce, riceve, ricovera, necessita, subisce e usata.

