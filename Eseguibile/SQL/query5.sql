SELECT o.id, o.nome, COUNT(t_in.id_sacca) AS sacche_ricevute, COUNT(t_out.id_sacca) AS sacche_spedite, COUNT(t_in.id_sacca) - COUNT(t_out.id_sacca) AS differenza
FROM ospedale o LEFT JOIN TRASFERIMENTO t_in ON o.id = t_in.id_ospedale_mittente 
LEFT JOIN TRASFERIMENTO t_out ON o.id = t_out.id_ospedale_destinatario 
GROUP BY o.id, o.nome 
HAVING COUNT(t_in.id_sacca) > COUNT(t_out.id_sacca) 
ORDER BY differenza DESC; 