#ifndef CUBE_H
#define CUBE_H

#include "mousequeherramientas.h"

#include "rectangle.h"
#include "figure_list.h"

class obj_Cube : public figure  {
    public:
        obj_Point box_min;
        obj_Point box_max;
        figure_list sides;

        obj_Cube() {}
        obj_Cube(const obj_Point& point0, const obj_Point& point1, shared_ptr<obj_Mat> material);

        virtual bool hit(const obj_Ray& r, double t_min, double t_max, obj_Record& rec) const override;

        virtual bool bounding_box(obj_AABB& box) const override {
            box = obj_AABB(box_min, box_max);
            return true;
        }
        
};

obj_Cube::obj_Cube(const obj_Point& point0, const obj_Point& point1, shared_ptr<obj_Mat> material) {
    box_min = point0;
    box_max = point1;

    sides.add(make_shared<obj_XYrect>(point0.get_value0(), point1.get_value0(), point0.get_value1(), point1.get_value1(), point1.get_value2(), material));
    sides.add(make_shared<obj_XYrect>(point0.get_value0(), point1.get_value0(), point0.get_value1(), point1.get_value1(), point0.get_value2(), material));

    sides.add(make_shared<obj_XZrect>(point0.get_value0(), point1.get_value0(), point0.get_value2(), point1.get_value2(), point1.get_value1(), material));
    sides.add(make_shared<obj_XZrect>(point0.get_value0(), point1.get_value0(), point0.get_value2(), point1.get_value2(), point0.get_value1(), material));

    sides.add(make_shared<obj_YZrect>(point0.get_value1(), point1.get_value1(), point0.get_value2(), point1.get_value2(), point1.get_value0(), material));
    sides.add(make_shared<obj_YZrect>(point0.get_value1(), point1.get_value1(), point0.get_value2(), point1.get_value2(), point0.get_value0(), material));
}

bool obj_Cube::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    return sides.hit(ray, t_min, t_max, record);
}

#endif