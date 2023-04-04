#ifndef TEXTURE_H
#define TEXTURE_H

#include "mousequeherramientas.h"

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

#endif