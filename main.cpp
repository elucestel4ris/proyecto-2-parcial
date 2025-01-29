#include "ILista.hpp"
#include "ListaSimple.hpp"
#include "ListaDoble.hpp"
#include "ListaCircular.hpp"
#include "Sort.hpp"
#include "Libro.hpp"
#include "Autor.hpp"
#include "Validaciones.hpp"
#include "ArbolBinario.hpp"
#include "Formularios.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <functional>

//// GETCH ////
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)

#include <conio.h>

#else

#include <termios.h> 
#include <unistd.h> 
#include <stdio.h>
int getch()
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

#endif
//// GETCH ////

//// DEFINICIONES ////
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ESCAPE 27

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define KEY_ENTER 13
#define CLEAR_COMMAND "cls"
#else
#define KEY_ENTER 10
#define CLEAR_COMMAND "clear"
#endif
//// DEFINICIONES ////


void cargar(ILista<Libro>* ListaLibros, ILista<Autor>* listaAutores, std::string directorio = ".");
void guardar(ILista<Libro>* listaLibros, ILista<Autor>* listaAutores, std::string directorio = ".", bool backup = false);

template <typename T, typename U>
int busquedaBinaria(const int& k, ILista<T>& lista, std::function<U(const T&)> attributeGetter);

// FUNCION PARA CONSEGUIR EL PUNTERO DE UN LIBRO/AUTOR EXISTENTE USANDO SU ID
Autor* punteroAutorEnLista(std::string cedula, ILista<Autor>* listaAutores);
Libro* punteroLibroEnLista(std::string id, ILista<Libro>* listaLibros);

