// Le code s'execute avec "g++ main.cpp" apres, ".\a.exe > image.ppm" 
// important .\a.exe puet etre reemplacé pour le fichier créé par "g++ main.cpp"

#include <iostream>

#include "mousequeherramientas.h"
#include "figure_list.h"
#include "color.h"
#include "sphere.h"
#include "camera.h"

// blender fonction, le secret c'est que quand t = 1, on aura (1 - t) = 0, donc on
// efface tout le bleu, c'est a dire, on aura que du blanc, le cas contraire on aura
// que du bleu, et donc la partie centrale fera une transition plus naturelle.

// avec world, le difference c'est que tous les rays ne passent pas pour une figure 
// mais par une liste des figures
obj_Color blender_ray(const obj_Ray& ray, const figure_list world, int limit) {
    obj_Record record;

    // Si on est arrivé a la limit, on arrete de colleter plus de lumiere
    if (limit <= 0) {
        return obj_Color(0, 0, 0);
    }

    if (world.hit(ray, 0.001, infinity, record)) {
        // Pour créer des ray aleatoires et par consequence crée le "diffuse materials"
        //obj_Point target = record.attri_Point + record.attri_Normal + random_Unit_Vec();   PAS UTILISÉ POUR LE MOMMENT
        obj_Point target = record.attri_Point + random_Hemis(record.attri_Normal); 
        return 0.5 * blender_ray(obj_Ray(record.attri_Point, target - record.attri_Point), world, limit - 1);
 
        // Si hit return true, donc, on designe une esphere avec
        // le couleur qui vient de ses normals
    }

    // Si on a pas touche une figure, on fais le couleur du monde
    obj_Vector dir_vec = unit_vector(ray.get_dir()); // Normalisation
    double hit_t = 0.5 * (dir_vec.get_value1() + 1); // hit_t = y --> range (0,1)
    return (1 - hit_t) * obj_Color(1, 1, 1) + hit_t * obj_Color(0.5, 0.7, 1); 
    // On fait le blending dans le background
}

int main() {
    
    /////////// IMAGE ///////////

    const double relation = 3/2;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / relation); // Pour change
    // que l'image_width, EFFACER CONST APRES CRÉER LA FONCTION
    const int samples_per_pixel = 100;
    const int depth = 50;


    /////////// WORLD ///////////

    figure_list world;
    world.add(make_shared<obj_Sphere>(obj_Point(0, 0, -1), 0.5));
    world.add(make_shared<obj_Sphere>(obj_Point(0, -100.5, -1), 100));


    ////////// CAMERA /////////////

    camera cam;

    /////////// RENDER ///////////

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
                color += blender_ray(color_ray, world, depth);
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