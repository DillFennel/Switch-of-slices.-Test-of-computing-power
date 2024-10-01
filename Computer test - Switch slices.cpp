#include <iostream>
#include <fstream> //Для работы с файлами
#include <string> //Строки
#include <algorithm> //Для поворота строк
#include <ctime> //Для замера времени
using namespace std;

void set_len_test(int len_test) { //Вводим размер теста
    ofstream fout("len_test.txt");
    fout << len_test;
    fout.close();
}

int get_len_test() {
    ifstream fin("len_test.txt");
    int len_test;
    fin >> len_test;
    fin.close();
    return len_test;
}

void set_test(string test) {
    ofstream fout("test.txt");
    fout << test;
    fout.close();
}

string get_test() {
    ifstream fin("test.txt");
    string test;
    fin >> test;
    fin.close();
    return test;
}

void generate_random_test() { 
    int len_test = get_len_test();
    string test;
    int w;
    for (int i = 0; i < len_test; i++) {
        test += char(33+rand()%94);
    }
    set_test(test);
}

void set_parameters(int m, int n, int p) { // 0 < m < n < p. Мы меняем местами срезы [m:n] и [n+1:p]
    ofstream fout("parameters.txt");
    fout << m<<' '<< n << ' ' << p<<'\n';
    fout.close();
}

pair<int,  pair<int, int>> get_parameters() {
    ifstream fin("parameters.txt");
    pair<int,  pair<int, int>> ret;
    string parameters;
    fin >> ret.first;
    fin >> ret.second.first;
    fin >> ret.second.second;
    fin.close();
    return ret;
}

void generate_random_parameters() {
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

void measure(string (*func)()){
    unsigned int start_time =  clock();
    string answer = (*func)();
    unsigned int end_time = clock();
    if(get_len_test() <= 100){
        cout<<"Получила ответ: "<<answer<<endl;
    }
    cout<<"Потратила времени: "<<end_time - start_time<<endl;
}

void hello(){
    cout<<"Функция:               Кнопка:"<<endl;
    cout<<"Ввести размер теста     -   0"<<endl;
    cout<<"Сгенерировать тест      -   1"<<endl;
    cout<<"Сгенерировать срезы     -   2"<<endl;
    cout<<"Получить данные о тесте -   3"<<endl;
    cout<<"Провести замеры         -   4"<<endl;
    cout<<"Завершить программу     -   5"<<endl;
}

int main()
{
    hello();
    int swt;
    while(swt != 5){
        cout<<"Нажмите кнопку"<<endl;
        cin>>swt;
        switch(swt)
        {
            case 0:
            {
                int n;
                cout<<"Введите размер текста"<<endl;
                cin>>n;
                set_len_test(n);
                cout<<"Введен размер теста: "<<get_len_test()<<endl;
                break;
            }
            case 1:
            {
                generate_random_test();
                if(get_len_test() <= 100){
                    cout<<"Получившийся тест: "<<get_test()<<endl;
                }
                else{
                    cout<<"Тест сгенерирован"<<endl;
                }
                break;
            }
            case 2:
            {
                generate_random_parameters();
                pair<int,  pair<int, int>> parameters = get_parameters();
                cout<<"Получившиеся срезв: "<<parameters.first<<", "<<parameters.second.first<<", "<<parameters.second.second<<endl;
                break;
            }
            case 3:
            {
                cout<<"Размер теста: "<<get_len_test()<<endl;
                if(get_len_test() <= 100){
                    cout<<"Тест: "<<get_test()<<endl;
                }
                pair<int,  pair<int, int>> parameters = get_parameters();
                cout<<"Параметры: "<<parameters.first<<", "<<parameters.second.first<<", "<<parameters.second.second<<endl;
                break;
            }
            case 4:
            {
                cout<<"Первая функция"<<endl;
                measure(&func1);
                cout<<"Вторая функция"<<endl;
                measure(&func2);
                break;
            }
            case 5:
            {
                cout<<"Завершаем программу..."<<endl;
            }
            default:
            {
                cout<<"Такой команды нет. Список существующих команд:"<<endl;
                hello();
            }
        }
    }
    cout<<get_test()<<'\n';
    measure(&func1);
    measure(&func2);
    
}
