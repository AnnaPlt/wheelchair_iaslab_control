#include "JoyUtility.hpp"
#include <cmath>

JoyUtility::JoyUtility() : current_profile_(0), current_speed_percentage_(0.0f) // Default to 0%
{
    // Initialize speed mappings
    speed_mappings_ = {
        {5, 0.138889f},
        {10, 0.222222f},
        {15, 0.333333f},
        {20, 0.416667f},
        {25, 0.527778f},
        {30, 0.638889f},
        {35, 0.722222f},
        {40, 0.833333f},
        {45, 0.944444f},
        {50, 1.05556f},
        {55, 1.13889f},
        {60, 1.25f},
        {65, 1.33333f},
        {70, 1.44444f},
        {75, 1.55556f},
        {80, 1.66667f},
        {85, 1.75f},
        {90, 1.86111f},
        {95, 1.94444f},
        {100, 2.0f}
    };
}

void JoyUtility::initFromParams(ros::NodeHandle& nh)
{
    // Get current profile from parameter server
    nh.param("current_profile", current_profile_, 1);
    
    // Load speed levels for current profile
    loadProfileLevels(nh, current_profile_);
}

void JoyUtility::loadProfileLevels(ros::NodeHandle& nh, int profile)
{
    std::string profile_name = "profiles/profile" + std::to_string(profile);
    
    // Load speed levels for the profile
    nh.param(profile_name + "/max_forward_level", current_levels_.max_forward, 100);
    nh.param(profile_name + "/min_forward_level", current_levels_.min_forward, 25);
    nh.param(profile_name + "/max_backward_level", current_levels_.max_backward, 25);
    nh.param(profile_name + "/min_backward_level", current_levels_.min_backward, 20);
    nh.param(profile_name + "/max_turning_level", current_levels_.max_turning, 20);
    nh.param(profile_name + "/min_turning_level", current_levels_.min_turning, 20);
}

void JoyUtility::setSpeedPercentage(float percentage)
{
    // Clamp between 0% and 100%
    if(percentage > 100.0f || percentage < 0.0f){
        ROS_WARN("Speed percentage %f out of range (0-100)", percentage);
        return;
    }
    current_speed_percentage_ = percentage;
    ROS_INFO("Speed percentage set to: %f", current_speed_percentage_);
}

geometry_msgs::Twist JoyUtility::joyToVelocity(const sensor_msgs::Joy::ConstPtr& joy_msg)
{
    geometry_msgs::Twist twist;
    
    float linear_x = joy_msg->axes[1]; // -1 to 1
    float angular_z = joy_msg->axes[0]; // -1 to 1
    
    //ROS_INFO("Input joystick values - linear: %f, angular: %f", linear_x, angular_z);
    
    // Calculate speed factor based on current percentage (20% -> 0.2, 100% -> 1.0)
    float speed_factor = current_speed_percentage_ / 100.0f;
    //ROS_INFO("Current speed percentage: %f, speed factor: %f", current_speed_percentage_, speed_factor);
    
    // Apply speed limits based on direction
    if (linear_x > 0) {
        // Forward movement
        float min_speed = speed_mappings_[current_levels_.min_forward];
        float max_speed = speed_mappings_[current_levels_.max_forward];
        //ROS_INFO("Forward speeds - min: %f, max: %f", min_speed, max_speed);
        
        // Calculate the current maximum speed based on speed percentage
        float current_max_speed = min_speed + (max_speed - min_speed) * speed_factor;
        //ROS_INFO("Current max forward speed: %f", current_max_speed);
        
        // Calculate final speed based on joystick position
        float speed = current_max_speed * linear_x;
        twist.linear.x = linear_x;//speed;
    } else {
        // Backward movement
        float min_speed = speed_mappings_[current_levels_.min_backward];
        float max_speed = speed_mappings_[current_levels_.max_backward];
        //ROS_INFO("Backward speeds - min: %f, max: %f", min_speed, max_speed);
        
        // Calculate the current maximum speed based on speed percentage
        float current_max_speed = min_speed + (max_speed - min_speed) * speed_factor;
        //ROS_INFO("Current max backward speed: %f", current_max_speed);
        
        // Calculate final speed based on joystick position
        float speed = current_max_speed * linear_x;
        twist.linear.x = linear_x; //speed;
    }
    
    // Apply turning limits
    float min_turning = speed_mappings_[current_levels_.min_turning];
    float max_turning = speed_mappings_[current_levels_.max_turning];
    //ROS_INFO("Turning speeds - min: %f, max: %f", min_turning, max_turning);
    
    // Calculate the current maximum turning speed based on speed percentage
    float current_max_turning = min_turning + (max_turning - min_turning) * speed_factor;
    //ROS_INFO("Current max turning speed: %f", current_max_turning);
    
    // Calculate final turnin speed based on joystick position
    float turning_speed = current_max_turning * angular_z;
    twist.angular.z = angular_z;// turning_speed / radius;
    
    //ROS_INFO("Output twist - linear: %f, angular: %f", twist.linear.x, twist.angular.z);
    
    return twist;
}

