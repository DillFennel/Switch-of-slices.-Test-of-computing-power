#include <iostream>
#include <fstream> //Для работы с файлами
#include <string> //Строки
#include <algorithm> //Для поворота строк
#include <ctime> //Для замера времени
using namespace std;

void set_len_test(int len_test) { //Вводим размер тестовой строки
    ofstream fout("len_test.txt");
    fout << len_test;
    fout.close();
}

int get_len_test() { //Выдает длину тестовой строки
    ifstream fin("len_test.txt");
    int len_test;
    fin >> len_test;
    fin.close();
    return len_test;
}

void set_test(string test) { //Заносит тестовую строку в файл
    ofstream fout("test.txt");
    fout << test;
    fout.close();
}

string get_test() { //Получает тестовую строку из файла
    ifstream fin("test.txt");
    string test;
    fin >> test;
    fin.close();
    return test;
}

void generate_random_test() { //Генерирует рандомную тестовую строку указанной в файле len_test длины и записывает в файл test
    int len_test = get_len_test();
    string test;
    for (int i = 0; i < len_test; i++) {
        test += char(33+rand()%94);
    }
    set_test(test);
}

void set_parameters(int m, int n, int p) { //Устанавливает границы срезов в строке. 0 <= m < n < p. Мы меняем местами срезы [m:n] и [n+1:p]
    ofstream fout("parameters.txt");
    fout << m<<' '<< n << ' ' << p<<'\n';
    fout.close();
}

pair<int,  pair<int, int>> get_parameters() { //Выдает границы срезов в строке
    ifstream fin("parameters.txt");
    pair<int,  pair<int, int>> ret;
    string parameters;
    fin >> ret.first;
    fin >> ret.second.first;
    fin >> ret.second.second;
    fin.close();
    return ret;
}

void generate_random_parameters() { //Генерирует рандомные срезы подходящие для длины теста и устанавливает их
    int len_test = get_len_test();
    string test;
    int m = rand()%(len_test-2);
    int n = m+1 + rand()%(len_test-2-m);
    int p = n+1 + rand()%(len_test-1-n);
    set_parameters(m, n, p);
}

void swap_eq(char* a, char* b, int len){ //Меняет местами одинаковые по размеру куски
    for(int i = 0; i<len; i++){
        swap(*(a+i), *(b+i));
    }
}

string func1() //Алгоритм последовательным перебором
{
    string ret1 = get_test();
    pair<int,  pair<int, int>> parameters = get_parameters();
    int i = parameters.second.first - parameters.first;
    int j = parameters.second.second - parameters.second.first;
    while(i+1 != j){
        if(i+1>j){
            swap_eq(&ret1[parameters.second.first-i], &ret1[parameters.second.first+1], j);
            i = i-j;
        }
        else{
            swap_eq(&ret1[parameters.second.first-i], &ret1[parameters.second.first+j-i], i+1);
            j = j-i-1;
        }
    }
    swap_eq(&ret1[parameters.second.first-i], &ret1[parameters.second.first+1], i+1);
    return ret1;
}

string func2() //Алгоритм на основе двойной инверсии
{
    string ret2 = get_test();
    pair<int,  pair<int, int>> parameters = get_parameters();
    reverse(ret2.begin()+parameters.first, ret2.begin()+parameters.second.second+1);
    reverse(ret2.begin()+parameters.first, ret2.begin()+parameters.first+parameters.second.second-parameters.second.first);
    reverse(ret2.begin()+parameters.first+parameters.second.second-parameters.second.first, ret2.begin()+parameters.second.second+1);
    return ret2;
}

void measure(string (*func)()){ //Замеряет, сколько времени потратил алгоритм на свое выполнение
    unsigned int start_time =  clock();
    string answer = (*func)();
    unsigned int end_time = clock();
    if(get_len_test() <= 1000){ //Выводим ответ только если тест маленький
        cout<<"Получила ответ: "<<answer<<endl;
    }
    cout<<"Потратила времени: "<<end_time - start_time<<endl;
}