int main()
{
    ILista<Libro>* listaLibros = new ListaSimple<Libro>();
    ILista<Autor>* listaAutores = new ListaSimple<Autor>();


    cargar(listaLibros, listaAutores, ".");

    /////////////////////// COMPROBACIÓN DE EXISTENCIA DE UN ARCHIVO DE AUTOGUARDADO ////////////////////////
    
    if (std::filesystem::is_directory("unsaved"))
    {
        ListaSimple<Libro> listaLibrosTemp;
        ListaSimple<Autor> listaAutoresTemp;
        cargar(&listaLibrosTemp, &listaAutoresTemp, "unsaved");

        std::cout << "Lista de Libros guardada: " << std::endl;
        listaLibros->mostrar(" =>\n ");
        std::cout << std::endl << "\033[1;30mLista de Libros sin guardar recuperada:" << std::endl;
        listaLibrosTemp.mostrar(" =>\n ");

        std::cout << "\033[0m" << std::endl << std::endl;
        
        std::cout << "Lista de Autores guardada: " << std::endl;
        listaAutores->mostrar(" =>\n ");
        std::cout << std::endl << "\033[1;30mLista de Autores sin guardar recuperada:" << std::endl;
        listaAutoresTemp.mostrar(" =>\n ");

        std::cout << std::endl << std::endl << "\033[1;34mLa aplicación no se ha cerrado apropiadamente en la última sesión, ¿Desea restaurar los cambios sin guardar? (Y/n): \033[0m";
        char opcion = tolower(getch());

        if (opcion != 'n')
        {
            delete listaLibros;
            delete listaAutores;
            listaLibros = new ListaSimple<Libro>();
            listaAutores = new ListaSimple<Autor>();

            cargar(listaLibros, listaAutores, "unsaved");
            std::cout << std::endl << "DATOS RESTAURADOS";
            getch();
        }
    }

    /////////////////////// DECLARACIÓN DE LOS MENÚS ////////////////////////

    const std::string menuListas =
        "1. Libros\n"
        "2. Autores\n"
        "3. Cambiar Lista\n"
        "4. Backup\n"
        "5. Salida\n";

    const std::string menuOpLibros =
        "1. Insertar\n"
        "2. Buscar\n"
        "3. Eliminar\n"
        "4. Mostrar\n"
        "5. Guardar\n"
        "6. Filtrar\n"
        "7. Ordenar Libros\n"
        "8. Enviar a Arbol\n"
        "9. Volver\n";

    const std::string menuOpAutores =
        "1. Insertar\n"
        "2. Buscar\n"
        "3. Eliminar\n"
        "4. Mostrar\n"
        "5. Guardar\n"
        "6. Enviar a Arbol\n"
        "6. Volver\n";

    const std::string menuCambio =
        "1. Lista Simple\n"
        "2. Lista Doble\n"
        "3. Lista Circular\n"
        "4. Cancelar\n";

    const std::string menuAtributosLibros =
        "1. ID\n"
        "2. Titulo\n"
        "3. Autor\n"
        "4. Fecha\n"
        "5. Cancelar\n";

    const std::string menuAtributosAutores =
        "1. ID\n"
        "2. Nombre\n"
        "3. Nombre 2\n"
        "3. Apellido\n"
        "5. Cancelar\n";
    
    const std::string menuArbol =
        "1. Arbol Binario de Busqueda\n"
        ". Cancelar\n";
    
    const std::string menuOpArbol =
        "1. Insertar\n"
        "2. Buscar\n"
        "3. Eliminar\n"
        "4. Mostrar\n"
        "5. Volver\n";


    /////////////////////// FUNCIÓN LAMBDA QUE IMPRIME UN MENÚ ////////////////////////
    // Resalta el elemento seleccionado
    ///////////////////////////////////////////////////////////////////////////////////
    auto imprimirMenu = [](const std::string& menu, int eleccion)
        {
            std::stringstream ss(menu);
            std::string s;
            int cont = 0;
            while (getline(ss, s))
            {
                cont++;
                std::cout << ((cont == eleccion) ? "\033[5m>\033[25m \033[7;1m" : "  ") << s << std::endl;
                if (cont == eleccion)
                    std::cout << "\033[0m";
            }
        };

    /////////////////////// FUNCIÓN LAMBDA QUE RESTRINGE LOS VALORES DE ELECCION ////////////////////////
    // la entrada se realiza mediante las flechas arriba (aumentar), abajo (disminuir) y el botón enter
    // TODO: decidir si es apropiado cambiar el nombre de la función
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    auto procesarEntrada = [](int& eleccion, int entrada, int eleccionMax) -> bool
        {
            //
        // PARA LINUX, se procesan 3 caracteres al presionar las flechas: '\033', '[' y una letra

            if (entrada == KEY_ESCAPE)
            {                
                char nextInput = getch();

                if (nextInput == KEY_ESCAPE)
                {
                    entrada = KEY_ESCAPE;
                }
                else
                {
                    switch (getch())
                    {
                    case 'A':   // KEY_UP
                        entrada = KEY_UP;
                        break;
                    case 'B':   // KEY_DOWN
                        entrada = KEY_DOWN;
                        break;
                    case 'C':   // KEY_RIGHT
                        entrada = KEY_ENTER;
                        break;  
                    case 'D':   // KEY_LEFT
                        break;  // no cambia, entrada permanece en KEY_ESCAPE

                    default:
                        break;
                    }
                }
            }

            switch (entrada)
            {
            case KEY_UP:
                if (eleccion == 1)
                    eleccion = eleccionMax;
                else
                    eleccion--;

                break;
            case KEY_DOWN:
                if (eleccion == eleccionMax)
                    eleccion = 1;
                else
                    eleccion++;
                break;
            
            case KEY_RIGHT:
            case KEY_ENTER:
                return true;
                break;
            case KEY_LEFT:
            case KEY_ESCAPE:
                eleccion = eleccionMax;
                return true;
                break;


            default:
                if (entrada > '0' && entrada < eleccionMax + '0' + 1)
                {
                    eleccion = entrada - '0';
                    // return true;     // Remover comentario para que entre inmediatamente en la opción
                }
            }
            return false;
        };

   
    /////////////////////// DECLARACIÓN DE ENUMERACIONES ////////////////////////
    // No son necesarias, pero hacen que el código sea más legible (no ha sido
    // suficiente), especialmente cuando existen varios submenús.
    /////////////////////////////////////////////////////////////////////////////
    enum eleccionListas { LIBROS = 1, AUTORES, CAMBIAR_LISTA, BACKUP, SALIR };
    enum eleccionOpLibros { LIBROS_INSERTAR = 1, LIBROS_BUSCAR, LIBROS_ELIMINAR, LIBROS_MOSTRAR, LIBROS_GUARDAR, LIBROS_FILTRAR, LIBROS_ORDENAR, LIBROS_ARBOL, SALIR_LIBROS };
    enum eleccionOpAutores { AUTORES_INSERTAR = 1, AUTORES_BUSCAR, AUTORES_ELIMINAR, AUTORES_MOSTRAR, AUTORES_GUARDAR, AUTORES_ARBOL, AUTORES_SALIR };
    enum eleccionSort { SORT_BURBUJA = 1, SORT_QUICKSORT, SORT_SHELLSORT, SORT_INSERCION, SORT_DISTRIBUCION, SORT_RADIX, SORT_MERGESORT, SORT_MEZCLA_DIRECTA, SORT_MEZCLA_NATURAL, SORT_CANCELAR };
    enum eleccionArbol { ARBOL_BINARIO_BUSQUEDA = 1, ARBOL_CANCELAR };
    enum eleccionOpArbol { ARBOL_INSERTAR = 1, ARBOL_BUSCAR, ARBOL_ELIMINAR, ARBOL_MOSTRAR, ARBOL_SALIR };
    enum eleccionCambio { LISTA_SIMPLE = 1, LISTA_DOBLE, LISTA_CIRCULAR, CANCELAR_CAMBIO };
    enum atributosLibro { ID = 1, TITULO, AUTOR, FECHA, CANCELAR };

    /////////////////////// FUNCIÓN LAMBDA QUE MUESTRA UN MENÚ PARA ELEGIR MÉTODO DE SORTING ////////////////////////
    // - Usado en operarLibros
    // la entrada se realiza mediante las flechas arriba (aumentar), abajo (disminuir) y el botón enter
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto elegirMetodoSort = [&imprimirMenu, &menuArbol, &procesarEntrada]<typename T, typename U>(ILista<T>* lista, std::function<const U& (const T&)> attributeGetter)
    {
        int eleccion = 1;
        char entrada;
        bool salir;

        do
        {
            system(CLEAR_COMMAND);
            std::cout << "============ ELEGIR MÉTODO ============" << std::endl;
            imprimirMenu(menuArbol, eleccion);
            entrada = getch();
            system(CLEAR_COMMAND);  // borrar esta línea?

            salir = procesarEntrada(eleccion, entrada, SORT_CANCELAR);

            if (salir)
            {
                if (eleccion != SORT_CANCELAR)
                {
                    switch (eleccion)
                    {
                    case SORT_BURBUJA:
                        Sort::bubbleSortObj(*lista, attributeGetter);
                        break;
                    case SORT_QUICKSORT:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_SHELLSORT:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_INSERCION:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_DISTRIBUCION:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_RADIX:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_MERGESORT:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_MEZCLA_DIRECTA:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;
                    case SORT_MEZCLA_NATURAL:
                        std::cout << "NO IMPLEMENTADO" << std::endl;
                        break;

                    default:
                        break;
                    }
                    std::cout << "LISTA ORDENADA " << std::endl;
                    getch();

                }
                else
                {
                    std::cout << "NO ORDENADO" << std::endl;
                    getch();
                }
            }

        } while (!salir);
    };

    /////////////////////// FUNCIÓN LAMBDA QUE MUESTRA UN MENÚ PARA ELEGIR ÁRBOL ////////////////////////
    // - Usado en operarAutores y operarLibros
    // la entrada se realiza mediante las flechas arriba (aumentar), abajo (disminuir) y el botón enter
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto elegirArbolAutor = [&]<typename T>(ILista<Autor>*lista, std::function<const T & (const Autor&)> attributeGetter)
    {
        int eleccion = 1;
        char entrada;
        bool salir;
        ListaSimple<Autor> listaAutoresTemp;

        do
        {
            system(CLEAR_COMMAND);
            std::cout << "============ ELEGIR MÉTODO ============" << std::endl;
            imprimirMenu(menuArbol, eleccion);
            entrada = getch();
            system(CLEAR_COMMAND);

            salir = procesarEntrada(eleccion, entrada, ARBOL_CANCELAR);

            if (salir)
            {
                if (eleccion != ARBOL_CANCELAR)
                {
                    switch (eleccion)
                    {
                    case ARBOL_BINARIO_BUSQUEDA:
                    {
                        ArbolBinario<const Autor&> arbol;

                        int n = lista->contar();
                        for (int i = 0; i < n; i++)
                        {
                            ArbolBinario<const Autor&>::insertar(arbol, lista->conseguirDato(i), attributeGetter);
                        }

                        bool salirArbol = false;
                        int eleccionArbol = 1;
                        do
                        {
                            system(CLEAR_COMMAND);
                            std::cout << "============ OPERAR ÁRBOL ============" << std::endl;
                            imprimirMenu(menuOpArbol, eleccionArbol);
                            entrada = getch();

                            if (procesarEntrada(eleccionArbol, entrada, ARBOL_SALIR))
                            {
                                switch (eleccionArbol)
                                {
                                case ARBOL_INSERTAR:
                                {
                                    formularioInsercionAutor(listaAutores);
                                    const Autor& ultimoAutor = listaAutores->getCola()->getDato();
                                    ArbolBinario<const Autor&>::insertar(arbol, ultimoAutor, attributeGetter);
                                    break;
                                }
                                case ARBOL_BUSCAR:
                                {
                                    std::cout << "ingrese el id del autor que desea buscar" << std::endl;
                                    std::string id = Validaciones::leerCedula();
                                    if (ArbolBinario<const Autor&>::buscar(arbol, id, attributeGetter))
                                    {
                                        std::cout << "El autor existe en el arbol" << std::endl;
                                    }
                                    else
                                    {
                                        std::cout << "El autor no existe en el arbol";
                                    }
                                    getch();
                                    break;
                                }
                                case ARBOL_ELIMINAR:
                                {

                                    formularioEliminacionLibro(listaLibros);
                                    getch();
                                    break;
                                }
                                case ARBOL_MOSTRAR:
                                    arbol.mostrar();
                                    getch();
                                    break;
                                case ARBOL_SALIR:
                                    salirArbol = true;
                                    break;
                                default:
                                    std::cout << "NO ES UNA OPCIÓN VÁLIDA" << std::endl;
                                    getch();
                                    break;
                                }
                            }
                        } while (!salirArbol);

                        break;
                    }
                    default:
                        break;
                    }
                    getch();
                }
            }
        } while (!salir);
    };
    
    auto elegirArbolLibro = [&]<typename T>(ILista<Libro>*lista, std::function<const T & (const Libro&)> attributeGetter)
    {
        int eleccion = 1;
        char entrada;
        bool salir;

        do
        {
            system(CLEAR_COMMAND);
            std::cout << "============ ELEGIR MÉTODO ============" << std::endl;
            imprimirMenu(menuArbol, eleccion);
            entrada = getch();
            system(CLEAR_COMMAND);

            salir = procesarEntrada(eleccion, entrada, ARBOL_CANCELAR);

            if (salir)
            {
                if (eleccion != ARBOL_CANCELAR)
                {
                    switch (eleccion)
                    {
                    case ARBOL_BINARIO_BUSQUEDA:
                    {
                        ArbolBinario<const Libro&> arbol;

                        int n = lista->contar();
                        for (int i = 0; i < n; i++)
                        {
                            ArbolBinario<const Libro&>::insertar(arbol, lista->conseguirDato(i), attributeGetter);
                        }

                        bool salirArbol = false;
                        int eleccionArbol = 1;
                        do
                        {
                            system(CLEAR_COMMAND);
                            std::cout << "============ OPERAR ÁRBOL ============" << std::endl;
                            imprimirMenu(menuOpArbol, eleccionArbol);
                            entrada = getch();

                            if (procesarEntrada(eleccionArbol, entrada, ARBOL_SALIR))
                            {
                                switch (eleccionArbol)
                                {
                                case ARBOL_INSERTAR:
                                {
                                    formularioInsercionLibro(listaLibros, listaAutores);
                                    const Libro& ultimoLibro = listaLibros->getCola()->getDato();
                                    ArbolBinario<const Libro&>::insertar(arbol, ultimoLibro, attributeGetter);
                                    break;
                                }
                                case ARBOL_BUSCAR:
                                {
                                    std::cout << "ingrese el id del libro que desea buscar" << std::endl;
                                    std::string id = Validaciones::leerNumero();

                                    formularioBusquedaLibro(listaLibros);
                                    getch();
                                    break;
                                }
                                case ARBOL_ELIMINAR:
                                    formularioEliminacionLibro(listaLibros);
                                    std::cout << "LIbro Eliminado";
                                    getch();
                                    break;
                                case ARBOL_MOSTRAR:
                                    arbol.mostrar();
                                    getch();
                                    break;
                                case ARBOL_SALIR:
                                    salirArbol = true;
                                    break;
                                default:
                                    std::cout << "NO ES UNA OPCIÓN VÁLIDA" << std::endl;
                                    getch();
                                    break;
                                }
                            }
                        } while (!salirArbol);

                        break;
                    }
                    default:
                        break;
                    }
                    getch();
                }
            }
        } while (!salir);
    };

    /////////////////////// FUNCIÓN LAMBDA QUE OPERA SOBRE LA LISTA DE AUTORES ////////////////////////
    // Es sólo un switch
    // Va primero porque las operaciones en libros necesita acceder a las de autores
    //////////////////////////////////////////////////////////////////////////////////////////////////
    auto operarAutores = [&](int eleccion)
        {
            std::string cedula;

            
            std::function<const std::string& (const Autor&)> conseguirIdAutor = [](const Autor& a) -> const std::string&
                {
                    return a.getId();
                };

            switch (eleccion)
            {
            case AUTORES_INSERTAR:
                formularioInsercionAutor(listaAutores);
                break;
            case AUTORES_BUSCAR:
                formularioBusquedaAutor(listaAutores);
                break;
            case AUTORES_ELIMINAR:
                formularioEliminacionAutor(listaAutores, listaLibros);
                break;
            case AUTORES_MOSTRAR:
                std::cout << std::endl;
                listaAutores->mostrar(" =-=\n ");
                std::cout << std::endl;
                getch();
                break;
            case AUTORES_GUARDAR:
                guardar(listaLibros, listaAutores, ".", 1);
            case AUTORES_ARBOL:
            {
                enum atributosAutores { ID = 1, NOMBRE, NOMBRE2, APELLIDO, CANCELAR };
                int eleccion = 1;
                char entrada;
                bool salir;

                do
                {
                    system(CLEAR_COMMAND);
                    std::cout << "============ MIEMBRO DE ORDEN ============" << std::endl;
                    imprimirMenu(menuAtributosAutores, eleccion);
                    entrada = getch();

                    salir = procesarEntrada(eleccion, entrada, CANCELAR);

                    if (salir && eleccion != CANCELAR)
                    {
                        switch (eleccion)
                        {
                        case ID:
                            elegirArbolAutor(listaAutores, (std::function<const std::string & (const Autor&)>)[](const Autor& a) -> const std::string& { return a.getId(); });
                            break;
                        case NOMBRE:
                            elegirArbolAutor(listaAutores, (std::function<const std::string & (const Autor&)>)[](const Autor& a) -> const std::string& {return a.getNombre();});
                            break;
                        case NOMBRE2:
                            elegirArbolAutor(listaAutores, (std::function<const std::string & (const Autor&)>)[](const Autor& a) -> const std::string& {return a.getNombre2();});
                            break;
                        case APELLIDO:
                            elegirArbolAutor(listaAutores, (std::function<const std::string & (const Autor&)>)[](const Autor& a) -> const std::string& {return a.getApellido();});
                            break;
                        default:
                            break;
                        }
                    }

                } while (!salir);

                break;
            }
            case AUTORES_SALIR:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "NO ES UNA OPCIÓN VÁLIDA" << std::endl;
                getch();
                break;
            }
        };

    
    /////////////////////// FUNCIÓN LAMBDA QUE OPERA SOBRE LA LISTA DE LIBROS ////////////////////////
    // Es sólo un switch
    // La lista de autores también está capturada, para la asignación de autores
    //////////////////////////////////////////////////////////////////////////////////////////////////
    
    auto operarLibros = [&](int eleccion)
        {
            std::string id;

            std::function<const std::string& (const Libro&)> conseguirIdLibro = [](const Libro& l) -> const std::string&
                {
                    return l.getId();
                };

            switch (eleccion)
            {
            case LIBROS_INSERTAR:
                formularioInsercionLibro(listaLibros, listaAutores);
                break;
            case LIBROS_BUSCAR:
                formularioBusquedaLibro(listaLibros);
                break;
            case LIBROS_ELIMINAR:
            {
                formularioEliminacionLibro(listaLibros);
                break;
            }
            case LIBROS_MOSTRAR:
                std::cout << std::endl;
                listaLibros->mostrar(" =-=\n ");
                std::cout << std::endl;
                getch();
                break;
            case LIBROS_GUARDAR:
            {
                guardar(listaLibros, listaAutores, ".", 1);
                break;
            }
            case LIBROS_FILTRAR:
            {
                std::time_t anioInicio, anioFin;
                struct tm anioInicioStruct, anioFinStruct;

                // Solicitar al usuario el rango de años
                std::cout << "Ingrese el año inicial del rango: ";
                anioInicioStruct.tm_year = Validaciones::leerInt() - 1900;
                anioInicioStruct.tm_mon = 0;
                anioInicioStruct.tm_mday = 0;
                anioInicioStruct.tm_hour = 0;
                anioInicioStruct.tm_min = 0;
                anioInicioStruct.tm_sec = 0;
                anioInicio = mktime(&anioInicioStruct);

                std::cout << "Ingrese el año final del rango: ";
                anioFinStruct.tm_year = Validaciones::leerInt() - 1900;
                anioFinStruct.tm_mon = 0;
                anioFinStruct.tm_mday = 0;
                anioFinStruct.tm_hour = 0;
                anioFinStruct.tm_min = 0;
                anioFinStruct.tm_sec = 0;
                anioFin = mktime(&anioFinStruct);

                // Validar el rango
                if (anioInicio > anioFin) {
                    std::cout << "Rango de anios no valido. El anio inicial debe ser menor o igual al anio final." << std::endl;
                    getch();
                    break;
                }

                // Definir la lambda para filtrar los libros

                ListaSimple<Libro> listaRango;

                auto librosEnRango = [&listaRango, &listaLibros](std::time_t anioInicio, std::time_t anioFin)
                    {
                        int n = listaLibros->contar();
                        for (int i = 0; i < n; i++)
                        {
                            std::time_t tiempoLibro = listaLibros->conseguirDato(i).getFecha().getTiempo();
                            if (tiempoLibro > anioInicio && tiempoLibro < anioFin)
                            {
                                listaRango.insertarACola(listaLibros->conseguirDato(i));
                            }
                        }
                    };

                librosEnRango(anioInicio, anioFin);
                listaRango.mostrar(" =>\n ");

                getch();
                break;
            }
            case LIBROS_ORDENAR:
            {
                enum atributosLibros { ID = 1, TITULO, AUTOR, FECHA, CANCELAR };
                int eleccion = 1;
                char entrada;
                bool salir;

                do
                {
                    system(CLEAR_COMMAND);
                    std::cout << "============ POR MIEMBRO ============" << std::endl;
                    imprimirMenu(menuAtributosLibros, eleccion);
                    entrada = getch();

                    salir = procesarEntrada(eleccion, entrada, CANCELAR);

                    if (salir && eleccion != CANCELAR)
                    {
                        switch (eleccion)
                        {
                        case ID:
                            elegirMetodoSort(listaLibros, (std::function<const std::string& (const Libro&)>)[](const Libro& l) -> const std::string& { return l.getId(); });
                            break;
                        case TITULO:
                            elegirMetodoSort(listaLibros, (std::function<const std::string & (const Libro&)>)[](const Libro& l) -> const std::string& {return l.getTitulo();});
                            break;
                        case AUTOR:
                            elegirMetodoSort(listaLibros, (std::function<const std::string & (const Libro&)>)[](const Libro& l) -> const std::string& {return l.getAutor()->getId();});
                            break;
                        case FECHA:
                            elegirMetodoSort(listaLibros, (std::function<const std::time_t & (const Libro&)>)[](const Libro& l) -> const std::time_t& {return l.getFecha().getTiempo();});
                            break;
                        default:
                            break;
                        }
                    }

                } while (!salir);

                break;
            }
            case LIBROS_ARBOL:
            {
                enum atributosLibros { ID = 1, TITULO, AUTOR, FECHA, CANCELAR };
                int eleccion = 1;
                char entrada;
                bool salir;

                do
                {
                    system(CLEAR_COMMAND);
                    std::cout << "============ MIEMBRO DE ORDEN ============" << std::endl;
                    imprimirMenu(menuAtributosLibros, eleccion);
                    entrada = getch();

                    salir = procesarEntrada(eleccion, entrada, CANCELAR);

                    if (salir && eleccion != CANCELAR)
                    {
                        switch (eleccion)
                        {
                        case ID:
                            elegirArbolLibro(listaLibros, (std::function<const std::string & (const Libro&)>)[](const Libro& l) -> const std::string& { return l.getId(); });
                            break;
                        case TITULO:
                            elegirArbolLibro(listaLibros, (std::function<const std::string & (const Libro&)>)[](const Libro& l) -> const std::string& {return l.getTitulo();});
                            break;
                        case AUTOR:
                            elegirArbolLibro(listaLibros, (std::function<const std::string & (const Libro&)>)[](const Libro& l) -> const std::string& {return l.getAutor()->getId();});
                            break;
                        case FECHA:
                            elegirArbolLibro(listaLibros, (std::function<const std::time_t & (const Libro&)>)[](const Libro& l) -> const std::time_t& {return l.getFecha().getTiempo();});
                            break;
                        default:
                            break;
                        }
                    }

                } while (!salir);

                break;
            }
            case SALIR_LIBROS:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "NO ES UNA OPCIÓN VÁLIDA" << std::endl;
                getch();
                break;
            }
        };

    /*****************************************************************/
    /*███████████████████████████████████████████████████████████████*/
    /*////////////////////██ LÓGICA PRINCIPAL ██/////////////////////*/
    /*███████████████████████████████████████████████████████████████*/
    /*v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v v*/

    int listaActual = LISTA_SIMPLE;
    int eleccion = 1;
    int entrada;
    bool salir = false;
    do
    {
        system(CLEAR_COMMAND);
        std::cout << "============ LISTAS ============" << std::endl;
        imprimirMenu(menuListas, eleccion);
        entrada = getch();

        if (procesarEntrada(eleccion, entrada, SALIR))
        {
            bool salir2 = false;
            int eleccion2 = 1;
                switch (eleccion)
                {
                case LIBROS:
                    do
                    {
                        system(CLEAR_COMMAND);
                        std::cout << "============ LISTA LIBROS ============" << std::endl;
                        imprimirMenu(menuOpLibros, eleccion2);
                        entrada = getch();

                        if (procesarEntrada(eleccion2, entrada, SALIR_LIBROS))
                        {
                            operarLibros(eleccion2);

                            if (eleccion2 == LIBROS_GUARDAR)
                                std::filesystem::remove_all("unsaved");

                            if (eleccion2 == LIBROS_INSERTAR || eleccion2 == LIBROS_ELIMINAR)
                                guardar(listaLibros, listaAutores, "unsaved");

                            if (eleccion2 == SALIR_LIBROS)
                                salir2 = true;
                        }

                    } while (!salir2);
                    break;
                case AUTORES:
                    do
                    {
                        system(CLEAR_COMMAND);
                        std::cout << "============ LISTA AUTOR ============" << std::endl;
                        imprimirMenu(menuOpAutores, eleccion2);
                        entrada = getch();

                        if (procesarEntrada(eleccion2, entrada, AUTORES_SALIR))
                        {
                            operarAutores(eleccion2);

                            if (eleccion2 == AUTORES_GUARDAR)
                                std::filesystem::remove_all("unsaved");

                            if (eleccion2 == AUTORES_INSERTAR || eleccion2 == AUTORES_ELIMINAR)
                                guardar(listaLibros, listaAutores, "unsaved");

                            if (eleccion2 == AUTORES_SALIR)
                                salir2 = true;
                        }

                    } while (!salir2);
                    break;
                case CAMBIAR_LISTA:
                    do
                    {
                        system(CLEAR_COMMAND);
                        std::cout << "============ ELEGIR LISTA ============" << std::endl;
                        imprimirMenu(menuCambio, eleccion2);
                        entrada = getch();

                        salir2 = procesarEntrada(eleccion2, entrada, CANCELAR_CAMBIO);

                        if (salir2 && eleccion2 != CANCELAR_CAMBIO)
                        {
                            if (listaActual != eleccion2)
                            {

                                guardar(listaLibros, listaAutores, "temp");
                                delete listaLibros;
                                delete listaAutores;

                                switch (eleccion2)
                                {
                                case LISTA_SIMPLE:
                                    listaLibros = new ListaSimple<Libro>();
                                    listaAutores = new ListaSimple<Autor>();
                                    cargar(listaLibros, listaAutores, "temp");
                                    listaActual = LISTA_SIMPLE;
                                    break;
                                case LISTA_DOBLE:
                                    listaLibros = new ListaDoble<Libro>();
                                    listaAutores = new ListaDoble<Autor>();
                                    cargar(listaLibros, listaAutores, "temp");
                                    listaActual = LISTA_DOBLE;
                                    break;
                                case LISTA_CIRCULAR:
                                    listaLibros = new ListaCircular<Libro>();
                                    listaAutores = new ListaCircular<Autor>();
                                    cargar(listaLibros, listaAutores, "temp");
                                    listaActual = LISTA_CIRCULAR;
                                    break;
                                default:
                                    break;
                                }

                                std::filesystem::remove_all("temp");
                                std::cout << "LISTA CAMBIADA";
                                getch();
                            }
                            else
                            {
                                std::cout << "LA LISTA ACTUAL YA ES DE ESE TIPO";
                                getch();
                            }
                        }

                    } while (!salir2);

                    break;
                case BACKUP:
                {
                    ListaSimple<std::string> listaBackup;
                    
                    int cant = 0;
                    for (const auto& entry : std::filesystem::directory_iterator("./backup"))
                    {
                        listaBackup.insertarACola(entry.path().string());
                        cant++;
                    }

                    std::function<time_t(const std::string&)> conseguirTiempo = [](const std::string& s)
                        {
                            std::stringstream ssTiempo(s.substr(9));
                            struct std::tm structTiempo;

                            ssTiempo >> std::get_time(&structTiempo, "%a %b %d %H.%M.%S %G");

                            return mktime(&structTiempo);
                        };
                    Sort::bubbleSortObj(listaBackup, conseguirTiempo, false);

                    std::string stringBackups;
                    
                    for (int i = 0; i < cant; i++)
                    {
                        stringBackups.append(listaBackup.conseguirDato(i) + '\n');
                    }
                    cant++; // +1 para la opción de cancelar
                    stringBackups.append("CANCELAR");

                    do
                    {
                        system(CLEAR_COMMAND);
                        std::cout << "============ ELEGIR BACKUP ============" << std::endl;
                        imprimirMenu(stringBackups, eleccion2);
                        entrada = getch();

                        salir2 = procesarEntrada(eleccion2, entrada, cant);

                        if (salir2)
                        {
                            if (eleccion2 != cant)  // cant es CANCELAR
                            {
                                std::string dirBackup = listaBackup.conseguirDato(eleccion2 - 1); // -1 porque el índice comienza en 0
                                ListaSimple<Libro> listaLibrosTemp;
                                ListaSimple<Autor> listaAutoresTemp;
                                cargar(&listaLibrosTemp, &listaAutoresTemp, dirBackup);

                                std::cout << "Lista de Libros actual: " << std::endl;
                                listaLibros->mostrar(" =-=\n ");
                                std::cout << std::endl << "\033[1;30mLista de Libros de " << dirBackup << std::endl;
                                listaLibrosTemp.mostrar(" =>\n ");

                                std::cout << "\033[0m" << std::endl << std::endl;

                                std::cout << "Lista de Autores actual: " << std::endl;
                                listaAutores->mostrar(" =-=\n ");
                                std::cout << std::endl << "\033[1;30mLista de Autores de " << dirBackup << std::endl;
                                listaAutoresTemp.mostrar(" =>\n ");

                                std::cout << std::endl << std::endl << "\033[0;4mSi carga ahora los datos actuales serán borrados, desea continuar?\033[0m (y/N): ";

                                char opcion = tolower(getch());
                                if (opcion == 'y')
                                {
                                    delete listaLibros;
                                    delete listaAutores;
                                    listaLibros = new ListaSimple<Libro>();
                                    listaAutores = new ListaSimple<Autor>();

                                    cargar(listaLibros, listaAutores, dirBackup);

                                    guardar(listaLibros, listaAutores, "unsaved");

                                    std::cout << std::endl << "DATOS RESTAURADOS" << std::endl;
                                }
                                else
                                {
                                    std::cout << std::endl << "NO SE HA CAMBIADO" << std::endl;
                                }

                                getch();
                            }
                        }

                    } while (!salir2);

                    break;
                }
                case SALIR:
                    salir = true;
                    break;
                default:
                    break;
                }
        }

    } while (!salir);

    return 0;
}

