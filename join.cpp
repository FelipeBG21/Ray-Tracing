// Le code s'execute avec "g++ main.cpp" apres, ".\a.exe > image.ppm" 
// important .\a.exe puet etre reemplacé pour le fichier créé par "g++ main.cpp"

#include <iostream>
#include <ctime>
#include <vector>
#include "Screen_copy.hpp"
#include <SFML/Graphics.hpp>
#include <fstream>

#include "mousequeherramientas.h"
#include "camera.h"
#include "color.h"
#include "figure_list.h"
#include "rectangle.h"
//#include "node.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"
#include "cube.h"


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


bool render(const double relation, const int image_width, const int samples_per_pixel, const int depth, obj_Cam cam, obj_Color world_Color, figure_list world){
    
    //clock_t start = clock();

    const int image_height = static_cast<int>(image_width / relation); // Pour change
    //std::vector<obj_Color> listcolor;

    // Abrir archivo de salida
    std::ofstream outfile;
    outfile.open("image.ppm");

    outfile << "P3\n" << image_width << " " << image_height << "\n255\n";

    
    //std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    //#pragma omp parallel for shared(world) private(color) num_threads(12) //collapse(1)
    for (int j = image_height-1; j >= 0; --j) {
        //std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush; // Pour voir le progres
        for (int i = 0; i < image_width; ++i) {
            obj_Color color(0, 0, 0);
            for (int n = 0; n < samples_per_pixel; ++n) { // Samples_per_pixel = Rayons par pixel
                // Normalisation
                double u = (double(i) + random_double()) / (image_width - 1);
                double v = (double(j) + random_double()) / (image_height - 1);
                obj_Ray color_ray = cam.coloring_ray(u, v);
                color += blender_ray(color_ray, world_Color, world, depth); // Reescribo el color (un solo rayo)
            }
            std::cout << color.get_value0() << ' ' << color.get_value1() << ' ' << color.get_value2() << std::endl;

            double r = color.get_value0();
            double g = color.get_value1();
            double b = color.get_value2();

            //diviser le couleur par le numero des samples et gamma-correct avec gamma = 2
            r = sqrt(r/samples_per_pixel);
            g = sqrt(g/samples_per_pixel);
            b = sqrt(b/samples_per_pixel);
            

            //std::cout << r << ' ' << g << ' ' << b << std::endl;
            outfile << static_cast<int>(256 * clamp(r, 0.00, 0.999)) << ' '
                    << static_cast<int>(256 * clamp(g, 0.00, 0.999)) << ' '
                    << static_cast<int>(256 * clamp(b, 0.00, 0.999)) << '\n';

        }
    }
    //color_printer(std::cout, listcolor, samples_per_pixel); // Entender mejor
    outfile.close();
    std::cerr << "\nDone.\n";
    bool rend_fin=true;
    return rend_fin;
};