void hello(){ //Выводит пояснение к программе для пользователя
    cout<<"Функция:               Кнопка:"<<endl;
    cout<<"Сгенерировать тест      -   0"<<endl;
    cout<<"Сгенерировать срезы     -   1"<<endl;
    cout<<"Ввести свой тест        -   2"<<endl;
    cout<<"Ввести свои срезы       -   3"<<endl;
    cout<<"Получить данные о тесте -   4"<<endl;
    cout<<"Провести замеры         -   5"<<endl;
    cout<<"Завершить программу     -   6"<<endl;
}

int main()
{
    hello();
    int swt;
    while(swt != 6){
        cout<<"Нажмите кнопку"<<endl;
        cin>>swt;
        switch(swt)
        {
            case 0: //Сгенерировать тест
            {
                int n;
                cout<<"Введите размер текста"<<endl;
                cin>>n;
                set_len_test(n);
                cout<<"Введенный размер текста: "<<get_len_test()<<endl;
                generate_random_test();
                if(get_len_test() <= 1000){ //Выводим тест только если он маленький
                    cout<<"Получившийся тест: "<<get_test()<<endl;
                }
                else{
                    cout<<"Тест сгенерирован"<<endl;
                }
                break;
            }
            case 1: //Сгенерировать срезы
            {
                generate_random_parameters();
                pair<int,  pair<int, int>> parameters = get_parameters();
                cout<<"Получившиеся срезв: "<<parameters.first<<", "<<parameters.second.first<<", "<<parameters.second.second<<endl;
                break;
            }
            case 2: //Ввести свой тест
            {
                string test;
                cout<<"Введите тест"<<endl;
                cin>>test;
                set_len_test(test.size());
                set_test(test);
                cout<<"Введенный тест: "<<get_test()<<endl;
                cout<<"Размер теста: "<<get_len_test()<<endl;
                break;
            }
            case 3: //Ввести свои срезы
            {
                int m, n, p;
                cout<<"Введите начало первого среза:"<<endl;
                cin>>m;
                int lt = get_len_test();
                if(m<0 or m>lt-3){
                    cout<<"Такой срез не возможен, начните ввод заново"<<endl;
                    break;
                }
                cout<<"Введите конец первого среза:"<<endl;
                cin>>n;
                if(n<=m or n>lt-2){
                    cout<<"Такой срез не возможен, начните ввод заново"<<endl;
                    break;
                }
                cout<<"Введите конец второго среза:"<<endl;
                cin>>p;
                if(p<=n or p>lt-1){
                    cout<<"Такой срез не возможен, начните ввод заново"<<endl;
                    break;
                }
                set_parameters(m, n, p);
                pair<int,  pair<int, int>> parameters = get_parameters();
                cout<<"Введеные параметры: "<<parameters.first<<", "<<parameters.second.first<<", "<<parameters.second.second<<endl;
                break;
            }
            case 4: //Получить данные о тесте
            {
                cout<<"Размер теста: "<<get_len_test()<<endl;
                if(get_len_test() <= 1000){ //Выводим тест только если он маленький
                    cout<<"Тест: "<<get_test()<<endl;
                }
                pair<int,  pair<int, int>> parameters = get_parameters();
                cout<<"Параметры: "<<parameters.first<<", "<<parameters.second.first<<", "<<parameters.second.second<<endl;
                break;
            }
            case 5: //Провести замеры
            {
                cout<<"Первая функция"<<endl;
                measure(&func1);
                cout<<"Вторая функция"<<endl;
                measure(&func2);
                break;
            }
            case 6: //Завершить программу
            {
                cout<<"Завершаем программу..."<<endl;
                break;
            }
            default:
            {
                cout<<"Такой команды нет. Список существующих команд:"<<endl;
                hello();
            }
        }
    }
}
