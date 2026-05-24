# Base di Dati di un Sistema Informativo per la Gestione della Donazione del Sangue



## Indice



1. Abstract  

2. Analisi dei Requisiti  

3. Progettazione Concettuale  

4. Progettazione logica



---



# 1 Abstract



Questo progetto sviluppa una base di dati progettata per gestire in modo strutturato e coerente le informazioni relative ai donatori di sangue, ai prelievi effettuati, alle sacche prodotte, ai trasferimenti tra ospedali e alle trasfusioni effettuate sui pazienti.



L’obiettivo principale è fornire un sistema che supporti le attività di raccolta, conservazione, trasporto e utilizzo del sangue e del plasma, garantendo la tracciabilità completa delle sacche durante tutto il loro ciclo di vita. La base di dati consente inoltre di monitorare le richieste di sangue effettuate dagli ospedali e la gestione dei pazienti sottoposti a trasfusione.



Nel contesto sanitario, il sistema distingue i centri prelievi dagli ospedali, permettendo di registrare i prelievi effettuati dai donatori, la creazione delle sacche, il loro stato di disponibilità e il loro eventuale trasferimento tra strutture ospedaliere. Ogni sacca può essere utilizzata in una trasfusione, collegata a una specifica richiesta di sangue e a un determinato paziente.



Questa base di dati è progettata per garantire un’archiviazione efficiente e organizzata delle informazioni sanitarie, migliorando il recupero dei dati e il controllo delle operazioni di gestione del sangue all’interno delle strutture ospedaliere.



---



# 2 Analisi dei Requisiti



Questa sezione riassume i requisiti a cui deve sottostare la base di dati.



## Donatori



Ogni donatore è identificato tramite:

- Codice fiscale.

- Nominativo.

- Numero di telefono.

- Gruppo sanguigno.

- Fattore Rh.



Un donatore può effettuare più prelievi in momenti differenti.



---



## Prelievi



Ogni prelievo è identificato da:

- ID_prelievo.

- Data del prelievo.



Ogni prelievo:

- è effettuato da un solo donatore;

- viene eseguito presso un centro prelievi;

- genera una sola sacca.




---



## Centri Prelievi



Ogni centro prelievi è identificato da:

- ID_centro.

- Nome.

- Indirizzo.



L’indirizzo contiene:

- Via.

- Civico.

- CAP.

- Città.

- Provincia.



Ogni centro prelievi può:

- eseguire più prelievi;

- rifornire un solo ospedale.



---



## Sacche



Ogni sacca è identificata da:

- ID_sacca.

- Gruppo sanguigno.

- Fattore Rh.

- Stato della sacca.

- Data di scadenza.



Lo stato della sacca può assumere valori differenti, come:

- in attesa;

- disponibile;

- prenotata;

- utilizzata.



Ogni sacca viene generata da un solo prelievo.



Le sacche si distinguono in:

- sangue;

- plasma.



Una sacca può:

- essere immagazzinata in un ospedale;

- essere contenuta in un trasferimento;

- essere utilizzata in una trasfusione.



---



## Ospedali



Ogni ospedale è identificato da:

- ID_ospedale.

- Nome.

- Indirizzo.



L’indirizzo contiene:

- Via.

- Civico.

- CAP.

- Città.

- Provincia.



Ogni ospedale può:

- ricevere trasferimenti;

- spedire trasferimenti;

- immagazzinare sacche;

- ricoverare pazienti.



---



## Trasferimenti



Ogni trasferimento è identificato da:

- ID_spedizione.

- Data di spedizione.

- Data di arrivo.

- Stato della spedizione.



Ogni trasferimento:

- contiene una o più sacche;

- viene spedito da un ospedale;

- viene ricevuto da un ospedale.



Un ospedale può spedire più trasferimenti e può riceverne più di uno.



---



## Pazienti



Ogni paziente è identificato tramite:

- Codice fiscale.

- Nominativo.

- Gruppo sanguigno.

- Fattore Rh.



Un paziente può essere ricoverato in più ospedali nel tempo.



---



## Richieste di sangue



Ogni richiesta di sangue è identificata da:

- ID_richiesta.

