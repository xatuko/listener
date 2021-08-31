#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <atomic>
#include <thread>
#include <memory>
#include <sys/poll.h>
#include <unistd.h>
#include <queue>
#include <mutex>

#include "itcpserver.h"

class CTcpServer : public ITcpServer
{
private: // members
    int m_sock;                   // Дескриптор сокета.
    
    sockaddr_in m_sock_addr;      // Адрес.
    socklen_t   m_sock_len;       // Размер структуры адреса.
    
    std::shared_ptr<std::thread>  m_thread;          // Поток чтения сообщений из сокета.
    std::queue<ByteArray>         m_buffer;          // Буфер прочитанных сообщений.
    std::atomic_bool              m_run {false};     // Флаг запуска процесса.
    std::mutex                    m_mutex;           // Мьютекс синхронизации доступа к буферу.
    pollfd                      * m_pfd;             // Пол на входящие сообщения.

private: // methods
    /**
     * @brief Прочитать данные из сокета.
     * @param fd Дескриптор сокета.
     * @param data Буфер данных.
     * @return size_t Кол-во прочитанных байт.
     */
    size_t readData(const int & fd, ByteArray & data);

    /**
     * @brief Записать данные в сокет.
     * @param fd Дескриптор сокета.
     * @param data Данные.
     * @return size_t Кол-во записанных байт.
     */
    size_t writeData(const int & fd, const ByteArray & data);

    /**
     * @brief Поток обработки входящих соединений.
     */
    void process();

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

public: // methods
    CTcpServer();
    ~CTcpServer() override;

    /**
     * @brief Инициализация сокета.
     * @return 1 - успешная инициализация, -1 - ошибка.
     */
    int init() override;

    /**
     * @brief Начать прослушивание сокета.
     * @return -1 - ошибка прослушивания.
     */
    int startListen() override;

    /**
     * @brief Закрыть сокет.
     */
    void closeSock() override;

    /**
     * @brief Задать адрес сокета.
     * @param ip IP-адрес.
     * @param port Порт.
     */
    void setAddr(const std::string & ip, const int & port) override;

    /**
     * @brief Получить сообщение из буфера.
     * @param message Сообщение.
     * @return true - есть сообщение, false - нет сообщения.
     */
    bool getMessage(ByteArray & message) override;
};

#endif // CTCPSERVER_H