/////////////////////// FUNCIÓN DE CARGADO ////////////////////////
// Toma por defecto el directorio actual (ver declaración)
// Puede elegirse cualquier carpeta en la que se haya usado la
// función guardar (tendrá un archivo autores.txt y libros.txt)
////////////////////////////////////////////////////////////////////
void cargar(ILista<Libro>* ListaLibros, ILista<Autor>* listaAutores, std::string directorio)
{
     // CARGA AUTORES, va primero porque en el archivo de autores no están listados sus libros
    std::ifstream archivoAutores(directorio + '/' + "autores.txt");

    std::string lineaArchivo;

    while (getline(archivoAutores, lineaArchivo))
    {
        std::istringstream ss(lineaArchivo);
        std::string cedula, nombre, nombre2, apellido;

        getline(ss, cedula, ',');
        getline(ss, nombre, ',');
        getline(ss, nombre2, ',');
        getline(ss, apellido);

        listaAutores->insertarACola(Autor(cedula, nombre, nombre2, apellido));
    }

    archivoAutores.close();

    //  CARGA LIBROS en el archivo de libros se encuentran los cedula de sus autores
    std::ifstream archivoLibros(directorio + '/' + "libros.txt");


    while (getline(archivoLibros, lineaArchivo))
    {
        std::istringstream ss(lineaArchivo);
        std::string id, titulo, idAutor, tiempoS;
        Autor* autor;

        getline(ss, id, ',');
        getline(ss, titulo, ',');
        getline(ss, idAutor, ',');
        getline(ss, tiempoS);
        std::time_t tiempo = 0;
        if (!tiempoS.empty())
        {
            tiempo = std::stoll(tiempoS);
        }
        autor = punteroAutorEnLista(idAutor, listaAutores);

        // BUSCA EL PUNTERO DEL AUTOR Y LO ASIGNA AL LIBRO
        ListaLibros->insertarACola(Libro(id, titulo, autor, tiempo));
        // TOMA EL PUNTERO DEL LIBRO INGRESADO MÁS RECIENTEMENTE Y LO AÑADE A LOS LIBROS DEL AUTOR
        autor->libros.insertarACola(&(ListaLibros->getCola()->dato));
    }

    archivoLibros.close();
    // ENLAZA LIBROS Y AUTORES
}