- Quantità richiesta.

- Emocomponente.

- Tipo operazione.

- Stato.

- CF del medico.



Ogni richiesta:

- appartiene a un paziente;

- può portare a una trasfusione.



---



## Trasfusioni



Ogni trasfusione è identificata da:

- ID_trasfusione.

- Data della trasfusione.



Ogni trasfusione:

- utilizza una sola sacca;


- deriva da una richiesta di sangue.



---



# 3 Progettazione Concettuale



Il diagramma Entità-Relazione (E-R) riassume i requisiti descritti nella sezione precedente.



Ogni donatore può effettuare uno o più prelievi, mentre ogni prelievo appartiene a un solo donatore. I prelievi vengono eseguiti presso i centri prelievi, che rappresentano le strutture incaricate della raccolta del sangue e del plasma.



Ogni prelievo genera una sola sacca. La sacca rappresenta l’elemento centrale del sistema ed è caratterizzata dal gruppo sanguigno, dal fattore Rh, dalla data di scadenza e dallo stato corrente. Le sacche possono essere di sangue oppure di plasma; per questo motivo il modello include una generalizzazione dell’entità Sacca nelle entità Sangue e Plasma.



I centri prelievi possono rifornire più ospedali, mentre ogni ospedale può ricevere sacche da più centri differenti. Gli ospedali costituiscono le strutture principali di gestione delle sacche disponibili e possono immagazzinare il materiale ricevuto.



Il trasferimento delle sacche tra ospedali viene rappresentato tramite l’entità Trasferimento, che contiene le informazioni relative alla spedizione e all’arrivo delle sacche. Ogni trasferimento può contenere più sacche ed è associato a un ospedale mittente e a un ospedale destinatario, distinti rispettivamente tramite le relazioni spedisce e riceve.



Il sistema tiene inoltre traccia dei pazienti e delle richieste di sangue effettuate per essi. Ogni richiesta può essere associata a una trasfusione, che utilizza una specifica sacca.



La relazione tra paziente e ospedale è di tipo molti-a-molti, poiché un paziente può essere ricoverato in più ospedali nel tempo e un ospedale può ricoverare più pazienti.



Il modello E-R contiene quindi le entità Donatore, Prelievo, Centro Prelievi, Sacca, Sangue, Plasma, Ospedale, Trasferimento, Paziente, Richiesta sangue e Trasfusione, collegate tramite le relazioni dona, esegue, crea, rifornisce, immagazzina, contiene, spedisce, riceve, ricovera, necessita, subisce e usata.



---



# 4 Progettazione logica

In questa sezione viene illustrato il processo di ristrutturazione dello schema concettuale, con l'obiettivo di ottenere uno schema facilmente traducibile nel modello relazionale. In particolare, vengono analizzate le generalizzazioni presenti nel diagramma E-R, vengono scelti gli identificatori delle entità e infine viene effettuata la traduzione verso il modello relazionale.

## 4.1 Eliminazione delle generalizzazioni

Nel diagramma E-R è presente una sola generalizzazione, rappresentata dall'entità Sacca, suddivisa nelle due entità figlie Sangue intero e Plasma.

Poiché la distinzione tra le due categorie riguarda esclusivamente il contenuto della sacca e non introduce attributi o relazioni aggiuntive, si è scelto di eliminare la generalizzazione accorpando le entità figlie nell'entità padre.

L'eliminazione viene effettuata introducendo nell'entità Sacca l'attributo contenuto, il cui dominio è costituito dai valori:

- Sangue intero
- Plasma

Questa soluzione consente di ridurre il numero di entità presenti nello schema, semplificando la successiva traduzione nel modello relazionale senza perdita di informazione. Inoltre, evita la creazione di tabelle aggiuntive e riduce la complessità delle interrogazioni che coinvolgono le sacche, mantenendo invariata la capacità espressiva del modello.

## 4.2 Analisi delle rindondanze

Dall'analisi dello schema E-R emerge una possibile ridondnaza relativa agli attributi `gruppo_sanguigno` e `fattore_RH` presenti nell'entità  `SACCA`.

Informazioni che possono essere prelevate attraverso il percorso:

