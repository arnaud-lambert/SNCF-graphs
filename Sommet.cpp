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

void Sommet::dessiner(Svgfile&svgout)
{
    svgout.addDisk( m_x*100 , m_y*100 , 5, "black");
    svgout.addText( m_x*100 - 5 , m_y*100 - 10, m_nom, "black" );
}
