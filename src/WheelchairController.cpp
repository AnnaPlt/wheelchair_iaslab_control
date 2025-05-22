#include "../include/WheelchairController.hpp"

WheelchairController::WheelchairController(ros::NodeHandle& nh) : nh_(nh) {
    // Subscribe to command velocity and modality topics
    cmd_vel_sub_ = nh_.subscribe("/cmd_vel", 10, &WheelchairController::cmdVelCallback, this);
    modality_sub_ = nh_.subscribe("/wheelchair_modality", 10, &WheelchairController::modalityCallback, this);
    
    // Initialize JoyUtility with parameters
    joy_utility_.initFromParams(nh_);
    first_cmd_vel = false;
    
    ROS_INFO("WheelchairController initialized");
}

WheelchairController::~WheelchairController() {
    // Ensure the wheelchair is in a safe state when shutting down
    controller_handler_.disableJailbreakMode();
}

void WheelchairController::cmdVelCallback(const geometry_msgs::Twist::ConstPtr& msg) {
    //ROS_INFO("Received command velocity");
    // Convert velocity to joystick values using JoyUtility
    joy_msg = joy_utility_.velocityToJoy(*msg);
    ROS_INFO("Sending command velocity: linear: %f, angular: %f", joy_msg.axes[1], joy_msg.axes[0]);
    first_cmd_vel = true;
}

void WheelchairController::modalityCallback(const std_msgs::Int8::ConstPtr& msg) {
    switch(msg->data) {
        case 0: // Disable
            ROS_INFO("Disabling jailbreak mode");
            controller_handler_.disableJailbreakMode();
            break;
        case 1: // Increase profile
            ROS_INFO("Increasing profile");
            joy_utility_.increaseProfile();
            controller_handler_.increaseProfile();
            break;
        case 2: // Decrease profile
            ROS_INFO("Decreasing profile");
            joy_utility_.decreaseProfile();
            controller_handler_.decreaseProfile();
            break;
        case 3: // Increase speed
            ROS_INFO("Increasing speed");
            joy_utility_.increaseSpeed();
            controller_handler_.increaseSpeed();
            break;
        case 4: // Decrease speed
            ROS_INFO("Decreasing speed");
            joy_utility_.decreaseSpeed();
            controller_handler_.decreaseSpeed();
            break;
        default:
            ROS_WARN("Unknown modality command: %d", msg->data);
            break;
    }
}


void WheelchairController::run(){
    ros::Rate rate(150);

    while (ros::ok()) {
        if(first_cmd_vel){
            controller_handler_.setJoystick(joy_msg.axes[0], joy_msg.axes[1]);
        }
        ros::spinOnce();
        rate.sleep();
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "wheelchair_controller");
    ros::NodeHandle nh;

    try {
        WheelchairController wheelchair_controller(nh);
        ROS_INFO("Wheelchair controller started. Listening for velocity and modality commands...");
        wheelchair_controller.run();

    } catch (const std::exception& e) {
        ROS_ERROR("Exception in main: %s", e.what());
    } catch (...) {
        ROS_ERROR("Unknown exception in main");
    }

    return 0;
}