`SACCA → PRELIEVO → DONATORE`

poiché  ogni sacca deriva da un prelievo effettuato da un donatore e il gruppo sanguigno ed il fattore Rh del donatore coincidono con quelli sacca prodotta.

La presenza di tali attributi nell'entità `SACCA` introduce ad una ridondanza informativa da analizzareper valutare se conviene mantenerla oppure no.

Si considerano le seguenti operazioni:

- Operazione 1 (100 volte al giorno): registrazione di una nuova sacca a seguito di un prelivo.
- Operazione 2 (300 volte al giorno): ricerca di sacche compatibili per una richiesta di sangue.

Si assumono i seguenti volumi:

| Concetto | Costrutto | Volume |
|-----------|------------|---------|
| DONATORE | E | 300 |
| PRELIEVO | E | 1000 |
| SACCA | E | 1000 |

### Con ridondanza

Operazione 1

| Concetto | Accessi | Tipo |
|-----------|---------|------|
| PRELIEVO | 1 | L |
| DONATORE | 1 | L |
| SACCA | 1 | S |

Costo = 1 + 1 + 2 = 4

Operazione 2

| Concetto | Accessi | Tipo |
|-----------|---------|------|
| SACCA | N | L |

Costo = N = volume concetto sacca = 1000

Costo totale = 100 x 4 + 300 x 1000 = 300400

### Senza ridondanza

Operazione 1

| Concetto | Accessi | Tipo |
|-----------|---------|------|
| PRELIEVO | 1 | L |
| DONATORE | 1 | L |
| SACCA | 1 | S |

Costo = 1 + 1 + 2 = 4

Operazione 2

| Concetto | Accessi | Tipo |
|-----------|---------|------|
| PRELIEVO | N | L |
| DONATORE | N | L |
| SACCA | N | S |

Costo = 3N = 3000

Costo totale = 100 x 4 + 300 x 3000 = 900400

Dall'analisi effettuata emerge che il mantenimento degli attributi `gruppo_sanguigno` e `fattore_rh` all'interno dell'entità `SACCA` comporta una ridondanza informativa, in quanto tali dati possono essere ricavati attraverso il percorso `SACCA → PRELIEVO → DONATORE`.

Tuttavia, il confronto tra i costi delle operazioni mostra come la presenza della ridondanza riduca significativamente il numero di accessi necessari durante le interrogazioni più frequenti, in particolare quelle finalizzate alla ricerca di sacche compatibili per le richieste di sangue. Inoltre, la memorizzazione diretta di tali informazioni nella sacca consente di limitare l'accesso ai dati anagrafici del donatore, garantendo una maggiore separazione tra dati clinici e dati personali.

Per questi motivi si è scelto di mantenere gli attributi `gruppo_sanguigno` e `fattore_rh` nell'entità `SACCA`, privilegiando l'efficienza delle interrogazioni e la tutela della riservatezza dei dati rispetto alla completa eliminazione della ridondanza.

## 4.3 Scelta degli identificatori

In questa fase vengono analizzati gli identificatori delle entità presenti nel modello, verificando che ciascuna istanza possa essere identificata in maniera univoca e stabile nel tempo.

Per le entità Donatore e Paziente è stato scelto come identificatore il codice fiscale. Tale attributo rappresenta un identificatore naturale, univoco a livello nazionale e immutabile nel tempo, risultando quindi adatto all'identificazione delle persone coinvolte nel sistema.

Per le entità Prelievo, Sacca, Trasferimento, Richiesta di sangue e Trasfusione si è scelto di utilizzare identificatori artificiali numerici (`id_prelievo`, `id_sacca`, `id_trasferimento`, `id_richiesta`, `id_trasfusione`). Questa soluzione consente di garantire l'univocità delle istanze evitando l'utilizzo di chiavi composte da numerosi attributi e semplificando la definizione delle chiavi esterne nelle relazioni.

L'entità Ospedale viene identificata mediante un identificatore numerico (`id_ospedale`). Sebbene il nome dell'ospedale possa risultare significativo, esso non garantisce necessariamente unicità e stabilità nel tempo; per questo motivo è stato preferito un identificatore artificiale.

