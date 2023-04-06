#ifndef FIGURE_LIST_H
#define FIGURE_LIST_H

#include "mousequeherramientas.h"
#include "figure.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class figure_list : public figure {
    public:
        std::vector<shared_ptr<figure>> attri_List;

    public:
        figure_list() {}
        figure_list(shared_ptr<figure> object) { add(object); } // ajoute une figure a object

        void clear() { attri_List.clear(); }
        void add(shared_ptr<figure> object) { attri_List.push_back(object); }

        virtual bool hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const override;

        virtual bool bounding_box(obj_AABB& box) const override;
};

bool figure_list::hit(const obj_Ray& ray, double t_min, double t_max, obj_Record& record) const {
    obj_Record temp_record;
    bool find = false;
    double closest = t_max;

    for (const shared_ptr<figure>& object : attri_List) {
        if (object -> hit(ray, t_min, closest, temp_record)) {
            find = true;
            closest = temp_record.attri_t;
            record = temp_record;
        }
    }

    return find;
}

bool figure_list::bounding_box(obj_AABB& box) const {
    if (attri_List.empty()) {
        return false;
    }

    obj_AABB temp_box;
    bool first_box = true;

    for (const auto& list : attri_List) {
        if (list -> bounding_box(temp_box) == false) {
            return false;
        }
        
        if (first_box) {
            box = temp_box;
            first_box = false;
        } else {
            box = surrounding_box(box, temp_box);
        }

    }

    return true;
}

#endif