#pragma once
#include "NodoBinario.hpp"
#include <iostream>
#include <string>
#include <functional>

template <typename T>
class ArbolBinario
{
private:
    NodoBinario<T>* origen;
    void insertarRecursivo(const T& dato, NodoBinario<T>* actual);
    NodoBinario<T>* buscarRecursivo(const T& dato, NodoBinario<T>* actual);
    bool eliminarRecursivo(const T& dato, NodoBinario<T>* actual, NodoBinario<T>* anterior = NULL);

    void eliminarArbol(NodoBinario<T>* actual);

    void imprimirPreorderRecursivo(NodoBinario<T>* actual) const;
    void imprimirInorderRecursivo(NodoBinario<T>* actual) const;
    void imprimirPostorderRecursivo(NodoBinario<T>* actual) const;

    void mostrarRecursivo(NodoBinario<T>* actual, std::string preTexto, bool esDer) const;

    template <typename U>
    static void insertarRecursivo(const T& dato, NodoBinario<T>* actual, std::function<const U& (const T&)> attributeGetter);
    template <typename U>
    static bool eliminarRecursivo(const U& dato, NodoBinario<T>* actual, std::function<const U& (const T&)> attributeGetter, NodoBinario<T>* anterior = NULL);
    template <typename U>
    static bool buscarRecursivo(const U& dato, NodoBinario<T>* actual, std::function<const U& (const T&)> attributeGetter);
public:
    ArbolBinario();
    ArbolBinario(std::initializer_list<T> valoresIniciales);
    ~ArbolBinario();
    NodoBinario<T>* getOrigen() const;
    void insertar(const T& dato);
    bool buscar(const T& dato);
    bool eliminar(const T& dato);

    void imprimirPreorder() const;
    void imprimirInorder() const;
    void imprimirPostorder() const;

    void mostrar() const;

    template <typename U>
    static void insertar(ArbolBinario<T>& arbol, const T& dato, std::function<const U& (const T&)> attributeGetter);
    template <typename U>
    static bool eliminar(ArbolBinario<T>& arbol, const U& dato, std::function<const U& (const T&)> attributeGetter);
    template <typename U>
    static bool buscar(ArbolBinario<T>& arbol, const U& dato, std::function<const U& (const T&)> attributeGetter);
};

template<typename T>
inline void ArbolBinario<T>::insertarRecursivo(const T& dato, NodoBinario<T>* actual)
{
    if (dato < actual->getDato())
    {
        if (actual->getIzq() != NULL)
            insertarRecursivo(dato, actual->getIzq());
        else
        {
            actual->setIzq(new NodoBinario<T>(dato));
        }
    }
    else
    {
        if (actual->getDer() != NULL)
            insertarRecursivo(dato, actual->getDer());
        else
        {
            actual->setDer(new NodoBinario<T>(dato));
        }
    }

}

template<typename T>
inline NodoBinario<T>* ArbolBinario<T>::buscarRecursivo(const T& dato, NodoBinario<T>* actual)
{
    if (actual == NULL)
        return false;

    if (dato == actual->getDato())
    {
        return true;
    }
    else
    {

        if (dato < actual->getDato())
            return buscarRecursivo(dato, actual->getIzq());
        else
            return buscarRecursivo(dato, actual->getDer());
    }
}

template<typename T>
inline bool ArbolBinario<T>::eliminarRecursivo(const T& dato, NodoBinario<T>* actual, NodoBinario<T>* anterior)
{
    if (actual == NULL)
        return false;

    if (dato == actual->getDato())
    {
        if (actual->getIzq() != NULL)
        {
            NodoBinario<T>* aux = actual->getIzq();

            while (aux->getDer() != NULL)
            {
                aux = aux->getDer();
            }

            actual->setDato(aux->getDato());
            eliminarRecursivo(aux->getDato(), actual->getIzq(), actual);

        }
        else if (actual->getDer() != NULL)
        {
            NodoBinario<T>* aux = actual->getDer();

            while (aux->getIzq() != NULL)
            {
                aux = aux->getIzq();
            }

            actual->setDato(aux->getDato());
            eliminarRecursivo(aux->getDato(), actual->getDer(), actual);
        }
        else
        {
            if (anterior->getDer() == actual)
                anterior->setDer(NULL);
            else
                anterior->setIzq(NULL);

            delete actual;
        }

    }
    else
    {

        if (dato < actual->getDato())
            return eliminarRecursivo(dato, actual->getIzq(), actual);
        else
            return eliminarRecursivo(dato, actual->getDer(), actual);
    }
    return true;
}

