# SawAnalyser v0.1

Программа SawAnalyser v0.1 производит простейший анализ текстовых файлов, содержащих имитацию данных с датчика.

#### Структура директорий
+ ConsoleApplication -– исходный код SawAnalyser
+ LibSawAnalyzing -- исходный код библиотеки анализа сигнала
+ SawGen – исходный код скрипта генератора тестовых сигналов
+ TestSamples – подготовленные тестовые выборки
+ binDebug – скомпилированная программа с искусственно замедленным режимом работы, возможно, будет удобно при тестрировании.
+ binRealese -- скомпилированная программа без искусственного замедления.


### Общее описание программы
В папку TestData попадают текстовые файлы, содержание выборки с датчика. (Примечание: к сожалению, не реализовал проверку на корректность данных в этих файлах) Генерацию этих данных обеспечивает python скрипт, расположенный в директории SawGen. При его запуске генерируется текстовые файлы, которые можно подать на выход SawAnalyser.

 SawAnalyser v0.1 производит простейший анализ этих файлов, анализируется:
+ Минимальная и максимальная амплитуда сигнала, допускается некоторое отклонение (AMPLITUDE_ERROR), которое можно задать.
+ Длительность импульса, подразумевается, что частота дискретизации остается постоянная на протяжении всего времени работы датчика.
+ Форма сигнала, производиться простейшая оценка формы сигнала. Сравнивается амплитуда одного семпла и амплитуда следующего за ним. Амплитуда последующего семпла всегда должна быть строго больше предыдущего.

Каждый текстовый файл, содержит выборку одного пилообразного импульса с 
датчика.

### Описание алгоритма работы программы SawAnalyser
Программа с интервалом 1 секунда просматривает содержимое рабочей директории TestData, на наличие текстовых файлов, созданных скриптом SawGen. Как только, там появляется файл, он переносится в папку Tested, затем программа создает отдельный поток, в котором происходит анализ сигнала, содержащегося в файле. 
Функция анализа на время вывода результатов в стандартный поток вывода, захватывает мьютекс m_ostream, чтобы не допустить мешанины данных из разных потоков.
Результаты выводятся в формате csv. Если перенаправить поток вывода на csv файл, то в дальнейшем его возможно будет открыть, например, табличным процессором OpenOffice Calc. 

### Конфигурирование SawAnalyser
Программа допускает несколько способов конфигурирования параметров оценки сигнала: 
+ При запуске программы с помощью аргументов (Примечание: есть простейшая проверка на корректность данных).
+ Во время работы с помощью набора с клавиатуры строки “cfg” (Примечание: есть простейшая проверка на корректность данных). При получении данной строки программы приостановит обработку файлов и будет дожидаться, ввода новых значений параметров. После ввода значений SawAnalyser продолжит свою работу.

При начале работы программа выведет сообщение: “Start working …”.
При завершение обработки всех файлов в рабочей пачке программа выведет сообщение: “Work done”.

Если некоторые данные не введены или введены некорректные значения, то будут установлены значения по умолчанию.

Параметры конфигурирования:
+ Error_only (-erronly (ключе аргумента)): 0 – выводит, результаты анализа даже если сигнал прошел все проверки, 1 – выводит только результаты тех сигналов, которые не прошли проверку (bool).
+ Amplitude_max  (-amax) максимальная амплитуда сигнала (int).
+ Amplitude_min  (-amin) минимальная амплитуда сигнала (int).
+ Amplitude_error (-aerr) допустимое отклонение максимальной и минимальной амплитуды от номинального (int).
+ Width (-width) длительность импульса (int).
Все параметры оценки вводятся в условных единицах.

 Структура данных, описывающая контрольные параметры:
 
 ```c++
        struct TEST_PARAM
        {
            int aml_min;
            int aml_max;
            int err;
            int width;
        }test_parameter;

 ```        
Пример вывода данной структуры в стандартный поток вывода.
 
 ```
AMPLITUDE_MIN,0
AMPLITUDE_MAX,480
AMPLITUDE_ERR,20
PULSE_WIDTH,50

 ```
### Описание результатов анализов
Результат анализа выводиться в формате csv. Результат содержит краткую информацию об анализе сигнала. Для наглядности буду описывать непосредственно саму структуры данных результата.


 ```c++
struct RESULT
{
    bool form_tst; -- признак прохождение теста формы сигнала
    int  bad_sample_num; -- номер семпла в файле, который содержит помеху

    AMPLITUDE_ERRORS ampl_test; -- краткий результат прохождения теста амплитуды (описание см. ниже)
    int aml_min_err; -- минимальная амплитуда сигнала, текущего сигнала.
    int aml_max_err; -- максимальная амплитуда сигнала, текущего сигнала.

    bool widht_test; -- признак прохождения теста длительности импульса.
    int  width_err; -- значение длительности импульса, текущего сигнала.
}result_t;

enum AMPLITUDE_ERRORS
{
    MPL_NORM, -- амплитуда в норме
    AMPL_TOO_LOW, -- превышен нижний предел
    AMPL_TOO_HIGHT, -- превышен верхний предел
    AMPL_OUT_BORDER – превышен и верхний и нижний предел
};

 ```
 
