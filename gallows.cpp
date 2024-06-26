﻿#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <windows.h>    
#include <conio.h>      
using namespace std;

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
//функция, которая возвращает дескриптор стандартного выходного потока
//c помощью этого дескриптора программы могут выполнять операции записи или вывода на стандартный выходной поток

vector <int> wordsKey = { 1, 2, 3 };
string word, guessedLetters;
int length, key;

void SetXY(short X, short Y)     //функция для получения координат курсора
{
    COORD coord = { X, Y };
    SetConsoleCursorPosition(hStdOut, coord);    //функция, которая устанавливает положение курсора в указанном буфере экрана консоли
}

enum ConsoleColor     //перечисление, которое определяет константы, определяющие основной цвет и цвет фона консоли
{
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

void SetColor(ConsoleColor text, ConsoleColor background)   //выбор цвета и фона текста
{
    SetConsoleTextAttribute(hStdOut, (WORD)((background) | text));     //функция для настройки цвета
}


void Start()   //выбор темы
{
    SetColor(Cyan, Black); SetXY(12, 3); cout << "Для начала игры, выбери тему:" << endl;
    SetColor(LightCyan, Black);
    SetXY(12, 5); cout << "Легко (3 буквы) - 'S'        Средне (4 буквы) - 'M' " << endl;
    SetXY(12, 8); cout << "             Сложно (6 букв) - 'H' " << endl;
    SetXY(12, 11); cout << "Режим вдвоем - 'D'           Рандомный режим - 'R' " << endl;

    SetColor(DarkGray, Black); SetXY(10, 16); cout << "(Для выбора нажми букву на клавиатуре)" << endl;

    while (!(key == 's' || key == 'S' || key == 'm' || key == 'M' || key == 'h' || key == 'H' || key == 'd' || key == 'D')) //проверяет какая клавиша была нажата
    {
        if (_kbhit())    //ожидание нажатия кнопки
        {
            key = _getch();   //код нажатой кнопки
            if (key == 'r' || key == 'R')
            {
                key = wordsKey[rand() % wordsKey.size()];
                break;
            }
        }
    }
}


void clearScreen()  //отчистика терминала
{
    system("cls");
}


string PlayingTogether() //выбор слова в совместной игре
{
    clearScreen();

    string word_input;
    SetColor(LightCyan, Black);
    SetXY(12, 2); cout << "Игрок 1, закрой глаза !!! ";
    SetXY(12, 5); cout << "Игрок 2, введи слово, которое будет угадывать Игрок 1: ";
    SetColor(DarkGray, Black);
    cin >> word_input; //запрос загадать слово

    return word_input;

}


void ChooseWord() //вызов выбранной темы и рандомного слово из нее
{
    string fName, sTitle;

    if (key == 's' || key == 'S' || key == 1) //легко
    {
        fName = "S.txt";
        sTitle = "Режим: легко.";
    }
    else if (key == 'm' || key == 'M' || key == 2) //средне
    {
        fName = "M.txt";
        sTitle = "Режим: средне.";
    }
    else if (key == 'h' || key == 'H' || key == 3) //сложно
    {
        fName = "H.txt";
        sTitle = "Режим: сложно.";
    }
    else if (key == 'd' || key == 'D') //вдвоем
    {
        word = PlayingTogether();

        clearScreen();
        SetColor(Cyan, Black); SetXY(3, 17); cout << "Режим: вдвоем." << endl;
        return;
    }

    SetColor(Cyan, Black); SetXY(3, 17); cout << sTitle << endl;

    ifstream wordsFile(fName);    //заполнение вектора словами из выбранного текстового файла
    istream_iterator<string> start(wordsFile), end;
    vector<string> words(start, end);

    if (words.size() == 0)     //проверка вызванного файла
    {
        clearScreen();
        SetColor(Red, Black); SetXY(3, 17); cout << "Ошибка чтения словаря!" << endl;
        return;
    }
    word = words[rand() % words.size()];
}


void Walls()  //поле для виселицы
{
    SetColor(Blue, Black);

    for (int m = 15; m < 31; m++)
    {
        SetXY(m, 5); cout << "*";
        SetXY(m, 14); cout << "*";
    }

    for (int m = 6; m < 14; m++)
    {
        SetXY(15, m); cout << "*";
        SetXY(30, m); cout << "*";
    }
}


bool isWordGuessed()    //проверяет, были ли все буквы из переменной word угаданы, находятся ли они в переменной "guessedLetters"
{
    for (char c : word)
    {
        if (guessedLetters.find(c) == string::npos) //если хотя бы одна буква из загаданного слова не была угадана, функция вернет false, иначе вернет true
        {
            return false;
        }
    }
    return true;
}

void printHangman(int attemptsLeft)    //рисует виселицу по кол-ву ошибок
{
    SetColor(Brown, Black);
    if (attemptsLeft == 7) //рисует постамент
    {
        for (int m = 19; m < 26; m++)
        {
            SetXY(m, 6); cout << "_";
        }

        for (int m = 7; m < 13; m++)
        {
            SetXY(18, m); cout << "|";
        }
    }
    else if (attemptsLeft == 6) //добавление частей тела в зависимости от номера ошибки
    {
        SetColor(DarkGray, Black); SetXY(26, 7); cout << "|" << endl;
    }
    else if (attemptsLeft == 5)
    {
        SetColor(White, Black); SetXY(26, 8); cout << "0" << endl;
    }
    else if (attemptsLeft == 4)
    {
        SetColor(White, Black); SetXY(26, 9); cout << "|" << endl;
    }
    else if (attemptsLeft == 3)
    {
        SetColor(White, Black); SetXY(25, 9); cout << "/" << endl;
    }
    else if (attemptsLeft == 2)
    {
        SetColor(White, Black); SetXY(27, 9); cout << "\\" << endl;
    }
    else if (attemptsLeft == 1)
    {
        SetColor(White, Black); SetXY(25, 10); cout << "/" << endl;
    }
    else if (attemptsLeft == 0)
    {
        SetColor(White, Black); SetXY(27, 10); cout << "\\" << endl;
    }
}

void printGuessedWord()     //вывод угаданных букв и пропусков в слове word на экране
{
    SetColor(LightCyan, Black);
    int m = (23 - length / 2); //начальная позиция по оси X для вывода слова
    SetXY(m, 15);
    for (char c : word)
    {
        if (guessedLetters.find(c) != string::npos) //если буква содержится в "guessedLetters", то она выводится на экран на нужной позиции
        {
            cout << c;
        }
        else { cout << "_"; }
    }
}

int main()   //-----------ИГРА
{
    //setlocale(LC_ALL, "Russian");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    srand(time(NULL));
    SetColor(LightGreen, Black); SetXY(19, 1); cout << "Добро пожаловать в Виселицу!" << endl;

    while (true)
    {
        Start();
        clearScreen();

        ChooseWord();
        Walls();


        length = word.length(); //длина загаданного слова

        int attemptsLeft = 7; //оставшиеся попытки

        SetXY(3, 18); cout << "Введи букву: ";

        int enterX = 20;
        char guess;

        while (attemptsLeft > 0 && !isWordGuessed()) //игра идет пока не кончатся попытки или не отгадается слово
        {
            SetColor(Cyan, Black); SetXY(5, 2); cout << "У тебя   попыток ввести букву.";
            SetXY(12, 2); cout << attemptsLeft;

            printHangman(attemptsLeft);

            SetColor(Cyan, Black);
            printGuessedWord();

            enterX++;
            SetColor(LightCyan, Black); SetXY(enterX, 18);
            guess = _getch();
            cout << guess;

            if (word.find(guess) != string::npos) //если буква угадана правильно
            {
                SetColor(Green, Black); SetXY(19, 4); cout << "Правильно!" << endl;
                guessedLetters += guess;
            }
            else //если буква угадана неправильно
            {
                SetColor(Red, Black); SetXY(19, 4); cout << "   Ошибка! " << endl;
                attemptsLeft--;
            }
        }

        printHangman(attemptsLeft);

        if (isWordGuessed()) //если слово угадано правильно
        {
            int m = (23 - length / 2);
            SetColor(LightCyan, Black); SetXY(m, 15); cout << word;
            m = 58 - ((39 + length) / 2);
            SetColor(Green, Black); SetXY(m, 8); cout << "Поздравляю, ты угадал слово: " << word << '.' << endl;
        }
        else //если слово угадано неправильно
        {
            int m = 58 - ((30 + length) / 2);
            SetColor(Red, Black); SetXY(m, 8); cout << "Ты не угадал слово: " << word << '.' << endl;
        }

        key = 0; //обнуление ключа темы для выбора дальнейшего действия

        SetColor(White, Black);
        SetXY(43, 10); cout << "Для начала игры, нажми 'S'." << endl;
        SetXY(43, 11); cout << "Для выхода из игры, нажми 'Esc'." << endl;

        while (true)
        {
            if (_kbhit())
            {
                key = _getch();   //код нажатой кнопки
                if (key == 's' || key == 'S')
                {
                    clearScreen();
                    break;
                }
                if (key == VK_ESCAPE) //если игрок выбрал выйти из игры
                    return 1;
            }
        }


        length = key = 0; //обнуление переменных для продолжения игры
        word = "";
        guessedLetters = "";
    }

    return 1;
}