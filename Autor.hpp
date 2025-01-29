#pragma once

#include "ListaDoble.hpp"
#include "Libro.hpp"
#include <string>

class Autor
{
private:
    std::string cedula;
    std::string nombre;
    std::string nombre2;
    std::string apellido;
public:
    ListaDoble<Libro*> libros;

public:
    Autor(std::string cedula, std::string nombre, std::string nombre2, std::string apellido);
    const std::string& getId() const;
    const std::string& getNombre() const;
    const std::string& getNombre2() const;
    const std::string& getApellido() const;
    void setId(const std::string& nuevoId);
    void setNombre(const std::string& nuevoNombre);
    void setNombre2(const std::string& nuevoNombre2);
    void setApellido(const std::string& nuevoApellido);

    bool operator==(const Autor& otroAutor) const;

    friend std::ostream& operator<<(std::ostream& os, const Autor& autor);

};