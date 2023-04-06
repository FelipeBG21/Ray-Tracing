#ifndef SPHERE_H
#define SPHERE_H

#include "figure.h"
#include "vector.h"

class obj_Sphere : public figure {
    public:
        obj_Point attri_Cen;
        double attri_R;
        shared_ptr<obj_Mat> atrri_Mat;

    public:
        obj_Sphere() {};
        obj_Sphere(obj_Point center, double radius, shared_ptr<obj_Mat> material) : attri_Cen(center), attri_R(radius), atrri_Mat(material) {}; 

        bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const override; // si no se pone override podria haber comportamientos inesperados, pero funciono si el
        // virtual es muy importante si alguien hereda de esta clase, y si no se pone, no se puede heredar de esta clase (o al menos no se puede sobreescribir)
        // On appelle la fonctionne hit qui viens de object, et on la recrie

        virtual bool bounding_box(obj_AABB& box) const override;

    private:
        static void get_sphere_uv(const obj_Point& point, double& u, double& v) {
            // p: a given point on the sphere of radius one, centered at the origin.
            // u: returned value [0,1] of angle around the Y axis from X=-1.
            // v: returned value [0,1] of angle from Y=-1 to Y=+1.
            //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
            //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
            //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

            auto theta = acos(-point.get_value1());
            auto phi = atan2(-point.get_value2(), point.get_value0()) + pi;

            u = phi / (2*pi);
            v = theta / pi;
        }
};

bool obj_Sphere::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    obj_Vector vec = ray.get_orig() - attri_Cen; // Vecteur jusqu'a le centre
    // Tout aprés c'est le équation : t^2*b*b + 2*t*b*(A-C) + (A-C)*(A-C) - r
    //                t^2*ray.get_dir + 2*t*ray.get_dir*vec + vec*vec - radius
    double a = ray.get_dir().length_squared(); 
    double b = 2 * dot(vec, ray.get_dir());
    double c = vec.length_squared() - attri_R * attri_R;
    double discriminant = (b * b) - (4 * a * c);
    
    if (discriminant < 0) { // Si le root est pas réel
        return false;
    }  

    // Si le root (qui est = t) est réel, on cherche la plus proche
    // root ou t sont où je touch l'esphere
    double root = (-b - sqrt(discriminant)) / (2 * a); // -b^2 -...
    if (root < t_min || root > t_max) {
        root = (-b + sqrt(discriminant)) / (2 * a); // -b^2 +...
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    // On garde dans le record les informations. (le t plus proche), son point et normal
    record.attri_t = root;
    record.attri_Point = ray.function(record.attri_t); // le point au hauteur du t
    obj_Vector normal_dir = (record.attri_Point - attri_Cen) / attri_R;
    record.normal_face(ray, normal_dir);
    get_sphere_uv(normal_dir, record.attri_u, record.attri_v);
    record.atrri_Mat_ptr = atrri_Mat;
    
    return true; 
}

bool obj_Sphere::bounding_box(obj_AABB& box) const {
    box = obj_AABB(attri_Cen - obj_Vector(attri_R, attri_R, attri_R),
                    attri_Cen + obj_Vector(attri_R, attri_R, attri_R));
    return true;
}

#endif