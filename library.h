#ifndef VAPI_RT_H
#define VAPI_RT_H

#include "mousequeherramientas.h"
#include "camera.h"
#include "color.h"
#include "figure_list.h"
//#include "rectangle.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"
#include "cube.h"

#include <iostream>

class vapi_RT {
    public:
        vapi_RT() {}
        //vapi_RT(const obj_Point point_of_view, const obj_Point point_to_view, const obj_Vector plane_camera, double degrees_overture, const obj_Color background_color);
        //vapi_RT(const obj_Point point_of_view, const obj_Point point_to_view, const obj_Vector plane_camera, double degrees_overture, const obj_Color background_color, const figure_list& list);
        //vapi_RT(const obj_Point point_of_view, const obj_Point point_to_view, const obj_Vector plane_camera, double degrees_overture, const obj_Color background_color, const std::vector<shared_ptr<figure>>& list_o);

        void create_image(int image_width, int samples_per_pixel, int depth, double relation);

        //void modify_view(const obj_Point point_of_view, const obj_Point point_to_view, const obj_Vector plane_camera, double degrees_overture);

        void create_sphere(double radius, const int center_x, const int center_y, const int center_z, figure_list& world);
        //void create_sphere(double radius, const int center_x, const int center_y, const int center_z, const std::shared_ptr<obj_Mat> material = make_shared<mat_Diffu>(obj_Color(0.9, 1, 0.6)));
        /*
        void define_material(const int red = 0.9, const int green = 1, const int blue = 0.6);
        
        void define_material_metal(const int red = 0.9, const int green = 1, const int blue = 0.6, double fuzz = 0.5);
        
        void create_cube(const int x_min = -10, const int x_max = 10, const int y_min = -10, const int y_max = 10, const int z_min = -10 , const int z_max = 10, const std::shared_ptr<obj_Mat> material = make_shared<mat_Diffu>(obj_Color(0.9, 1, 0.6)));
        */
        void render_world(figure_list& world);      

        figure_list create_world() {
            attri_world = figure_list();
            return attri_world;
        }

    public:
        /////// World ///////
        figure_list attri_world;

        /////// Image ///////
        double attri_relation = 3.0/2.0;
        int attri_width = 400;
        int attri_height;
        int attri_samples = 100;
        int attri_depth = 50;
        obj_Color attri_world_color = obj_Color(0.5, 0.5, 0.5);

        /////// View ///////
        obj_Point attri_pov = obj_Point(0, 2, 1);
        obj_Point attri_ptv = obj_Point(0, 0, -1);
        obj_Vector attri_plane = obj_Vector(0, 1, 0);
        double attri_degrees = 90;

        /////// Camera ///////
        obj_Point attri_camera;
};

obj_Color blender_ray(const obj_Ray& ray, const obj_Color& world_Color, const figure_list world, int limit) {
    obj_Record record;

    // Si on est arrivé a la limit, on arrete de colleter plus de lumiere
    if (limit <= 0) {
        return obj_Color(0, 0, 0);
    }

    if(world.hit(ray, 0.001, infinity, record) == false) {
        if (world_Color.get_value0() == 1 && world_Color.get_value1() == 1 && world_Color.get_value2() == 1) {
            obj_Vector dir_vec = unit_vector(ray.get_dir()); // Normalisation
            double hit_t = 0.5 * (dir_vec.get_value1() + 1); // hit_t = y --> range (0,1)
            return (1 - hit_t) * obj_Color(1, 1, 1) + hit_t * obj_Color(0.5, 0.7, 1); 
        } else {
            return world_Color;
        }
    }

    obj_Ray scattered;
    obj_Color attenuation;
    obj_Color ligth_Emitted = record.atrri_Mat_ptr -> emitted(record.attri_u, record.attri_v, record.attri_Point);

    if (record.atrri_Mat_ptr -> scatter(ray, record, attenuation, scattered) == false) {
        return ligth_Emitted;
    }

    return ligth_Emitted + attenuation * blender_ray(scattered, world_Color, world, limit - 1);
}

void vapi_RT::render_world(figure_list& world) {
    obj_Cam camera = obj_Cam(attri_pov, attri_ptv, attri_plane, attri_degrees, attri_relation);

    std::cout << "P3\n" << attri_width << ' ' << attri_height << "\n255\n";

    for (int j = attri_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < attri_width; ++i) {
            obj_Color color(0, 0, 0);
            for (int n = 0; n < attri_samples; ++n) {
                double u = (double(i) + random_double()) / (attri_width - 1);
                double v = (double(j) + random_double()) / (attri_height - 1);

                obj_Ray color_ray = camera.coloring_ray(u, v);
                color += blender_ray(color_ray, attri_world_color, world, attri_depth);
            }
            color_printer(std::cout, color, attri_samples); // Entender mejor
        }
    }
    std::cerr << "\nDone.\n";
}

