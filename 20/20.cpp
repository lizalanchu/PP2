#include <mpi.h>        // Подключение библиотеки MPI
#include <iostream>     // Подключение стандартной библиотеки ввода-вывода

int main(int argc, char* argv[]) {
    // Инициализация MPI-окружения
    MPI_Init(&argc, &argv);

    int rank;  // Ранг текущего процесса
    
    // Получаем номер текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Блок для процесса с рангом 0 (отправитель)
    if (rank == 0) {
        int message = 45;  // Сообщение для отправки
        MPI_Request request;  // Объект запроса для отслеживания операции
        
        // Неблокирующая отправка сообщения:
        // &message - данные для отправки
        // 1 - количество элементов
        // MPI_INT - тип данных
        // 1 - ранг получателя
        // 0 - тег сообщения
        // MPI_COMM_WORLD - коммуникатор
        // &request - объект для отслеживания операции
        MPI_Isend(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        
        std::cout << "Process 0 initiated non-blocking send" << std::endl;
        
        // Ожидание завершения операции отправки
        // &request - указатель на объект запроса
        // MPI_STATUS_IGNORE - игнорировать статус операции
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        
        std::cout << "Process 0 completed sending message: " << message << std::endl;
    }
    // Блок для процесса с рангом 1 (получатель)
    else if (rank == 1) {
        int received_message;  // Буфер для приема
        MPI_Request request;   // Объект запроса
        
        // Неблокирующий прием сообщения:
        // &received_message - буфер для данных
        // 1 - количество ожидаемых элементов
        // MPI_INT - тип данных
        // 0 - ранг отправителя
        // 0 - ожидаемый тег
        // MPI_COMM_WORLD - коммуникатор
        // &request - объект для отслеживания
        MPI_Irecv(&received_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        
        std::cout << "Process 1 initiated non-blocking receive" << std::endl;
        
        // Ожидание завершения операции приема
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        
        std::cout << "Process 1 received message: '" << received_message << "'" << std::endl;
    }

    // Завершение работы с MPI (все процессы вызывают Finalize)
    MPI_Finalize();
    
    return 0;
}