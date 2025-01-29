#pragma once

template <typename T>
class NodoBinario
{
public:
    T dato;
    NodoBinario<T>* izq;
    NodoBinario<T>* der;
public:
    NodoBinario(const T& dato);
    const T& getDato() const;
    NodoBinario<T>* getIzq() const;
    NodoBinario<T>* getDer() const;
    void setDato(T& nuevoDato);
    void setIzq(NodoBinario<T>* nuevoIzq);
    void setDer(NodoBinario<T>* nuevoDer);
};

template<typename T>
inline NodoBinario<T>::NodoBinario(const T& dato)
    : dato(dato),
    izq(NULL),
    der(NULL)
{
}

template<typename T>
inline const T& NodoBinario<T>::getDato() const
{
    return dato;
}

template<typename T>
inline NodoBinario<T>* NodoBinario<T>::getIzq() const
{
    return izq;
}

template<typename T>
inline NodoBinario<T>* NodoBinario<T>::getDer() const
{
    return der;
}

template<typename T>
inline void NodoBinario<T>::setDato(T& nuevoDato)
{
    dato = nuevoDato;
}

template<typename T>
inline void NodoBinario<T>::setIzq(NodoBinario<T>* nuevoIzq)
{
    izq = nuevoIzq;
}

template<typename T>
inline void NodoBinario<T>::setDer(NodoBinario<T>* nuevoDer)
{
    der = nuevoDer;
}
