#include <mpi.h>        // Подключение библиотеки MPI
#include <iostream>     // Подключение стандартной библиотеки ввода-вывода

int main(int argc, char* argv[]) {
    // Инициализация MPI-окружения
    MPI_Init(&argc, &argv);

    int rank;  // Переменная для хранения номера (ранга) текущего процесса
    
    // Получение номера текущего процесса в глобальном коммуникаторе
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Блок кода для процесса с рангом 0 (отправитель)
    if (rank == 0) {
        int message = 45;  // Создаем сообщение для отправки
        
        // Отправка сообщения:
        // &message   - указатель на данные для отправки
        // 1          - количество элементов для отправки
        // MPI_INT    - тип данных (целое число)
        // 1          - ранг процесса-получателя
        // 0          - тег сообщения (идентификатор)
        // MPI_COMM_WORLD - коммуникатор
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        
        std::cout << "Process 0 sent message: " << message << std::endl;
    }
    // Блок кода для процесса с рангом 1 (получатель)
    else if (rank == 1) {
        int received_message;  // Буфер для приема сообщения
        
        // Прием сообщения:
        // &received_message - указатель на буфер приема
        // 1          - максимальное количество элементов для приема
        // MPI_INT    - ожидаемый тип данных
        // 0          - ранг процесса-отправителя
        // 0          - ожидаемый тег сообщения
        // MPI_COMM_WORLD - коммуникатор
        // MPI_STATUS_IGNORE - игнорировать статус сообщения
        MPI_Recv(&received_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        std::cout << "Process 1 received message: '" << received_message << "'" << std::endl;
    }

    // Завершение работы с MPI
    MPI_Finalize();
    
    return 0;
}