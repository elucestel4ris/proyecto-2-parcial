#pragma once

#include "ILista.hpp"
#include "Nodo.hpp"
#include <string>
#include <iostream>
#include <functional>

template <typename T>
class ListaSimple : public ILista<T>
{
private:
    Nodo<T>* cabeza;
public:
    ListaSimple();
    ListaSimple(const ListaSimple& otraLista);
    ListaSimple(std::initializer_list<T> valoresIniciales);
    ~ListaSimple();
    Nodo<T>* getCabeza() const override;
    Nodo<T>* getCola() const override;

    void insertarACabeza(const T& dato) override;
    void insertarACola(const T& dato) override;
    void insertar(const T& dato, int posicion) override;
    void intercambiar(int pos1, int pos2) override;

    int buscar(const T& dato) const override;
    const T& conseguirDato(int posicion) const override;
    Nodo<T>* conseguirNodo(int posicion) const override;

    bool eliminar(const T& dato);
    void eliminarPos(int posicion);

    int contar() const;

    void mostrar(const std::string& separador = " => ") const;

    void executarEnLista(std::function<void(T)> funcion) override;
};

template<typename T>
inline ListaSimple<T>::ListaSimple()
    : cabeza(NULL)
{
}

template<typename T>
inline ListaSimple<T>::ListaSimple(const ListaSimple& otraLista)
    : cabeza(NULL)
{
    Nodo<T>* aux = otraLista.getCabeza();
    while (aux != NULL)
    {
        insertarACola(aux->getDato());
        aux = aux->getSiguiente();
    }
}

template<typename T>
inline ListaSimple<T>::ListaSimple(std::initializer_list<T> valoresIniciales)
    : cabeza(NULL)
{
    for (auto valor : valoresIniciales)
        insertarACola(valor);
}

template<typename T>
inline ListaSimple<T>::~ListaSimple()
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
inline Nodo<T>* ListaSimple<T>::getCabeza() const
{
    return cabeza;
}

template<typename T>
inline Nodo<T>* ListaSimple<T>::getCola() const
{
    Nodo<T>* aux = cabeza;

    while (aux->siguiente != NULL)
    {
        aux = aux->siguiente;
    }

    return aux;
}

template<typename T>
inline void ListaSimple<T>::insertarACabeza(const T& dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL)
    {
        cabeza = nuevo;
    }
    else
    {
        nuevo->setSiguiente(cabeza);
        cabeza = nuevo;
    }
}

template<typename T>
inline void ListaSimple<T>::insertarACola(const T& dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL)
        cabeza = nuevo;
    else
    {
        Nodo<T>* aux = cabeza;

        while (aux->siguiente != NULL)
            aux = aux->siguiente;

        aux->siguiente = nuevo;
    }
}

template<typename T>
inline void ListaSimple<T>::insertar(const T& dato, int posicion)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);
    Nodo<T>* anterior = NULL;

    if (cabeza == NULL)
    {
        cabeza = nuevo;
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
        {
            anterior = aux;
            aux = aux->siguiente;
        }

        nuevo->siguiente = aux;

        if (anterior != NULL)
            anterior->siguiente = nuevo;
    }
}

template<typename T>
inline void ListaSimple<T>::intercambiar(int pos1, int pos2)
{
    Nodo<T>* nodo1 = conseguirNodo(pos1);
    Nodo<T>* nodo1Ant = NULL;
    if (pos1 > 0)
        nodo1Ant = conseguirNodo(pos1 - 1);

    Nodo<T>* nodo2 = conseguirNodo(pos2);
    Nodo<T>* nodo2Ant = NULL;
    if (pos2 > 0)
        nodo2Ant = conseguirNodo(pos2 - 1);

    if (nodo1 == cabeza)
        cabeza = nodo2;
    else if (nodo2 == cabeza)
        cabeza = nodo1;

    if (nodo1Ant != NULL)
        nodo1Ant->siguiente = nodo2;
    if (nodo2Ant != NULL && nodo2Ant != nodo1)
        nodo2Ant->siguiente = nodo1;

    Nodo<T>* nodo1Sig = nodo1->siguiente;

    nodo1->siguiente = nodo2->siguiente;

    if (nodo2 != nodo1Sig)
        nodo2->siguiente = nodo1Sig;
    else
        nodo2->siguiente = nodo1;
}

template<typename T>
inline int ListaSimple<T>::buscar(const T& dato) const
{
    Nodo<T>* aux = cabeza;
    int posicion = 0;

    while (aux != NULL)
    {
        if (aux->dato == dato)
            return posicion;

        aux = aux->siguiente;
        posicion++;
    }

    return -1;
}

template<typename T>
inline const T& ListaSimple<T>::conseguirDato(int posicion) const
{
    return conseguirNodo(posicion)->dato;
}

template<typename T>
inline Nodo<T>* ListaSimple<T>::conseguirNodo(int posicion) const
{
    Nodo<T>* aux = cabeza;

    for (posicion; posicion > 0; posicion--) // ERROR INTENCIONADO SI ESTÁ FUERA DE RANGO
        aux = aux->siguiente;

    return aux;
}

template<typename T>
inline bool ListaSimple<T>::eliminar(const T& dato)
{
    Nodo<T>* aux = cabeza;
    Nodo<T>* anterior = NULL;

    while (aux != NULL)
    {
        if (aux->dato == dato)
        {
            if (aux == cabeza)
                cabeza = aux->siguiente;
            else
                anterior->siguiente = aux->siguiente;

            delete aux;
            return true;
        }

        anterior = aux;
        aux = aux->siguiente;
    }

    return false;
}

template<typename T>
inline void ListaSimple<T>::eliminarPos(int posicion)
{
    Nodo<T>* aux = conseguirNodo(posicion);
    Nodo<T>* auxAnt = NULL;
    if (posicion > 0)
        auxAnt = conseguirNodo(posicion - 1);


    if (aux == cabeza)
        cabeza = aux->siguiente;
    else
    {
        if (auxAnt)
            auxAnt->siguiente = aux->siguiente;
    }
    delete aux;
}

template<typename T>
inline int ListaSimple<T>::contar() const
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
inline void ListaSimple<T>::mostrar(const std::string& separador) const
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        std::cout << aux->dato << separador;
        aux = aux->siguiente;
    }

    std::cout << "NULL";
}

template<typename T>
inline void ListaSimple<T>::executarEnLista(std::function<void(T)> funcion)
{
    Nodo<T>* aux = cabeza;

    while (aux != NULL)
    {
        funcion(aux->dato);

        aux = aux->siguiente;
    }
}