<!-- legenda -->
chiave primaria 🔑 <!-- vincolo: unico -->
chiave esterna 🔗 <!-- collegato a -->


OSPADALE{
    id_ospedale 🔑 VARCHAR(64) not null <!-- vincolo: unico -->
    nome VARCHAR(128) 
    civico VARCHAR(16) not null
    via VARCHAR(128) not null
    città VARCHAR(128) not null
    provincia CHAR(2) not null
    cap CHAR(5) not null <!-- per verifica incrociata della correttezza di città e provincia -->
}

CENTRO_PRELIEVI{
    id_centro_prelievi 🔑 VARCHAR(64) not null <!-- vincolo: unico -->
    nome VARCHAR(128)
    civico VARCHAR(16) not null
    via VARCHAR(128) not null
    città VARCHAR(128) not null
    provincia CHAR(2) not null
    cap CHAR(5) not null <!-- per verifica incrociata della correttezza di città e provincia -->
    id_ospedale 🔗 VARCHAR(64) not null <!-- collegato a OSPEDALE -->
}

DONATORE{
    cf_donatore 🔑 CHAR(16) <!-- vincolo: unico -->
    nome VARCHAR(128) not null
    cognome VARCHAR(128) not null
    gruppo_sanguigno ENUM [Valori: 'A', 'B', 'AB', '0'] not null
    fattore_rh ENUM [Valori: '+', '-'] not null
    numero_di_telefono VARCHAR(15) not null <!-- Formato standard E.164: prefisso internazionale (+ seguito da 1 a 3 cifre) e numero del Paese locale (fino a 11 cifre) -->
}

PAZIENTE{
    cf_paziente 🔑 CHAR(16) not null <!-- vincolo: unico -->
    nome VARCHAR(128) not null
    cognome VARCHAR(128) not null
    gruppo_sanguigno ENUM [Valori: 'A', 'B', 'AB', '0'] not null
    fattore_rh ENUM [Valori: '+', '-'] not null
    id_ospedale_ricovero 🔗 VARCHAR(64) not null <!-- collegato a OSPEDALE -->
}

PRELIEVO{
    id_prelievo 🔑 INT(64) not null <!-- vincolo: unico, incrementale automatico -->
    cf_donatore 🔗 CHAR(16) not null <!-- collegato a DONATORE -->
    id_centro_prelievi 🔗 VARCHAR(64) not null <!-- collegato a CENTRO_PRELIEVI -->
    data_prelievo TIMESTAMP(MM:DD, HH:MM) <!-- Formato per l'orario senza fuso orario, GIORNO:MESE , ORE:MINUTI -->
}

SACCA{
    id_sacca 🔑 INT(64) not null <!-- vincolo: unico, incrementale automatico -->
    id_prelievo 🔗 INT(64) not null  <!-- collegato a PRELIEVO -->
    contenuto ENUM [Valori: 'Sangue intero', 'Plasmaferesi'] not null
    gruppo_sanguigno ENUM [Valori: 'A', 'B', 'AB', '0'] not null
    fattore_rh ENUM [Valori: '+', '-'] not null
    data_scadenza TIMESTAMP(YYYY:MM:DD, HH:MM) <!-- non ricavabile tramite operazioni matematiche per legge -->
    stato_sacca ENUM [Valori: 'Disponibile','In uso', 'Scartata', 'In transito', 'Esaurita' ]
    id_ospedale 🔗 VARCHAR(64) not null.  <!-- collegato a OSPEDALE -->
}

RICHIESTA_SANGUE{
    id_richiesta 🔑 INT(64) not null <!-- identificativo numerico univoco (auto-incrementale?) --> <!-- vincolo: unico, incrementale automatico -->
    cf_paziente 🔗 CHAR(16) not null  <!-- collegato a PAZIENTE -->
    cf_medico_richiedente CHAR(16) not null <!-- per motivi di tracciabilità e trasparenza -->
    data_richiesta TIMESTAMP(YYYY:MM:DD, HH:MM) <!-- Formato per l'orario senza fuso orario, GIORNO:MESE , ORE:MINUTI -->
    quantita_sacche INT(32) not null <!-- CHECK: 0 < quantità sacche <= 32 >> -->
    tipo_operazione ENUM [Valori: 'Pronto Soccorso', 'Chirurgia', 'Oncologia ed Ematologia', 'Ostetricia', 'Terapia Intensiva e Rianimazione', 'Degenza ordinaria']
    stato ENUM [Valori: 'Non evasa', 'Evasa parzialmente', 'Soddisfatta', 'Annullata'] <!-- default: non evasa -->
}

TRASFERIMENTO{
    id_trasferimento 🔑 INT(64) not null <!-- vincolo: unico, incrementale automatico -->
    id_sacca 🔗 INT(64) not null <!-- collegato a SACCA -->
    id_ospedale_mittente 🔗 VARCHAR(64) not null <!-- collegato a OSPEDALE -->
    id_ospedale_destinatario 🔗 VARCHAR(64) not null <!-- collegato a OSPEDALE -->
    data_spedizione TIMESTAMP(YYYY:MM:DD, HH:MM) not null
    data_arrivo TIMESTAMP(YYYY:MM:DD, HH:MM) not null
    stato_spedizione ENUM [Valori: 'In elaborazione', 'Consegnata', 'In transito'] <!-- default: In elaborazione -->
}

TRASFUSIONE{
    id_trasfusione 🔑 INT(64) not null <!-- vincolo: unico, incrementale automatico -->
    id_sacca 🔗 INT(64) not null <!-- collegato a SACCA -->
    id_richiesta 🔗 INT(64) not null <!-- collegato a RICHIESTA_SANGUE -->
    data_trasfusione TIMESTAMP(YYYY:MM:DD, HH:MM) not null
}
