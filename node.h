#ifndef NODE_H
#define NODE_H

#include "mousequeherramientas.h"

#include "figure.h"
#include "figure_list.h"

#include <algorithm>

class obj_Node : public figure {
    public:
        shared_ptr<figure> attri_Left;
        shared_ptr<figure> attri_Right;
        obj_AABB attri_Box;

        obj_Node();
        obj_Node(const figure_list& list) : obj_Node(list.attri_List, 0, list.attri_List.size()) {}

        obj_Node(const std::vector<shared_ptr<figure>>& list_o, size_t start, size_t end);

        virtual bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const override;

        virtual bool bounding_box(obj_AABB& box) const override;
};

bool obj_Node::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    if (attri_Box.hit(ray, t_min, t_max) == false) {
        return false;
    }

    bool hit_left = attri_Left -> hit(ray, t_min, t_max, record);
    bool hit_right;
    if (hit_left) {
        hit_right = attri_Right -> hit(ray, t_min, record.attri_t, record);
    } else {
        hit_right = attri_Right -> hit(ray, t_min, t_max, record);
    }

    /*
    if (hit_left == true || hit_right == true) {
        return true;
    } 
    */

    return hit_left || hit_right;
} 

bool obj_Node::bounding_box(obj_AABB& box) const {
    box = attri_Box;
    return true;
}

bool box_compare(const shared_ptr<figure> list_a, const shared_ptr<figure> list_b, int axis) {
    obj_AABB box_a;
    obj_AABB box_b;

    if (list_a -> bounding_box(box_a) == false || list_b -> bounding_box(box_b) == false) {
        std::cerr << "No bounding box in box_x_compare.\n";
    }

    return box_a.min().attri_V[axis] < box_b.min().attri_V[axis];
}

bool box_x_compare(const shared_ptr<figure> list_a, const shared_ptr<figure> list_b) {
    return box_compare(list_a, list_b, 0);
}

bool box_y_compare(const shared_ptr<figure> list_a, const shared_ptr<figure> list_b) {
    return box_compare(list_a, list_b, 1);
}

bool box_z_compare(const shared_ptr<figure> list_a, const shared_ptr<figure> list_b) {
    return box_compare(list_a, list_b, 2);
}

obj_Node::obj_Node(const std::vector<shared_ptr<figure>>& list_o, size_t start, size_t end) {
    auto list_obj = list_o; // Create a modifiable array of the source scene objects

    int axis = random_int(0, 2);
    
    /*
    if (axis == 0) {
        bool comparator = box_x_compare;
    } else if (axis == 1) {
        bool comparator = box_y_compare;
    } else {
        bool comparator = box_z_compare;
    }
    */

    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;
   
    size_t size = end - start;

    if (size == 1) {
        attri_Left = attri_Right = list_obj[start];
    } else if (size == 2) {
        if (comparator(list_obj[start], list_obj[start + 1])) {
            attri_Left = list_obj[start];
            attri_Right = list_obj[start + 1];
        } else {
            attri_Left = list_obj[start + 1];
            attri_Right = list_obj[start];
        }
    } else {
        std::sort(list_obj.begin() + start, list_obj.begin() + end, comparator);

        size_t mid = start + size/2;
        attri_Left = make_shared<obj_Node>(list_obj, start, mid);
        attri_Right = make_shared<obj_Node>(list_obj, mid, end);
    }

    obj_AABB box1;
    obj_AABB box2;

    // No se usa aun, es para cuando se mande un objeto sin bounding box
    if(attri_Left -> bounding_box(box1) == false || attri_Right -> bounding_box(box2) == false) {
        std::cerr << "No bounding box in obj_Node constructor.\n";
    }

    attri_Box = surrounding_box(box1, box2);
}

#endif