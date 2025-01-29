#include "Libro.hpp"
#include "Autor.hpp"

// Constructor
Libro::Libro(const std::string& id, const std::string& titulo, Autor* autor, Fecha fecha)
    : id(id), titulo(titulo), autor(autor), fecha(fecha)
{
}

Libro::Libro(const std::string& id, const std::string& titulo, Autor* autor, std::time_t tiempo)
    : id(id), titulo(titulo), autor(autor), fecha(Fecha(tiempo))
{
}

Libro::Libro(const Libro& otroLibro)
    : id(otroLibro.id), titulo(otroLibro.titulo), fecha(otroLibro.fecha)
{
    autor = otroLibro.autor;
}

Libro::~Libro()
{
    // autor->libros.eliminar(this); // YA ESTARÁ DESTRUIDA, DESENLAZAR MANUALMENTE
}

// Getters y Setters
const std::string& Libro::getId() const {
    return id;
}

void Libro::setId(const std::string& nuevoId) {
    id = nuevoId;
}

const std::string& Libro::getTitulo() const {
    return titulo;
}

void Libro::setTitulo(const std::string& titulo) {
    this->titulo = titulo;
}

Autor* Libro::getAutor() const {
    return autor;
}

void Libro::setAutor(Autor* autor) {
    this->autor = autor;
}

const Fecha& Libro::getFecha() const
{
    return fecha;
}

void Libro::setFecha(const Fecha& otraFecha)
{
    fecha = otraFecha;
}

// Métodos
std::string Libro::toString() const {
    std::ostringstream oss;
    time_t tiempo = fecha.getTiempo();
    std::string tiempoStr = ctime(&tiempo);
    tiempoStr.pop_back();
    
    oss << "Id: " << id << ", Título: " << titulo << ", Autor: " << autor->getNombre() << " " << autor->getApellido()
        << ", Año: " << tiempoStr;
    return oss.str();
}

// Sobrecarga de operadores
std::ostream& operator<<(std::ostream& os, const Libro& libro) {
    os << libro.toString();
    return os;
}

std::istream& operator>>(std::istream& is, Libro& libro) {
    std::cout << "Ingrese el Id: ";
    std::getline(is, libro.id);
    std::cout << "Ingrese Título: ";
    std::getline(is, libro.titulo);
    std::cout << "Ingrese Autor: ";
    return is;
}

bool Libro::operator==(const Libro& otro) const {
    return id == otro.id;
}
