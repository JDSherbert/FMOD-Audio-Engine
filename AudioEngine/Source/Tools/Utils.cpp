// ©2023 JDSherbert. All rights reserved.

#include "Utils.h"

#include <math.h>

float Utils::ConvertdBToVolume(float dB)
{
    return powf(10.0f, 0.05f * dB);
}

float Utils::ConvertVolumeTodB(float volume)
{
    return 20.0f * log10f(volume);
}
