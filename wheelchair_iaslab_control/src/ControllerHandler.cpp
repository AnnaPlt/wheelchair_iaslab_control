#include "../include/ControllerHandler.hpp"

ControllerHandler::ControllerHandler() : can_handler(0) {
    joystick_x = 0x00;
    joystick_y = 0x00;

    speed = 0;

    is_jailbreak_mode = false;

    profile = 0;
    setProfile(profile);

    injectRnetJoystickFrame();
    injectRNETjailbreakFrame();
    
    setSpeed(speed);
    
}

//destructor
ControllerHandler::~ControllerHandler() {
    disableJailbreakMode();
}

void ControllerHandler::injectRnetJoystickFrame() {
    std::stringstream ss;
    ss << RNET_JOYSTICK_ID << "#";
    ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(joystick_x);
    ss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(joystick_y);
    
    std::string canStr = ss.str();
        
    // Send the frame
    if (can_handler.sendFrame(canStr)) {
    } else {
        std::cerr << "Failed to send CAN frame" << std::endl;
    }
}

void ControllerHandler::setJoystick(float x, float y){
    
    float x_axis = x;  // Left stick horizontal (-1 to 1)
    float y_axis = y;  // Left stick vertical (-1 to 1)
    
    int16_t x_value = static_cast<int16_t>(x_axis * 32767.0);
    int16_t y_value = static_cast<int16_t>(y_axis * 32767.0);

    uint16_t new_x;
    uint16_t new_y;

    // Process X axis
    if(std::abs(x_value) > X_THRESHOLD){
        int16_t inverted_x = -x_value;
        new_x = (0x100 + static_cast<int>(inverted_x * 100.0 / 128.0)) >> 8 & 0xFF;
    }
    
    else{
        new_x = 0;
    }

    // Process Y axis 
    if(std::abs(y_value) > Y_THRESHOLD){

        int16_t inverted_y = -y_value;
        new_y = (0x100 - static_cast<int>(inverted_y * 100.0 / 128.0)) >> 8 & 0xFF;
    }
    else{
        new_y = 0;
    }

    joystick_x = new_x;
    joystick_y = new_y;

    // Inject the frame
    injectRnetJoystickFrame();
}

void ControllerHandler::increaseProfile(){
    uint8_t prev_profile = this->profile;
    uint8_t new_profile;

    if(prev_profile == numProfiles - 1){
        new_profile = 0;
    } else {
        new_profile = prev_profile + 1;
    }
    this->profile = new_profile;

    setProfile(new_profile);
}

void ControllerHandler::decreaseProfile(){
    uint8_t prev_profile = this->profile;
    uint8_t new_profile;

    if(prev_profile == 0){
        new_profile = numProfiles - 1;
    } else {
        new_profile = prev_profile - 1;
    }

    this->profile = new_profile;

    setProfile(new_profile);
}

void ControllerHandler::setProfile(uint8_t profile){
     
     if(profile < numProfiles && profile >= 0){
        this->profile = profile;
        std::stringstream ss;
        ss << "051#000" << std::setfill('0') << std::setw(1) << std::hex << static_cast<int>(this->profile) << "0000";
        std::cout << "Sending CAN frame: " << ss.str() << std::endl;
        can_handler.sendFrame(ss.str());
     }
     else{
        std::cerr << "Invalid profile: " << profile << std::endl;
     }
}

void ControllerHandler::setSpeed(uint16_t s){
    if(s > 100 || s < 0){
        return;
    }
    std::stringstream ss;
    ss << RNET_SPEED_ID << "#" << std::setfill('0') << std::setw(2) << std::hex << s;
    can_handler.sendFrame(ss.str());
}

void ControllerHandler::setHorn(){
    can_handler.sendFrame("0C040100#");
}

void ControllerHandler::disableHorn(){
    can_handler.sendFrame("0C040101#");
}

void ControllerHandler::injectRNETjailbreakFrame() {

    for(int i = 0; i < 3; i++)
    {
        can_handler.sendFrame("0c000000#");
    }

    is_jailbreak_mode = true;
    can_handler.sendFrame("181c0100#0260000000000000");
}

void ControllerHandler::disableJailbreakMode(){
    is_jailbreak_mode = false;
    this->profile = 0;
    std::stringstream ss;
    
    ss << "051#000" << std::setfill('0') << std::setw(1) << std::hex << this->profile << "0000";

    can_handler.sendFrame(ss.str());
    
    can_handler.sendFrame("181c0100#0260000000000000");
    can_handler.sendFrame("000#R");
}

void ControllerHandler::increaseSpeed() {

    if(speed >= 100){
        return;
    }
    speed += 10;
    std::stringstream ss;
    ss << RNET_SPEED_ID << "#" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(speed);
    can_handler.sendFrame(ss.str());
}

void ControllerHandler::decreaseSpeed() {
    if(speed <= 0){
        return;
    }
    speed -= 10;
    std::stringstream ss;
    ss << RNET_SPEED_ID << "#" << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(speed);
    can_handler.sendFrame(ss.str());
}

const std::string& ControllerHandler::getCANInterface() const {
    return CAN_INTERFACE;
}

const std::string& ControllerHandler::getRnetJoystickId() const {
    return RNET_JOYSTICK_ID;
}

const std::string& ControllerHandler::getRnetSpeedId() const {
    return RNET_SPEED_ID;
}

uint16_t ControllerHandler::getJoystickX() const {
    return joystick_x;
}

uint16_t ControllerHandler::getJoystickY() const {
    return joystick_y;
}

uint16_t ControllerHandler::getSpeed() const {
    return speed;
}
