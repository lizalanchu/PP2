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

    int message;  // Переменная для хранения сообщения

    // Блок для процесса с рангом 0 (начало цепочки)
    if (rank == 0) {
        message = rank;  // Инициализируем сообщение своим рангом
        
        // Отправляем сообщение следующему процессу в цепочке (ранг + 1)
        // &message - данные для отправки
        // 1 - количество элементов
        // MPI_INT - тип данных
        // rank + 1 - получатель
        // 0 - тег сообщения
        // MPI_COMM_WORLD - коммуникатор
        MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        
        std::cout << "[" << rank << "]: sent message '" << message << "'" << std::endl;
    }

    // Блок для всех процессов кроме 0 (получатели и передатчики)
    if (rank > 0) {
        // Принимаем сообщение от предыдущего процесса в цепочке (ранг - 1)
        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "[" << rank << "]: received message '" << message << "'" << std::endl;
        
        message++;  // Модифицируем сообщение (увеличиваем на 1)
        
        // Если это не последний процесс в цепочке, отправляем дальше
        if (rank < size - 1) {
            MPI_Send(&message, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            std::cout << "[" << rank << "]: forwarded message '" << message << "'" << std::endl;
        }
    }

    // Завершение работы с MPI
    MPI_Finalize();
    
    return 0;
}