DROP VIEW IF EXISTS num_sacche_reparto;

CREATE VIEW num_sacche_reparto AS
SELECT o.id, o.nome, rs.reparto_destinazione, SUM(rs.quantita_sacche) AS Num_sacche
FROM RICHIESTA_SANGUE rs JOIN OSPEDALE o ON rs.id_ospedale_ricovero = o.id
GROUP BY o.id, o.nome, rs.reparto_destinazione
ORDER BY Num_sacche DESC;

SELECT id, nome, reparto_destinazione, Num_sacche
FROM num_sacche_reparto
WHERE Num_sacche = (SELECT MAX(Num_sacche)
					FROM num_sacche_reparto);