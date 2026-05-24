SELECT o.nome, s.gruppo_sanguigno, s.fattore_rh, COUNT(*) AS numero_sacche
FROM sacca s JOIN ospedale o ON s.id_ospedale = o.id
WHERE s.stato_sacca = 'Disponibile'
GROUP BY o.nome, s.gruppo_sanguigno, s.fattore_rh
HAVING COUNT(*) < $1
ORDER BY  o.nome, s.gruppo_sanguigno, s.fattore_rh;