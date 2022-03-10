/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"
#include <cmath>

void ImageTransform::grayscale()
{
    float intensity = 0.0;
    int x,y;
    for(y = 0; y < cfg->height; y++){
        for(x = 0; x < cfg->width; x++){
            RGB color = getPixel(x, y);
            intensity = 0.299*color.r + 0.587*color.g + 0.114*color.b;
            color.r = color.g = color.b = intensity;
            setPixel(x, y, color);   
        }
    }
    
}

void ImageTransform::threshold()
{
    uint32_t x,y;
    ImageTransform::grayscale();
    for(y = 0; y < cfg->height; y++){
        for(x = 0; x < cfg->width; x++){
            RGB color = getPixel(x, y);
            if(color.r < cfg->threshold){
                setPixel(x, y, COLOR_BLACK);
            }
            else{
                setPixel(x, y, COLOR_WHITE);
            }
        }
    }
}

void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::orderedDithering()
{
    uint32_t matrix[m_side][m_side] = {{0, 204, 51, 255}, {68, 136, 187, 119}, {34, 238, 17, 221}, {170, 102, 153, 85}};
    ImageTransform::grayscale();
    uint32_t x, y, a, b;
    for(y = 0; y < cfg->height; y++){
        for(x = 0; x < cfg->width; x++){
            a = x%m_side;
            b = y%m_side;
            RGB color = getPixel(x, y);
            if(color.r < matrix[b][a]){
                setPixel(x,y, COLOR_BLACK);
            }
            else{
                setPixel(x,y, COLOR_WHITE);
            }
        }
    }

}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{
    
    int k;
    RGB color = getPixel(x, y);
    k = color.r + std::round(err);
    if( k < 0){
        k = COLOR_BLACK.r;
    }
    if(k > 255){
        k = COLOR_WHITE.r;
    }
    color.r = color.b = color.g = k;
    setPixel(x, y, color);
}

void ImageTransform::errorDistribution()
{
    int error = 0;
    uint32_t y, x;
    ImageTransform::grayscale();

    for(y = 0; y < cfg->height; y++){
        for(x = 0; x < cfg->width; x++){
            RGB color = getPixel(x,y);
            if(color.r > cfg->threshold){
                error = color.r - COLOR_WHITE.r;
                setPixel(x, y, COLOR_WHITE);
            }
            else{
                error = color.r;
                setPixel(x, y, COLOR_BLACK);
            }
            updatePixelWithError(x+1, y, error * 3.0/8.0);
            updatePixelWithError(x, y+1, error * 3.0/8.0);
            updatePixelWithError(x+1, y+1, error * 2.0/8.0);
        }
    }
}

