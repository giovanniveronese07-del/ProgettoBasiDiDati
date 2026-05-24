SELECT o.provincia, s.gruppo_sanguigno, fattore_rh, COUNT(*) AS num_scorte
FROM ospedali o JOIN sacche s ON o.id = s.id_ospedale
WHERE s.stato = 'disponibile'
GROUP BY o.provincia, s.gruppo_sanguigno, fattore_rh
ORDER BY o.provincia, s.gruppo_sanguigno, fattore_rh;