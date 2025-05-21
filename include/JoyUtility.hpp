#ifndef JOY_UTILITY_H
#define JOY_UTILITY_H

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <map>
#include <string>

/**
 * @class JoyUtility
 * @brief A utility class for handling joystick input and converting it to wheelchair commands.
 *
 * This class provides functionality to convert between joystick input and velocity commands,
 * manage speed profiles, and handle speed percentage settings for wheelchair control.
 */
class JoyUtility {
public:
    /**
     * @brief Default constructor.
     */
    JoyUtility();
    
    /**
     * @brief Default destructor.
     */
    ~JoyUtility() = default;

    /**
     * @brief Initialize the utility with parameters from ROS parameter server.
     * @param nh ROS node handle for accessing parameters.
     */
    void initFromParams(ros::NodeHandle& nh);

    /**
     * @brief Convert joystick input to velocity command.
     * @param joy_msg Pointer to the joystick message.
     * @return The corresponding velocity command.
     */
    geometry_msgs::Twist joyToVelocity(const sensor_msgs::Joy::ConstPtr& joy_msg);

    /**
     * @brief Convert velocity command to joystick values (for visualization/debugging).
     * @param twist The velocity command to convert.
     * @return The corresponding joystick message.
     */
    sensor_msgs::Joy velocityToJoy(const geometry_msgs::Twist& twist);

    /**
     * @brief Set the current profile (0-4).
     * @param profile The profile number to set.
     */
    void setProfile(int profile);

    /**
     * @brief Get the current profile number.
     * @return The current profile number.
     */
    int getCurrentProfile() const { return current_profile_; }

    /**
     * @brief Set the speed percentage (0-100).
     * @param percentage The speed percentage to set.
     */
    void setSpeedPercentage(float percentage);

    /**
     * @brief Increase the current profile number.
     */
    void increaseProfile();
    
    /**
     * @brief Decrease the current profile number.
     */
    void decreaseProfile();

    /**
     * @brief Increase the current speed percentage.
     */
    void increaseSpeed();
    
    /**
     * @brief Decrease the current speed percentage.
     */
    void decreaseSpeed();
    
    /**
     * @brief Get the current speed percentage.
     * @return The current speed percentage.
     */
    float getCurrentSpeedPercentage() const { return current_speed_percentage_; }

private:
    std::map<int, float> speed_mappings_; ///< Mapping of speed levels to actual speeds in m/s
    int current_profile_; ///< Current profile number (0-4)
    float current_speed_percentage_; ///< Current speed percentage (0-100)
    const float radius = 0.28f;

    /**
     * @brief Structure to hold speed level settings for a profile.
     */
    struct SpeedLevels {
        int max_forward; ///< Maximum forward speed level
        int min_forward; ///< Minimum forward speed level
        int max_backward; ///< Maximum backward speed level
        int min_backward; ///< Minimum backward speed level
        int max_turning; ///< Maximum turning speed level
        int min_turning; ///< Minimum turning speed level
    } current_levels_;

    /**
     * @brief Load speed levels for a specific profile from ROS parameters.
     * @param nh ROS node handle for accessing parameters.
     * @param profile The profile number to load settings for.
     */
    void loadProfileLevels(ros::NodeHandle& nh, int profile);
};

#endif // JOY_UTILITY_H 
