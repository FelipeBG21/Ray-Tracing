#ifndef COLOR_H
#define COLOR_H

#include <iostream>

#include "vector.h"

void color_printer(std::ostream &out, obj_Color color, int n_Samples){
    double r = color.get_value0();
    double g = color.get_value1();
    double b = color.get_value2();

    //diviser le couleur par le numero des samples et gamma-correct avec gamma = 2
    r = sqrt(r/n_Samples);
    g = sqrt(g/n_Samples);
    b = sqrt(b/n_Samples);
    

    //std::cout << r << ' ' << g << ' ' << b << std::endl;

    //Ecrire
    out << static_cast<int>(256 * clamp(r, 0.00, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.00, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.00, 0.999)) << '\n';
}

#endif