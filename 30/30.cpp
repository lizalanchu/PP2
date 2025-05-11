#include <mpi.h>      // Библиотека MPI для межпроцессного параллелизма
#include <omp.h>      // Библиотека OpenMP для внутрипроцессного параллелизма
#include <iostream>   // Для вывода информации (printf вместо cout для thread-safety)

int main(int argc, char** argv) {
    // Объявление переменных MPI:
    int rank_mpi;     // Уникальный идентификатор MPI-процесса
    int size_mpi;     // Общее количество MPI-процессов
    int n = 3;        // Количество OpenMP-потоков на каждый MPI-процесс

    // Инициализация MPI (должна быть выполнена до создания потоков)
    MPI_Init(&argc, &argv);
    
    // Получение номера текущего MPI-процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);
    
    // Получение общего количества MPI-процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);

    // Создание параллельной области OpenMP
    // num_threads(n) - создает ровно n потоков на каждом MPI-процессе
    #pragma omp parallel num_threads(n)
    {
        // Эти переменные уникальны для каждого потока:
        int rank_omp = omp_get_thread_num();  // Номер потока (0..n-1)
        int size_omp = n;                     // Количество потоков
        
        // Вывод информации о потоке
        // Используем printf (потокобезопасный) вместо cout
        printf("I am %d thread from %d process. Number of hybrid threads = %d.\n",
               rank_omp, rank_mpi, size_omp * size_mpi);
        
        // Все потоки автоматически синхронизируются здесь
    }

    // Завершение работы MPI (после завершения всех потоков)
    MPI_Finalize();
    return 0;
}