// Le code s'execute avec "g++ main.cpp" apres, ".\a.exe > image.ppm" 
// important .\a.exe puet etre reemplacé pour le fichier créé par "g++ main.cpp"

#include <iostream>
#include <ctime>

#include <omp.h>

#include "mousequeherramientas.h"
#include "camera.h"
#include "color.h"
#include "figure_list.h"
#include "rectangle.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"
#include "cube.h"
#include "node.h"

// blender fonction, le secret c'est que quand t = 1, on aura (1 - t) = 0, donc on
// efface tout le bleu, c'est a dire, on aura que du blanc, le cas contraire on aura
// que du bleu, et donc la partie centrale fera une transition plus naturelle.

// avec world, le difference c'est que tous les rays ne passent pas pour une figure 
// mais par une liste des figures

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

///////////////////////////////////// TYPES_OF_WORLD ///////////////////////////////////////
figure_list simple_spheres() {
    figure_list world;

    auto material_ground = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0);
    auto material_center = make_shared<mat_Ligth>(obj_Color(2, 2, 2));
    auto material_left   = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.8);
    auto material_right  = make_shared<mat_Diffu>(obj_Color(0.1,1,1));

    auto earth_texture = make_shared<mat_Image>("grass.jpg");
    auto grass = make_shared<mat_Diffu>(earth_texture);


    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, -1.0), 0.5, grass));
    world.add(make_shared<obj_Sphere>(obj_Point(-1.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<obj_Sphere>(obj_Point(1.5, 0.9,-0.5), 0.5, material_center));
    world.add(make_shared<obj_Sphere>(obj_Point( 1.0, 0.0, -1.0), 0.5, material_right));
    //world.add(make_shared<obj_Cube>(obj_Point(-3.0,-3.0, 1.0), obj_Point(3, 0, 4), material_center));

    return world;

}

figure_list grama() {
    figure_list world;

    auto material_ground = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0);
    auto material_center = make_shared<mat_Ligth>(obj_Color(5, 0, 10));
    auto material_left   = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.8);
    auto material_right  = make_shared<mat_Diffu>(obj_Color(0.1,1,1));

    auto earth_texture = make_shared<mat_Image>("grass.jpg");
    auto grass = make_shared<mat_Diffu>(earth_texture);


    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, -1.0), 0.5, grass));
    world.add(make_shared<obj_Sphere>(obj_Point(-1.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<obj_Sphere>(obj_Point( 1.0, 0.0, -1.0), 0.5, material_right));
    //world.add(make_shared<obj_Cube>(obj_Point(-3.0,-3.0, 1.0), obj_Point(3, 0, 4), material_center));

    return world;

}

figure_list not_so_random_scene() {
    figure_list world;

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            double choose_mat = random_double();
            obj_Point center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - obj_Point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<obj_Mat> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    obj_Color type_diffuse = obj_Color::random() * obj_Color::random();
                    sphere_material = make_shared<mat_Diffu>(type_diffuse);
                    world.add(make_shared<obj_Sphere>(center, 0.2, sphere_material));
                } else {
                    // metal
                    obj_Color type_diffuse = obj_Color::random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<mat_Met>(type_diffuse, fuzz);
                    world.add(make_shared<obj_Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto ground_material = make_shared<mat_Diffu>(obj_Color(0.5, 0.5, 0.5));
    auto material1 = make_shared<mat_Diffu>(obj_Color(0.4, 0.2, 0.1));
    auto material2 = make_shared<mat_Met>(obj_Color(0.7, 0.6, 0.5), 0);

    world.add(make_shared<obj_Sphere>(obj_Point(0, -1000, 0), 1000, ground_material));
    world.add(make_shared<obj_Sphere>(obj_Point(-4, 1, 0), 1.0, material1));
    world.add(make_shared<obj_Sphere>(obj_Point(4, 1, 0), 1.0, material2));

    return world;
}

figure_list ligth_spheres() {
    figure_list world;

    auto material_ligth = make_shared<mat_Ligth>(obj_Color(1, 1, 1));
    auto material_left   = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.4);
    auto material_center  = make_shared<mat_Met>(obj_Color(0.8, 0.6, 0.2), 1);
    auto material_right = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0);

    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100.0, material_ligth));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<obj_Sphere>(obj_Point(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<obj_Sphere>(obj_Point( 1.0, 0.0, -1.0), 0.5, material_right));

    return world;

}

