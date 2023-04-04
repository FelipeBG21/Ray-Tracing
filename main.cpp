// Le code s'execute avec "g++ main.cpp" apres, ".\a.exe > image.ppm" 
// important .\a.exe puet etre reemplacé pour le fichier créé par "g++ main.cpp"

#include <iostream>

#include "mousequeherramientas.h"
#include "figure_list.h"
#include "color.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "rectangle.h"

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

    return attenuation * blender_ray(scattered, world_Color, world, limit - 1);
}


///////////////////////////////////// TYPES_OF_WORLD ///////////////////////////////////////
figure_list simple_spheres() {
    figure_list world;

    auto material_ground = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0);
    auto material_center = make_shared<mat_Ligth>(obj_Color(1, 1, 1));
    auto material_left   = make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 0.4);
    auto material_right  = make_shared<mat_Met>(obj_Color(0.8, 0.6, 0.2), 1);

    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<obj_Sphere>(obj_Point(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<obj_Sphere>(obj_Point( 1.0, 0.0, -1.0), 0.5, material_right));

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
                    obj_Color type_diffuse = random() * random();
                    sphere_material = make_shared<mat_Dif>(type_diffuse);
                    world.add(make_shared<obj_Sphere>(center, 0.2, sphere_material));
                } else {
                    // metal
                    obj_Color type_diffuse = random(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<mat_Met>(type_diffuse, fuzz);
                    world.add(make_shared<obj_Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto ground_material = make_shared<mat_Dif>(obj_Color(0.5, 0.5, 0.5));
    auto material1 = make_shared<mat_Dif>(obj_Color(0.4, 0.2, 0.1));
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
    world.add(make_shared<obj_XYRect>(-1, 1, 0, 1.5, -1.5, material_ligth));
    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, 0.0, -1.0), 0.5, material_blue_reflect));
    world.add(make_shared<obj_Sphere>(obj_Point(-1.0, 0.0, -1.0), 0.5, material_blue_diffu));
    world.add(make_shared<obj_Sphere>(obj_Point( 1.0, 0.0, -1.0), 0.5, material_green));

    return world;
}

int main() {
    //////////////// IMAGE ////////////////
    const double relation = 3/2;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / relation); // Pour change que l'image_width, EFFACER CONST APRES CRÉER LA FONCTION
    const int samples_per_pixel = 150;
    const int depth = 50;


    //////////////// WORLD ////////////////
    figure_list world;

    obj_Point look_From;
    obj_Point look_At;
    obj_Vector up_Vec(0, 1, 0);
    double vert_FOV = 85;
    obj_Color world_Color(0.7, 0.8, 1);
    
    switch (4) {
        case 1:
            world = simple_spheres();
            look_From = obj_Point(0, 1, 2);
            look_At = obj_Point(0, 0, -1);
            world_Color = obj_Color(0, 0, 0);
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
    }


    //////////////// CAMERA ////////////////
    obj_Cam cam(look_From, look_At, up_Vec, vert_FOV, relation);


    ///////////////////////////////// RENDER /////////////////////////////////

    //Imprimer la taille
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Avec color_panel, on crée le fichier qui PPM va lire
    // Le vecteur "pixel" est en train de passer pour tout l'image en creant un vecteur 
    // pour chaque pixel

    // Aprés ça on fait le couleur blending
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            obj_Color color(0, 0, 0);
            for (int n = 0; n < samples_per_pixel; ++n) {
                double u = (double(i) + random_double()) / (image_width - 1);
                double v = (double(j) + random_double()) / (image_height - 1);
                //std::cout << u;
                // double in the i and j is necessary, if not, image is black
                obj_Ray color_ray = cam.coloring_ray(u, v);
                color += blender_ray(color_ray, world_Color, world, depth);
            }
            color_printer(std::cout, color, samples_per_pixel); // Entender mejor
        }
    }
    
    std::cerr << "\nDone.\n";







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