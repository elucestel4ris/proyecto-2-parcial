#pragma once

#include "ILista.hpp"
#include "Nodo.hpp"
#include <string>
#include <iostream>
#include <functional>

template <typename T>
class ListaDoble : public ILista<T>
{
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
public:
    ListaDoble();
    ListaDoble(const ListaDoble& otraLista);
    ListaDoble(std::initializer_list<T> valoresIniciales);
    ~ListaDoble();
    Nodo<T>* getCabeza() const override;
    Nodo<T>* getCola() const override;

    void insertarACabeza(const T& dato) override;
    void insertarACola(const T& dato) override;
    void insertar(const T& dato, int posicion) override;
    void intercambiar(int pos1, int pos2) override;

    int buscar(const T& dato) const override;
    const T& conseguirDato(int posicion) const override;
    Nodo<T>* conseguirNodo(int posicion) const override;

    bool eliminar(const T& dato) override;
    void eliminarPos(int posicion) override;

    int contar() const override;

    void mostrar(const std::string& separador = " <=> ") const override;

    void executarEnLista(std::function<void(T)> funcion) override;
};

template<typename T>
inline ListaDoble<T>::ListaDoble()
    : cabeza(NULL),
    cola(NULL)
{
}

template<typename T>
inline ListaDoble<T>::ListaDoble(const ListaDoble& otraLista)
    : cabeza(NULL),
    cola(NULL)
{
    Nodo<T>* aux = otraLista.getCabeza();
    while (aux != NULL)
    {
        insertarACola(aux->getDato());
        aux = aux->getSiguiente();
    }
}

template<typename T>
inline ListaDoble<T>::ListaDoble(std::initializer_list<T> valoresIniciales)
    : cabeza(NULL),
    cola(NULL)
{
    for (auto valor : valoresIniciales)
        insertarACola(valor);
}

template<typename T>
inline ListaDoble<T>::~ListaDoble()
{
    Nodo<T>* aEliminar;
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        aEliminar = aux;
        aux = aux->getSiguiente();
        delete aEliminar;
    }
}

template<typename T>
inline Nodo<T>* ListaDoble<T>::getCabeza() const
{
    return cabeza;
}

template<typename T>
inline Nodo<T>* ListaDoble<T>::getCola() const
{
    return cola;
}

template<typename T>
inline void ListaDoble<T>::insertarACabeza(const T& dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL)
    {
        cabeza = nuevo;
        cola = nuevo;
    }
    else
    {
        cabeza->setAnterior(nuevo);
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
}

template<typename T>
inline void ListaDoble<T>::insertarACola(const T& dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL) {
        cabeza = nuevo;
        cola = nuevo;
    }
    else {
        cola->setSiguiente(nuevo);
        nuevo->setAnterior(cola);
        cola = nuevo;
    }
}

template<typename T>
inline void ListaDoble<T>::insertar(const T& dato, int posicion)   // EDITAR LUEGO
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL)
    {
        cabeza = nuevo;
        cola = nuevo;
    }
    else
    {
        int cantidad = contar();

        if (posicion < 0)
            posicion = cantidad + posicion;

        if (posicion > cantidad)
            posicion = cantidad;

        Nodo<T>* aux = cabeza;

        for (posicion; posicion > 0; posicion--)
            aux = aux->siguiente;

        nuevo->anterior = aux->anterior;

        nuevo->siguiente = aux;

        if (aux->anterior != NULL)
            aux->anterior->siguiente = nuevo;

        aux->anterior = nuevo;
    }
}

template<typename T>
inline void ListaDoble<T>::intercambiar(int pos1, int pos2)
{
    Nodo<T>* nodo1 = conseguirNodo(pos1);
    Nodo<T>* nodo2 = conseguirNodo(pos2);

    if (nodo1 == cabeza)
        cabeza = nodo2;
    else if (nodo2 == cabeza)
        cabeza = nodo1;

    if (nodo1 == cola)
        cola = nodo2;
    else if (nodo2 == cola)
        cola = nodo1;

    if (nodo1->anterior != NULL)
        nodo1->anterior->siguiente = nodo2;
    if (nodo1->siguiente != NULL && nodo1->siguiente != nodo2)
        nodo1->siguiente->anterior = nodo2;
    if (nodo2->anterior != NULL && nodo2->anterior != nodo1)
        nodo2->anterior->siguiente = nodo1;
    if (nodo2->siguiente != NULL)
        nodo2->siguiente->anterior = nodo1;

    Nodo<T>* nodo1Ant = nodo1->anterior;
    Nodo<T>* nodo1Sig = nodo1->siguiente;

    if (nodo1 != nodo2->anterior)
        nodo1->anterior = nodo2->anterior;
    else
        nodo1->anterior = nodo2;

    nodo1->siguiente = nodo2->siguiente;

    nodo2->anterior = nodo1Ant;

    if (nodo2 != nodo1Sig)
        nodo2->siguiente = nodo1Sig;
    else
        nodo2->siguiente = nodo1;

}

template<typename T>
inline int ListaDoble<T>::buscar(const T& dato) const
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        if (aux->dato == dato)
            return true;

        aux = aux->siguiente;
    }

    return false;
}

template<typename T>
inline const T& ListaDoble<T>::conseguirDato(int posicion) const
{
    return conseguirNodo(posicion)->dato;
}

// PRECAUCIÓN AL USAR ESTA FUNCIÓN FUERA DE LA ESTRUCTURA
template<typename T>
inline Nodo<T>* ListaDoble<T>::conseguirNodo(int posicion) const
{
    Nodo<T>* aux = cabeza;

    for (posicion; posicion > 0; posicion--) // ERROR INTENCIONADO SI ESTÁ FUERA DE RANGO
        aux = aux->siguiente;

    return aux;
}

template<typename T>
inline bool ListaDoble<T>::eliminar(const T& dato)
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        if (aux->dato == dato)
        {
            if (aux == cabeza)
                cabeza = aux->siguiente;
            else
                aux->anterior->siguiente = aux->siguiente;

            if (aux == cola)
                cola = aux->anterior;
            else
                aux->siguiente->anterior = aux->anterior;

            delete aux;
            return true;
        }

        aux = aux->siguiente;
    }

    return false;
}

template<typename T>
inline void ListaDoble<T>::eliminarPos(int posicion)
{
    Nodo<T>* aux = conseguirNodo(posicion);

    if (aux == cabeza)
        cabeza = aux->siguiente;
    else
        aux->anterior->siguiente = aux->siguiente;

    if (aux == cola)
        cola = aux->anterior;
    else
        aux->siguiente->anterior = aux->anterior;

    delete aux;
}

template<typename T>
inline int ListaDoble<T>::contar() const
{
    int cont = 0;
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        cont++;
        aux = aux->siguiente;
    }

    return cont;
}

template<typename T>
inline void ListaDoble<T>::mostrar(const std::string& separador) const
{
    Nodo<T>* aux = cabeza;

    std::cout << "NULL" << separador;

    while (aux != NULL)
    {
        std::cout << aux->dato << separador;
        aux = aux->siguiente;
    }

    std::cout << "NULL";
}

template<typename T>
inline void ListaDoble<T>::executarEnLista(std::function<void(T)> funcion)
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        funcion(aux->dato);

        aux = aux->siguiente;
    }
}