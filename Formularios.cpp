#include "Formularios.hpp"
#include "Validaciones.hpp"
#include <string>
int getch();

// TODO: remover los getch, deberían ir en la lógica principal, no aquí

namespace
{
    const std::string& conseguirIdAutor(const Autor& a)
    {
        return a.getId();
    }

    const std::string& conseguirIdLibro(const Libro& l)
    {
        return l.getId();
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
}

void formularioInsercionAutor(ILista<Autor>* listaAutores)
{
    std::cout << "Ingrese el cedula del autor a insertar (0 para cancelar): ";
    std::string cedula = Validaciones::leerCedula();
    std::cout << std::endl;

    if (ILista<Autor>::buscar(*listaAutores, cedula, std::function<const std::string&(const Autor&)>(conseguirIdAutor)) != listaAutores->contar())
    {
        std::cout << "El cedula ya existe en la lista";
        getch();
        return;
    }

    if (cedula == "0" || cedula == "")
    {
        std::cout << "No se ha insertado";
        getch();
        return;
    }

    if (!Validaciones::validarCedula(cedula))
    {
        std::cout << "CÉDULA NO VÁLIDA";
        getch();
        return;
    }

    std::cout << "Ingrese el nombre a insertar (vacío para cancelar): ";
    std::string nombre = Validaciones::leerPalabra();
    std::cout << std::endl;

    if (nombre.empty())
    {
        std::cout << "No se ha insertado";
        getch();
        return;
    }

    std::cout << "Ingrese el segundo nombre a insertar: ";
    std::string nombre2 = Validaciones::leerPalabra();
    std::cout << std::endl;


    std::cout << "Ingrese el apellido a insertar: ";
    std::string apellido = Validaciones::leerPalabra();
    std::cout << std::endl;

    listaAutores->insertarACola(Autor(cedula, nombre, nombre2, apellido));
}

void formularioBusquedaAutor(ILista<Autor>* listaAutores)
{
    std::cout << "Ingrese el numero del libro a buscar: ";
    std::string cedula = Validaciones::leerNumero();
    std::cout << std::endl;

    if (ILista<Autor>::buscar(*listaAutores, cedula, std::function<const std::string & (const Autor&)>(conseguirIdAutor)) != listaAutores->contar())
        std::cout << "El autor con cedula " << cedula << " se encuentra en la lista" << std::endl;
    else
        std::cout << "El autor con cedula " << cedula << " NO se encuentra en la lista" << std::endl;
    getch();
}

void formularioEliminacionAutor(ILista<Autor>* listaAutores, ILista<Libro>* listaLibros)
{
    std::cout << "Ingrese el cedula del autor a eliminar: ";
    std::string cedula = Validaciones::leerCedula();
    std::cout << std::endl;

    if (!Validaciones::validarCedula(cedula))
    {
        std::cout << "CÉDULA NO VÁLIDA";
        getch();
        return;
    }

    Autor* punteroAutor = punteroAutorEnLista(cedula, listaAutores);
    if (punteroAutor)
    {
        int nLibrosAutor = punteroAutor->libros.contar();
        int nLibrosTotal = listaLibros->contar();

        for (int i = 0; i < nLibrosAutor; i++)
        {
            for (int j = 0; j < nLibrosTotal; j++)
            {
                if (punteroAutor->libros.conseguirDato(i) == &(listaLibros->conseguirDato(j)))
                {
                    listaLibros->eliminarPos(j);
                    break;
                }
            }
        }

        ILista<Autor>::eliminar(*listaAutores, cedula, std::function<const std::string & (const Autor&)>(conseguirIdAutor));
        std::cout << "El autor con numero " << cedula << " ha sido eliminado de la lista";
    }
    else
        std::cout << "El autor con numero " << cedula << " NO se encuentra en la lista";

    getch();
}

void formularioInsercionLibro(ILista<Libro>* listaLibros, ILista<Autor>* listaAutores)
{
    std::cout << "Ingrese el id del libro a insertar (0 para cancelar): ";
    std::string id = Validaciones::leerNumero();
    std::cout << std::endl;

    if (ILista<Libro>::buscar(*listaLibros, id, std::function<const std::string&(const Libro&)>(conseguirIdLibro)) != listaLibros->contar())
    {
        std::cout << "El id ya existe en la lista";
        getch();
        return;
    }

    if (id == "0" || id == "")
    {
        std::cout << "No se ha insertado";
        getch();
        return;
    }

    std::cout << "Ingrese el título del libro a insertar (vacío para cancelar): ";
    std::string titulo = Validaciones::leerTitulo();
    std::cout << std::endl;

    if (titulo.empty())
    {
        std::cout << "No se ha insertado";
        getch();
        return;
    }

    std::cout << "Ingrese el año del libro a insertar: ";
    int year = Validaciones::leerInt() - 1900;
    std::cout << std::endl;

    if (year < 0)
    {
        std::cout << "AÑO NO VALIDO" << std::endl;
        getch();
        return;
    }




    Autor* punteroAutorAEnlazar = NULL;
    // para comprobar que un autor se ha insertado (la cola ha cambiado)
    Autor* punteroAutorEnCola = &(listaAutores->getCola()->dato);

    std::cout << "Ingrese el cedula del autor del libro (0 para cancelar, vacío para nuevo autor): \033[s";

    do
    {
        std::cout << "\033[u\033[J" << std::endl << std::endl;

        int n = listaAutores->contar();
        for (int i = 0; i < n; i++)
        {
            std::cout << listaAutores->conseguirDato(i) << std::endl;
        }

        std::cout << "\033[u";
        std::string idAutor = Validaciones::leerCedula();
        std::cout << "\033[J";

        std::cout << "\033[J" << std::endl;

        if (idAutor == "0")
        {
            std::cout << "No se ha insertado";
            getch();
            return; // sale de la función lambda
        }

        if (idAutor.empty())    // si el campo del autor se ha dejado vacío
        {
            formularioInsercionAutor(listaAutores);    // llama la función para insertar un nuevo autor

            if (punteroAutorEnCola != &(listaAutores->getCola()->dato)) // comprueba que un nuevo autor sí haya sido insertado
                punteroAutorAEnlazar = &(listaAutores->getCola()->dato);    // si es así, se lo asigna a punteroAutor
        }
        else    // si el campo de autor sí se ha rellenado
        {
            if (!Validaciones::validarCedula(idAutor))  // si no es una cédula válida
            {
                std::cout << "CÉDULA NO VÁLIDA";
                getch();
                return; // sale de la función lambda
            }

            punteroAutorAEnlazar = punteroAutorEnLista(idAutor, listaAutores);

            // si no lo encuentra (punteroAutorAEnlazar es NULL)
            if (punteroAutorAEnlazar == NULL)
            {
                std::cout << "El id ingresado no se encuentra en la lista, desea añadir un autor? (Y/n): ";
                char c = tolower(getch());
                std::cout << std::endl;
                if (c != 'n')
                {
                    formularioInsercionAutor(listaAutores);    // llama la función para insertar autores

                    if (punteroAutorEnCola != &(listaAutores->getCola()->dato)) // comprueba que un nuevo autor sí haya sido insertado
                        punteroAutorAEnlazar = &(listaAutores->getCola()->dato);
                }

            }
        }
    } while (punteroAutorAEnlazar == NULL);

    listaLibros->insertarACola(Libro(id, titulo, punteroAutorAEnlazar, Fecha(0, 0, year)));
    punteroAutorAEnlazar->libros.insertarACola(&(listaLibros->getCola()->dato));
}

void formularioBusquedaLibro(ILista<Libro>* listaLibros)
{
    std::cout << "Ingrese el numero del libro a buscar: ";
    std::string id = Validaciones::leerNumero();
    std::cout << std::endl;

    if (ILista<Libro>::buscar(*listaLibros, id, std::function<const std::string & (const Libro&)>(conseguirIdLibro)) != listaLibros->contar())
        std::cout << "El libro con numero " << id << " se encuentra en la lista" << std::endl;
    else
        std::cout << "El libro con numero " << id << " NO se encuentra en la lista" << std::endl;
    getch();
}

void formularioEliminacionLibro(ILista<Libro>* listaLibros)
{
    std::cout << "Ingrese el numero del libro a eliminar: ";
    std::string id = Validaciones::leerNumero();
    std::cout << std::endl;
    Libro* punteroLibro = punteroLibroEnLista(id, listaLibros);
    if (punteroLibro)
    {
        punteroLibro->getAutor()->libros.eliminar(punteroLibro);
        ILista<Libro>::eliminar(*listaLibros, id, std::function<const std::string & (const Libro&)>(conseguirIdLibro));
        std::cout << "El libro con numero " << id << " ha sido eliminado de la lista";
    }
    else
        std::cout << "El libro con numero " << id << " NO se encuentra en la lista";

    getch();
}
