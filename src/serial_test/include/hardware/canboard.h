#ifndef _CANBOARD_H_
#define _CANBOARD_H_
#include <iostream>
#include <vector>
#include "canport.h"
#include "ros/ros.h"
class canboard
{
private:
    int CANport_num;
    ros::NodeHandle n;
    std::vector<canport> CANport;
    // std::vector<motor> motor;
    // std::vector<std::shared_ptr<canport>> CANport;

public:
    canboard(int _CANboard_ID, std::vector<lively_serial *> *ser,std::condition_variable *_cv,std::mutex *_mtx)
    {
        if (n.getParam("robot/CANboard/No_" +
                        std::to_string(_CANboard_ID) + "_CANboard/CANport_num",
                       CANport_num))
        {
            // ROS_INFO("Got params CANport_num: %d",CANport_num);
        }
        else
        {
            ROS_ERROR("Faile to get params CANport_num");
        }
        for (size_t j = 1; j <= CANport_num; j++) // 一个串口对应一个CANport
        {
            // std::cout << (_CANboard_ID - 1) * CANport_num + j - 1 << std::endl;
            CANport.push_back(canport(j, _CANboard_ID, (*ser)[(_CANboard_ID - 1) * CANport_num + j - 1],_cv,_mtx));
            // auto port = std::make_shared<canport>(j, _CANboard_ID, (*ser)[(_CANboard_ID - 1) * CANport_num + j - 1],_cv);
            // CANport.push_back(port);
        }
    }
    ~canboard() {}
    int get_CANport_num()
    {
        return CANport_num;
    }
    void push_CANport(std::vector<canport> *_CANport)
    // void push_CANport(std::vector<std::shared_ptr<canport>> _CANport)
    {
        for (canport &c : CANport)
        // for (std::shared_ptr<canport> c : CANport)
        {
            _CANport->push_back(c);
        }
    }
    void enable_send()
    {
        // for (std::shared_ptr<canport> c : CANport)
        for (canport& c : CANport)
        {
            // c->enable_send();
            c.enable_send();
        }
        // ROS_INFO("canboard ok");

    }
};
#endif