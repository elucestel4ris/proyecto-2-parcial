#include <chrono>
#include <ctime>
#include <iostream>

class Fecha
{
private:
    std::time_t tiempo;

public:
    Fecha(int day = 0, int month = 0, int year = 0);
    Fecha(std::time_t tiempo);
    const std::time_t& getTiempo() const;
    void setTiempo(int day, int month, int year);
    void setTiempo(const std::time_t& nuevoTiempo);
    static std::time_t createTime(int day, int month, int year);

    friend std::ostream& operator<<(std::ostream& os, const Fecha& fecha);
};