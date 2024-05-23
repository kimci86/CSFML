////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Audio/SoundBuffer.h>
#include <SFML/Audio/SoundBufferStruct.h>
#include <SFML/CallbackStream.h>
#include <SFML/Internal.h>


////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_createFromFile(const char* filename)
{
    auto soundBuffer = sf::SoundBuffer::loadFromFile(filename);
    if (!soundBuffer)
        return nullptr;

    return new sfSoundBuffer{std::move(*soundBuffer)};
}


////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_createFromMemory(const void* data, size_t sizeInBytes)
{
    auto soundBuffer = sf::SoundBuffer::loadFromMemory(data, sizeInBytes);
    if (!soundBuffer)
        return nullptr;

    return new sfSoundBuffer{std::move(*soundBuffer)};
}



////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_createFromStream(sfInputStream* stream)
{
    CSFML_CHECK_RETURN(stream, nullptr);

    CallbackStream sfmlStream(stream);
    auto soundBuffer = sf::SoundBuffer::loadFromStream(sfmlStream);
    if (!soundBuffer)
        return nullptr;

    return new sfSoundBuffer{std::move(*soundBuffer)};
}


////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_createFromSamples(const int16_t*    samples,
                                               uint64_t          sampleCount,
                                               unsigned int      channelCount,
                                               unsigned int      sampleRate,
                                               sfSoundChannel*   channelMapData,
                                               size_t            channelMapSize)
{
    std::vector<sf::SoundChannel> channelMap(channelMapSize);
    for (std::size_t i = 0; i < channelMap.size(); ++i)
        channelMap[i] = static_cast<sf::SoundChannel>(channelMapData[i]);

    auto soundBuffer = sf::SoundBuffer::loadFromSamples(samples, sampleCount, channelCount, sampleRate, channelMap);
    if (!soundBuffer)
        return nullptr;

    return new sfSoundBuffer{std::move(*soundBuffer)};
}


////////////////////////////////////////////////////////////
sfSoundBuffer* sfSoundBuffer_copy(const sfSoundBuffer* soundBuffer)
{
    CSFML_CHECK_RETURN(soundBuffer, nullptr);

    return new sfSoundBuffer(*soundBuffer);
}


////////////////////////////////////////////////////////////
void sfSoundBuffer_destroy(sfSoundBuffer* soundBuffer)
{
    delete soundBuffer;
}


////////////////////////////////////////////////////////////
bool sfSoundBuffer_saveToFile(const sfSoundBuffer* soundBuffer, const char* filename)
{
    CSFML_CALL_RETURN(soundBuffer, saveToFile(filename), false);
}


////////////////////////////////////////////////////////////
const int16_t* sfSoundBuffer_getSamples(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, getSamples(), nullptr);
}


////////////////////////////////////////////////////////////
uint64_t sfSoundBuffer_getSampleCount(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, getSampleCount(), 0);
}


////////////////////////////////////////////////////////////
unsigned int sfSoundBuffer_getSampleRate(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, getSampleRate(), 0);
}


////////////////////////////////////////////////////////////
unsigned int sfSoundBuffer_getChannelCount(const sfSoundBuffer* soundBuffer)
{
    CSFML_CALL_RETURN(soundBuffer, getChannelCount(), 0);
}


////////////////////////////////////////////////////////////
sfTime sfSoundBuffer_getDuration(const sfSoundBuffer* soundBuffer)
{
    sfTime time = {0};
    CSFML_CHECK_RETURN(soundBuffer, time);

    time.microseconds = soundBuffer->This.getDuration().asMicroseconds();
    return time;
}
