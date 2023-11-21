#include "serial/serial.h"
#include "ros/ros.h"
int serial_read(serial::Serial &ser, std::string &result)
{
    result = ser.read(ser.available());
    return 0;
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "my_serial_port_imu");
    ros::NodeHandle n;
    serial::Serial ser;
    ser.setPort("/dev/ttyACM0");                               // 设置打开的串口名称
    ser.setBaudrate(4000000);                                  // 设置串口的波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(1000); // 创建timeout
    ser.setTimeout(to);                                        // 设置串口的timeout
    // 打开串口
    try
    {
        ser.open(); // 打开串口
    }
    catch (const std::exception &e)
    {
        ROS_ERROR_STREAM("Unable to open port "); // 打开串口失败，打印信息
        return -1;
    }
    if (ser.isOpen())
    {
        ROS_INFO_STREAM("IMU Serial Port initialized."); // 成功打开串口，打印信息
    }
    else
    {
        return -1;
    }
    ros::Rate loop_rate(1);
    std::string w_data, r_data;
    serial_read(ser, r_data);
    std::cout << r_data << std::endl;
    w_data = 'nh';
    while (ros::ok())
    {
        serial_read(ser, r_data);
        if (r_data.length() > 0)
        {
            // std::cout << r_data << std::endl;
            ROS_INFO_STREAM("");
            for (char s : r_data)
            {
                printf("0x%02X ", s);
            }

            std::cout << std::endl;
        }

        ser.write(w_data);
        loop_rate.sleep();
    }
}