int main() {
    //////////////// IMAGE ////////////////
    double relation = 3/2;
    int image_width = 400;
    int samples_per_pixel = 100;
    const int depth = 50;


    //////////////// WORLD ////////////////
    figure_list world;

    obj_Point look_From;
    obj_Point look_At;
    obj_Vector up_Vec(0, 1, 0);
    double vert_FOV = 85;
    double focus = 0;
    double dist = 21;
    
    obj_Color world_Color(1, 1, 1);
    look_From = obj_Point(1, 0, 1);
    look_At = obj_Point(0, 0, -1);
      

    int image_height = static_cast<int>(image_width / relation);

    ////////////////  CAMERA ////////////////
    obj_Cam cam(look_From, look_At, up_Vec, vert_FOV, relation, focus, dist);

    Screen Screen(800, 600, "SFML Window");
    bool rend = false;

    Button B1(sf::Vector2f(50.f, 200.f), sf::Vector2f(200.f, 70.f),"Sphere", sf::Color::White, sf::Color::Red);
    Button B2(sf::Vector2f(50.f, 350.f), sf::Vector2f(200.f, 70.f),"Cube", sf::Color::White, sf::Color::Red);
    DropDownListMat D1({"Metal", "Diffuse", "Light"}, 200.f, 50.f, sf::Vector2f(50.f, 50.f));
    ProgressBar progressBar(sf::Vector2f(350.0f, 170.0f), sf::Vector2f(100.0f, 20.0f), 0.0f, 1.0f, 0.0f);
    ColorPaq Paquet(13,9,25);
    InputText inputText(24, sf::Color::Red, sf::Vector2f(630, 400));
    Button Upload(sf::Vector2f(50.f, 550.f), sf::Vector2f(300.f, 40.f),"Upload", sf::Color::Yellow, sf::Color::Transparent);
    Button Play(sf::Vector2f(450.f, 550.f), sf::Vector2f(300.f, 40.f),"Play", sf::Color::Green, sf::Color::Transparent);
    CounterButton button(sf::Vector2f(50, 470), sf::Vector2f(170, 50), "Ratio: ", sf::Color::Red, sf::Color::White);

    while (Screen.window_.isOpen())
    {
        //Default
        sf::Font font;
        font.loadFromFile("Fonts/Metropolian-Display.ttf");
        
        //Material
        sf::Text Conf("Material", font, 30);
        Conf.setPosition(500 , 70);
        Conf.setFillColor(sf::Color::White);

        //Conf obj
        sf::Text Conf1("Objet configurations", font, 40);
        Conf1.setPosition(315 , 20);
        Conf1.setFillColor(sf::Color::White);

        //Color
        sf::Text Conf2("Color", font, 40);
        Conf2.setPosition(370 , 220);
        Conf2.setFillColor(sf::Color::White);

        //Position
        sf::Text Conf3("Position", font, 40);
        Conf3.setPosition(620 , 280);
        Conf3.setFillColor(sf::Color::White);

        //Position2 
        sf::Text Conf4("Objet", font, 40);
        Conf4.setPosition(640 , 320);
        Conf4.setFillColor(sf::Color::White);    

        //Position3
        sf::Text Conf5("Write x , y , z", font, 20);
        Conf5.setPosition(620 , 450);
        Conf5.setFillColor(sf::Color::Red);    

        sf::Event evento;
        while (Screen.window_.pollEvent(evento)) 
        {
            
            sf::Vector2i mousePosition = sf::Mouse::getPosition(Screen.window_);
            sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
            if (evento.type == sf::Event::Closed || (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) 
            {
                Screen.window_.close();
            }   
            
            double progress = progressBar.getValue();
            B1.handleEvents(evento, mousePositionF); // Handle events for the button
            B2.handleEvents(evento, mousePositionF); // Handle events for the button
            Upload.handleEvents(evento, mousePositionF); // Handle events for the button
            Play.handleEvents(evento, mousePositionF); // Handle events for the button
            D1.handleEvents(evento, mousePositionF, progress); // Handle events for the DropdownList
            progressBar.handleEvent(evento);
            Paquet.handleEvent(evento, mousePosition);
            std::vector<int> col = Paquet.getRGB();
            inputText.handleEvent(evento);     
            button.handleEvent(evento, mousePositionF);

            if(Paquet.getColSel()==true){
                std::cout << "Color: " << col[0] << " " << col[1] << " " << col[2] << std::endl;
            }
          
            //Make sure that only one button is clicked
            if(B1.click()==true){
                B2.setRectClic(false);
            }
            if(B2.click()==true){
                B1.setRectClic(false);
            }
            //Render shpere  
            if (B1.click()==true && D1.getSelectedIndex()!=-1 && Paquet.getColSel()==true && inputText.getInputString()!=" " && Upload.click()==true) {
                std::vector<double> centro = inputText.getInputStringToInt();
                //std::cout << "EXITOOOO" << std::endl;
                if (D1.getSelectedIndex()==0) {
                    //std::cout << "Metal" << std::endl;
                    std::cout  << progress << std::endl;
                    world.add(make_shared<obj_Sphere>(obj_Point(centro[0],centro[1],centro[2]), button.count, make_shared<mat_Met>(obj_Color(col[0]/255.0, col[1]/255.0, col[2]/255.0), progress/10)));
                    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100, make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 1)));
                }
                else if (D1.getSelectedIndex()==1) {
                    //std::cout << "Diffuse" << std::endl;
                    world.add(make_shared<obj_Sphere>(obj_Point(centro[0],centro[1],centro[2]), button.count, make_shared<mat_Diffu>(obj_Color(col[0]/255.0, col[1]/255.0, col[2]/255.0))));
                }
                else if (D1.getSelectedIndex()==2) {
                    //std::cout << "Light" << std::endl;
                    world.add(make_shared<obj_Sphere>(obj_Point(centro[0],centro[1],centro[2]), button.count, make_shared<mat_Ligth>(obj_Color(1,1,1))));
                }

                B1.setRectClic(false);
                Paquet.col_sel=false;
                inputText.sel_string=" ";
                Upload.setRectClic(false);

            }
            if(B2.click()==true && D1.getSelectedIndex()!=-1 && Paquet.col_sel==true && inputText.getInputString()!=" " && Upload.click()==true){
                std::vector<double> centro = inputText.getInputStringToInt();
    
                if (D1.getSelectedIndex()==0) {
                    //std::cout << "Metal" << std::endl;
                    std::cout  << progress << std::endl;
                    world.add(make_shared<obj_Cube>(obj_Point(centro[0],centro[1],centro[2]), obj_Point(centro[3],centro[4],centro[5]), make_shared<mat_Met>(obj_Color(col[0]/255.0, col[1]/255.0, col[2]/255.0), progress/10)));
                    //world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 300, make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 1)));
                    
                }
                else if (D1.getSelectedIndex()==1) {
                    //std::cout << "Diffuse" << std::endl;
                    
                    world.add(make_shared<obj_Cube>(obj_Point(centro[0],centro[1],centro[2]), obj_Point(centro[3],centro[4],centro[5]), make_shared<mat_Diffu>(obj_Color(col[0]/255.0, col[1]/255.0, col[2]/255.0))));
                    world.add(make_shared<obj_Sphere>(obj_Point( 0.0, -100.5, -1.0), 100, make_shared<mat_Met>(obj_Color(0.8, 0.8, 0.8), 1)));
                    
                }
                else if (D1.getSelectedIndex()==2) {
                    //std::cout << "Light" << std::endl;
                    world.add(make_shared<obj_Cube>(obj_Point(centro[0],centro[1],centro[2]), obj_Point(centro[3],centro[4],centro[5]), make_shared<mat_Ligth>(obj_Color(1, 1, 1))));
                
                }
                B2.setRectClic(false);
                Paquet.col_sel=false;
                inputText.sel_string=" ";
                Upload.setRectClic(false);
                
            }

            if (Play.click()==true) {
                std::cout << "Play" << std::endl;
                rend = true;
                Screen.window_.close();
            }
            
        }
        
        Screen.window_.clear();
        Screen.window_.draw(Conf);
        Screen.window_.draw(Conf1);
        Screen.window_.draw(Conf2);
        Screen.window_.draw(Conf3);
        Screen.window_.draw(Conf4);
        Screen.window_.draw(Conf5);
        B1.render(Screen.window_); // Render the button
        B2.render(Screen.window_); // Render the button
        D1.render(Screen.window_); // Render the DropdownList
        Upload.render(Screen.window_); // Render the button
        Play.render(Screen.window_); // Render the button
        inputText.render(Screen.window_);
        button.render(Screen.window_);
        bool Op=D1.getOpen();
        if (Op==true) {
            progressBar.render(Screen.window_);
        }
        Paquet.render(Screen.window_);
        Screen.window_.display();
    }
    
    ///////////////////////////////// RENDER /////////////////////////////////

    //int num_threads = std::thread::hardware_concurrency();
    bool rend_fin = false;
    //std::cerr << "Number of threads: " << num_threads << std::endl;
    if (Screen.window_.isOpen()==false && rend==true){
        clock_t start = clock();
        rend_fin=render(relation,image_width,samples_per_pixel,depth,cam, world_Color, world);
        std::cerr << "Done." << std::endl;
        //render_parallelise(num_threads,3/2,400,100,50,cam, world);
        clock_t end = clock();
        double duration = (double)(end - start) / CLOCKS_PER_SEC;

        std::cerr << "Time taken by method: " << duration << " seconds" << std::endl;
    }

    if(rend_fin==true){
        system("convert image.ppm -scale 700x result.png");
        Image img("result.png",50.f,50.f);
        
        sf::RenderWindow newWindow(sf::VideoMode(800, 800), "Nueva ventana de SFML");
  

        while (newWindow.isOpen())
        {
            sf::Event evento;
            while (newWindow.pollEvent(evento)) 
            {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(newWindow);
                sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
                if (evento.type == sf::Event::Closed || (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape)) 
                {
                    newWindow.close();
                }
            }
            
            newWindow.clear();
            img.render(newWindow); // Render the image
            newWindow.display();
        }
    }
    

    return 0;
};
