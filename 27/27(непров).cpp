#include <mpi.h>       // Основная библиотека MPI для параллельных вычислений
#include <iostream>    // Для ввода/вывода (cout, endl)

int main(int argc, char** argv) {
    // Объявление переменных:
    int rank, size;    // Текущий ранг и общее количество процессов в MPI_COMM_WORLD
    int n = 3;         // Количество дочерних процессов для запуска
    MPI_Comm intercomm; // Интеркоммуникатор для связи с дочерними процессами

    // Инициализация MPI-окружения
    MPI_Init(&argc, &argv);
    
    // Получение номера текущего процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Получение общего количества процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Блок для родительского процесса (ранг 0)
    if (rank == 0) {
        // Вывод информации о родительском процессе
        std::cout << "I am " << rank << " process from " << size 
                  << " processes! My parent is none." << std::endl;

        // Запуск дочерних процессов с помощью MPI_Comm_spawn:
        // 1. "MPI.exe" - имя исполняемого файла дочерних процессов
        // 2. MPI_ARGV_NULL - аргументы командной строки (отсутствуют)
        // 3. n - количество запускаемых процессов
        // 4. MPI_INFO_NULL - дополнительная информация (отсутствует)
        // 5. 0 - ранг процесса, который запускает дочерние процессы
        // 6. MPI_COMM_SELF - коммуникатор для запуска
        // 7. &intercomm - создаваемый интеркоммуникатор
        // 8. MPI_ERRCODES_IGNORE - игнорирование кодов ошибок
        MPI_Comm_spawn("MPI.exe", MPI_ARGV_NULL, n, MPI_INFO_NULL, 
                      0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);

        // Получение количества запущенных дочерних процессов
        int remote_size;
        MPI_Comm_remote_size(intercomm, &remote_size);
        std::cout << "Dynamically spawned " << remote_size << " processes." << std::endl;
    }
    else {
        // Блок для других процессов родительского коммуникатора (если есть)
        std::cout << "I am " << rank << " process from " << size 
                  << " processes! My parent is none." << std::endl;
    }

    // Блок для проверки дочерних процессов
    if (MPI_COMM_NULL != MPI_COMM_WORLD) {
        MPI_Comm parent; // Коммуникатор для связи с родителем
        
        // Получение родительского коммуникатора
        MPI_Comm_get_parent(&parent);

        // Если процесс является дочерним (имеет родителя)
        if (parent != MPI_COMM_NULL) {
            int child_rank, child_size; // Ранг и размер в дочернем коммуникаторе
            
            // Получение номера дочернего процесса
            MPI_Comm_rank(MPI_COMM_WORLD, &child_rank);
            
            // Получение количества дочерних процессов
            MPI_Comm_size(MPI_COMM_WORLD, &child_size);

            // Вывод информации о дочернем процессе
            std::cout << "I am " << child_rank << " process from " << child_size 
                      << " processes! My parent is 0." << std::endl;
        }
    }

    // Завершение работы MPI (вызывается всеми процессами)
    MPI_Finalize();
    return 0;
}