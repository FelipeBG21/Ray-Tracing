#ifndef CAMERA_H
#define CAMERA_H

#include "mousequeherramientas.h"


class camera {
    public:
        camera() {

            /////////// CAMERA ///////////
            const double relation = 3/2;

            const int camera_height = 2;
            const double camera_width = relation * camera_height;
            const int focal_length = 1;

            // creation du petit view
            attri_Ori = obj_Point(0, 0, 0);
            attri_Hori = obj_Vector(camera_width, 0, 0);
            attri_Vert = obj_Vector(0, camera_height, 0);
            attri_Left_Cor = attri_Ori - attri_Hori/2 - attri_Vert/2 - obj_Point(0, 0, focal_length);

        }

        obj_Ray coloring_ray(double arg_u, double arg_v) const {
            return obj_Ray(attri_Ori, attri_Left_Cor + arg_u * attri_Hori + arg_v * attri_Vert - attri_Ori);
        }

    private:
        obj_Point attri_Ori;
        obj_Point attri_Left_Cor;
        obj_Vector attri_Hori;
        obj_Vector attri_Vert;
};

#endif