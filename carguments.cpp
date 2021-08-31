#include "carguments.h"

CArguments::CArguments() : IArguments() { }

CArguments::~CArguments() { }

void CArguments::init(const arguments & args)
{
    m_args.args.resize(args.size);
    m_args = args;
}

int CArguments::setArgs(const char ** arg, const int & n)
{
    if (n == 1)
        return 1;
    
    for (int i = 1; i < n;)
    {
        std::string arg_name(arg[i]);
        if (arg_name == "--help")
        {
            printUsage();
            return -1;
        }
        
        int idx;
        if ((idx = findArg(arg_name)) != -1)
        {
            if (m_args.args[idx].is_param)
            {
                m_args.args[idx].value = std::string(arg[i+1]);
                i++;
            }
            i++;
        }
        else
        {
            print("Неверный парметр: " + std::string(arg[i]));
            printUsage();
            return -1;
        }
    }
    
    return 1;
}

int CArguments::findArg(const std::string & arg_name)
{
    for (int i = 0; i < m_args.size; i++)
        if (m_args.args[i].name == arg_name)
            return i;
    return -1;
}

bool CArguments::getInt(const std::string & arg_name, int & value)
{
    int idx;
    if ((idx = findArg(arg_name)) == -1)
    {
        print("Параметр " + arg_name + " не найден.");
        return false;
    }

    if (!m_args.args[idx].is_param)
    {
        print("У параметра " + arg_name + " нет значения.");
        return false;
    }

    value = atoi(m_args.args[idx].value.data());
    return true;
}

bool CArguments::getDouble(const std::string & arg_name, double & value)
{
    int idx;
    if ((idx = findArg(arg_name)) == -1)
    {
        print("Параметр " + arg_name + " не найден.");
        return false;
    }

    if (!m_args.args[idx].is_param)
    {
        print("У параметра " + arg_name + " нет значения.");
        return false;
    }

    value = atof(m_args.args[idx].value.data());
    return true;
}

bool CArguments::getString(const std::string & arg_name, std::string & value)
{
    int idx;
    if ((idx = findArg(arg_name)) == -1)
    {
        print("Параметр " + arg_name + " не найден.");
        return false;
    }

    if (!m_args.args[idx].is_param)
    {
        print("У параметра " + arg_name + " нет значения.");
        return false;
    }

    value = m_args.args[idx].value;
    return true;
}

void CArguments::print(const std::string & text)
{
    std::cout << "[CArguments] " << text << std::endl;
}

void CArguments::error(const std::string & text)
{
    print(text);
    print("ERROR: " + std::to_string(errno));
}

void CArguments::printUsage()
{
    std::cout << "Options:" << std::endl
              << std::setw(10) << std::right << "-p" << std::setw(15) << std::right << " [port] " << " - установка номера порта" << std::endl
              << std::setw(10) << std::right << "--ipaddr" << std::setw(15) << std::right << " [ip addres] " << " - установка ip-адреса" << std::endl;
}