void vapi_RT::create_image(int image_width, int samples_per_pixel, int depth, double relation) {

    if (image_width <= 0) {
        std::cerr << "Error: image width must be greater than 0" << std::endl;
        exit(1);
    }

    if (samples_per_pixel <= 0) {
        std::cerr << "Error: samples per pixel must be greater than 0" << std::endl;
        exit(1);
    }
    
    if (depth <= 0) {
        std::cerr << "Error: depth must be greater than 0" << std::endl;
        exit(1);
    }

    attri_width = image_width;
    attri_samples = samples_per_pixel;
    attri_depth = depth;
}

obj_Cam modify_view(const obj_Point point_of_view, const obj_Point point_to_view, const obj_Vector plane_camera, double degrees_overture, const obj_Color background_color, double relation) {
    if(degrees_overture > 180 || degrees_overture < 0) {
        std::cerr << "Error: degrees of view must be between 0 and 180" << std::endl;
        exit(1);
    }

    if(background_color.get_value0() > 1 || background_color.get_value0() < 0 || background_color.get_value1() > 1 || background_color.get_value1() < 0 || background_color.get_value2() > 1 || background_color.get_value2() < 0) {
        std::cerr << "Error: color must be between 0 and 1" << std::endl;
        exit(1);
    }

    if(plane_camera.get_value0() == 0 && plane_camera.get_value1() == 0 && plane_camera.get_value2() == 0) {
        std::cerr << "Error: up vector must be different from (0, 0, 0)" << std::endl;
        exit(1);
    }

    if(point_of_view.get_value0() == point_to_view.get_value0() && point_of_view.get_value1() == point_to_view.get_value1() && point_of_view.get_value2() == point_to_view.get_value2()) {
        std::cerr << "Error: point of view must be different from point to view" << std::endl;
        exit(1);
    }

    if (relation <= 0) {
        std::cerr << "Error: relation must be greater than 0" << std::endl;
        exit(1);
    }

    if(point_of_view.empty()) {
        obj_Point look_From(0, 1, 2);
    }

    if(point_to_view.empty()) {
        obj_Point look_At(0, 0, -1);
    }

    if(plane_camera.empty()) {
        obj_Vector up_Vec(0, 1, 0);
    }

    if(background_color.empty()) {
        obj_Color background_color(1, 1, 1);
    }

    obj_Cam camera = obj_Cam(point_of_view, point_to_view, plane_camera, degrees_overture, relation);
    return camera;
}

void vapi_RT::create_sphere(double radius, const int center_x, const int center_y , const int center_z, figure_list& world) {

    if(radius <= 0) {
        std::cerr << "Error: radius must be greater than 0" << std::endl;
        exit(1);
    }

    std::shared_ptr<obj_Mat> material = make_shared<mat_Ligth>(obj_Color(2, 2, 2));

    obj_Point center = obj_Point(center_x, center_y, center_z); 

    world.add(make_shared<obj_Sphere>(center, radius, material));
}
/*
void vapi_RT::create_sphere(double radius, const int center_x, const int center_y , const int center_z, const std::shared_ptr<obj_Mat> material) {
    if(radius <= 0) {
        std::cerr << "Error: radius must be greater than 0" << std::endl;
        exit(1);
    }

    if (material == nullptr) {
        std::cerr << "Error: material must be different from nullptr" << std::endl;
        exit(1);
    }

    obj_Point center = obj_Point(center_x, center_y, center_z);

    attri_world.add(make_shared<obj_Sphere>(center, radius, material));
}


void vapi_RT::create_cube(const int x_min = -10, const int x_max = 10, const int y_min = -10, const int y_max = 10, const int z_min = -10 , const int z_max = 10, const std::shared_ptr<obj_Mat> material = make_shared<mat_Diffu>(obj_Color(0.9, 1, 0.6))) {
    if (material == nullptr) {
        std::cerr << "Error: material must be different from nullptr" << std::endl;
        exit(1);
    }

    if(x_min >= x_max || y_min >= y_max || z_min >= z_max) {
        std::cerr << "Error: minimun values must be less than maximun values" << std::endl;
        exit(1);
    }

    obj_Point point1 = obj_Point(x_min, y_min, z_min);
    obj_Point point2 = obj_Point(x_max, y_max, z_max);

    attri_world.add(make_shared<obj_Cube>(point1, point2, material));
}

std::shared_ptr<obj_Mat> define_material(const int red = 0.9, const int green = 1, const int blue = 0.6) {
    if (red > 1 || red < 0 || green > 1 || green < 0 || blue > 1 || blue < 0) {
        std::cerr << "Error: color must be between 0 and 1" << std::endl;
        exit(1);
    }
    return make_shared<mat_Diffu>(obj_Color(red, green, blue));
}

std::shared_ptr<obj_Mat> define_material_metal(const int red = 0.9, const int green = 1, const int blue = 0.6, double fuzz = 0.5) {
    if (red > 1 || red < 0 || green > 1 || green < 0 || blue > 1 || blue < 0) {
        std::cerr << "Error: color must be between 0 and 1" << std::endl;
        exit(1);
    }

    if (fuzz < 0 || fuzz > 1) {
        std::cerr << "Error: fuzz must be between 0 and 1" << std::endl;
        exit(1);
    }

    return make_shared<mat_Met>(obj_Color(red, green, blue), fuzz);
}
*/

#endif