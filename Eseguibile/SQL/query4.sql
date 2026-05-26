DROP VIEW IF EXISTS donazioni_per_donatore;

CREATE VIEW donazioni_per_donatore AS
SELECT d.cf, d.nome, d.cognome, d.gruppo_sanguigno, d.fattore_rh, COUNT(p.id) AS numero_prelievi
FROM DONATORE d JOIN PRELIEVO p ON d.cf = p.cf_donatore
GROUP BY d.cf, d.nome, d.cognome, d.gruppo_sanguigno, d.fattore_rh; 

SELECT * 
FROM donazioni_per_donatore d1
WHERE numero_prelievi =(SELECT MAX(d2.numero_prelievi)
						FROM donazioni_per_donatore d2
						WHERE d2.gruppo_sanguigno = d1.gruppo_sanguigno
						AND d2.fattore_rh = d1.fattore_rh)
						ORDER BY gruppo_sanguigno, fattore_rh;