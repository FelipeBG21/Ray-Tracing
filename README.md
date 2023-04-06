# Introduction to Ray-Tracing vani_RT

This is the Ray Tracer by Vanessa Lopez and Felipe, also known as vapi_RT for its abbreviation to the nicknames Vane and Pipe plus the RT which stand for Ray Tracing. Created with the guides from the series "Ray tracing In One Weekend Series", we'll leave the aknowlegments at the end of the document:

This ray tracer was created for the IN204 class proyect and its intented to have a graphic interface from where different objects can be created and then rendered, other implementation was a convertion an exportable library so it can be use by other programmers in their codes and finally it can be implemented from a main.cpp like a normal C++ code.

Those are the objectfis and at the time of writting this README.md, the two first ones haven't been met.

This Ray Tracer creates images based in the RGB color code and they are expresed as image.ppm file

# Basic code, without interface or library like importation

1. Requirements

    This version doesn't have any specifyque requirements

2. Executing

    In this implementation the code will give us an image.ppm file which we will need to read with the help of other tool, we do not provide such a tool but we recommend you the next 
    website that will do just fine : https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html

    For executing the code you must run the next command line in the terminal :
        g++ main.cpp

    Which will create an executable file normally named "a.out" or "a.exe", < be careful, this may not always be the case >.

    After locating the executable file, < with whatever name it was created, we will asume "a.out" >, we must execute the next command line to create a file image.ppm :
        In Ubunto : ./a.out > image.ppm  
        In Windows : .\a.out > image.ppm  

    You should see a countdown that represents the programm progress. Once you see the "Done." message you will have an image.ppm file that you can read in the given website or any 
    other reader you find.