Пример вывода данной структуры в стандартный поток:

 ```
TestFile,120.txt // анализируемый файл с выборками
FormTest,1 // тест формы сигнала пройден успешно
AmplTest,SawAnalyzingFunc::AMPL_TOO_HIGHT,MAX_ERR,490 // Амлитуда слишком высокая, значение сигнала 490
WidthTest,0,WIDTH_CURRENT,50 // тест длительности не пройден, текущая длительности сигнала

 ```

### Краткое описание скрипта saw_gen
Данный скрипт предназначен для генерации тестовый файлов. Скрипт написан на Python 3. Для генерации тестовых сигналов используется 3 параметра.

```python
A_MAX = 500 # максимальная амплитуда сигнала
INC = 40 # инкремент сигнала за один семпл, влияет на длительность, чем 
# больше тем длительность меньше
NUM_PULSE = 5 # кол-во генерируемых импульсов (равно кол-ву файлов)
```

Чтобы не запускать данный скрипт в директории TestSamples, лежат файлы со следующими параметрами генерации:

```python
A_MAX = 500
INC = 10
NUM_PULSE = 555
```

### Процедура сборки

Для сборки программы SawAnalyser, требуется утилита cmake не ниже версии 3.0, а также версия gcc и g++ не ниже версии 8.

Команды для сборки
```bash
mkdir build
cd build
cmake ..
make
```

Результат успешной сборки:
```bash
viacheslav@virtualbox:~/TestApp$ mkdir build
viacheslav@virtualbox:~/TestApp$ cd build/
viacheslav@virtualbox:~/TestApp/build$ cmake ..
-- The C compiler identification is GNU 8.4.0
-- The CXX compiler identification is GNU 8.4.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Looking for pthread.h
-- Looking for pthread.h - found
-- Looking for pthread_create
-- Looking for pthread_create - not found
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Found Threads: TRUE  
-- Configuring done
-- Generating done
-- Build files have been written to: /home/viacheslav/TestApp/build
viacheslav@virtualbox:~/TestApp/build$ make
Scanning dependencies of target LibSawAnalyzing
[ 25%] Building CXX object LibSawAnalyzing/CMakeFiles/LibSawAnalyzing.dir/LibSawAnalyzing.cpp.o
[ 50%] Linking CXX static library libLibSawAnalyzing.a
[ 50%] Built target LibSawAnalyzing
Scanning dependencies of target App
[ 75%] Building CXX object ConsoleApplication/CMakeFiles/App.dir/ConsoleApplication.cpp.o
[100%] Linking CXX executable App
[100%] Built target App
```

### Пример тестирования
Для проверки интерактивного изменения контрольных параметров, рекомендуется запустить приложение из директории binDebug. Работа приложения здесь искусственно замедлено, это поможет спокойно изменять конфигурацию на небольшом кол-ве данных.

Пример запуска.
```bash
./App -erronly 1 -amin 0 -amax 500 -aerr 20 -w 50
```
Для оценки производительности можно запусить приложение аналогичныи образом из директории binRealese.

Пример диалога конфигурации:
```
Welcome to SawAnalyser v0.1
ErrorsOnly,1
TestParameters
AMPLITUDE_MIN,0
AMPLITUDE_MAX,480
AMPLITUDE_ERR,20
PULSE_WIDTH,50
Start working... 

cfg 
----Enter config param---
err_only:1
amplitude_min:0
amplitude_max:500
amplitude error:5
pulse width:50
TestParameters
AMPLITUDE_MIN,0
AMPLITUDE_MAX,500
AMPLITUDE_ERR,5
PULSE_WIDTH,50
---Config accept---
```
### Недостатки программы
+ Использование текстовых файлов слишком громоздко, возможно, лучше было бы использовать бинарные файлы.
+ Нет проверки корректности тестовых данных
+ Не очень удобный способ отладки, приходиться вручную копировать фалы в директорию TestData. Лучше было бы использовать скрипт на питоне, чтобы он сразу генерировал в рабочей директории файлы.
+ Хардкод рабочих директорий, удобней было использовать конфигурируемые параметры.
+ Очень простые способы анализа сигнала.
+ Бедный отчет по результатам тестирования
+ Слабый анализ данных получаемых от пользователя
