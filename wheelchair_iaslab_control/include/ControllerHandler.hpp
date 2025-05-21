#ifndef CONTROLLERHANDLER_HPP
#define CONTROLLERHANDLER_HPP

#include <iomanip>
#include <functional>
#include "CANHandler.hpp"
#include <string>

/**
 * @class ControllerHandler
 * @brief A class to handle wheelchair controller operations.
 *
 * This class provides methods to control a wheelchair via CAN communication,
 * including joystick input handling, speed control, and mode management.
 */
class ControllerHandler {
private:

    const std::string CAN_INTERFACE = "can0";       ///< CAN interface name
    const std::string RNET_JOYSTICK_ID = "02000000"; ///< Static ID for joystick frame
    const std::string RNET_SPEED_ID = "0A040100";   ///< ID for speed frame
    const int numModes = 4;                         ///< Number of available modes
    const int numProfiles = 5;                      ///< Number of available profiles

    CANHandler can_handler;                         ///< Handler for CAN communication
    uint16_t joystick_x;                            ///< X-axis joystick value
    uint16_t joystick_y;                            ///< Y-axis joystick value

    const int X_THRESHOLD = 8 * 0x10000 / 128;      ///< Threshold for X-axis input
    const int Y_THRESHOLD = 8 * 0x10000 / 128;      ///< Threshold for Y-axis input

    uint16_t speed;                                 ///< Current speed setting

    bool is_jailbreak_mode;                         ///< Flag for jailbreak mode status

    uint8_t profile;                                ///< Current profile setting

public:
    /**
     * @brief Default constructor.
     */
    ControllerHandler();
    
    /**
     * @brief Destructor.
     */
    ~ControllerHandler();

    /**
     * @brief Injects a joystick frame into the RNET system.
     */
    void injectRnetJoystickFrame();
    
    /**
     * @brief Injects a jailbreak frame into the RNET system.
     */
    void injectRNETjailbreakFrame();
    
    /**
     * @brief Increases the wheelchair speed.
     */
    void increaseSpeed();
    
    /**
     * @brief Decreases the wheelchair speed.
     */
    void decreaseSpeed();

    /**
     * @brief Sets the speed.
     * @param s The speed value to set (0-100)
     */

    void setHorn();

    /**
     * @brief Disables the horn.
     */
    void disableHorn();

    /**
     * @brief Gets the CAN interface name.
     * @return The CAN interface name.
     */
    const std::string& getCANInterface() const;
    
    /**
     * @brief Gets the RNET joystick ID.
     * @return The RNET joystick ID.
     */
    const std::string& getRnetJoystickId() const;
    
    /**
     * @brief Gets the RNET speed ID.
     * @return The RNET speed ID.
     */
    const std::string& getRnetSpeedId() const;

    /**
     * @brief Gets the current X-axis joystick value.
     * @return The X-axis joystick value.
     */
    uint16_t getJoystickX() const;
    
    /**
     * @brief Gets the current Y-axis joystick value.
     * @return The Y-axis joystick value.
     */
    uint16_t getJoystickY() const;
    
    /**
     * @brief Gets the current speed setting.
     * @return The current speed value.
     */
    uint16_t getSpeed() const;

    /**
     * @brief Gets the jailbreak mode status.
     * @return True if jailbreak mode is enabled, false otherwise.
     */
    bool getIsJailbreakMode() const;

    /**
     * @brief Sets the joystick position.
     * @param x The X-axis value (-1.0 to 1.0).
     * @param y The Y-axis value (-1.0 to 1.0).
     */
    void setJoystick(float x, float y);
    
    /**
     * @brief Sets the speed value.
     * @param s The speed value to set.
     */
    void setSpeed(uint16_t s);
    
    /**
     * @brief Sets the last button press time.
     * @param time The time of the last button press.
     */

    void setIsJailbreakMode(bool mode);
    
    /**
     * @brief Disables the jailbreak mode.
     */
    void disableJailbreakMode();
    
    /**
     * @brief Sets the wheelchair profile.
     * @param profile True to increase profile, false to decrease.
     */
    void setProfile(uint8_t profile);

    /**
     * @brief Increases the wheelchair profile.
     */
    void increaseProfile();
    
    /**
     * @brief Decreases the wheelchair profile.
     */
    void decreaseProfile();

    /**
     * @brief Sets the RNET mode.
     * @param mode True to increase mode, false to decrease.
     */
    void rnetSetMode(bool mode);
    
    /**
     * @brief Removes the RNET mode.
     * @param mode True to increase mode, false to decrease.
     */
    void rnetRemoveMode(bool mode);
};

#endif // CONTROLLERHANDLER_HPP
