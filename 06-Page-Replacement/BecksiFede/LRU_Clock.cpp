#include <iostream>
#include <cstdlib> // Per rand()

struct {
    int id;
    void* frame; // personal abstraction 
    bool reference;
} typedef page;

bool is_in_page_table(const page* page_memory, const size_t size_page_table, int id) {
    for (int i = 0; i < size_page_table; ++i)
        if (page_memory[i].id == id)
            return true;

    return false;
}

void print_page_table(const page* page_table, const size_t size_page_table) {
    std::cout << "Page Table State:\n";
    for (size_t i = 0; i < size_page_table; ++i) {
        std::cout << "Frame " << i << ": Page ID = " << page_table[i].id
                  << ", Reference Bit = " << page_table[i].reference << "\n";
    }
    std::cout << "\n";
}

int main() {
    const int n_request = 50;
    const int n_pages = 10;
    const size_t size_page_table = 5; // Number of frames in main memory

    int count_index_page = 0;
    int count_page_fault = 0;
    int count_ref_changed = 0;

    int page_request[n_request];
    page page_table[size_page_table];

    // Inizializza la page table
    for (size_t i = 0; i < size_page_table; ++i) {
        page_table[i] = {-1, nullptr, false}; // -1 indica che il frame è vuoto
    }

    // Genera richieste di pagine casuali
    std::cout << "Page Requests:\n";
    for (int i = 0; i < n_request; ++i) {
        page_request[i] = rand() % n_pages;
        std::cout << page_request[i] << " ";
    }
    std::cout << "\n\n";

    // Simula l'algoritmo LRU Clock con Second Chance
    for (int i = 0; i < n_request; ++i) {
        page current_page{page_request[i], nullptr, true};

        // Controlla se la pagina è già nella page table
        if (is_in_page_table(page_table, size_page_table, current_page.id)) {
            continue;
        }

        // Se c'è spazio libero nella page table
        if (count_index_page < size_page_table) {
            page_table[count_index_page++] = current_page;
            ++count_page_fault;
            continue;
        }

        // Cerca una pagina non usata (algoritmo Second Chance)
        bool condition = true;
        int index_page_table = 0;
        do {
            if (page_table[index_page_table % size_page_table].reference == false) {
                // Sostituisci la pagina
                page_table[index_page_table % size_page_table] = {
                    current_page.id,
                    current_page.frame,
                    true
                };
                condition = false;
                ++count_page_fault;
            } else {
                // Resetta il bit di riferimento e passa alla prossima pagina
                page_table[index_page_table % size_page_table].reference = false;
                ++count_ref_changed;
                ++index_page_table;
            }
        } while (condition);
    }

    // Mostra i risultati
    std::cout << "Simulation Results:\n";
    std::cout << "Total Page Faults: " << count_page_fault << "\n";
    std::cout << "Total Reference Bit Changes: " << count_ref_changed << "\n\n";

    // Stampa lo stato finale della page table
    print_page_table(page_table, size_page_table);

    return 0;
}