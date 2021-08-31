#include "ctcpserver.h"

CTcpServer::CTcpServer() : ITcpServer() { }

CTcpServer::~CTcpServer() { closeSock(); }

void CTcpServer::setAddr(const std::string & ip, const int & port)
{
    m_sock_addr.sin_addr.s_addr = inet_addr(ip.data());
    m_sock_addr.sin_family = AF_INET;
    m_sock_addr.sin_port = htons(port);
    
    m_sock_len = sizeof(m_sock_addr);
}

int CTcpServer::init()
{
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (m_sock < 0) 
    {
        error("Ошибка создания сокета.");
        return -1;
    }

    if (bind(m_sock, (sockaddr*)&m_sock_addr, m_sock_len) == -1)
    {
        error("Ошибка привязки сокета.");
        return -1;
    }

    return 1;
}

size_t CTcpServer::readData(const int & fd, ByteArray & data)
{
    size_t res = read(fd, &data[0], data.size());
    data.resize(res);
    return res;
}

size_t CTcpServer::writeData(const int & fd, const ByteArray & data)
{
    return write(fd, data.data(), data.size());
}

int CTcpServer::startListen()
{
    if (listen(m_sock, 0) == -1)
    {
        error("Ошибка прослушки сокета.");
        return -1;
    }

    m_run = true;
    m_thread = std::make_shared<std::thread>(&CTcpServer::process, this);

    return 1;
}

void CTcpServer::process()
{
    while (m_run)
    {
        sockaddr_in sock_addr;
        socklen_t sock_len = sizeof(sock_addr);
        int sock = accept(m_sock, (sockaddr*)&sock_addr, &sock_len);
        if (sock == 1)
        {
            error("Ошибка соединения.");
            return;
        }
        print("Соединения установлено.");
        print("Входящий IP: " + std::string(inet_ntoa(sock_addr.sin_addr)));
        print("Порт: " + std::to_string(sock_addr.sin_port));

        m_pfd = new pollfd[1];

        m_pfd[0].fd      = sock;
        m_pfd[0].events  = POLLIN;
        m_pfd[0].revents = 0;

        while(m_run)
        {
            int res = poll(m_pfd, 1, 1000);

            if (res > 0 && (m_pfd[0].revents & POLLIN))
            {
                ByteArray data(256);
                if (readData(sock, data) > 0)
                {
                    std::unique_lock<std::mutex> lock(m_mutex);
                    m_buffer.push(data);
                    lock.unlock();
                }
                m_pfd[0].revents = 0;
            }
        }
        close(sock);
    }
}

bool CTcpServer::getMessage(ByteArray & message)
{
    if (m_buffer.size() == 0)
        return false;

    std::unique_lock<std::mutex> lock(m_mutex);

    message = m_buffer.front();
    m_buffer.pop();

    lock.unlock();

    return true;
}

void CTcpServer::closeSock()
{
    m_run = false;
    if (m_thread && m_thread->joinable())
        m_thread->join();
    
    close(m_sock);
}

void CTcpServer::print(const std::string & text)
{
    std::cout << "[CTcpServer] " << text << std::endl;
}

void CTcpServer::error(const std::string & text)
{
    std::cout << "[CTcpServer] "        << text                  << std::endl
              << "[CTcpServer] ERROR: " << std::to_string(errno) << std::endl;
}