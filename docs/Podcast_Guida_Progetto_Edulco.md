# Podcast mini guida - Progetto EdulcoWaterSmart

Questa guida e pensata come traccia podcast breve da inserire in repository.
Obiettivo: spiegare in modo semplice e pratico come avviare, capire e testare il progetto passo passo.

## Durata consigliata

15-20 minuti.

## Pubblico

- Sviluppatori che aprono il progetto per la prima volta
- Tecnici che devono fare test rapidi su ESP32
- Chi vuole integrare i comandi HTTP in Home Assistant

## Scaletta episodio

## 1) Intro (1 minuto)

Testo suggerito:
Ciao, in questa puntata vediamo come usare EdulcoWaterSmart in modalita pratica.
Partiamo dalla struttura del repository, carichiamo l esempio su ESP32, poi facciamo i test via HTTP con i comandi smartcommand.
Alla fine saprai come aggiungere un comando nuovo in sicurezza.

## 2) Panoramica repository (2 minuti)

Spiega questi punti:
- La libreria principale sta nella cartella src e contiene logica sensori e controlli.
- Gli esempi stanno in examples.
- Per la demo web e Home Assistant usa examples/EdulcoHomeAssistant/EdulcoHomeAssistant.ino.
- In docs trovi questa guida e materiale di supporto.

## 3) Setup rapido hardware e firmware (3 minuti)

Passaggi da raccontare:
1. Collega ESP32 al PC.
2. Apri examples/EdulcoHomeAssistant/EdulcoHomeAssistant.ino in Arduino IDE.
3. Imposta board ESP32 e porta seriale corretta.
4. Inserisci SSID e password Wi-Fi nel file esempio.
5. Compila e carica.
6. Apri monitor seriale a 115200 per verificare startup e IP assegnato.

## 4) Cosa fa l esempio (2 minuti)

Spiegazione semplice:
- Inizializza EdulcoWaterSmart.
- Connette ESP32 alla rete Wi-Fi.
- Avvia server HTTP su porta 80.
- Espone endpoint POST /smartcommand.
- Stampa periodicamente i valori sensori su seriale.

## 5) Test comandi HTTP passo passo (4 minuti)

Usa un PC nella stessa rete del modulo.
Sostituisci IP con quello visto nel monitor seriale.

Comandi base:
- curl -X POST http://IP/smartcommand -d "cmd=getdeviceabout"
- curl -X POST http://IP/smartcommand -d "cmd=getcontrolstatus&ctrl=0"
- curl -X POST http://IP/smartcommand -d "cmd=setrelay&relay=1&status=true"
- curl -X POST http://IP/smartcommand -d "cmd=setrelay&relay=1&status=false"

Controlli errore consigliati:
- cmd sconosciuto
- relay fuori range
- status non valido

## 6) Come aggiungere un comando nuovo (4 minuti)

Workflow consigliato:
1. Apri funzione WSER_handleSmartCommand.
2. Aggiungi nuovo ramo else if con nome cmd.
3. Valida tutti i parametri in ingresso.
4. Esegui azione usando API gia presenti su g_device.
5. Rispondi con HTTP 200 su successo o 400 su errore.
6. Testa con curl e verifica anche i casi invalidi.

Regole pratiche:
- Mantieni i comandi semplici e leggibili.
- Non introdurre dipendenze non necessarie nella prima release.
- Evita logica complessa nel parser HTTP, delega il piu possibile alla libreria.

## 7) Chiusura episodio (1 minuto)

Testo suggerito:
In questa guida abbiamo visto struttura, upload firmware, test API e aggiunta di nuovi comandi.
Nel prossimo episodio possiamo coprire autenticazione endpoint, logging avanzato e integrazione completa con automazioni Home Assistant.

## Check finale per publish in repository

- Titolo chiaro
- Durata episodio
- Prerequisiti minimi
- Passi numerati
- Comandi di verifica
- Errori comuni
- Prossimi step

Fine guida.
