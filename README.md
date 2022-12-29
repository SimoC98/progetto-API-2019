# progetto-API-2019
Si vuole implementare un meccanismo di monitoraggio di relazioni tra entità (per esempio persone) che cambiano nel tempo. Si immagini, per esempio, un social network, in cui nuovi utenti possono registrarsi, e utenti esistenti possono cancellare il proprio account, diventare “amici” di altri utenti, rompere la relazione di amicizia, ecc. Le relazioni tra entità non sono necessariamente simmetriche. Per esempio, Alice può essere “amica” di Bruno, ma l’amicizia non è reciprocata (Bruno non è amico di Alice)
### scopo del progetto
Implementazione in linguaggio C standard (con la sola libc) di un programma che legge da standard input una sequenza di comandi, ognuno corrispondente ad un cambiamento nelle entità o nelle relazioni tra entità e, quando richiesto, produce su standard output, per ogni tipo di relazione monitorata, l'identificativo dell'entità che è il ricevente del maggior numero di istanze di quella relazione, e il numero di relazioni che l'entità riceve
### comandi
• addent ⟨id_ent⟩ : aggiunge un'entità identificata da "id_ent" all'insieme delle entità monitorate; se l'entità è già monitorata, non fa nulla
• delent ⟨id_ent⟩ : elimina l'entità identificata da "id_ent" dall'insieme delle entità monitorate; elimina tutte le relazioni di cui "id_ent" fa
parte (sia come origine, che come destinazione)
• addrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩ : aggiunge una relazione – identificata da "id_rel" – tra le entità "id_orig" e "id_dest", in cui "id_dest" è il ricevente della
relazione. Se la relazione tra "id_orig" e "id_dest" già esiste, o se almeno una delle entità non è monitorata, non fa
nulla. Il monitoraggio del tipo di relazione "id_rel" inizia implicitamente con il primo comando "addrel" che la riguarda.
• delrel ⟨id_orig⟩ ⟨id_dest⟩ ⟨id_rel⟩ : elimina la relazione identificata da "id_rel" tra le entità "id_orig" e "id_dest" (laddove "id_dest" è il ricevente della
relazione); se non c'è relazione "id_rel" tra "id_orig" e "id_dest" (con "id_dest" come ricevente), non fa nulla
• report : emette in output l’elenco delle relazioni, riportando per ciascuna le entità con il maggior numero di relazioni entranti
• end : termine della sequenza di comandi
