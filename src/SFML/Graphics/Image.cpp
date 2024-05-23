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
#include <SFML/Graphics/Image.h>
#include <SFML/Graphics/ImageStruct.h>
#include <SFML/System/BufferStruct.h>
#include <SFML/Internal.h>
#include <SFML/CallbackStream.h>


////////////////////////////////////////////////////////////
sfImage* sfImage_create(unsigned int width, unsigned int height)
{
    return new sfImage{sf::Image({width, height})};
}


////////////////////////////////////////////////////////////
sfImage* sfImage_createFromColor(unsigned int width, unsigned int height, sfColor color)
{
    return new sfImage{sf::Image({ width, height }, sf::Color(color.r, color.g, color.b, color.a))};
}


////////////////////////////////////////////////////////////
sfImage* sfImage_createFromPixels(unsigned int width, unsigned int height, const uint8_t* data)
{
    return new sfImage{sf::Image({ width, height }, data)};
}


////////////////////////////////////////////////////////////
sfImage* sfImage_createFromFile(const char* filename)
{
    auto image = sf::Image::loadFromFile(filename);
    if (!image)
        return nullptr;

    return new sfImage{std::move(*image)};
}


////////////////////////////////////////////////////////////
sfImage* sfImage_createFromMemory(const void* data, size_t sizeInBytes)
{
    auto image = sf::Image::loadFromMemory(data, sizeInBytes);
    if (!image)
        return nullptr;

    return new sfImage{std::move(*image)};
}


////////////////////////////////////////////////////////////
sfImage* sfImage_createFromStream(sfInputStream* stream)
{
    CSFML_CHECK_RETURN(stream, nullptr);

    CallbackStream sfmlStream(stream);
    auto image = sf::Image::loadFromStream(sfmlStream);
    if (!image)
        return nullptr;

    return new sfImage{std::move(*image)};
}


////////////////////////////////////////////////////////////
sfImage* sfImage_copy(const sfImage* image)
{
    CSFML_CHECK_RETURN(image, nullptr);

    return new sfImage(*image);
}


////////////////////////////////////////////////////////////
void sfImage_destroy(sfImage* image)
{
    delete image;
}


////////////////////////////////////////////////////////////
bool sfImage_saveToFile(const sfImage* image, const char* filename)
{
    CSFML_CALL_RETURN(image, saveToFile(filename), false);
}


////////////////////////////////////////////////////////////
bool sfImage_saveToMemory(const sfImage* image, sfBuffer* output, const char* format)
{
    CSFML_CHECK_RETURN(output, false);
    CSFML_CHECK_RETURN(image, false);

    auto data = image->This.saveToMemory(format);

    if (data) 
    {
        output->buffer = std::move(*data);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////
void sfImage_createMaskFromColor(sfImage* image, sfColor colorKey, uint8_t alpha)
{
    CSFML_CALL(image, createMaskFromColor(sf::Color(colorKey.r, colorKey.g, colorKey.b, colorKey.a), alpha));
}


////////////////////////////////////////////////////////////
bool sfImage_copyImage(sfImage* image, const sfImage* source, unsigned int destX, unsigned int destY, sfIntRect sourceRect, bool applyAlpha)
{
    CSFML_CHECK_RETURN(image, false);
    CSFML_CHECK_RETURN(source, false);

    sf::IntRect sfmlRect({ sourceRect.left, sourceRect.top }, { sourceRect.width, sourceRect.height });

    return image->This.copy(source->This, { destX, destY }, sfmlRect, applyAlpha);
}


////////////////////////////////////////////////////////////
void sfImage_setPixel(sfImage* image, unsigned int x, unsigned int y, sfColor color)
{
    CSFML_CALL(image, setPixel({ x, y }, sf::Color(color.r, color.g, color.b, color.a)));
}


////////////////////////////////////////////////////////////
sfColor sfImage_getPixel(const sfImage* image, unsigned int x, unsigned int y)
{
    sfColor color = {0, 0, 0, 0};
    CSFML_CHECK_RETURN(image, color);

    sf::Color sfmlColor = image->This.getPixel({ x, y });

    return sfColor_fromRGBA(sfmlColor.r, sfmlColor.g, sfmlColor.b, sfmlColor.a);
}


////////////////////////////////////////////////////////////
const uint8_t* sfImage_getPixelsPtr(const sfImage* image)
{
    CSFML_CALL_RETURN(image, getPixelsPtr(), nullptr);
}


////////////////////////////////////////////////////////////
sfVector2u sfImage_getSize(const sfImage* image)
{
    sfVector2u size = {0, 0};
    CSFML_CHECK_RETURN(image, size);

    sf::Vector2u sfmlSize = image->This.getSize();

    size.x = sfmlSize.x;
    size.y = sfmlSize.y;

    return size;
}


////////////////////////////////////////////////////////////
void sfImage_flipHorizontally(sfImage* image)
{
    CSFML_CALL(image, flipHorizontally());
}


////////////////////////////////////////////////////////////
void sfImage_flipVertically(sfImage* image)
{
    CSFML_CALL(image, flipVertically());
}