L'entità Centro Prelievi è identificata mediante la coppia (`id_ospedale`, `nome`). Tale scelta deriva dal fatto che ogni centro prelievi è gestito da un solo ospedale ed il nome del centro risulta univoco solamente all'interno dell'ospedale di appartenenza.

Infine, per l'entità derivata dalla relazione molti-a-molti Ricovero, è stato adottato come identificatore la combinazione (`cf_paziente`, `id_ospedale`, `data_ricovero`). La presenza della data di ricovero permette infatti di distinguere eventuali ricoveri multipli dello stesso paziente presso il medesimo ospedale in momenti differenti.

La scelta effettuata privilegia identificatori stabili, di dimensione ridotta e facilmente utilizzabili nella definizione delle relazioni tra le diverse entità del sistema.

## 4.4 Traduzione verso il modello relazionale

Terminata la fase di ristrutturazione dello schema E-R, si procede con la traduzione verso il modello relazionale.

Le entità del diagramma vengono trasformate in relazioni mantenendo come chiave primaria l'identificatore scelto nella fase precedente. Gli attributi semplici vengono riportati direttamente nelle rispettive relazioni.

Le relazioni uno-a-molti vengono implementate mediante l'introduzione di una chiave esterna nella relazione corrispondente all'entità partecipante dal lato molti. Ad esempio, la relazione tra Centro Prelievi e Ospedale viene tradotta inserendo l'identificatore dell'ospedale all'interno della relazione CENTRO_PRELIEVI.

Le relazioni molti-a-molti vengono invece trasformate in apposite relazioni autonome. In particolare, la relazione Ricovera tra Paziente e Ospedale viene tradotta nella relazione RICOVERO, contenente le chiavi delle entità partecipanti e gli eventuali attributi propri della relazione.

La generalizzazione presente nello schema concettuale relativa all'entità Sacca è stata eliminata durante la fase di ristrutturazione mediante l'introduzione dell'attributo contenuto, evitando la creazione di relazioni separate per Sangue intero e Plasma.

La relazione Contiene tra Trasferimento e Sacca non è stata tradotta mediante una relazione autonoma poiché, in seguito alle scelte progettuali adottate, ogni sacca può appartenere ad un solo trasferimento. La relazione è stata pertanto implementata tramite una chiave esterna che collega direttamente la sacca al trasferimento di appartenenza.

Lo schema relazionale ottenuto è il seguente:

- OSPEDALE(id, nome, civico, via, citta, provincia, cap)\

- CENTRO_PRELIEVI(id_ospedale, nome, nome, civico, via, citta, provincia, cap)
  CENTRO_PRELIEVI.id_ospedale → OSPEDALE.id

- DONATORE(cf, nome, cognome, gruppo_sanguigno, fattore_rh, numero_di_telefono)

- PAZIENTE(cf, nome, cognome, gruppo_sanguigno, fattore_rh)

- RICOVERO(cf_paziente, id_ospedale, data_ricovero, reparto)
  RICOVERO.cf_paziente → PAZIENTE.cf
  RICOVERO.id_ospedale → OSPEDALE.id

- PRELIEVO(id, cf_donatore, id_ospedale, nome, data_ora)

- SACCA(id, contenuto, id_prelievo, gruppo_sanguigno, fattore_rh, data_scadenza, stato_sacca, id_trasferimento)

- TRASFERIMENTO(id, id_ospedale_mittente, id_ospedale_destinatario, data_spedizione, data_arrivo)

- RICHIESTA_SANGUE(id, cf_medico_richiedente, cf_paziente, id_ospedale_ricovero, data_richiesta, quantita_sacche, emocomponente_richiesto, reparto_destinazione, stato)
  RICHIESTA_SANGUE.cf_paziente → PAZIENTE.cf
  RICHIESTA_SANGUE.id_ospedale_ricovero → OSPEDALE.id

- TRASFUSIONE(id, data_operazione, id_sacca, id_richiesta)
  TRASFUSIONE.id_sacca → SACCA.id
  TRASFUSIONE.id_richiesta → RICHIESTA_SANGUE.id