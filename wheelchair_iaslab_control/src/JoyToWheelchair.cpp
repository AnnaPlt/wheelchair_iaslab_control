#include "../include/JoyToWheelchair.hpp"

JoyToWheelchair::JoyToWheelchair(ros::NodeHandle& nh) : nh_(nh) {
    // Initialize publishers and subscribers
    joy_sub_ = nh_.subscribe("/joy", 10, &JoyToWheelchair::joyCallback, this);
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    modality_pub_ = nh_.advertise<std_msgs::Int8>("/wheelchair_modality", 10);
    
    // Initialize JoyUtility with parameters
    joy_utility_.initFromParams(nh_);
    
    last_button_time_ = ros::Time::now();
    
    ROS_INFO("JoyToWheelchair node initialized");
}

JoyToWheelchair::~JoyToWheelchair() {
    // Publish zero velocity when shutting down
    geometry_msgs::Twist zero_vel;
    zero_vel.linear.x = 0;
    zero_vel.angular.z = 0;
    cmd_vel_pub_.publish(zero_vel);
}

void JoyToWheelchair::joyCallback(const sensor_msgs::Joy::ConstPtr& msg) {
    // Process button presses for modality changes
    if (ros::Time::now() - last_button_time_ > button_delay_) {
        bool modality_changed = false;
        
        // Profile up button
        if (msg->buttons[PROFILE_UP_BUTTON] == 1) {
            ROS_INFO("Profile up command");
            std_msgs::Int8 modality_msg;
            modality_msg.data = 1; // 1 means increase profile
            joy_utility_.increaseProfile();
            modality_pub_.publish(modality_msg);
            modality_changed = true;
        }
        // Profile down button
        else if (msg->buttons[PROFILE_DOWN_BUTTON] == 1) {
            ROS_INFO("Profile down command");
            std_msgs::Int8 modality_msg;
            modality_msg.data = 2; // 2 means decrease profile
            joy_utility_.decreaseProfile();
            modality_pub_.publish(modality_msg);
            modality_changed = true;
        }
        // Speed up button
        else if (msg->buttons[SPEED_UP_BUTTON] == 1) {
            ROS_INFO("Speed up command");
            std_msgs::Int8 modality_msg;
            modality_msg.data = 3; // 3 means increase speed
            joy_utility_.increaseSpeed();
            modality_pub_.publish(modality_msg);
            modality_changed = true;
        }
        // Speed down button
        else if (msg->buttons[SPEED_DOWN_BUTTON] == 1) {
            ROS_INFO("Speed down command");
            std_msgs::Int8 modality_msg;
            modality_msg.data = 4; // 4 means decrease speed
            joy_utility_.decreaseSpeed();
            modality_pub_.publish(modality_msg);
            modality_changed = true;
        }
        // Disable button
        else if (msg->buttons[DISABLE_BUTTON] == 1) {
            ROS_INFO("Disable command");
            std_msgs::Int8 modality_msg;
            modality_msg.data = 0; // 0 means disable
            modality_pub_.publish(modality_msg);
            modality_changed = true;
        }
        
        if (modality_changed) {
            last_button_time_ = ros::Time::now();
        }
    }
    
    // Convert joystick input to velocity using JoyUtility
    geometry_msgs::Twist cmd_vel = joy_utility_.joyToVelocity(msg);
    
    // Publish velocity command
    cmd_vel_pub_.publish(cmd_vel);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "joy_to_wheelchair");
    ros::NodeHandle nh;
    
    try {
        JoyToWheelchair joy_to_wheelchair(nh);
        
        ROS_INFO("JoyToWheelchair node started. Converting joystick input to wheelchair commands...");
        
        // Set the loop rate to 100Hz
        ros::Rate rate(100);
        
        while (ros::ok()) {
            ros::spinOnce();
            rate.sleep();
        }
    } catch (const std::exception& e) {
        ROS_ERROR("Exception in JoyToWheelchair main: %s", e.what());
    } catch (...) {
        ROS_ERROR("Unknown exception in JoyToWheelchair main");
    }
    
    return 0;
} 