/////////////////////// FUNCIÓN DE GUARDADO ////////////////////////
// Se explica sólo
// Toma por defecto el directorio actual (ver declaración)
// Generará libros.txt y autores.txt en el directorio señalado
// Puede eligirse si crear un backup a partir del archivo guardado
// El backup se genera en una carpera llamada backup, donde existen
// subcarpetas con la fecha y hora de su creación
////////////////////////////////////////////////////////////////////
void guardar(ILista<Libro>* listaLibros, ILista<Autor>* listaAutores, std::string directorio, bool backup)
{
    if (!directorio.empty())
        std::filesystem::create_directories(directorio);

    // GUARDA LIBROS AL ARCHIVO
    std::ofstream archivoLibros(directorio + '/' + "libros.txt");

    int nL = listaLibros->contar();
    for (int i = 0; i < nL; i++)
    {
        const Libro & l = listaLibros->conseguirDato(i);
        archivoLibros << l.getId() << ","
            << l.getTitulo() << ","
            << l.getAutor()->getId() << ","
            << l.getFecha().getTiempo()
            << std::endl;
    }

    archivoLibros.close();

    // GUARDA AUTORES AL LIBRO
    std::ofstream archivoAutores(directorio + '/' + "autores.txt");

    int nA = listaAutores->contar();
    for (int i = 0; i < nA; i++)
    {
        const Autor& a = listaAutores->conseguirDato(i);
        archivoAutores << a.getId() << ","
            << a.getNombre() << ","
            << a.getNombre2() << ","
            << a.getApellido()
            << std::endl;
    }

    archivoAutores.close();

    if (backup)
    {
        std::time_t tiempo = time(NULL);
        {
            /* data */
        };
        
        std::string nombreCarpeta(ctime(&tiempo));
        nombreCarpeta.pop_back();   // elimina el salto de línea
        std::replace(nombreCarpeta.begin(), nombreCarpeta.end(), ':', '.');

        std::string direccionBackup = "backup/" + nombreCarpeta;

        if (!std::filesystem::is_directory(direccionBackup))
        {
            std::filesystem::create_directories(direccionBackup);
            std::filesystem::copy_file(directorio + '/' + "libros.txt", direccionBackup + '/' + "libros.txt");
            std::filesystem::copy_file(directorio + '/' + "autores.txt", direccionBackup + '/' + "autores.txt");
        }
    }
}

