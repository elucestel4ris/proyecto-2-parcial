#pragma once

#include "Fecha.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

class Autor;

class Libro {
private:
    std::string id;
    std::string titulo;
    Fecha fecha;
    Autor* autor;

public:
    Libro(const std::string& id, const std::string& titulo, Autor* autor, Fecha fecha);
    Libro(const std::string& id, const std::string& titulo, Autor* autor, std::time_t tiempo);
    Libro(const Libro& otroLibro);
    ~Libro();
    
    const std::string& getId() const;
    void setId(const std::string& nuevoId);

    const std::string& getTitulo() const;
    void setTitulo(const std::string& titulo);

    Autor* getAutor() const;
    void setAutor(Autor* autor);

    const Fecha& getFecha() const;
    void setFecha(const Fecha& otraFecha);

    
    std::string toString() const;

    // Sobrecarga de operadores
    friend std::ostream& operator<<(std::ostream& os, const Libro& libro);
    friend std::istream& operator>>(std::istream& is, Libro& libro);

    bool operator==(const Libro& otro) const;
};