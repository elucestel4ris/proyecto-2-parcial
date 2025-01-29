#pragma once

#include "Nodo.hpp"
#include <string>
#include <functional>

template <typename T>
class ILista
{

public:
    virtual ~ILista();
    virtual Nodo<T>* getCabeza() const = 0;
    virtual Nodo<T>* getCola() const = 0;

    virtual void insertarACabeza(const T& dato) = 0;
    virtual void insertarACola(const T& dato) = 0;
    virtual void insertar(const T& dato, int posicion) = 0;
    virtual void intercambiar(int pos1, int pos2) = 0;

    virtual int buscar(const T& dato) const = 0;
    virtual const T& conseguirDato(int posicion) const = 0;
    virtual Nodo<T>* conseguirNodo(int posicion) const = 0;

    virtual bool eliminar(const T& dato) = 0;
    virtual void eliminarPos(int posicion) = 0;

    virtual void mostrar(const std::string& = " =-= ") const = 0;

    virtual int contar() const = 0;

    virtual void executarEnLista(std::function<void(T)> funcion) = 0;

    template <typename U>
    static int buscar(const ILista& lista, const U& id, std::function<const U& (const T&)> identifierGetter);
    template <typename U>
    static bool eliminar(ILista& lista, const U& id, std::function<const U& (const T&)> identifierGetter);

};

template<typename T>
inline ILista<T>::~ILista()
{
}

template<typename T>
template<typename U>
inline int ILista<T>::buscar(const ILista& lista, const U& id, std::function<const U& (const T&)> identifierGetter)
{
    int n = lista.contar();
    int i = 0;
    for (i; i < n; i++)
    {
        if (identifierGetter(lista.conseguirDato(i)) == id)
            return i;
    }
    return i;


}

template<typename T>
template<typename U>
inline bool ILista<T>::eliminar(ILista& lista, const U& id, std::function<const U& (const T&)> identifierGetter)
{
    int n = lista.contar();
    int i = 0;
    for (i; i < n; i++)
    {
        if (identifierGetter(lista.conseguirDato(i)) == id)
        {
            lista.eliminarPos(i);
            return true;
        }
    }
    return false;
}
