#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <iostream>

using std::sqrt;
using std::fabs;

class obj_Vector { 
    public:
        double attri_V[3];

    public:
        // Constructores
        obj_Vector() : attri_V{0,0,0} {}
        obj_Vector(double value0, double value1, double value2) 
                    : attri_V{value0, value1, value2} 
        {}

        // Metodos get
        double get_value0() const {
            return attri_V[0];
        }

        double get_value1() const {
            return attri_V[1];
        }

        double get_value2() const {
            return attri_V[2];
        }

        // Operadores
        // Turns negative all values, but the result its a copy
        obj_Vector operator -() const {
            return obj_Vector(-attri_V[0], -attri_V[1], -attri_V[2]);
        }

        double operator [](int arg_i) const {
            return attri_V[arg_i];
        }

        // Let us use the objet directly and modifie it if we want to
        // Le & permet d'accéder à la variable double qui existe déjà
        double& operator [](int arg_i) {
            return attri_V[arg_i];
        }

        // Makes additions of himself on himself, it doesnt create a copy
        obj_Vector& operator += (const obj_Vector &arg_v) {
            attri_V[0] += arg_v.attri_V[0];
            attri_V[1] += arg_v.attri_V[1];
            attri_V[2] += arg_v.attri_V[2];
            return *this;  // Returns the same object in which the methode was called
        }

        // Makes a multiplication with a constant, it is directly to the object, not a copy
        obj_Vector& operator *= (const double arg_t) {
            attri_V[0] *= arg_t;
            attri_V[1] *= arg_t;
            attri_V[2] *= arg_t;
            return *this;  // Returns the same object in which the methode was called
        }
        
        double length_squared() const {
            return attri_V[0]*attri_V[0] + attri_V[1]*attri_V[1] + attri_V[2]*attri_V[2];
        }

        double length() const {
            return sqrt(length_squared());
        }

        bool near_zero() const { // Returns true if the vector is close to zero in all dimensions.
            const auto a = 1e-8;
            return (fabs(attri_V[0]) < a) && (fabs(attri_V[1]) < a) && (fabs(attri_V[2]) < a);
        }

        bool empty() const {
            return true;    // Define la función empty() como verdadera siempre
        }

        static obj_Vector random() { // Pour créer une random vecteur
            return obj_Vector(random_double(), random_double(), random_double());
        }

        static obj_Vector random(double min, double max) { // Pour créer une random vecteur
            return obj_Vector(random_double(min,max), random_double(min,max), random_double(min,max));
        }
        
};

// Que pour faciliter la differentiation 
using obj_Point = obj_Vector;   // 3D point
using obj_Color = obj_Vector;   // RGB color

// Definition du multiplication entre des vecteurs
obj_Vector operator *(const obj_Vector &arg_v1, const obj_Vector &arg_v2) {
    return obj_Vector(arg_v1.get_value0() * arg_v2.get_value0(), 
                        arg_v1.get_value1() * arg_v2.get_value1(), 
                        arg_v1.get_value2() * arg_v2.get_value2());
}

// Operateur pour multiplication, definition du multiplication fois un scalar
obj_Vector operator *(double arg_a, const obj_Vector &arg_v) {
    return obj_Vector(arg_a*arg_v.get_value0(), arg_a*arg_v.get_value1(), arg_a*arg_v.get_value2());
}

// REVISARRRR!!!!!, SON IGUALES, SI ALGO BORRAR EL DE ARRIBA NO EL DE ABAJO, ESCRIBEME ANTES
obj_Vector operator *(const obj_Vector &arg_v, double arg_a) {
    return arg_v * arg_a;  ;
}

// Definition de la division
obj_Vector operator /(obj_Vector arg_v, double arg_a) {
    return (1/arg_a) * arg_v;
}

// Definition du sum entre des vecteurs
obj_Vector operator +(const obj_Vector &arg_v1, const obj_Vector &arg_v2) {
    return obj_Vector(arg_v1.get_value0() + arg_v2.get_value0(), 
                        arg_v1.get_value1() + arg_v2.get_value1(), 
                        arg_v1.get_value2() + arg_v2.get_value2());
}

// Definition de la substraction entre des vecteurs
obj_Vector operator -(const obj_Vector &arg_v1, const obj_Vector &arg_v2) {
    return obj_Vector(arg_v1.get_value0() - arg_v2.get_value0(), 
                        arg_v1.get_value1() - arg_v2.get_value1(), 
                        arg_v1.get_value2() - arg_v2.get_value2());
}

double dot(const obj_Vector &arg_v1, const obj_Vector &arg_v2) {
    return arg_v1.get_value0() * arg_v2.get_value0()
            + arg_v1.get_value1() * arg_v2.get_value1()
            + arg_v1.get_value2() * arg_v2.get_value2();
}

obj_Vector cross(const obj_Vector &arg_v1, const obj_Vector &arg_v2) {
    return obj_Vector(arg_v1.get_value1() * arg_v2.get_value2() - arg_v1.get_value2() * arg_v2.get_value1(),
                        arg_v1.get_value2() * arg_v2.get_value0() - arg_v1.get_value0() * arg_v2.get_value2(),
                        arg_v1.get_value0() * arg_v2.get_value1() - arg_v1.get_value1() * arg_v2.get_value0());
}

// Unitarisation du vecteur
obj_Vector unit_vector(obj_Vector arg_v) {
    return arg_v / arg_v.length();
}

obj_Vector random_in_unit_disk() {
    while (true) {
        auto point = obj_Vector(random_double(-1, 1), random_double(-1, 1), 0);
        if (point.length_squared() >= 1) continue;
        return point;
    }
}

obj_Vector random_Unit_Sph() { // Analyse si le random vecteur est dedans une sphere unitaire, sinon, on fait "continue"
    while (true) {
        auto point = obj_Vector::random(-1, 1);
        if (point.length_squared() >= 1) continue;
        return point;
    }
}

obj_Vector random_Unit_Vec() { // On normalise le random vecteur dedans le sphere unitaire
    return unit_vector(random_Unit_Sph());
}

obj_Vector random_Hemis(const obj_Vector& arg_v) {
    obj_Vector in_unit_sphere = random_Unit_Sph();
    if (dot(in_unit_sphere, arg_v) > 0) // Dans le meme hemisphere que le normale
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

obj_Vector reflect(const obj_Vector& arg_v1, const obj_Vector& arg_v2) {
    return arg_v1 - 2*dot(arg_v1, arg_v2) * arg_v2;
}

#endif