template<typename T>
inline void ArbolBinario<T>::eliminarArbol(NodoBinario<T>* actual)
{
    if (actual != NULL)
    {
        eliminarArbol(actual->getDer());
        eliminarArbol(actual->getIzq());
        delete actual;
    }
}

template<typename T>
inline void ArbolBinario<T>::imprimirPreorderRecursivo(NodoBinario<T>* actual) const
{
    if (actual != NULL)
    {
        std::cout << actual->dato << " -- ";
        imprimirPreorderRecursivo(actual->getIzq());
        imprimirPreorderRecursivo(actual->getDer());
    }
}

template<typename T>
inline void ArbolBinario<T>::imprimirInorderRecursivo(NodoBinario<T>* actual) const
{
    if (actual != NULL)
    {
        imprimirInorderRecursivo(actual->getIzq());
        std::cout << actual->dato << " -- ";
        imprimirInorderRecursivo(actual->getDer());
    }
}

template<typename T>
inline void ArbolBinario<T>::imprimirPostorderRecursivo(NodoBinario<T>* actual) const
{
    if (actual != NULL)
    {
        imprimirPostorderRecursivo(actual->getIzq());
        imprimirPostorderRecursivo(actual->getDer());
        std::cout << actual->dato << " -- ";
    }
}

template<typename T>
inline void ArbolBinario<T>::mostrarRecursivo(NodoBinario<T>* actual, std::string preTexto, bool esDer) const
{
    if (actual != nullptr)
    {
        std::cout << preTexto;

        std::cout << (esDer ? "├──" : "└──");

        // print the value of the node
        std::cout << actual->getDato() << std::endl;

        // enter the next tree level - left and right branch


        mostrarRecursivo(actual->getDer(), preTexto + (esDer ? "│   " : "    "), true);
        mostrarRecursivo(actual->getIzq(), preTexto + (esDer ? "│   " : "    "), false);
    }
}

template<typename T>
inline ArbolBinario<T>::ArbolBinario()
    : origen(NULL)
{
}

template<typename T>
inline ArbolBinario<T>::ArbolBinario(std::initializer_list<T> valoresIniciales)
{
    for (T valor : valoresIniciales)
        insertar(valor);
}

template<typename T>
inline ArbolBinario<T>::~ArbolBinario()
{
    eliminarArbol(origen);
}

template<typename T>
inline NodoBinario<T>* ArbolBinario<T>::getOrigen() const
{
    return origen;
}

template<typename T>
inline void ArbolBinario<T>::insertar(const T& dato)
{
    if (origen == NULL)
        origen = new NodoBinario<T>(dato);
    else
        insertarRecursivo(dato, origen);
}

template<typename T>
inline bool ArbolBinario<T>::buscar(const T& dato)
{
    return buscarRecursivo(dato, origen);
}

template<typename T>
inline bool ArbolBinario<T>::eliminar(const T& dato)
{
    return eliminarRecursivo(dato, origen);
}

template<typename T>
inline void ArbolBinario<T>::imprimirPreorder() const
{
    imprimirPreorderRecursivo(origen);
}

template<typename T>
inline void ArbolBinario<T>::imprimirInorder() const
{
    imprimirInorderRecursivo(origen);
}

template<typename T>
inline void ArbolBinario<T>::imprimirPostorder() const
{
    imprimirPostorderRecursivo(origen);
}

