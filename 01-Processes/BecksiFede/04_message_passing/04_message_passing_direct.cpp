#include <iostream>
#include <fstream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

struct task_msg {
    long mtype;
    struct {
        int **matrix;
        int steps;
        size_t nRows;
        size_t startCol;
        size_t index;
    } content;
};

struct result_msg {
    long mtype;
    struct {
        int row_index;
        int sum;
    } content;
};

void readFile(const char *filename, int **matrix, const size_t rows, const size_t cols) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (size_t r = 0; r < rows; ++r) {
        for (size_t c = 0; c < cols; ++c) {
            file >> matrix[r][c];
            file.ignore(1);  // Per ignorare ',' o '\n'
        }
    }

    file.close();
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <filename> <num_righe> <num_colonne>" << std::endl;
        return 1;
    }

    key_t key = ftok(".", 'M');
    int msqid = msgget(key, IPC_CREAT | 0666);
    if (msqid == -1) {
        std::cerr << "Errore nella creazione della message queue" << std::endl;
        return 1;
    }

    char *filename = argv[1];
    size_t rows = std::stoi(argv[2]);
    size_t cols = std::stoi(argv[3]);

    int **matrix = new int *[rows];
    for (size_t i = 0; i < rows; ++i) {
        matrix[i] = new int[cols];
    }

    readFile(filename, matrix, rows, cols);

    std::cout << "Contenuto della matrice:" << std::endl;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    size_t nproc;
    std::cout << "Inserisci il numero dei processi: ";
    std::cin >> nproc;
    std::cout << "Numero di processi: " << nproc << std::endl;

    pid_t *pids = new pid_t[nproc];

    int colsxproc = cols / nproc;
    int colsxproc_resto = cols % nproc;

    for (size_t i = 0; i < nproc; ++i) {
        pids[i] = fork();

    
        if (pids[i] < 0) {
            std::cerr << "fork fallita" << std::endl;
            return 1;
        }else if(pids[i] == 0){
            std::cout << "pid figlio" << getpid() << std::endl;
            struct task_msg task;
            msgrcv(msqid, &task, sizeof(task.content), i + 1, 0);

            int sum = 0;
            for (int col = task.content.startCol; col < task.content.startCol + task.content.steps; ++col) {
                for (int row = 0; row < task.content.nRows; ++row)
                    sum += task.content.matrix[row][col];
            }

            struct result_msg result;
            result.mtype = 100 + i;
            result.content.row_index = task.content.index;
            result.content.sum = sum;

            msgsnd(msqid, &result, sizeof(result.content), 0);
            return 0;
        }else{
            /*
            std::cout << "pid parent" << getppid() << std::endl;
            */
        }
    }

    for (int i = 0; i < nproc; i++) {
        struct task_msg task;
        size_t start_col = i * colsxproc;
        size_t steps = (i == nproc - 1) ? colsxproc + colsxproc_resto : colsxproc;

        task.mtype = i + 1;
        task.content.index = i;
        // it work only in reading 'cause the fork copies the heap but if the child process modifies it'll created a personal copy for it
        task.content.matrix = matrix; 
        task.content.nRows = rows;
        task.content.startCol = start_col;
        task.content.steps = steps;

        msgsnd(msqid, &task, sizeof(task.content), 0);
    }

    bool pidsOk = true;
    for (size_t i = 0; i < nproc; ++i) {
        if (pids[i] > 0) {
            waitpid(pids[i], NULL, 0);
        } else {
            pidsOk = false;
        }
    }

    if (pidsOk) {
        std::cout << "Tutti i processi sono terminati correttamente." << std::endl;

        long sum = 0;
        for (size_t i = 0; i < nproc; ++i) {
            struct result_msg result;
            msgrcv(msqid, &result, sizeof(result.content), 100 + i, 0);
            std::cout << "[Padre] Somma riga " << result.content.row_index << " = " << result.content.sum << std::endl;
            sum += result.content.sum;
        }

        std::cout << "Somma totale: " << sum << std::endl;
    } else {
        std::cout << "Almeno un processo non ha terminato correttamente." << std::endl;
    }

    for (size_t i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;
    delete[] pids;

    msgctl(msqid, IPC_RMID, nullptr);
    return 0;
}
