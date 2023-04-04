#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "mousequeherramientas.h"
#include "figure.h"

class obj_XYRect : public figure {
    public:
        shared_ptr<obj_Mat> attri_Mat;
        double attri_Coord1;
        double attri_Coord2;
        double attri_Coord3;
        double attri_Coord4;
        double attri_Plane;

        obj_XYRect() {}
        obj_XYRect(double x0, double x1, double y0, double y1, double z, shared_ptr<obj_Mat> material) 
            : attri_Coord1(x0), attri_Coord2(x1), attri_Coord3(y0), attri_Coord4(y1), attri_Plane(z), attri_Mat(material) {}

        virtual bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const;

};

class obj_XZRect : public figure {
    public:
        shared_ptr<obj_Mat> attri_Mat;
        double attri_Coord1;
        double attri_Coord2;
        double attri_Coord3;
        double attri_Coord4;
        double attri_Plane;

        obj_XZRect() {}
        obj_XZRect(double x0, double x1, double z0, double z1, double y, shared_ptr<obj_Mat> material) 
            : attri_Coord1(x0), attri_Coord2(x1), attri_Coord3(z0), attri_Coord4(z1), attri_Plane(y), attri_Mat(material) {}

        virtual bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const;

};

class obj_YZRect : public figure {
    public:
        shared_ptr<obj_Mat> attri_Mat;
        double attri_Coord1;
        double attri_Coord2;
        double attri_Coord3;
        double attri_Coord4;
        double attri_Plane;

        obj_YZRect() {}
        obj_YZRect(double y0, double y1, double z0, double z1, double x, shared_ptr<obj_Mat> material) 
            : attri_Coord1(y0), attri_Coord2(y1), attri_Coord3(z0), attri_Coord4(z1), attri_Plane(x), attri_Mat(material) {}

        virtual bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const;

};

// Se usa la funcion hit de XYRect para detectar si el rayo intersecta con el rectangulo
// la normal del rectangulo es (0, 0, 1) y el plano es z = 0
bool obj_XYRect::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    double t = (attri_Plane - ray.get_orig().get_value2()) / ray.get_dir().get_value2(); // z
    if (t < t_min || t > t_max) {
        return false;
    }
    double y = ray.get_orig().get_value1() + t * ray.get_dir().get_value1();
    double x = ray.get_orig().get_value0() + t * ray.get_dir().get_value0();
    if (x < attri_Coord1 || x > attri_Coord2 || y < attri_Coord3 || y > attri_Coord4) {
        return false;
    }
    
    record.attri_u = (x - attri_Coord1) / (attri_Coord2 - attri_Coord1);
    record.attri_v = (y - attri_Coord3) / (attri_Coord4 - attri_Coord3);
    record.attri_t = t;
    obj_Vector normal_dir(0, 0, 1);
    record.normal_face(ray, normal_dir);
    record.atrri_Mat_ptr = attri_Mat;
    record.attri_Point = ray.function(t);
    return true;
}

// la normal del rectangulo es (0, 1, 0) y el plano es y = 0
bool obj_XZRect::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    double t = (attri_Plane - ray.get_orig().get_value1()) / ray.get_dir().get_value1(); // y
    if (t < t_min || t > t_max) {
        return false;
    }
    double z = ray.get_orig().get_value2() + t * ray.get_dir().get_value2();
    double x = ray.get_orig().get_value0() + t * ray.get_dir().get_value0();
    if (x < attri_Coord1 || x > attri_Coord2 || z < attri_Coord3 || z > attri_Coord4) {
        return false;
    }

    record.attri_u = (x - attri_Coord1) / (attri_Coord2 - attri_Coord1);
    record.attri_v = (z - attri_Coord3) / (attri_Coord4 - attri_Coord3);
    record.attri_t = t;
    obj_Vector normal_dir(0, 1, 0);
    record.normal_face(ray, normal_dir);
    record.atrri_Mat_ptr = attri_Mat;
    record.attri_Point = ray.function(t);
    return true;
}

// la normal del rectangulo es (1, 0, 0) y el plano es x = 0
bool obj_YZRect::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    double t = (attri_Plane - ray.get_orig().get_value0()) / ray.get_dir().get_value0(); // x
    if (t < t_min || t > t_max) {
        return false;
    }
    double z = ray.get_orig().get_value2() + t * ray.get_dir().get_value2();
    double y = ray.get_orig().get_value1() + t * ray.get_dir().get_value1();
    if (y < attri_Coord1 || y > attri_Coord2 || z < attri_Coord3 || z > attri_Coord4) {
        return false;
    }

    record.attri_u = (y - attri_Coord1) / (attri_Coord2 - attri_Coord1);
    record.attri_v = (z - attri_Coord3) / (attri_Coord4 - attri_Coord3);
    record.attri_t = t;
    obj_Vector normal_dir(1, 0, 0);
    record.normal_face(ray, normal_dir);
    record.atrri_Mat_ptr = attri_Mat;
    record.attri_Point = ray.function(t);
    return true;
}

#endif