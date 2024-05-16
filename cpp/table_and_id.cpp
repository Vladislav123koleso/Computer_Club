#include "../header/table.h"


// метод занятия стола
void Table::occupied(const std::string& clientName, tm& time)
{
    // если стол не занят
    if(!_isOccupied)
    {
        _userName = clientName;
        _isOccupied = true;
        _timeStartOccupied = time;
        _timePay.tm_min = time.tm_min;
        _timePay.tm_hour = time.tm_hour + 1;
    }
    else
    {
        std::cout << std::endl;
        std::cerr << "Стол занят!" << std::endl;
    }
}

// метод освобождения стола
void Table::exemption(const std::string& clientName, const int& cost, tm& time)
{
    // если имя клиента совпадает с которым этот стол занят
    if(_userName == clientName)
    {
        // Преобразуем время в минуты
        int timeStartInMinutes = _timeStartOccupied.tm_hour * 60 +  _timeStartOccupied.tm_min;
        int timeInMinutes = time.tm_hour * 60 + time.tm_min;

        _userName = "";
        _isOccupied = false;
        
        int hour = ((timeInMinutes - timeStartInMinutes) + 59) / 60;
        _revenue += cost * (hour % 60); //кол-во часов которое стол был занят округленное в бОльшую сторону * на стоимость часа стола
        
        int timeOccupiedInMinutes = timeInMinutes - timeStartInMinutes;
        _timeOccupied.tm_hour += timeOccupiedInMinutes / 60; 
        _timeOccupied.tm_min += timeOccupiedInMinutes % 60;
    }
    else
    {
        std::cout << std::endl;
        std::cerr << "Этот стол занят другим!" << std::endl;
    }
}



//-------------------- функции id ------------------

