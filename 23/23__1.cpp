﻿// Подключаем необходимые библиотеки
#include <mpi.h>        // Основная библиотека для работы с MPI (параллельные вычисления)
#include <iostream>     // Для ввода/вывода данных (cout, cin)
#include <string>       // Для работы со строками string
#include <vector>       // Для использования динамического массива vector

int main(int argc, char** argv) {
    // Объявляем переменные:
    int rank;       // Уникальный идентификатор (номер) текущего процесса
    int size;       // Общее количество процессов в системе
    int n;          // Длина анализируемой строки
    std::string buf;    // Строка, которую будем анализировать
    std::vector<int> counts(26, 0); // Вектор для подсчета букв (26 элементов, инициализированных нулями)

    // Инициализация параллельной среды MPI
    // Все MPI-программы начинаются с инициализации
    MPI_Init(&argc, &argv);
    
    // Получаем номер текущего процесса (ранг)
    // MPI_COMM_WORLD - это коммуникатор, объединяющий все процессы
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    // Получаем общее количество процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Процесс с рангом 0 будет управляющим (master)
    // Он отвечает за ввод исходных данных
    if (rank == 0) {
        // Запрашиваем у пользователя длину строки
        std::cout << "Введите длину строки: ";
        std::cin >> n;
        
        // Запрашиваем саму строку для анализа
        std::cout << "Введите строку: ";
        std::cin >> buf;

        // Проверяем, что введенная строка соответствует указанной длине
        if (buf.length() != n) {
            // Если длина не совпадает - выводим ошибку
            std::cerr << "Ошибка: длина строки не соответствует введенному значению.\n";
            // Аварийно завершаем все процессы
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Широковещательная рассылка (broadcast) длины строки всем процессам
    // Параметры MPI_Bcast:
    // &n - указатель на данные для рассылки
    // 1 - количество элементов
    // MPI_INT - тип данных (целое число)
    // 0 - ранг процесса-отправителя (в данном случае процесс 0)
    // MPI_COMM_WORLD - коммуникатор (все процессы)
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Рассылка самой строки всем процессам
    if (rank == 0) {
        // Процесс-отправитель (ранг 0) передает данные
        // &buf[0] - указатель на начало строки (преобразование string в char*)
        // n - количество символов для отправки
        // MPI_CHAR - тип данных (символ)
        MPI_Bcast(&buf[0], n, MPI_CHAR, 0, MPI_COMM_WORLD);
    }
    else {
        // Остальные процессы-получатели
        // Сначала подготавливаем буфер нужного размера
        buf.resize(n);
        // Затем принимаем данные
        MPI_Bcast(&buf[0], n, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    // Отладочный вывод - каждый процесс показывает что он получил
    std::cout << "Процесс " << rank << " получил n = " << n 
              << ", строка = " << buf << std::endl;

    // Основная часть: распределенный подсчет букв
    char a = 'a';  // Начало алфавита (символ 'a')
    
    // Каждый процесс обрабатывает свою часть алфавита
    // Цикл начинается с rank и шагает на size
    // Например для 3 процессов:
    // Процесс 0: 0, 3, 6... (буквы a, d, g...)
    // Процесс 1: 1, 4, 7... (буквы b, e, h...)
    // Процесс 2: 2, 5, 8... (буквы c, f, i...)
    for (int i = rank; i < 26; i += size) {
        // Вычисляем текущую букву для подсчета
        char target = static_cast<char>(a + i);
        
        // Подсчитываем сколько раз эта буква встречается в строке
        for (char c : buf) {  // Цикл по всем символам строки
            if (c == target) {  // Если символ совпадает с искомой буквой
                counts[i]++;    // Увеличиваем счетчик
            }
        }
    }

    // Вывод результатов подсчета
    for (int i = 0; i < 26; ++i) {
        // Выводим только те буквы, которые встречались (count > 0)
        if (counts[i] > 0) {
            std::cout << "Процесс " << rank << ": " 
                      << static_cast<char>(a + i) << " = " 
                      << counts[i] << std::endl;
        }
    }

    // Синхронизация всех процессов перед завершением
    // MPI_Barrier блокирует выполнение, пока все процессы не достигнут этой точки
    MPI_Barrier(MPI_COMM_WORLD);
    
    // Корректное завершение работы с MPI
    MPI_Finalize();
    
    return 0;  // Успешное завершение программы
}