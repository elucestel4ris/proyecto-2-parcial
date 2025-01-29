#include "ILista.hpp"
#include "Libro.hpp"
#include "Autor.hpp"

void formularioInsercionAutor(ILista<Autor>* listaAutores);
void formularioBusquedaAutor(ILista<Autor>* listaAutores);
void formularioEliminacionAutor(ILista<Autor>* listaAutores, ILista<Libro>* listaLibros);

void formularioInsercionLibro(ILista<Libro>* listaLibros, ILista<Autor>* listaAutores);
void formularioBusquedaLibro(ILista<Libro>* listaLibros);
void formularioEliminacionLibro(ILista<Libro>* listaLibros);