// Клиент ушел
void id_11(tm& time,tm&timeEnd, const std::string& client,std::vector<std::string>& clients, std::queue<std::string>& queue,std::vector<Table>& tables, const int& cost) 
{

    int timeInMin = time.tm_hour * 60 + time.tm_min;
    int timeEndInMin = timeEnd.tm_hour * 60 + timeEnd.tm_min;

    //уход клиента из клуба если очередь большая и рабочие часы клуба не закончились
    if(timeEndInMin > timeInMin)
    {
        clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
        std::cout << std::put_time(&timeEnd,"%H:%M") << " 11 " << client <<std::endl;
    }
    else // уход клиентов из клуба во время закрытия клуба
    {
        std::sort(clients.begin(), clients.end());
        while(!clients.empty())
        {
            // освобождаем все 

            std::cout << std::put_time(&timeEnd,"%H:%M") << " 11 " << clients[0] <<std::endl;
            for(auto table : tables)
            {
                 if(clients[0] == table._userName)
                {
                    table.exemption(clients[0],cost, timeEnd);
                }
            }
            clients.erase(clients.begin());
        }
    }
}
// Клиент сел за стол
void id_12(tm& time, const int& cost,const std::string& clientName, std::vector<std::string>& clients,std::vector<Table>& tables, std::queue<std::string>& queue,int numberTable) 
{

    //взятие первого клиента из очереди
    std::string client = queue.front();
    queue.pop();

    tables[numberTable - 1].occupied(client,time);
    std::cout << std::endl << std::put_time(&time,"%H:%M") << " 12 " << clientName << " " << numberTable;
}
// Ошибки
void id_13(int idError) 
{
    switch (idError)
    {
    case 1:// Клуб не работает!
        std::cerr << "NotOpenYet" ;
        break;
    case 2: // Место занято!
        std::cerr << "PlaceInBusy";
        break;    
    case 3: // Есть свободные места!
        std::cerr << "ICanWaitNoLonger";
        break;
    case 4: // Клиент не в клубе
        std::cerr << "ClientUnknown";
        break;
    default:
        break;
    }
}
// Клиент пришел
void id_1(tm& startTime,tm& endTime,tm& timeVisit ,const std::string& clientName,std::vector<std::string>& clients) 
{
    // Преобразуем время в секунды
    int startTimeInSeconds = startTime.tm_hour * 3600 + startTime.tm_min * 60;
    int endTimeInSeconds = endTime.tm_hour * 3600 + endTime.tm_min * 60;
    int timeVisitInSeconds = timeVisit.tm_hour * 3600 + timeVisit.tm_min * 60;

    if((startTimeInSeconds <= timeVisitInSeconds) && (timeVisitInSeconds <= endTimeInSeconds))
    {
        clients.push_back(clientName);
    }
    else // если клиент пришел в нерабочие часы
    {
        std::cout << std::endl;
        std::cout << std::put_time(&timeVisit,"%H:%M") << " " << "13" << " ";
        id_13(1);
    }

}
// Клиент сел за стол
void id_2(tm& time,const std::string& clientName,int numberTable,std::vector<Table>& tables,const int& cost) 
{
    // если за столом куда мы хотим посадить клиента уже сидят возвращаем ошибку
    if(tables[numberTable - 1]._userName != "")
    {
        std::cout << std::endl;
        std::cout << std::put_time(&time,"%H:%M") << " " << "13" << " ";
        id_13(2);
    }
    else
    {
        //  проверяем сидит ли пользователь за каким-то из столов,если уже сидит за столом пересаживаем его
        for(int i = 0; i < tables.size(); i++)
        {
            if(tables[i]._userName == clientName) // если сидит то освобождаем стол и сажаем за новый
            {
                tables[i].exemption(clientName, cost, time);
                tables[numberTable - 1].occupied(clientName, time);
                return;
            }
        }
        // если клиент не сидит за столом
        tables[numberTable - 1].occupied(clientName, time);
    }
    
}
// Клиент ожидает
void id_3(tm& time, tm& timeEnd, const std::string& clientName,std::vector<std::string>& clients,std::queue<std::string>& queue, std::vector<Table>& tables,const int& countTable, const int& cost) 
{
    //если в клубе есть свободные столы - ошибка
    for(int i = 0; i < tables.size(); i++)
    {
        if(tables[i]._isOccupied == false)
        {
            std::cout << std::endl;
            std::cout << std::put_time(&time,"%H:%M") << " " << "13" << " ";
            id_13(3);
            return;
        }

    }
    // если в очереди клиентов больше чем кол-ва столов - ошибка
    if(countTable < queue.size())
    {
        std::cout << std::endl;
        std::cout << std::put_time(&time,"%H:%M") << " " << "13" << " ";
        //std::cerr << "Клиент ушел, большая очередь" ;
        
        id_11(time, timeEnd,clientName,clients, queue, tables, cost);

        return;
    }
    // добавление клиента в очередь
    queue.push(clientName);

}
// Клиент ушел
void id_4(tm& time, const int& cost, const std::string& clientName, std::vector<std::string>& clients,std::vector<Table>& tables, std::queue<std::string>& queue) 
{
    int numberTable = 0;
    // если клиента нет в клубе - ошибка
    auto it = std::find(clients.begin(), clients.end(), clientName);
    if(it == clients.end())
    {
        std::cout << std::endl;
        std::cout << std::put_time(&time,"%H:%M") << " " << "13" << " ";
        id_13(4);
    }
    else
    {
        //находим какой стол занимал клиент и освобождаем его
        for(int i = 0; i < tables.size(); i++)
        {
            if(tables[i]._userName == clientName)
            {
                numberTable = i+1;
                tables[i].exemption(clientName,cost,time);
                break;
            }
        }
        // освободившийся стол занимает 1ый клиент в очереди если очередь не пуста
        if(!queue.empty())
        {
            id_12(time, cost, clientName, clients, tables, queue, numberTable);
        }
        // клиент уходит из клуба
        clients.erase(std::remove(clients.begin(),clients.end(),clientName),clients.end());
    }

}