3. Glossary

    Create diffuse material :
        For creating an opaque material that could be implemented in the objects you will create.
        The values specified define the color.

            auto diffuse_material = make_shared<mat_Diffu>(obj_Color(Color_value_R, Color_value_G, Color_value_B))

        with
            Color_value_R [0.0, 1.0] being 0 = 0 and 1 = 255
            Color_value_G [0.0, 1.0] being 0 = 0 and 1 = 255
            Color_value_B [0.0, 1.0] being 0 = 0 and 1 = 255

    Create metal material :
        For creating a metallic material that could be implemented in the objects you will create.
        The values specified define the color and the reflectivity 

            auto metal_material = make_shared<mat_Met>(obj_Color(Color_value_R, Color_value_G, Color_value_B), Reflectivity)

        with
            Color_value_R [0.0, 1.0] being 0 = 0 and 1 = 255
            Color_value_G [0.0, 1.0] being 0 = 0 and 1 = 255
            Color_value_B [0.0, 1.0] being 0 = 0 and 1 = 255

            Reflectivity [0.0, 1.0] being 0 the highest reflectivity and 1 the lowest

    Create source of light :
        For creating a source of light that could be implemented in the objects you will create.
        The values specified define the potence of the ligth

            auto ligth_material = make_shared<mat_Ligth>(obj_Color(Color_value_R, Color_value_G, Color_value_B))

        with
            Color_value_R [0.0, 1.0] being 0 = 0 and 1 = 255
            Color_value_G [0.0, 1.0] being 0 = 0 and 1 = 255
            Color_value_B [0.0, 1.0] being 0 = 0 and 1 = 255

    Create image material :
        For creating a material with an image as texture that could be implemented in the objects you will create.
        The value specified is an image as texture

            auto image_material = make_shared<mat_Diffu>(image_texture);

        with
            image_texture is a type of texture previusly defined --> go to Create image texture

    Create image texture :
        For creating a texture as an image that could be implemented in materials type image material.
        The value specified is an image.jpg

            auto image_texture = make_shared<mat_Image>("image.jpg")

        with
            image.jpg as the image you want as texture

    Create a sphere :
        For creating an sphere that will be shown in the scene after rendering
        The values specified define the center coordinates, the radius and the material of the sphere

            make_shared<obj_Sphere>(obj_Point(Coordinate_x, Coodinate_y, Coordinate_z), Radius, Material)
        
        with
            Coordinate_x [-inf, inf] type double
            Coordinate_y [-inf, inf] type double
            Coordinate_z [-inf, inf] type double
            Radius [0.0, inf]
            Material [material] previously defined

    Create a plane in XY:
        For creating a plane that will be shown in the scene after rendering
        The values specified define the coordinates to calculate the plane size in x and y, the z value where it will be position and the material

            make_shared<obj_XYrect>(Coordinate_x0, Coordinate_x1, Coordinate_y0, Coordinate_y1, Coordinate_z, Material)
        
        with
            Coordinate_x0 [-inf, inf] type double, being use to calculate the plane size --> Coordinate_x1 - Coodinate_x0 = size in x plane
            Coordinate_x1 [-inf, inf] type double,
            Coordinate_y0 [-inf, inf] type double, being use to calculate the plane size --> Coordinate_y1 - Coodinate_y0 = size in y plane
            Coordinate_y1 [-inf, inf] type double,
            Coordinate_z [-inf, inf] type double, being the point in the axis z where the plane will be rendered
            Material [material] previously defined

    Create a plane in XZ:
        For creating a plane that will be shown in the scene after rendering
        The values specified define the coordinates to calculate the plane size in x and z, the y value where it will be position and the material

            make_shared<obj_XYrect>(Coordinate_x0, Coordinate_x1, Coordinate_z0, Coordinate_z1, Coordinate_y, Material)
        
        with
            Coordinate_x0 [-inf, inf] type double, being use to calculate the plane size --> Coordinate_x1 - Coodinate_x0 = size in x plane
            Coordinate_x1 [-inf, inf] type double,
            Coordinate_z0 [-inf, inf] type double, being use to calculate the plane size --> Coordinate_z1 - Coodinate_z0 = size in z plane
            Coordinate_z1 [-inf, inf] type double,
            Coordinate_y [-inf, inf] type double, being the point in the axis y where the plane will be rendered
            Material [material] previously defined

    Create a plane in YZ:
        For creating a plane that will be shown in the scene after rendering
        The values specified define the coordinates to calculate the plane size in y and z, the x value where it will be position and the material

            make_shared<obj_XYrect>(Coordinate_y0, Coordinate_y1, Coordinate_z0, Coordinate_z1, Coordinate_x, Material)
        
        with
            Coordinate_y0 [-inf, inf] type double, being use to calculate the plane size --> Coordinate_y1 - Coodinate_y0 = size in y plane
            Coordinate_y1 [-inf, inf] type double,
            Coordinate_z0 [-inf, inf] type double, being use to calculate the plane size --> Coordinate_z1 - Coodinate_z0 = size in z plane
            Coordinate_z1 [-inf, inf] type double,
            Coordinate_x [-inf, inf] type double, being the point in the axis x where the plane will be rendered
            Material [material] previously defined

    Create a cube:
        For creating a cube that will be shown in the scene after rendering
        The values specified define the coordinates to calculate the size and placement in the carthesian plane and the material of the cube

            make_shared<obj_Cube>(obj_Point(Coordinate_x0, Coordinate_y0, Coordinate_z0), obj_Point(Coordinate_x1, Coordinate_y1, Coordinate_z1), Material)
        
        with
            Coordinate_x0 [-inf, inf] type double
            Coordinate_y0 [-inf, inf] type double
            Coordinate_z0 [-inf, inf] type double
            Coordinate_x1 [-inf, inf] type double
            Coordinate_y1 [-inf, inf] type double
            Coordinate_z1 [-inf, inf] type double
            Material [material] previously defined

            where 6 planes are created like this:
                plane 1 : size in x plane =      Coordinate_x1 - Coodinate_x0
                          size in y plane =      Coordinate_y1 - Coodinate_y0
                          position in plane z =  Coodinate_z0

                plane 2 : size in x plane =      Coordinate_x1 - Coodinate_x0
                          size in y plane =      Coordinate_y1 - Coodinate_y0
                          position in plane z =  Coodinate_z1

                plane 3 : size in x plane =      Coordinate_x1 - Coodinate_x0
                          size in z plane =      Coordinate_z1 - Coodinate_z0
                          position in plane y =  Coodinate_y0

                plane 4 : size in x plane =      Coordinate_x1 - Coodinate_x0
                          size in z plane =      Coordinate_z1 - Coodinate_z0
                          position in plane y =  Coodinate_y1

                plane 5 : size in y plane =      Coordinate_y1 - Coodinate_y0
                          size in z plane =      Coordinate_z1 - Coodinate_z0
                          position in plane x =  Coodinate_x0

                plane 5 : size in y plane =      Coordinate_y1 - Coodinate_y0
                          size in z plane =      Coordinate_z1 - Coodinate_z0
                          position in plane x =  Coodinate_x1
                        
    Add to world :
        For adding object to wold
        
            world.add(make_shared<Object>(obj_Point(130, 0, 65), obj_Point(295, 165, 230), white));

# Interface

1. Executing


# Library vapi_Rt

1. Executing







# Acknowledgments
Thanks to the guides from the series "Ray tracing In One Weekend Series" and their creators.

Title (series): “Ray Tracing in One Weekend Series”
Title (book): “Ray Tracing in One Weekend”
Author: Peter Shirley
Editors: Steve Hollasch, Trevor David Black
Version/Edition: v3.2.3
Date: 2020-12-07
URL (series): https://raytracing.github.io/
URL (book): https://raytracing.github.io/books/RayTracingInOneWeekend.html


