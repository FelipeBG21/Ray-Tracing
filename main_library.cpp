#include "library.h"

int main() {

    figure_list world;
    vapi_RT objeto;
    objeto.create_image(400, 100, 50, 3.0/2.0);
    objeto.modify_view(obj_Point(0, 2, 1), obj_Point(0, 0, -1), obj_Vector(0, 1, 0), 90, obj_Color(1, 1, 1));
    objeto.create_sphere(5.0, 0, 0, -1, world);
    objeto.render_world(world);
};