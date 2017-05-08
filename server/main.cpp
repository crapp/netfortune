#include <chrono>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main(void)
{
    boost::asio::io_service io;
    std::cout << "Running before timer" << std::endl;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
    t.wait();

    std::cout << "Running after timer" << std::endl;

    return 0;
}
