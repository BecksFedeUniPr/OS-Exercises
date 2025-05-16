## Guida alla contribuzione al repository

Questa guida ti aiuterà a contribuire a questo repository, mostrando come fare fork, commit e pull request.

## Indice
- [Guida alla contribuzione al repository](#guida-alla-contribuzione-al-repository)
  - [1. Fare un fork del repository](#1-fare-un-fork-del-repository)
  - [2. Clonare il repository localmente](#2-clonare-il-repository-localmente)
  - [3. Configurare il repository remoto originale](#3-configurare-il-repository-remoto-originale)
  - [4. Creare un branch per le tue modifiche](#4-creare-un-branch-per-le-tue-modifiche)
  - [5. Fare modifiche ai file](#5-fare-modifiche-ai-file)
  - [6. Verificare lo stato delle modifiche](#6-verificare-lo-stato-delle-modifiche)
  - [7. Committare le modifiche](#7-committare-le-modifiche)
  - [8. Fare push delle modifiche sul tuo fork](#8-fare-push-delle-modifiche-sul-tuo-fork)
  - [9. Creare una pull request](#9-creare-una-pull-request)
  - [10. Mantenere il tuo fork aggiornato](#10-mantenere-il-tuo-fork-aggiornato)
- [Riepilogo del flusso di lavoro per nuove modifiche](#riepilogo-del-flusso-di-lavoro-per-nuove-modifiche)
- [Esempio pratico completo](#esempio-pratico-completo)
- [Maintainer guida](#maintainer-guida)
  - [Gestione delle pull request](#gestione-delle-pull-request)
    - [Review e approvazione](#review-e-approvazione)
    - [Test locale delle PR](#test-locale-delle-pr)
    - [Opzioni di merge](#opzioni-di-merge)
    - [Comunicazione efficace](#comunicazione-efficace)
  - [Manutenzione del repository](#manutenzione-del-repository)
    - [Protezione dei branch](#protezione-dei-branch)
    - [Gestione delle Issues](#gestione-delle-issues)
    - [Versioni e tag](#versioni-e-tag)

### 1. Fare un fork del repository

Per iniziare a contribuire, devi **creare un fork una sola volta**. Il fork è una copia personale del repository:

1. Vai alla pagina del repository: https://github.com/FedeeSki/OS-Exercises
2. Clicca sul pulsante "Fork" nell'angolo in alto a destra
3. Seleziona il tuo account GitHub come destinazione del fork

**Nota importante**: Il fork va fatto solo una volta all'inizio. Non è necessario creare un nuovo fork per ogni modifica.

### 2. Clonare il repository localmente

Dopo aver fatto il fork, clona il repository sul tuo computer (da fare solo una volta):

```bash
# Sostituisci "TUO-USERNAME" con il tuo nome utente GitHub
git clone https://github.com/TUO-USERNAME/OS-Exercises.git
cd OS-Exercises
```

### 3. Configurare il repository remoto originale

Per mantenere il tuo fork aggiornato, aggiungi il repository originale come remote (da fare solo una volta):

```bash
git remote add upstream https://github.com/FedeeSki/OS-Exercises.git
```

### 4. Creare un branch per le tue modifiche

**Crea un nuovo branch per ogni modifica o funzionalità distinta**:

```bash
# Assicurati di essere su main/master aggiornato
git checkout main
git pull upstream main

# Crea un nuovo branch per la tua nuova modifica
git checkout -b nome-feature
```

**IMPORTANTE**: Devi creare un nuovo branch per ogni nuova modifica o funzionalità. Non riutilizzare lo stesso branch per modifiche diverse.

Esempi di nomi di branch significativi:
- `fix-cpu-scheduler-bug`
- `add-new-disk-example`
- `update-readme`

### 5. Fare modifiche ai file

Ora puoi modificare i file nel repository. Ecco alcuni esempi:

#### Esempio 1: Aggiungere un nuovo esempio di scheduling CPU

```bash
# Modifica il file di esempio
nano 03-CPU-Scheduling/examples/round_robin.c

# Oppure usa il tuo editor preferito
code 03-CPU-Scheduling/examples/round_robin.c
```

#### Esempio 2: Correggere un bug in un esempio di thread

```bash
# Modifica un file esistente
nano 02-Thread/examples/mutex_example.c
```

#### Esempio 3: Aggiungere documentazione

```bash
# Aggiungi documentazione a una cartella specifica
nano 06-Page-replacement/README.md
```

### 6. Verificare lo stato delle modifiche

Controlla quali file hai modificato:

```bash
git status
```

### 7. Committare le modifiche

Aggiungi i file modificati e crea un commit:

```bash
# Aggiungi file specifici
git add 03-CPU-Scheduling/examples/round_robin.c

# Oppure aggiungi tutti i file modificati
git add .

# Crea un commit con un messaggio descrittivo
git commit -m "Aggiunge esempio di scheduling Round Robin con gestione della starvation"
```

Consigli per un buon messaggio di commit:
- Sii specifico e conciso
- Usa il tempo presente ("Aggiunge" non "Aggiunto")
- Spiega cosa fa il commit e perché

### 8. Fare push delle modifiche sul tuo fork

Carica le modifiche sul tuo fork:

```bash
git push origin nome-feature
```

### 9. Creare una pull request

Per proporre le tue modifiche al repository originale:

1. Vai alla pagina del tuo fork su GitHub
2. Clicca su "Compare & pull request" (appare dopo il push)
3. Assicurati che il repository base sia `FedeeSki/OS-Exercises` e il branch di confronto sia il tuo `nome-feature`
4. Aggiungi un titolo e una descrizione dettagliata della tua pull request
5. Clicca su "Create pull request"

### 10. Mantenere il tuo fork aggiornato

Per aggiornare il tuo fork con le ultime modifiche dal repository originale:

```bash
# Passa al branch main
git checkout main

# Scarica gli ultimi aggiornamenti dal repository originale
git fetch upstream

# Unisci gli aggiornamenti nel tuo branch main locale
git merge upstream/main

# Carica gli aggiornamenti sul tuo fork
git push origin main
```

### Riepilogo del flusso di lavoro per nuove modifiche

1. **Una volta sola**:
   - Fork del repository
   - Clone del fork
   - Aggiunta del repository originale come remote upstream

2. **Per ogni nuova modifica**:
   - Aggiorna il tuo main: `git checkout main && git pull upstream main`
   - Crea un nuovo branch: `git checkout -b nuovo-feature`
   - Fai le modifiche ai file
   - Commit delle modifiche: `git add . && git commit -m "Descrizione"`
   - Push del branch: `git push origin nuovo-feature`
   - Crea una pull request su GitHub

### Esempio pratico completo

Ecco un esempio completo per aggiungere un nuovo esempio di mutex:

```bash
# Una volta sola: Clona il repository
git clone https://github.com/TUO-USERNAME/OS-Exercises.git
cd OS-Exercises

# Una volta sola: Aggiungi il remote upstream
git remote add upstream https://github.com/FedeeSki/OS-Exercises.git

# Per ogni nuova modifica: Aggiorna main e crea un nuovo branch
git checkout main
git pull upstream main
git checkout -b add-mutex-example

# Crea un nuovo file di esempio
mkdir -p 02-Thread/examples
touch 02-Thread/examples/advanced_mutex.c

# Modifica il file con il tuo editor preferito
nano 02-Thread/examples/advanced_mutex.c

# Aggiungi il file e committa
git add 02-Thread/examples/advanced_mutex.c
git commit -m "Aggiunge esempio avanzato di mutex con gestione timeout"

# Pusha sul tuo fork
git push origin add-mutex-example

# Ora vai su GitHub e crea la pull request
```

# Maintainer guida

## Gestione delle pull request

Come maintainer del repository, puoi gestire la maggior parte del processo direttamente tramite l'interfaccia di GitHub:

### Review e approvazione

1. **Controlla le notifiche**: Verifica regolarmente le PR in arrivo
2. **Esamina il codice**: Usa la scheda "Files changed" per vedere le modifiche
3. **Lascia commenti**: Fornisci feedback su specifiche linee di codice
4. **Approva o richiedi modifiche**: Usa i pulsanti "Review" per dare feedback

### Test locale delle PR

Se hai bisogno di testare una PR sul tuo computer:

```bash
# Sostituisci NUMERO con il numero effettivo della pull request (es. 17)
git fetch upstream pull/NUMERO/head:pr-NUMERO
git checkout pr-NUMERO

# Ora puoi testare il codice
# ...

# Torna al tuo branch principale quando hai finito
git checkout main
```

### Opzioni di merge

Quando sei soddisfatto della PR, puoi fare il merge:

1. **Direttamente dall'interfaccia**: Usa il pulsante "Merge pull request" su GitHub
2. **Scegli il tipo di merge**:
   - "Create a merge commit" (preserva tutti i commit)
   - "Squash and merge" (combina tutti i commit in uno solo)
   - "Rebase and merge" (linea storica pulita)

### Comunicazione efficace

1. **Sii chiaro e costruttivo** nel feedback
2. **Ringrazia i contributori** per il loro tempo
3. **Spiega i rifiuti**, se una PR non può essere accettata
4. **Usa emoji e formattazione** per rendere i commenti più leggibili

## Manutenzione del repository

### Protezione dei branch

Configura le regole di protezione del branch in Settings > Branches:
- Richiedi review delle PR prima del merge
- Richiedi che i branch siano aggiornati prima del merge
- Applica verifiche di stato (se usi CI/CD)

### Gestione delle Issues

1. **Usa le etichette** per categorizzare (bug, enhancement, help wanted)
2. **Assegna milestone** per raggruppare issues correlate
3. **Chiudi automaticamente** le issues con PR ("Fixes #123" nel messaggio di commit)

### Versioni e tag

Per rilasci importanti, crea tag e release:

```bash
git tag -a v1.0.0 -m "Versione 1.0.0"
git push origin v1.0.0
```

Poi vai su GitHub > Releases > Draft a new release per creare una release ufficiale.