figure_list ligth_rectangle() {
    figure_list world;

    auto material_ligth = make_shared<mat_Ligth>(obj_Color(4, 4, 4));
    auto material_metal  = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.4);
    auto material_blue_diffu  = make_shared<mat_Met>(obj_Color(0.2, 0.86, 1), 1);
    auto material_reflect = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0);
    auto material_blue_reflect = make_shared<mat_Met>(obj_Color(0.2, 0.86, 1), 0);
    auto material_green = make_shared<mat_Met>(obj_Color(0.5, 1, 0.4), 0.5);

    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100.0, material_reflect));
    world.add(make_shared<obj_XYrect>(-1, 1, 0, 1.5, -1.5, material_ligth));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, -1.0), 0.5, material_blue_reflect));
    world.add(make_shared<obj_Sphere>(obj_Point(-1.0, 0.0, -1.0), 0.5, material_blue_diffu));
    world.add(make_shared<obj_Sphere>(obj_Point( 1.0, 0.0, -1.0), 0.5, material_green));

    return world;
}

figure_list cornell_box() {
    figure_list world;

    auto red = make_shared<mat_Diffu>(obj_Color(.65, .05, .05));
    auto white = make_shared<mat_Diffu>(obj_Color(.73, .73, .73));
    auto green = make_shared<mat_Diffu>(obj_Color(.12, .45, .15));
    auto light = make_shared<mat_Ligth>(obj_Color(15, 15, 15));

    world.add(make_shared<obj_YZrect>(-1, 556, -1, 556, 556, green));
    world.add(make_shared<obj_YZrect>(-1, 556, -1, 556, -1, red));
    //world.add(make_shared<obj_XZrect>(213, 343, 227, 332, 554, light));
    world.add(make_shared<obj_XZrect>(-1, 556, -1, 556, -1, white));
    world.add(make_shared<obj_XZrect>(-1, 556, -1, 556, 556, white));
    world.add(make_shared<obj_XYrect>(-1, 556, -1, 556, -556, white));

    return world;
}

figure_list cornell_box_WORK() {
    figure_list world;

    auto red   = make_shared<mat_Diffu>(obj_Color(0.65, 0.05, 0.05));
    auto white = make_shared<mat_Diffu>(obj_Color(0.73, 0.73, 0.73));
    auto green = make_shared<mat_Diffu>(obj_Color(0.12, 0.45, 0.15));
    auto light = make_shared<mat_Ligth>(obj_Color(1.0, 1.0, 1.0));

    world.add(make_shared<obj_YZrect>(0.0, 555.0, 0.0, 555.0, 555.0, green));
    world.add(make_shared<obj_YZrect>(0.0, 555.0, 0.0, 555.0, 0.0, red));
    world.add(make_shared<obj_XZrect>(213.0, 343.0, 227.0, 332.0, 554.0, light));
    world.add(make_shared<obj_XZrect>(0.0, 555.0, 0.0, 555.0, 0.0, white));
    world.add(make_shared<obj_XZrect>(0.0, 555.0, 0.0, 555.0, 555.0, white));
    world.add(make_shared<obj_XYrect>(0.0, 555.0, 0.0, 555.0, 555.0, white));

    //world.add(make_shared<obj_Sphere>(obj_Point(275.0, 275.0, 275.0), 210.0, light));

    return world;
}

figure_list test() {
    figure_list world;

    auto material_ground = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0);
    auto material_center = make_shared<mat_Ligth>(obj_Color(1, 1, 1));
    auto material_left   = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.03);
    auto material_right  = make_shared<mat_Diffu>(obj_Color(0.8, 0.6, 0.2));

    //world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, 0), 25, material_left));
    world.add(make_shared<obj_Sphere>(obj_Point(50, 50, 0), 25, material_center));
    world.add(make_shared<obj_Sphere>(obj_Point( 0, -50, -50), 25, material_right));

    return world;
}

figure_list cubes() {
    figure_list world;
    auto white = make_shared<mat_Diffu>(obj_Color(0.73, 0.73, 0.73));
    auto light = make_shared<mat_Ligth>(obj_Color(1.0, 1.0, 1.0));
    auto metal  = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.1);

    world.add(make_shared<obj_Cube>(obj_Point(130, 0, 65), obj_Point(295, 165, 230), white));
    world.add(make_shared<obj_Cube>(obj_Point(265, 0, 295), obj_Point(430, 330, 460), light));
    world.add(make_shared<obj_Sphere>(obj_Point(200, -2000, 100), 2000.0, metal));

    return world;
}

figure_list earth() {
    figure_list world;
    
    auto earth_texture = make_shared<mat_Image>("earthmap.jpg");
    auto earth_surface = make_shared<mat_Ligth>(earth_texture);
    world.add(make_shared<obj_Sphere>(obj_Point(0,0,0), 2, earth_surface));

    return world;
}



