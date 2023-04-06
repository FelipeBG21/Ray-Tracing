#ifndef MATERIAL_H
#define MATERIAL_H

#include "mousequeherramientas.h"
#include "figure.h"
#include "texture.h"

struct obj_Record; // struct est nécessaire parce qu'il permet de notifier au code qu'il y a un objet "record" qui sera utilisé sans avoir à le définir dans le fichier

class obj_Mat {
    public:
        // On a besoin de la fonction scatter pour savoir si on a touché un objet ou pas ???
        // La función scatter nos dice si los rayos que chocan con el material se reflejan o no o cuanto se atenuan
        // virtual est nécessaire parce qu'il permet que les objets qui héritent de material peuvent redefinir scatter
        virtual bool scatter( const obj_Ray& ray_in, const obj_Record& record, 
                            obj_Color& color_Attenue, obj_Ray& scattered) const = 0;

        virtual obj_Color emitted(double u, double v, const obj_Point& point) const {
            return obj_Color(0, 0, 0);
        }

};

class mat_Diffu : public obj_Mat {
    public:
        shared_ptr<mat_Texture> attri_Diffu;

    public:

        mat_Diffu(const obj_Color& diffuse) : attri_Diffu(make_shared<mat_Color>(diffuse)) {} // con un obj_Color se crea un mat_Color y se guarda en attri_Diffu (shared_ptr<mat_Texture>)
                                                                                            // es decir, desde un obj_Color se crea un shared pointer
        mat_Diffu(shared_ptr<mat_Texture> diffuse) : attri_Diffu(diffuse) {} // Un shared pointer ya creado se guarda en attri_Diffu (shared_ptr<mat_Texture>)


        virtual bool scatter(const obj_Ray& ray_in, const obj_Record& record, 
                            obj_Color& color_Attenue, obj_Ray& scattered) const override {
            obj_Vector scatter_direction = record.attri_Normal + random_Unit_Sph();

            // If the scatter direction is near zero, we need to set it to the normal
            if (scatter_direction.near_zero()) // if near_zero() is true
                scatter_direction = record.attri_Normal; // scatter_direction = normal

            scattered = obj_Ray(record.attri_Point, scatter_direction);
            color_Attenue = attri_Diffu -> value(record.attri_u, record.attri_v, record.attri_Point);
            return true;
        }
};

class mat_Met : public obj_Mat {
    public:
        obj_Color attri_Met;
        double attri_Fuzz;

    public:
        mat_Met(const obj_Color& metal, double fuzz ) : attri_Met(metal) {  
            // this is still an atributte of the class
            // the idea is that if the sphere is to big, the fuzz would be bigger than 1 so we need to limit it
            if (fuzz < 1) {
                attri_Fuzz = fuzz;
            } else {
                attri_Fuzz = 1;
            }
        } 
        virtual bool scatter(
            const obj_Ray& ray_in, const obj_Record& record, obj_Color& color_Attenue, obj_Ray& scattered
        ) const override {
            obj_Vector reflected = reflect(unit_vector(ray_in.get_dir()), record.attri_Normal);
            scattered = obj_Ray(record.attri_Point, reflected + attri_Fuzz * random_Unit_Sph());
            color_Attenue = attri_Met;
            return (dot(scattered.get_dir(), record.attri_Normal) > 0);
        }
};

class mat_Ligth : public obj_Mat {
    public:
        shared_ptr<mat_Texture> attri_Ligth;

        mat_Ligth(shared_ptr<mat_Texture> light) : attri_Ligth(light) {}
        mat_Ligth(obj_Color light_Color) : attri_Ligth(make_shared<mat_Color>(light_Color)) {}

        virtual bool scatter(const obj_Ray& ray_in, const obj_Record& record, 
                            obj_Color& color_Attenue, obj_Ray& scattered) const override {
            return false;
        }

        virtual obj_Color emitted(double u, double v, const obj_Point& point) const override {
            return attri_Ligth -> value(u, v, point);
        }

};

#endif