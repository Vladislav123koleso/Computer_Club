#include "cpp/table_and_id.cpp"



int main(int argc,char* argv[])
{
    setlocale(LC_ALL,"ru");


    tm startTime = {}; // Начальное время работы клуба
    tm endTime = {}; // Конечное время работы клуба
    

    std::vector<Table> tables; // список столов
    std::vector<std::string> clients; // список клиентов в клубе

    std::string line; // строка
    
    int countTable = 0; // кол-во столов
    int numberLine = 1; // переменная для отслеживания номера строки
    int cost = 0;

    std::queue<std::string> clientsQueue; // очередь клиентов



    if(argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <File>" << std::endl;
        return 1;
    }
    // Получаем имя файла из аргументов командной строки
    std::string fileName = argv[1];
    // 
    std::ifstream file(fileName);

    if(!file)
    {
        std::cerr << "Unable to open file: " << fileName << std::endl;
        return 1;
    }




    while(std::getline(file, line))
    {
        // строковый поток
        std::istringstream iss(line);

        // для чтения частей строк
        //std::vector<std::string> words;
        std::string word;

        tm timeId = {}; // время события

        if(numberLine == 1)
        {
            if (!(iss >> countTable))
            {
                std::cerr << "Ошибка: Не удалось прочитать число из первой строки." << std::endl;
                return 1;
            }
            // Создаем экземпляры класса Table
            for (int i = 0; i < countTable; i++)
            {
                Table table(i+1);
                // Создаем экземпляр класса Table и добавляем его в вектор tables
                tables.push_back(table);
            }
            numberLine++;
        }
        else if(numberLine == 2)
        {
            while (iss >> word) {
                std::istringstream timeStream(word);
                // проверка является ли переменная начала работы нулю
                if(startTime.tm_hour == 0 && startTime.tm_min == 0)
                {
                    timeStream >> std::get_time(&startTime,"%H:%M");
                    // Проверяем, удалось ли успешно считать время
                    if (timeStream.fail()) {
                        std::cerr << "Ошибка: Некорректный формат времени." << std::endl;
                        return 1;
                    }
                    std::cout << std::put_time(&startTime,"%H:%M") << std::endl;    
                }
                else if(endTime.tm_hour == 0 && endTime.tm_min == 0)
                {
                    timeStream >> std::get_time(&endTime,"%H:%M");
                    // Проверяем, удалось ли успешно считать время
                    if (timeStream.fail()) {
                        std::cerr << "Ошибка: Некорректный формат времени." << std::endl;
                        return 1;
                    }  
                }
                else // если есть более 2ух данных в строке
                {
                    std::cerr << "Данных больше допустимого." << std::endl;
                    return 1;
                }
            }
            // Увеличиваем счетчика строки
            numberLine++;
        }
        else if(numberLine == 3) // установка стоимости часа за столом
        {
            iss >> cost;
            numberLine++;
        }
        else if(numberLine >= 4)
        {
            int numberWord = 1; // номер слова в строке
            int id = 0; // id
            int numberTable = 0; // номер стола
            std::string client = "";
            // проходимся по словам в строке
            while(iss >> word)
            {
                std::istringstream wordStream(word);
                

                if(numberWord == 1) // запись времени события
                {
                    wordStream >> std::get_time(&timeId,"%H:%M");

                    std::cout << std::put_time(&timeId,"%H:%M") << " ";
                }
                else if(numberWord == 2) // запись id
                {
                    wordStream >> id;

                    std::cout << id << " ";
                }
                else if(numberWord == 3) // запись имени клиента
                {
                    wordStream >> client;

                    std::cout << client;

                    if( id != 2)
                    {
                    switch(id) // в зависимости от id вызывается соответствующая ему функция
                    {
                            case 1: // Клиент пришел
                                // проверка если клиент уже в клубе (проходим по списку клиентов которые уже в клубе)
                                for(int i = 0; i < clients.size(); i++)
                                {
                                    if(clients[i] == client)
                                    {
                                        std::cerr << "Клиент есть в клубе!" ;
                                    }
                                }
                                //
                                id_1(startTime,endTime,timeId,client,clients);
                                break;
                            case 3: // Клиент ожидает
                                id_3(timeId, endTime, client, clients,clientsQueue, tables, countTable, cost);
                                break;
                            case 4: // Клиент ушел
                                id_4(timeId,cost,client,clients,tables,clientsQueue);
                                break;
                            case 11: // Клиент ушел
                                break;
                            case 12: // Клиент сел за стол
                                break;
                            default: // Ошибки
                                break;
                        }    
                    }
                }
                else if(numberWord == 4)
                {
                    //если клиента нет в клубе выдаем ошибку
                    auto it = std::find(clients.begin(), clients.end(), client);
                    if(it == clients.end())
                    {
                        std::cerr << "Клиент не в клубе!" << std::endl;
                    }
                    else
                    {
                        // запись номера стола
                        wordStream >> numberTable;
                        std::cout << " " << numberTable;
                        // сделать проверку есть ли он в клубе, меньше или равен номер стола кол-ву столов
                        id_2(timeId, client,numberTable, tables, cost);
                    }
                }

                numberWord++;

            } 
            std::cout << std::endl;
        }

    }

    tm timeId = endTime;
    std::string client = "";
    
    id_11(timeId,endTime,client,clients,clientsQueue,tables, cost); // уход всех клиентов под закрытие клуба

    std::cout << std::put_time(&endTime,"%H:%M") << std::endl;

    //вывод выручки столов
    for(auto table : tables)
    {
        std::cout << table._number << " " << table._revenue << " " << std::put_time(&table._timeOccupied,"%H:%M") << std::endl;
    }

    

    file.close();
    return 0;
}