int main() {
    
    //////////////// IMAGE ////////////////
    auto relation = 3.0/2.0;
    int image_width = 400;
    //int image_height = static_cast<int>(image_width / relation); // Pour change que l'image_width, EFFACER CONST APRES CRÉER LA FONCTION
    int samples_per_pixel = 100;
    const int depth = 50;


    //////////////// WORLD ////////////////
    figure_list world;

    obj_Point look_From;
    obj_Point look_At;
    obj_Vector up_Vec(0, 1, 0);
    double vert_FOV = 20;
    obj_Color world_Color(1, 1, 1);
    double focus = 0;
    //double dist = 21;
    
    switch (9) {
        case 1:
            world = grama();
            look_From = obj_Point(0, 1, 2);
            look_At = obj_Point(0, 0, -1);
            world_Color = obj_Color(1, 1, 1);

            break;

        case 2:
            world = not_so_random_scene();
            look_From = obj_Point(13,2,3);
            look_At = obj_Point(0, 0, 0);
            vert_FOV = 80;
            break;

        case 3:
            world = ligth_spheres();
            look_From = obj_Point(0, 1, 2);
            look_At = obj_Point(0, 0, -1);
            world_Color = obj_Color(0, 0, 0);
            break;

        case 4:
            world = ligth_rectangle();
            look_From = obj_Point(0, 1, 2);
            look_At = obj_Point(0, 0, -1);
            world_Color = obj_Color(0, 0, 0);
            break;

        case 5:
            world = cornell_box();
            look_From = obj_Point(278, 278, 1156);
            look_At = obj_Point(278, 278, 0);
            vert_FOV = 70;
            world_Color = obj_Color(1, 1, 1);
            break;

        case 6:
            world = test();
            look_From = obj_Point(220, 158, 130);
            look_At = obj_Point(0, 0, 0);
            world_Color = obj_Color(1, 1, 1);
            break;

        case 7:
            world = cornell_box_WORK();
            relation = 1;
            image_width = 500;
            samples_per_pixel = 100;
            world_Color = obj_Color(0.0,0.0,0.0);
            look_From = obj_Point(490, 278.0, -800.0);
            look_At = obj_Point(278.0, 278.0, 0.0);
            vert_FOV= 40.0;
            break;

        case 8:
            world = cubes();
            world_Color = obj_Color(1, 1, 1);
            look_From = obj_Point(278, 278, -700);
            look_At = obj_Point(278, 278, 0);
            break;

        case 9:
            world = earth();
            look_From = obj_Point(0, 0, 0);
            look_At = obj_Point(0, 0, -10);
            break;

        case 10:
            world = simple_spheres();
            look_From = obj_Point(0, 1, 2);
            look_At = obj_Point(0, 0, -1);
            world_Color = obj_Color(0, 0, 0);

            break;
    }

    int image_height = static_cast<int>(image_width / relation);
    double dist = (look_From - look_At).length();
    
    ////////////////  CAMERA ////////////////
    obj_Cam cam(look_From, look_At, up_Vec, vert_FOV, relation, focus, dist);
    
    
    ///////////////////////////////// RENDER /////////////////////////////////
    clock_t start = clock();        

    //Imprimer la taille
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //int j;
    //#pragma omp parallel for schedule(dynamic) num_threads(8)
    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            obj_Color color(0, 0, 0);
            for (int n = 0; n < samples_per_pixel; ++n) {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);

                obj_Ray color_ray = cam.coloring_ray(u, v);
                color += blender_ray(color_ray, world_Color, world, depth);
            }
            color_printer(std::cout, color, samples_per_pixel); // Entender mejor
        }
    }
    
    std::cerr << "\nDone.\n";

    clock_t end = clock();
    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    std::cerr << "Time taken by method: " << duration << " seconds" << std::endl;
    

    /*
    // vector.h tests
    obj_Vector vector3(1,2,3);
    std::cout << "\n" << vector3.get_value0() << vector3.get_value1() 
                << vector3.get_value2() << "\n";

    vector3 = vector3.operator-();
    std::cout << "\n" << vector3.get_value0() << vector3.get_value1() 
                << vector3.get_value2() << "\n";

    double a = vector3.operator[](1);
    double b = vector3.get_value1();
    double& c = vector3.operator[](1);
    std::cout << "\n" << a << b << c << "\n";

    obj_Vector vector1(1,2,3);
    vector3.operator+=(vector1);
    std::cout << "\n" << vector3.get_value0() << vector3.get_value1() 
                << vector3.get_value2() << "\n";
    std::cout << "\n" << vector1.get_value0() << vector1.get_value1() 
                << vector1.get_value2() << "\n";

    vector1.operator*=(2);
    std::cout << "\n" << vector1.get_value0() << vector1.get_value1() 
                << vector1.get_value2() << "\n";
    
    
    std::cout << "\n" << vector1.length_squared() << ' ' << vector1.length() 
                << vector1.get_value2() << "\n";
    
    
    obj_Vector v(1,2,3);
    obj_Vector u(-1,-2,-3);

    obj_Vector p = v * u;
    p = 10*p;
    obj_Color d = v * u;
    p = p + d;
    p = -p;
    std::cout << "\n" << d.get_value2() << " " << p.get_value1() << "\n";
    */
}