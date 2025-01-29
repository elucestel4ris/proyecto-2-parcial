#pragma once

#include<string>

class Validaciones
{
public:
    static int leerInt();
    static float leerFloat();
    static double leerDouble();
    static char leerLetra();
    static std::string leerPalabra();

    static std::string leerCedula();
    static std::string leerTitulo();
    static std::string leerNumero();
    static bool validarCedula(std::string cedula);
};