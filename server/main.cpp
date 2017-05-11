#include <chrono>
#include <functional>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

void simple_async()
{
    boost::asio::io_service io;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.async_wait([](const boost::system::error_code &) {
        std::cout << "Hello from async" << std::endl;
    });

    io.run();
}

void simple_repeated_async()
{
    boost::asio::io_service io;
    std::function<void(const boost::system::error_code &, int *,
                       boost::asio::deadline_timer *)>
        printer;
    printer = [&printer, &io](const boost::system::error_code &, int *count,
                              boost::asio::deadline_timer *t) {
        if (*count > 0) {
            (*count)--;
        }
        t->async_wait(
            boost::bind(printer, boost::asio::placeholders::error, &count, &t));
    };

    int count = 5;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.async_wait(
        boost::bind(printer, boost::asio::placeholders::error, &count, &t));
    io.run();
}

int main(void)
{
    simple_async();
    return 0;
}
