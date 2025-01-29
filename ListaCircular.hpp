#pragma once

#include "ILista.hpp"
#include "Nodo.hpp"
#include <string>
#include <iostream>
#include <functional>

template <typename T>
class ListaCircular : public ILista<T>
{
private:
    Nodo<T>* cabeza;
public:
    ListaCircular();
    ListaCircular(const ListaCircular& otraLista);
    ListaCircular(std::initializer_list<T> valoresIniciales);
    ~ListaCircular();
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
inline ListaCircular<T>::ListaCircular()
    : cabeza(NULL)
{
}

template<typename T>
inline ListaCircular<T>::ListaCircular(const ListaCircular& otraLista)
    : cabeza(NULL)
{
    Nodo<T>* aux = otraLista.getCabeza();
    if (aux != NULL)
    {
        do
        {
            insertarACola(aux->getDato());
            aux = aux->getSiguiente();
        } while (aux != otraLista.getCabeza());
    }
}

template<typename T>
inline ListaCircular<T>::ListaCircular(std::initializer_list<T> valoresIniciales)
    : cabeza(NULL)
{
    for (auto valor : valoresIniciales)
        insertarACola(valor);
}

template<typename T>
inline ListaCircular<T>::~ListaCircular()
{
    Nodo<T>* aEliminar;
    Nodo<T>* aux = cabeza;

    if (aux != NULL)
    {
        do
        {
            aEliminar = aux;
            aux = aux->getSiguiente();
            delete aEliminar;
        } while (aux != cabeza);
    }
}

template<typename T>
inline Nodo<T>* ListaCircular<T>::getCabeza() const
{
    return cabeza;
}

template<typename T>
inline Nodo<T>* ListaCircular<T>::getCola() const
{
    return cabeza->anterior;
}

template<typename T>
inline void ListaCircular<T>::insertarACabeza(const T& dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL)
    {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        cabeza = nuevo;
    }
    else
    {
        cabeza->anterior->siguiente = nuevo;
        nuevo->anterior = cabeza->anterior;
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
       
        cabeza = nuevo;
    }
}

template<typename T>
inline void ListaCircular<T>::insertarACola(const T& dato)
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        cabeza = nuevo;
    }
    else {
        cabeza->anterior->siguiente = nuevo;
        nuevo->anterior = cabeza->anterior;
        nuevo->siguiente = cabeza;
        cabeza->anterior = nuevo;
    }
}

template<typename T>
inline void ListaCircular<T>::insertar(const T& dato, int posicion)   // EDITAR LUEGO
{
    Nodo<T>* nuevo = new Nodo<T>(dato);

    if (cabeza == NULL)
    {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        cabeza = nuevo;
    }
    else
    {
        Nodo<T>* aux = cabeza;

        for (posicion; posicion > 0; posicion--)
            aux = aux->siguiente;

        nuevo->anterior = aux->anterior;

        nuevo->siguiente = aux;

        aux->anterior->siguiente = nuevo;

        aux->anterior = nuevo;
    }
}

template<typename T>
inline void ListaCircular<T>::intercambiar(int pos1, int pos2)
{
    Nodo<T>* nodo1 = conseguirNodo(pos1);
    Nodo<T>* nodo2 = conseguirNodo(pos2);

    if (nodo1 == cabeza)
        cabeza = nodo2;
    else if (nodo2 == cabeza)
        cabeza = nodo1;

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
inline int ListaCircular<T>::buscar(const T& dato) const
{
    Nodo<T>* aux = cabeza;

    if (aux != NULL)
    {
        do
        {
            if (aux->dato == dato)
                return true;

            aux = aux->siguiente;
        } while (aux != cabeza);
    }

    return false;
}

template<typename T>
inline const T& ListaCircular<T>::conseguirDato(int posicion) const
{
    return conseguirNodo(posicion)->dato;
}

// PRECAUCIÓN AL USAR ESTA FUNCIÓN FUERA DE LA ESTRUCTURA
template<typename T>
inline Nodo<T>* ListaCircular<T>::conseguirNodo(int posicion) const
{
    Nodo<T>* aux = cabeza;

    for (posicion; posicion > 0; posicion--) // No hay error, es circular
        aux = aux->siguiente;

    return aux;
}

template<typename T>
inline bool ListaCircular<T>::eliminar(const T& dato)
{
    Nodo<T>* aux = cabeza;

    if (aux != NULL)
    {
        do
        {
            if (aux->dato == dato)
            {
                if (aux == cabeza)
                    cabeza = aux->siguiente;
                else
                    aux->anterior->siguiente = aux->siguiente;

                aux->siguiente->anterior = aux->anterior;

                delete aux;
                return true;
            }

            aux = aux->siguiente;
        } while (aux != NULL);
    }

    return false;
}

template<typename T>
inline void ListaCircular<T>::eliminarPos(int posicion)
{
    Nodo<T>* aux = conseguirNodo(posicion);

    if (aux == cabeza)
        cabeza = aux->siguiente;
    else
        aux->anterior->siguiente = aux->siguiente;

    aux->siguiente->anterior = aux->anterior;

    delete aux;
}

template<typename T>
inline int ListaCircular<T>::contar() const
{
    int cont = 0;
    Nodo<T>* aux = cabeza;

    if (aux != NULL)
    {
        do
        {
            cont++;
            aux = aux->siguiente;
        } while (aux != cabeza);
    }

    return cont;
}

template<typename T>
inline void ListaCircular<T>::mostrar(const std::string& separador) const
{
    Nodo<T>* aux = cabeza;

    std::cout << "..." << separador;
    
    if (aux != NULL)
    {
        do
        {
            std::cout << aux->dato << separador;
            aux = aux->siguiente;
        } while (aux != cabeza);
    }

    std::cout << "...";
}

template<typename T>
inline void ListaCircular<T>::executarEnLista(std::function<void(T)> funcion)
{
    Nodo<T>* aux = cabeza;

    if (aux != NULL)
    {
        do
        {
            funcion(aux->dato);

            aux = aux->siguiente;
        } while (aux != NULL);
    }
}