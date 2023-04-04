#ifndef CAMERA_H
#define CAMERA_H

#include "mousequeherramientas.h"


class obj_Cam {
    private:
        obj_Point attri_Ori;
        obj_Point attri_Left_Cor;
        obj_Vector attri_Hori;
        obj_Vector attri_Vert;

    public:
        obj_Cam(obj_Point look_From, obj_Point look_At, obj_Vector up_Vec, double vert_FOV, double relation) {

            /////////// CAMERA ///////////
            //const double relation = 3/2;
            double theta = vert_FOV * pi / 180; // theta is a type of double
            double h = tan(theta/2);
            const int camera_height = 2 * h;
            const double camera_width = relation * camera_height;

            obj_Vector w = unit_vector(look_From - look_At);
            obj_Vector u = unit_vector(cross(up_Vec, w));
            obj_Vector v = cross(w, u);

            attri_Ori = look_From;
            attri_Hori = camera_width * u;
            attri_Vert = camera_height * v;
            attri_Left_Cor = attri_Ori - attri_Hori/2 - attri_Vert/2 - w;
            
        }

        obj_Ray coloring_ray(double arg_s, double arg_t) const {
            return obj_Ray(attri_Ori, attri_Left_Cor + arg_s * attri_Hori + arg_t * attri_Vert - attri_Ori);
        }
};

#endif