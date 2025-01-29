#include "Autor.hpp"

Autor::Autor(std::string cedula, std::string nombre, std::string nombre2, std::string apellido)
    : cedula(cedula), nombre(nombre), nombre2(nombre2), apellido(apellido)
{
}

const std::string& Autor::getId() const
{
    return cedula;
}

const std::string& Autor::getNombre() const
{
    return nombre;
}

const std::string& Autor::getNombre2() const
{
    return nombre2;
}

const std::string& Autor::getApellido() const
{
    return apellido;
}

void Autor::setId(const std::string& nuevoId)
{
    cedula = nuevoId;
}

void Autor::setNombre(const std::string& nuevoNombre)
{
    nombre = nuevoNombre;
}

void Autor::setNombre2(const std::string& nuevoNombre2)
{
    nombre2 = nuevoNombre2;
}

void Autor::setApellido(const std::string& nuevoApellido)
{
    apellido = nuevoApellido;
}

bool Autor::operator==(const Autor& otroAutor) const
{
    return cedula == otroAutor.cedula;
}

std::ostream& operator<<(std::ostream& os, const Autor& autor)
{
    os << "Id: " << autor.cedula << ", Nombre: " << autor.nombre << ", Nombre2: " << autor.nombre2 << ", Apellido: " << autor.apellido << ", Libros: (";

    Nodo<Libro*>* aux = autor.libros.getCabeza();
    while (aux != NULL)
    {
        os << aux->getDato()->getTitulo() << ",";
        aux = aux->getSiguiente();
    }

    os << "\b)";

    return os;
}
