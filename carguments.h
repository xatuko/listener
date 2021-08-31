#ifndef CARGUMENTS_H
#define CARGUMENTS_H

#include "common.h"
#include "iarguments.h"

// Струкутра одного параметра.
struct argument 
{
                std::string name;       // Имя параметра.
                std::string value;      // Значение.
                bool        is_param;   // Флаг наличия у параметра значения.

                argument & operator=(const argument & arg)
                {
                    name     = arg.name;
                    value    = arg.value;
                    is_param = arg.is_param;
                    
                    return *this;
                }
};

// Структура набора параметров.
struct arguments
{
                std::vector<argument> args;     // Массив параметров.
                int size;                       // Длина массива.

                arguments & operator=(const arguments & arg)
                {
                    if (&arg == this)
                        return *this;

                    args   = arg.args;
                    size   = arg.size;
                    return *this;
                }
};

class CArguments : public IArguments
{
private: // members
    arguments m_args;

private: // methods
    
    /**
     * @brief Печать сообщения класса.
     * @param text Текст сообщения.
     */
    void print(const std::string & text);

    /**
     * @brief Печать ошибки класса.
     * @param text Текст ошибки.
     */
    void error(const std::string & text);
    
    /**
     * @brief Вывод списка возможных параметров.
     */
    void printUsage();

    /**
     * @brief Получить индекс параметра.
     * @param arg_name Имя параметра.
     * @return int - индекс.
     */
    int findArg(const std::string & arg_name);

public: // methods
    CArguments();
    ~CArguments() override;

    /**
     * @brief Считаь аргументы запуска.
     * 
     * @param arg Массив аргументов.
     * @param n Размер массива arg.
     * @return -1 в случае ошибки
     */
    int setArgs(const char ** arg, const int & n) override;
    
    /**
     * @brief Задать набор параметров. 
     * 
     * @param arg_set Набор парметров.
     */
    void init(const arguments & arg_set) override;

    /**
     * @brief Получить целочисленное значение параметра.
     * 
     * @param arg_name Имя параметра.
     * @param value Ссылка на переменную параметра.
     */
    bool getInt(const std::string & arg_name, int & value) override;
    
    /**
     * @brief Получить значение параметра с плавающей точкой.
     * 
     * @param arg_name Имя параметра.
     * @param value Ссылка на переменную параметра.
     */
    bool getDouble(const std::string & arg_name, double & value) override;
    
    /**
     * @brief Получить строковое значение параметра.
     * 
     * @param arg_name Имя параметра.
     * @param value Ссылка на переменную параметра.
     */
    bool getString(const std::string & arg_name, std::string & value) override;
};

#endif // CARGUMENTS_H