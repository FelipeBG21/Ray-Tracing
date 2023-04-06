#ifndef AABB_H
#define AABB_H

#include "mousequeherramientas.h"

class obj_AABB {
    public:
        obj_Point attri_Min;
        obj_Point attri_Max;

        obj_AABB() {}
        obj_AABB(const obj_Point& a, const obj_Point& b) : attri_Min(a), attri_Max(b) {}

        obj_Point min() const {
            return attri_Min; 
        }

        obj_Point max() const {
            return attri_Max; 
        }

        /*
        bool hit(const obj_Ray& ray, double t_min, double t_max) const{
            for (int a = 0; a < 3; a++) {
                double t0 = fmin((attri_Min[a] - ray.get_orig()[a]) / ray.get_dir()[a],
                               (attri_Max[a] - ray.get_orig()[a]) / ray.get_dir()[a]);

                double t1 = fmax((attri_Min[a] - ray.get_orig()[a]) / ray.get_dir()[a],
                               (attri_Max[a] - ray.get_orig()[a]) / ray.get_dir()[a]);

                t_min = fmax(t0, t_min);
                t_max = fmin(t1, t_max);
                if (t_max <= t_min)
                    return false;
            }
            return true;
        }
        */

        bool hit(const obj_Ray& ray, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                double inv_Ray = 1.0f / ray.get_dir()[a];
                double t0 = (min()[a] - ray.get_orig()[a]) * inv_Ray;
                double t1 = (max()[a] - ray.get_orig()[a]) * inv_Ray;
                if (inv_Ray < 0.0f) {
                    double t = t0;
                    t0 = t1;
                    t1 = t;
                }

                if (t0 > t_min) {
                    t_min = t0;
                }
                if (t1 < t_max) {
                    t_max = t1;
                }
                if (t_max <= t_min) {
                    return false;
                }
            }
            return true;
        }
       
};

obj_AABB surrounding_box(obj_AABB arg_box0, obj_AABB arg_box1) {
    obj_Point small(fmin(arg_box0.min().get_value0(), arg_box1.min().get_value0()),
                    fmin(arg_box0.min().get_value1(), arg_box1.min().get_value1()),
                    fmin(arg_box0.min().get_value2(), arg_box1.min().get_value2()));

    obj_Point big(fmax(arg_box0.max().get_value0(), arg_box1.max().get_value0()),
                    fmax(arg_box0.max().get_value1(), arg_box1.max().get_value1()),
                    fmax(arg_box0.max().get_value2(), arg_box1.max().get_value2()));

    return obj_AABB(small,big);
}

#endif