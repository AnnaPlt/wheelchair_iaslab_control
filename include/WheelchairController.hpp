#ifndef WHEELCHAIRCONTROLLER_HPP
#define WHEELCHAIRCONTROLLER_HPP

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>
#include <sensor_msgs/Joy.h>
#include <functional>
#include "ControllerHandler.hpp"
#include "JoyUtility.hpp"
#include <string>

/**
 * @class WheelchairController
 * @brief A class to handle wheelchair control via velocity and modality commands.
 *
 * This class provides an interface between ROS velocity/modality messages and the
 * wheelchair controller, managing speed, profile, and joystick movements.
 */
class WheelchairController {
private:
    ros::NodeHandle nh_;
    ControllerHandler controller_handler_;
    JoyUtility joy_utility_;
    ros::Subscriber cmd_vel_sub_;
    ros::Subscriber modality_sub_;
    sensor_msgs::Joy joy_msg;
    bool first_cmd_vel;

public:
    /**
     * @brief Constructor for the wheelchair controller.
     * @param nh ROS node handle for creating subscribers.
     */
    WheelchairController(ros::NodeHandle& nh);

    /**
     * @brief Destructor for the wheelchair controller.
     */
    ~WheelchairController();

    /**
     * @brief Callback function for velocity command messages.
     * @param msg Pointer to the received velocity message.
     */
    void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& msg);

    void run();
    
    /**
     * @brief Callback function for modality command messages.
     * @param msg Pointer to the received modality message.
     */
    void modalityCallback(const std_msgs::Int8::ConstPtr& msg);
};

#endif // WHEELCHAIRCONTROLLER_HPP

