#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include <fstream>
#include <sstream>

#include <ctime>
#include <iomanip>

#include <queue>



// класс столов
class Table
{
    public:
    Table(int number) :
    _number(number), _revenue(0), _isOccupied(false),_userName(""), _timeOccupied({0,0,0}), _timeStartOccupied({0,0,0}), _timePay({0,0,0}){}

    int calculatingRevenue(int& revenue); // метод подсчета выручки
    
    std::tm calculatingTimeOccupied(std::tm& timeOccupied); // метод подсчета времени в течение которого стол был занят
    
    void occupied(const std::string& clientName, tm& time); // метод занятия стола
    void exemption(const std::string& clientName, const int& cost, tm& time); // метод освобождения стола
public:
    int _number; // номер стола 
    int _revenue; // сделанная выручка с этого стола
    bool _isOccupied; // состояние стола (сидит за ним гость или нет)
    std::string _userName; // имя пользователя который сидит за столом
    std::tm _timeOccupied; // время в течение которого стол был занят
    std::tm _timeStartOccupied; // время начала занятия пользователем
    std::tm _timePay; // время с которого будет списана следующая оплата за следующий час
};

