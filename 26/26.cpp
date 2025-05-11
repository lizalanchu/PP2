#include <mpi.h>       // Основная библиотека MPI для параллельных вычислений
#include <iostream>    // Для ввода/вывода (cout, cin)
#include <vector>      // Для использования динамических массивов (векторов)
#include <string>      // Для работы со строками string

int main(int argc, char** argv) {
    // Объявление переменных:
    int rank, size;    // Текущий ранг и общее количество процессов в MPI_COMM_WORLD
    int newrank, newsize; // Ранг и размер в новом коммуникаторе
    MPI_Group oldgroup, group; // Группы процессов (исходная и новая)
    MPI_Comm newcomm;  // Новый коммуникатор
    std::string message; // Строка для передачи сообщения

    // Инициализация MPI-окружения (должна быть первой MPI-операцией)
    MPI_Init(&argc, &argv);
    
    // Получение номера текущего процесса в глобальном коммуникаторе
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Получение общего количества процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Процесс с рангом 0 запрашивает ввод сообщения
    if (rank == 0) {
        std::cout << "Введите сообщение (от 1 до 10 символов): ";
        std::cin >> message;  // Чтение сообщения из консоли

        // Проверка длины сообщения
        if (message.length() < 1 || message.length() > 10) {
            std::cerr << "Ошибка: длина сообщения должна быть от 1 до 10 символов.\n";
            MPI_Abort(MPI_COMM_WORLD, 1);  // Аварийное завершение всех процессов
        }
    }

    // Создание списка рангов четных процессов (0, 2, 4...)
    std::vector<int> ranks;  // Вектор для хранения номеров процессов
    for (int i = 0; i < size; i += 2) {
        ranks.push_back(i);  // Добавляем только четные ранги
    }

    // Получаем группу, соответствующую MPI_COMM_WORLD
    MPI_Comm_group(MPI_COMM_WORLD, &oldgroup);

    // Создаем новую группу, включающую только четные процессы
    // Параметры:
    // oldgroup - исходная группа
    // ranks.size() - количество процессов в новой группе
    // ranks.data() - массив рангов для включения
    // &group - новая группа
    MPI_Group_incl(oldgroup, ranks.size(), ranks.data(), &group);

    // Создаем новый коммуникатор для группы четных процессов
    // Параметры:
    // MPI_COMM_WORLD - исходный коммуникатор
    // group - группа процессов
    // &newcomm - новый коммуникатор
    MPI_Comm_create(MPI_COMM_WORLD, group, &newcomm);

    // Получаем информацию о ранге и размере в новом коммуникаторе
    if (newcomm != MPI_COMM_NULL) {  // Если процесс вошел в новую группу
        MPI_Comm_rank(newcomm, &newrank);  // Ранг в новом коммуникаторе
        MPI_Comm_size(newcomm, &newsize);  // Размер нового коммуникатора
    } else {  // Если процесс не в новой группе
        newrank = -1;   // Специальные значения для "не членов"
        newsize = -1;
    }

    // Процесс 0 рассылает сообщение в новом коммуникаторе
    if (rank == 0) {
        if (newcomm != MPI_COMM_NULL) {  // Если процесс 0 в новой группе
            // Широковещательная рассылка сообщения
            MPI_Bcast(const_cast<char*>(message.c_str()), 
                     message.length(), 
                     MPI_CHAR, 
                     0, 
                     newcomm);
        }
    }

    // Прием сообщения процессами в новом коммуникаторе
    if (newcomm != MPI_COMM_NULL) {
        message.resize(10);  // Подготовка буфера для приема
        MPI_Bcast(&message[0],         // Буфер для приема
                 message.length(),    // Длина сообщения
                 MPI_CHAR,            // Тип данных
                 0,                   // Ранг отправителя
                 newcomm);            // Коммуникатор
    }

    // Вывод информации о процессах
    if (newcomm != MPI_COMM_NULL) {
        std::cout << "MPI_COMM_WORLD: " << rank << " from " << size 
                  << ". New comm: " << newrank << " from " << newsize 
                  << ". Message = " << message << std::endl;
    } else {
        std::cout << "MPI_COMM_WORLD: " << rank << " from " << size 
                  << ". New comm: no from no. Message = no" << std::endl;
    }

    // Освобождение ресурсов
    if (newcomm != MPI_COMM_NULL) {
        MPI_Comm_free(&newcomm);  // Удаление коммуникатора
    }
    MPI_Group_free(&group);      // Освобождение группы
    MPI_Group_free(&oldgroup);   // Освобождение исходной группы

    // Завершение работы MPI
    MPI_Finalize();
    return 0;
}