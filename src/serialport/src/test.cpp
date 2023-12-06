#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <ros/ros.h>

using namespace boost::asio;
using boost::system::error_code;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;

io_service io;
serial_port serial(io);
void async_read_some_data();
void read_handler(const error_code &ec, std::size_t bytes_transferred, vector<char>* buf) {
    if (!ec) {
        ROS_INFO("");
        for (size_t i = 0; i < bytes_transferred; i++)
        {
            // printf("0x%02X ",*(uint8_t *)&(*buf)[i]);
        }
        
        // cout.write(buf->data(), bytes_transferred);
        // cout << endl;

        // 继续异步读取
        async_read_some_data();
    } else {
        cerr << "Error: " << ec.message() << endl;
    }

    // 清理缓冲区
    delete buf;
}

void async_read_some_data() {
    // 创建一个新的缓冲区
    vector<char>* buf = new vector<char>(17);

    // 异步读取操作
    serial.async_read_some(buffer(*buf),
        [buf](const error_code &ec, std::size_t bytes_transferred) {
            read_handler(ec, bytes_transferred, buf);
        }
    );
}

int main() {
    try {
        serial.open("/dev/ttyUSB0");
        serial.set_option(serial_port_base::baud_rate(4000000));

        // 开始异步读取
        async_read_some_data();

        // io_service 的 run 方法将阻塞，直到所有异步操作完成或被取消
        io.run();
    } catch (std::exception &e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}