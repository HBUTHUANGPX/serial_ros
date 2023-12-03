#ifndef _CANPORT_H_
#define _CANPORT_H_
#include "ros/ros.h"
#include "motor.h"
#include <condition_variable>
#include <thread>
std::mutex mu;
class canport
{
private:
    int motor_num;
    int CANport_num;
    ros::NodeHandle n;
    std::vector<motor> Motors;
    bool sendEnabled;
    std::mutex *mtx;
    std::condition_variable *cv;
    int canboard_id, canport_id;

public:
    void send()
    {
        // ROS_INFO("%d %d ===1===",canboard_id,canport_id);
        while (true)
        {
            std::unique_lock<std::mutex> lock(mu);
            cv->wait(lock, [&]()
                     { return sendEnabled; }); // 等待发送开关打开
                                               // ROS_INFO("%d %d ===4===",canboard_id,canport_id);
            for (motor &m : Motors)
            {
                m.send();
            }
            sendEnabled = false;
            lock.unlock();
            ROS_INFO("%d %d ======", canboard_id, canport_id);
        }
    }
    void send_d()
    {
        while (1)
        {
            if (sendEnabled)
            {
                for (motor &m : Motors)
                {
                    m.send();
                }
                sendEnabled = false;
                ROS_INFO("%d %d ======", canboard_id, canport_id);
            }
        }
    }
    canport(int _CANport_num, int _CANboard_num, lively_serial *ser, std::condition_variable *_cv, std::mutex *_mtx) : cv(_cv), mtx(_mtx)
    {
        canboard_id = _CANboard_num;
        canport_id = _CANport_num;
        if (n.getParam("robot/CANboard/No_" +
                           std::to_string(_CANboard_num) +
                           "_CANboard/CANport/CANport_" + std::to_string(_CANport_num) + "/motor_num",
                       motor_num))
        {
            // ROS_INFO("Got params motor_num: %d",motor_num);
        }
        else
        {
            ROS_ERROR("Faile to get params motor_num");
        }
        for (size_t i = 1; i <= motor_num; i++)
        {
            Motors.push_back(motor(i, _CANport_num, _CANboard_num, ser));
        }  
        sendEnabled = false;
        // std::thread(&canport::send, this);
    }
    ~canport() {}
    void puch_motor(std::vector<motor> *_Motors)
    {
        for (motor &m : Motors)
        {
            _Motors->push_back(m);
        }
    }
    void enable_send()
    {
        // ROS_INFO("canport ok");
        sendEnabled = true;
    }
    int get_motor_num()
    {
        return motor_num;
    }
    int get_canboard_id()
    {
        return canboard_id;
    }
    int get_canport_id()
    {
        return canport_id;
    }
};
#endif