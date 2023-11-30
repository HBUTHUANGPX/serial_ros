#!/bin/sh

echo "hello,world"
source ./devel/setup.bash
# set password 0000
sudo chmod -R 777 /dev/tty*
# expect "password:"
# send "$password\r"
# interact
roslaunch serial_test serial_test.launch