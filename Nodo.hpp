#pragma once

#include <cstddef>

template <typename T>
class ListaDoble;

template <typename T>
class ListaSimple;

template <typename T>
class ListaCircular;

template <typename T>
class Nodo
{
    friend class ListaDoble<T>;
    friend class ListaSimple<T>;
    friend class ListaCircular<T>;

private:
    Nodo<T>* anterior;
    Nodo<T>* siguiente;
public:
    T dato;
public:
    Nodo();
    Nodo(const T& dato);
    const T& getDato() const;
    Nodo<T>* getAnterior() const;
    Nodo<T>* getSiguiente() const;
    void setDato(const T& nuevoDato);
    void setAnterior(Nodo<T>* nuevoAnterior);
    void setSiguiente(Nodo<T>* nuevoSiguiente);
};

template<typename T>
inline Nodo<T>::Nodo()
    : anterior(NULL), siguiente(NULL)
{
}

template<typename T>
inline Nodo<T>::Nodo(const T& dato)
    : dato(dato), anterior(NULL), siguiente(NULL)
{
}

template<typename T>
inline void Nodo<T>::setDato(const T& nuevoDato)
{
    dato = nuevoDato;
}

template<typename T>
inline void Nodo<T>::setAnterior(Nodo<T>* nuevoAnterior)
{
    anterior = nuevoAnterior;
}

template<typename T>
inline void Nodo<T>::setSiguiente(Nodo<T>* nuevoSiguiente)
{
    siguiente = nuevoSiguiente;
}

template<typename T>
inline const T& Nodo<T>::getDato() const
{
    return dato;
}

template<typename T>
inline Nodo<T>* Nodo<T>::getAnterior() const
{
    return nullptr;
}

template<typename T>
inline Nodo<T>* Nodo<T>::getSiguiente() const
{
    return siguiente;
}
