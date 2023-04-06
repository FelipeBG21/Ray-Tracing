# Introduction to Ray-Tracing vani_RT

This is the Ray Tracer by Vanessa Lopez and Felipe, also known as vapi_RT for its abbreviation to the nicknames Vane and Pipe plus the RT which stand for Ray Tracing. Created with the guides from the series "Ray tracing In One Weekend Series", we'll leave the aknowlegments at the end of the document:

This ray tracer was created for the IN204 class proyect and its intented to have a graphic interface from where different objects can be created and then rendered, other implementation was a convertion an exportable library so it can be use by other programmers in their codes and finally it can be implemented from a main.cpp like a normal C++ code.

Those are the objectfis and at the time of writting this README.md, the two first ones haven't been met.

This Ray Tracer creates images based in the RGB color code and they are expresed as image.ppm file

# Suggestions:

* The vision system handles a concept of camera location and image direction which is where the camera points to perceive the scene.
* It is strongly recommended to place the camera nearby in the rendering of spheres and move it away in the rendering of cubes to make them large.
* The spatial location of vision can be located as the Z-axis in the direction exiting the screen, the Y up and down and finally in X to the sides.

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
        The value specified define the object you will add to world

            world.add(Object)

        with
            Object = Sphere, Cube or Plane, previously defined

    
# Interface

The graphical interface have the purpose of facilitating the use of the library and the Ray Tracing program as a more visual tool. For this project, the SFML graphic library was implemented as it is one of the most popular, which facilitates the search for documentation for the construction of the same.
There is extensive documentation on this page https://www.sfml-dev.org/documentation/2.5.1/

1. Requirements
	
	To run the graphical interface you must download these libraries:

		*   libsfml-dev
		*   imagemagick-6.q16

	For download the libraries you must run the next command line in the terminal for Linux:
		sudo apt-get install libsfml-dev    and    sudo apt install imagemagick-6.q16 

	For Windows you must follow the tutorial in the official web: https://www.sfml-dev.org/download.php

2. Executing

    In this implementation, the code will display a popup window with the graphical interface to select the objects to be rendered in Ray Tracing. Two objects are available: spheres 
    and cubes with the possibility of applying 3 types of materials: opaque, light and metallic, the last one has an adjustable bar to choose the level of reflectance, where 0 is 
    totally reflective and 1 is not. Also, there is a large color panel, a button to increase the radius measurement in the case of spheres and a positioning of objects in space. You 
    must be careful when entering the values because the sphere receives 3 float values (But also receives integer values) separated by "," and the cubes receive 6 values referring to 
    two points in each plane of space.

    For example:

        Sphere 0.0,10.0,5.6  --> It means that we are going to enter the coordinates of the center of the sphere.
        Cube 130,0,65,295,165,230  --> It means that we are going to enter the following coordinates [X0=130, Y0=0, Z0=65] and [X1=295, Y1=165,Z1=230]

        Note that the separation of the float value is with a dot and the separation between chords planes is with a comma. 

    Restrictions:
        *   It is mandatory to choose a material to make an object, if your choice is metal type, you must use the adjustable bar by clicking on it while scrolling to the desired     
            value. When selecting a size you must click on the title metal to save the selection.
        *   Object positioning: Sphere (X,Y,Z) and Cube (X0,Y0,Z0,X1,Y1,Z1) 
        *   To increase the radius of the sphere, right click on the red "Ratio" counter and to decrease, left click (This button is configured by default to have a variation of 0.1   
            but it is possible to modify this range in the GUI library).
        *   To add an object to the image you must have selected an object, a material, a color, enter coordinates and click upload to follow.
        *   Finally, to go to the rendering part and create the world, you need to click on play.

    Note: On some objects, there is a giant sphere to make the floor effect and to see the reflective properties of the materials but the user can remove it if he wants to make a free 
    design.

        For executing the code you must run the next command line in the terminal:
        
        g++ -o exe join.cpp -lsfml-graphics -lsfml-window -lsfml-system
        
    Which will create an executable file normally named "exe".

    After locating the executable file, we must execute the following line command to calculate the rays of the image, render and create an mmp file, which converts with the help of 
    the "imagemagick" library to PNG:

        In Ubunto : ./exe
        In Windows : .\exe

    Finally, the terminal throws the remaining execution time and a pop-up window appears with the rendering of the photo.

# Library vapi_Rt

A library was made for future use. Which, it is intended to standardize some actions such as creating spheres, cubes; rendering, etc. This location is located in the "Library" file


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


