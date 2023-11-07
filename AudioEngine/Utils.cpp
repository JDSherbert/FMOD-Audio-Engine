#include "Utils.h"

float Utils::dBToVolume(float dB)
{
    return powf(10.0f, 0.05f * dB);
}

float Utils::VolumeTodB(float volume)
{
    return 20.0f * log10f(volume);
}
