#include "ILista.hpp"
#include <iostream>
#include <functional>

namespace Sort
{
    template<typename T, typename U>
    void bubbleSortObj(ILista<T>& lista, std::function<U (const T&)> atributeGetter, bool creciente = true);

    template<typename T>
    void bubbleSort(ILista<T>& lista, bool creciente = true);
}

template<typename T, typename U>
void Sort::bubbleSortObj(ILista<T>& lista, std::function<U (const T&)> atributeGetter, bool creciente)
{
    int n = lista.contar();
    bool haCambiado = false;

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            if (creciente)
            {
                if (atributeGetter(lista.conseguirDato(j)) > atributeGetter(lista.conseguirDato(j + 1)))
                {
                    lista.intercambiar(j, j + 1);
                    haCambiado = true;
                }
            }
            else
            {
                if (atributeGetter(lista.conseguirDato(j)) < atributeGetter(lista.conseguirDato(j + 1)))
                {
                    lista.intercambiar(j, j + 1);
                    haCambiado = true;
                }
            }
        }

        if (!haCambiado)
            break;
    }
}

template<typename T>
void Sort::bubbleSort(ILista<T>& lista, bool creciente)
{
    int n = lista.contar();
    bool haCambiado = false;

    for (int i = 1; i < n; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            if (creciente)
            {
                if (lista.conseguirDato(j) > lista.conseguirDato(j + 1))
                {
                    lista.intercambiar(j, j + 1);
                    haCambiado = true;
                }
            }
            else
            {
                if (lista.conseguirDato(j) < lista.conseguirDato(j + 1))
                {
                    lista.intercambiar(j, j + 1);
                    haCambiado = true;
                }
            }
        }

        if (!haCambiado)
            break;
    }
}