template<typename T>
inline void ArbolBinario<T>::mostrar() const
{
    mostrarRecursivo(origen, "", false);
}


template<typename T>
template<typename U>
inline void ArbolBinario<T>::insertarRecursivo(const T& dato, NodoBinario<T>* actual, std::function<const U& (const T&)> attributeGetter)
{
    if (attributeGetter(dato) < attributeGetter(actual->getDato()))
    {
        if (actual->getIzq() != NULL)
            insertarRecursivo(dato, actual->getIzq(), attributeGetter);
        else
        {
            actual->setIzq(new NodoBinario<T>(dato));
        }
    }
    else
    {
        if (actual->getDer() != NULL)
            insertarRecursivo(dato, actual->getDer(), attributeGetter);
        else
        {
            actual->setDer(new NodoBinario<T>(dato));
        }
    }
}

template<typename T>
template<typename U>
inline bool ArbolBinario<T>::eliminarRecursivo(const U& dato, NodoBinario<T>* actual, std::function<const U& (const T&)> attributeGetter, NodoBinario<T>* anterior)
{
    if (actual == NULL)
        return false;

    if (dato == attributeGetter(actual->getDato()))
    {
        if (actual->getIzq() != NULL)
        {
            NodoBinario<T>* aux = actual->getIzq();

            while (aux->getDer() != NULL)
            {
                aux = aux->getDer();
            }

            actual->setDato(aux->getDato());
            eliminarRecursivo(attributeGetter(aux->getDato()), actual->getIzq(), attributeGetter, actual);

        }
        else if (actual->getDer() != NULL)
        {
            NodoBinario<T>* aux = actual->getDer();

            while (aux->getIzq() != NULL)
            {
                aux = aux->getIzq();
            }

            actual->setDato(aux->getDato());
            eliminarRecursivo(attributeGetter(aux->getDato()), actual->getDer(), attributeGetter, actual);
        }
        else
        {
            if (anterior->getDer() == actual)
                anterior->setDer(NULL);
            else
                anterior->setIzq(NULL);

            delete actual;
        }

    }
    else
    {

        if (dato < attributeGetter(actual->getDato()))
            return eliminarRecursivo(dato, actual->getIzq(), attributeGetter, actual);
        else
            return eliminarRecursivo(dato, actual->getDer(), attributeGetter, actual);
    }
    return true;
}

template<typename T>
template<typename U>
inline bool ArbolBinario<T>::buscarRecursivo(const U& dato, NodoBinario<T>* actual, std::function<const U& (const T&)> attributeGetter)
{
    if (actual == NULL)
        return false;

    if (dato == attributeGetter(actual->getDato()))
    {
        return true;
    }
    else
    {

        if (dato < attributeGetter(actual->getDato()))
            return buscarRecursivo(dato, actual->getIzq(), attributeGetter);
        else
            return buscarRecursivo(dato, actual->getDer(), attributeGetter);
    }
}

template<typename T>
template<typename U>
inline void ArbolBinario<T>::insertar(ArbolBinario<T>& arbol, const T& dato, std::function<const U& (const T&)> attributeGetter)
{
    if (arbol.origen == NULL)
        arbol.origen = new NodoBinario<T>(dato);
    else
        insertarRecursivo(dato, arbol.getOrigen(), attributeGetter);
}

template<typename T>
template<typename U>
inline bool ArbolBinario<T>::eliminar(ArbolBinario<T>& arbol, const U& dato, std::function<const U& (const T&)> attributeGetter)
{
    return eliminarRecursivo(dato, arbol.getOrigen(), attributeGetter);
}

template<typename T>
template<typename U>
inline bool ArbolBinario<T>::buscar(ArbolBinario<T>& arbol, const U& dato, std::function<const U& (const T&)> attributeGetter)
{
    return buscarRecursivo(dato, arbol.getOrigen(), attributeGetter);
}
