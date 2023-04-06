#ifndef CAMERA_H
#define CAMERA_H

#include "mousequeherramientas.h"


class obj_Cam {
    private:
        obj_Point attri_Ori;
        obj_Point attri_Left_Cor;
        obj_Vector attri_Hori;
        obj_Vector attri_Vert;

        obj_Vector attri_w;
        obj_Vector attri_u;
        obj_Vector attri_v;
        double attri_Lens_Rad;

    public:
        obj_Cam(obj_Point look_From, obj_Point look_At, obj_Vector up_Vec, double vert_FOV, double relation, double aperture, double focus) {

            /////////// CAMERA ///////////
            //const double relation = 3/2;
            auto theta = vert_FOV * pi / 180.0; // theta is a type of double
            auto h = tan(theta/2);
            auto camera_height = 2.0 * h;
            auto camera_width = relation * camera_height;

            attri_w = unit_vector(look_From - look_At);
            attri_u = unit_vector(cross(up_Vec, attri_w));
            attri_v = cross(attri_w, attri_u);

            attri_Ori = look_From;
            attri_Hori = focus * camera_width * attri_u;
            attri_Vert = focus * camera_height * attri_v;
            attri_Left_Cor = attri_Ori - attri_Hori/2 - attri_Vert/2 - focus * attri_w;

            attri_Lens_Rad = aperture/2;
            
        }

        obj_Ray coloring_ray(double arg_s, double arg_t) const {
            obj_Vector rd = attri_Lens_Rad * random_in_unit_disk();
            obj_Vector offset = rd.get_value0() * attri_u + rd.get_value1() * attri_v;
            return obj_Ray(attri_Ori + offset, attri_Left_Cor + arg_s * attri_Hori + arg_t * attri_Vert - attri_Ori - offset);
        }
};

#endif