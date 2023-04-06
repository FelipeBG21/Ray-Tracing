#ifndef FIGURE_H
#define FIGURE_H

#include "mousequeherramientas.h"
#include "aabb.h"

class obj_Mat; // permite saber al archivo/clase que este objeto existe y que se va a usar, sin necesidad de definirlo

struct obj_Record {
    obj_Point attri_Point;
    obj_Vector attri_Normal;
    shared_ptr<obj_Mat> atrri_Mat_ptr;
    double attri_t;
    double attri_u;
    double attri_v;
    bool attri_dir_n;

    void normal_face(const obj_Ray& ray, const obj_Vector& normal_dir) {
        attri_dir_n = dot(ray.get_dir(), normal_dir) < 0; // Pour savoir le direction 
        // du le vecteur normal
        if (attri_dir_n = true) {
            attri_Normal = normal_dir;
        } else {
            attri_Normal = -normal_dir;
        }
    }
};

class figure {
    public:
        virtual bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const = 0;
        // abstract base class
        // on doit l'utiliser dans une autre class (heritage)

        virtual bool bounding_box(obj_AABB& output_box) const = 0;
};

#endif