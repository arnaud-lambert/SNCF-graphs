#include "Sommet.h"

Sommet::Sommet(int id, std::string nom, double x, double y)
        :m_id{id}, m_nom{nom}, m_x{x}, m_y{y}
{

}

Sommet::~Sommet()
{
    //dtor
}

void Sommet::affichage()const
{
    std::cout << m_id << " " << m_nom << " " << m_x << " " << m_y << std::endl;
}
