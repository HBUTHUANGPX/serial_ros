to坤武：

现在的代码使用，首先确保安装了ros的serial库，如果没有： sudo apt-get install ros-noetic-serial
插上ft4232后，你在终端中现尝试： ls /dev/ttyUSB*，如果连接成功你会看到 1/2/3/4
对于串口/CAN/电机是如何定义的，USB0/1号串口对应第一块H7的第1/2个串口，一个CAN口与一个串口对应，一个CAN口挂3个电机，电机的id在src/serial_test/robot_param/12dof_STM32H730_model_P_Orin_params.yaml这个文件中查看
拓扑结构为 robot->CANboard->CANport->motor->id


我写了一个sh脚本你可以直接使用: '. test.h'