#ifndef TEXTURE_H
#define TEXTURE_H

#include "mousequeherramientas.h"
#include "rtw_stb_image.h"

#include <iostream>

class mat_Texture {
    public:
        virtual obj_Color value(double u, double v, const obj_Point& point) const = 0;
};

class mat_Color : public mat_Texture {
    private:
        obj_Color attri_Color;

    public:
        mat_Color() {}
        mat_Color(obj_Color color) : attri_Color(color) {}

        mat_Color(double red, double green, double blue)
            : mat_Color(obj_Color(red, green, blue)) {}

        virtual obj_Color value(double u, double v, const obj_Vector& vec) const override {
            return attri_Color;
        }
};

class mat_Image : public mat_Texture {
    private:
        unsigned char *data;
        int width, height;
        int bytes_per_scanline;

    public:
        const int bytes_per_pixel = 3;

        mat_Image()
          : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        mat_Image(const char* filename) {
            auto components_per_pixel = bytes_per_pixel;

            data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }

            bytes_per_scanline = bytes_per_pixel * width;
        }

        ~mat_Image() {
            delete data;
        }

        virtual obj_Color value(double u, double v, const obj_Vector& point) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return obj_Color(0, 1, 1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates

            int i = static_cast<int>(u * width);
            int j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
            if (i >= width)  i = width - 1;
            if (j >= height) j = height - 1;

            const double color_scale = 1.0 / 255.0;
            auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

            return obj_Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
        }

};

#endif