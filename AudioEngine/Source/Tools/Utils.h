// ©2023 JDSherbert. All rights reserved.

#pragma once

class Utils
{

public:

    /** Convert Decibels to linear volume */
    static float ConvertdBToVolume(float dB);

    /** Convert linear volume to Decibels.*/
    static float ConvertVolumeTodB(float volume);
};
