#include "Fecha.hpp"
#include <iomanip>

Fecha::Fecha(int day, int month, int year)
    : tiempo(createTime(day, month, year))
{
}

Fecha::Fecha(std::time_t tiempo)
    : tiempo(tiempo)
{
}

const time_t& Fecha::getTiempo() const
{
    return tiempo;
}

void Fecha::setTiempo(int day, int month, int year)
{
    tiempo = createTime(day, month, year);
}

void Fecha::setTiempo(const std::time_t& nuevoTiempo)
{
    tiempo = nuevoTiempo;
}

std::ostream& operator<<(std::ostream& os, const Fecha& fecha)
{
    os << std::put_time(std::localtime(&fecha.tiempo), "%c %Z");
    return os;
}

std::time_t Fecha::createTime(int day, int month, int year)
{
    struct tm datetime = {};
    datetime.tm_year = year - 1900;
    datetime.tm_mon = month - 1;
    datetime.tm_mday = day;
    datetime.tm_hour = 0;
    datetime.tm_min = 0;
    datetime.tm_sec = 0;

    return mktime(&datetime);
}
