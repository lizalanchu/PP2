#include <mpi.h>        // Подключение библиотеки MPI
#include <iostream>     // Подключение стандартной библиотеки ввода-вывода

int main(int argc, char** argv) {
    // Инициализация MPI-окружения
    // argc и argv передаются для возможной обработки MPI-specific аргументов
    MPI_Init(&argc, &argv);

    int rank;    // Ранг (номер) текущего процесса
    int size;    // Общее количество процессов

    // Получаем ранг текущего процесса в коммуникаторе MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Получаем общее количество процессов в коммуникаторе MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Условный блок, который выполняется только в процессе с рангом 0
    // Процесс 0 обычно выступает как "главный" процесс
    if (rank == 0) {
        std::cout << "Total processes: " << size << std::endl;
    }

    // Разделяем процессы на две группы по четности ранга
    if (rank % 2 == 0) {
        // Выполняется для процессов с четными рангами (0, 2, 4...)
        std::cout << "Process " << rank << ": I'm in FIRST group!" << std::endl;
    }
    else {
        // Выполняется для процессов с нечетными рангами (1, 3, 5...)
        std::cout << "Process " << rank << ": I'm in SECOND group!" << std::endl;
    }

    // Завершаем работу с MPI
    // После этого вызова нельзя использовать MPI-функции
    MPI_Finalize();

    return 0;  // Код успешного завершения
}