Autor* punteroAutorEnLista(std::string id, ILista<Autor>* listaAutores)
{

    int n = listaAutores->contar();
    for (int i = 0; i < n; i++)     // itera por cada elemento la lista de autores
    {
        if (listaAutores->conseguirDato(i).getId() == id)  // comprueba que el elemento coincide con el campo ingresado
            return &(listaAutores->conseguirNodo(i)->dato);     // TODO: PENSAR EN ALGO MEJOR PARA NO USAR conseguirNodo()
    }

    return (Autor*)nullptr;
};

Libro* punteroLibroEnLista(std::string id, ILista<Libro>* listaLibros)
{
    int n = listaLibros->contar();
    for (int i = 0; i < n; i++)
    {
        if (listaLibros->conseguirDato(i).getId() == id)
            return &(listaLibros->conseguirNodo(i)->dato);
    }

    return (Libro*)nullptr;
};

template<typename T, typename U>
int busquedaBinaria(const U& k, ILista<T>& lista, std::function<U(const T&)> attributeGetter)
{
    int n = lista.contar();
    int nActual = n / 2;

    while (k != attributeGetter(lista.conseguirDato(nActual)))
    {
        if (nActual / 2 == nActual)
        {
            nActual = -1;
            break;
        }

        if (k < attributeGetter(lista.conseguirDato(nActual)))
        {
            nActual /= 2;
        }
        else
        {
            nActual = nActual * 3 / 2;
        }
    }
    return nActual;
}
