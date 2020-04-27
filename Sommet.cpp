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
    std::string couleur;
    switch((int)m_degre)
    {
        case 1 : couleur="cyan";
                 break;
        case 2 : couleur="green";
                 break;
        case 3 : couleur="blue";
                 break;
        case 4 : couleur="red";
                 break;
        default : couleur="black";
                 break;
    }
    svgout.addDisk( m_x*100 , m_y*100 , 5, couleur);
    svgout.addText( m_x*100 - 5 , m_y*100 - 10, m_nom, "black" );
}

void Sommet::setDegre (double degre, int ordre)
{
    m_degre = degre;
    m_degre_norm = degre/(double)ordre;
}
