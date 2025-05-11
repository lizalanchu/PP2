#include <mpi.h>        // Подключение библиотеки MPI
#include <iostream>     // Подключение стандартной библиотеки ввода-вывода

int main(int argc, char* argv[]) {
    // Инициализация MPI-окружения
    MPI_Init(&argc, &argv);

    int rank;  // Ранг текущего процесса
    int size;  // Общее количество процессов
    
    // Получаем номер текущего процесса в коммуникаторе MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Получаем общее количество процессов в коммуникаторе
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Разделение логики на Master (ранг 0) и Worker'ов (остальные ранги)
    if (rank == 0) {
        // Процесс с рангом 0 выступает в роли Master
        
        std::cout << "Master process waiting for messages..." << std::endl;
        
        // Принимаем сообщения от всех Worker процессов (ранги 1..size-1)
        for (int i = 1; i < size; i++) {
            int received_message;  // Буфер для приема сообщения
            
            // Прием сообщения от i-го процесса:
            // &received_message - указатель на буфер
            // 1 - количество принимаемых элементов
            // MPI_INT - тип данных
            // i - ранг отправителя
            // 0 - тег сообщения
            // MPI_COMM_WORLD - коммуникатор
            // MPI_STATUS_IGNORE - игнорируем дополнительную информацию
            MPI_Recv(&received_message, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            std::cout << "Master received message '" << received_message 
                      << "' from process " << i << std::endl;
        }
    }
    else {
        // Процессы с рангом > 0 выступают в роли Worker'ов
        
        // Отправляем свой ранг процессу Master (ранг 0)
        // &rank - данные для отправки (в данном случае сам ранг)
        // 1 - количество элементов
        // MPI_INT - тип данных
        // 0 - получатель (Master)
        // 0 - тег сообщения
        // MPI_COMM_WORLD - коммуникатор
        MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        
        std::cout << "Worker " << rank << " sent its rank to Master" << std::endl;
    }

    // Завершение работы с MPI (все процессы вызывают Finalize)
    MPI_Finalize();
    
    return 0;
}