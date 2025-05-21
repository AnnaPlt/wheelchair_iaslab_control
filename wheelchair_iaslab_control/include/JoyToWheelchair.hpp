#ifndef JOY_TO_WHEELCHAIR_HPP
#define JOY_TO_WHEELCHAIR_HPP

#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>
#include "JoyUtility.hpp"

/**
 * @class JoyToWheelchair
 * @brief A class to convert joystick input to wheelchair commands.
 *
 * This class subscribes to joystick messages and publishes velocity commands
 * and wheelchair modality settings.
 */
class JoyToWheelchair {
private:
    ros::NodeHandle nh_;
    ros::Subscriber joy_sub_;
    ros::Publisher cmd_vel_pub_;
    ros::Publisher modality_pub_;
    
    JoyUtility joy_utility_;  // Add JoyUtility member
    
    ros::Time last_button_time_;
    ros::Duration button_delay_ = ros::Duration(0.2); // in seconds
    
    int current_modality_ = 0;
    
    // Constants for button mapping
    const int PROFILE_UP_BUTTON = 7;
    const int PROFILE_DOWN_BUTTON = 6;
    const int SPEED_UP_BUTTON = 1;
    const int SPEED_DOWN_BUTTON = 2;
    const int DISABLE_BUTTON = 4;

public:
    /**
     * @brief Constructor for the JoyToWheelchair converter.
     * @param nh ROS node handle for creating subscribers and publishers.
     */
    JoyToWheelchair(ros::NodeHandle& nh);
    
    /**
     * @brief Destructor for the JoyToWheelchair converter.
     */
    ~JoyToWheelchair();
    
    /**
     * @brief Callback function for joystick messages.
     * @param msg Pointer to the received joystick message.
     */
    void joyCallback(const sensor_msgs::Joy::ConstPtr& msg);
};

#endif // JOY_TO_WHEELCHAIR_HPP 
