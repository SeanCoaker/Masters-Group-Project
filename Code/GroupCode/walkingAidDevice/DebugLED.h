/**
 * @file DebugLED.h
 * @author Sean Coaker (seancoaker@gmail.com)
 * @brief A class to handle the displaying of certain LED configs based on the state of the device.
 * @version 1.0
 * @date 12-04-2022
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "TinyPICO.h"

class DebugLED {

private:

    //! The TinyPICO object used to control the LEDs.
    TinyPICO tp;

public:

    /**
     * @brief Construct a new DebugLED object.
     * 
     */
    DebugLED();

    /**
     * @brief Displays a green LED for 1 second to signify successful power on.
     * 
     */
    void poweredOn();

    /**
     * @brief Displays a red LED to signify ESP-Now init error.
     * 
     */
    void initError();

    /**
     * @brief Displays an orange LED to signify an ESP-Now peer connection error.
     * 
     */
    void peerSetupError();
};