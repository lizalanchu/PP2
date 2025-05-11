#include <mpi.h>        // Подключение библиотеки MPI
#include <iostream>     // Подключение стандартной библиотеки ввода-вывода

int main(int argc, char* argv[]) {
    // Инициализация MPI-окружения
    MPI_Init(&argc, &argv);

    int rank;  // Ранг текущего процесса
    int size;  // Общее количество процессов
    
    // Получаем номер текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Получаем общее количество процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_value = rank;  // Каждый процесс отправляет свой ранг
    int recv_value;         // Буфер для приема значения

    // Вычисляем соседей в кольцевой топологии:
    int send_to = (rank + 1) % size;         // Следующий процесс в кольце
    int recv_from = (rank - 1 + size) % size; // Предыдущий процесс в кольце
    // Примечание: +size нужно для корректной работы с rank=0

    // Выводим информацию о связях
    std::cout << "Process " << rank << ": sending to " << send_to 
              << ", receiving from " << recv_from << std::endl;

    // Кольцевая передача данных:
    // 1. Отправляем свое значение следующему процессу
    MPI_Send(&send_value, 1, MPI_INT, send_to, 0, MPI_COMM_WORLD);
    
    // 2. Принимаем значение от предыдущего процесса
    MPI_Recv(&recv_value, 1, MPI_INT, recv_from, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Выводим полученное значение
    std::cout << "[" << rank << "]: received message '" << recv_value << "'" << std::endl;

    // Завершение работы с MPI
    MPI_Finalize();
    
    return 0;
}