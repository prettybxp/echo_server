
#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include <thread>
#include <memory>
#include <functional>
#include <boost/variant/variant.hpp>
#include <boost/asio.hpp>
//#include <boost/asio/yield.hpp>
//#include <boost/asio/coroutine.hpp>
#include <boost/asio/spawn.hpp>

using namespace std;
using namespace std::chrono_literals;
using namespace boost;
using namespace boost::asio;

class Session : public std::enable_shared_from_this<Session>, public coroutine
{
public:
    Session(io_service &service)
        : m_io(service), m_socket(service), m_timer(service)
    {
        cout << "session ctor" << std::hex << this << endl;
    }

    ~Session()
    {
        cout << "session dtor" << std::hex << this << endl;
    }

    void start()
    {
        cout << "accept a new connect" << endl;
        work();
        // m_timer.expires_from_now(posix_time::seconds(100));
        // m_timer.async_wait([self = shared_from_this()](system::error_code const &e) {
        //     self->stop();
        // });
    }

    void stop()
    {
        cout << "stop a session" << endl;
        m_socket.shutdown(socket_base::shutdown_both);
    }

#if 0
    void work()
    {
        m_socket.async_receive(buffer(m_data), [this, self = shared_from_this()](system::error_code const &e, std::size_t bytes_transferred) {
            cout << "error=" << e << endl;
            cout << "receive " << bytes_transferred << " byte" << endl;
            if (e)
            {
                return;
            }
            m_socket.async_send(buffer(m_data), [this, self = shared_from_this()](system::error_code const &e, std::size_t bytes_transferred) {
                if (e)
                {
                    return;
                }
                work();
            });
        });
    }
#elif 0

    void work(system::error_code const &e = system::error_code(), std::size_t bytes_transferred = 0)
    {
        cout << "work>> " << e << endl;
        cout << "error=" << e << endl;
        cout << "receive " << bytes_transferred << " byte" << endl;
        if (e)
        {
            return;
        }

        reenter(this) for (;;)
        {
            yield m_socket.async_receive(buffer(m_data), std::bind(&Session::work, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
            yield m_socket.async_send(buffer(m_data), std::bind(&Session::work, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
        }
    }

#else
    void work(system::error_code const &e = system::error_code(), std::size_t bytes_transferred = 0)
    {
        spawn(m_io, [this, self = shared_from_this()](yield_context yie) {
            for (;;)
            {
                system::error_code e;
                std::size_t bytes_transferred = m_socket.async_receive(buffer(m_data), yie[e]);
                cout << "spawn receive " << bytes_transferred << " byte" << endl;
                if (e)
                {
                    return;
                }
                m_socket.async_send(buffer(m_data), yie[e]);
                if (e)
                {
                    return;
                }
            }
        });
    }
#endif

    ip::tcp::socket &sock()
    {
        return m_socket;
    }

private:
    io_service &m_io;
    ip::tcp::socket m_socket;
    deadline_timer m_timer;
    std::array<char, 1024> m_data;
};

int main()
{
    int port = 18440;
    io_service io;
    spawn(io, [&io, port](yield_context yie) {
        ip::tcp::acceptor acceptor(io, ip::tcp::endpoint(ip::tcp::v4(), port));
        for (;;)
        {
            system::error_code e;
            auto session = std::make_shared<Session>(io);
            acceptor.async_accept(session->sock(), yie[e]);
            if (e)
            {
                return;
            }
            session->start();
        }
    });
    io.run();
    cout << "main end" << endl;
}