sensor_msgs::Joy JoyUtility::velocityToJoy(const geometry_msgs::Twist& twist)
{
    sensor_msgs::Joy joy_msg;
    joy_msg.axes.resize(2);
    
    //ROS_INFO("Input twist - linear: %f, angular: %f", twist.linear.x, twist.angular.z);
    
    float speed_factor = current_speed_percentage_ / 100.0f;
    //ROS_INFO("Current speed percentage: %f, speed factor: %f", current_speed_percentage_, speed_factor);
    
    /*Convert linear velocity to joystick value
    if (twist.linear.x > 0) {
        float min_speed = speed_mappings_[current_levels_.min_forward];
        float max_speed = speed_mappings_[current_levels_.max_forward];
        float current_max_speed = min_speed + (max_speed - min_speed) * speed_factor;
         // * current_max_speed;
        //ROS_INFO("Forward conversion - min: %f, max: %f, current_max: %f, result: %f", 
                //min_speed, max_speed, current_max_speed, joy_msg.axes[1]);
    } else {
        float min_speed = speed_mappings_[current_levels_.min_backward];
        float max_speed = speed_mappings_[current_levels_.max_backward];
        float current_max_speed = min_speed + (max_speed - min_speed) * speed_factor;
        joy_msg.axes[1] = twist.linear.x; //* current_max_speed;
        //ROS_INFO("Backward conversion - min: %f, max: %f, current_max: %f, result: %f", 
                //min_speed, max_speed, current_max_speed, joy_msg.axes[1]);
    }*/
    joy_msg.axes[1] = twist.linear.x;
    /*Convert angular velocity to joystick value
    float min_turning = speed_mappings_[current_levels_.min_turning];
    float max_turning = speed_mappings_[current_levels_.max_turning];
    float current_max_turning = min_turning + (max_turning - min_turning) * speed_factor;*/
    joy_msg.axes[0] = twist.angular.z; //* radius) * current_max_turning;
    //ROS_INFO("Turning conversion - min: %f, max: %f, current_max: %f, result: %f", 
        //min_turning, max_turning, current_max_turning, joy_msg.axes[0]);
        
    return joy_msg;
}

void JoyUtility::setProfile(int profile)
{
    // Make profile circular (0-4)
    if (profile < 0) {
        current_profile_ = 4;
    } else if (profile > 4) {
        current_profile_ = 0;
    } else {
        current_profile_ = profile;
    }
    
    ROS_INFO("Setting profile to: %d", current_profile_);
    
    // Load new profile levels
    ros::NodeHandle nh;
    loadProfileLevels(nh, current_profile_);
}

void JoyUtility::increaseProfile()
{
    setProfile(current_profile_ + 1);
}

void JoyUtility::decreaseProfile()
{
    setProfile(current_profile_ - 1);
}

void JoyUtility::increaseSpeed()
{
    setSpeedPercentage(current_speed_percentage_ + 10.0f);
}

void JoyUtility::decreaseSpeed()
{
    setSpeedPercentage(current_speed_percentage_ - 10.0f);
}


