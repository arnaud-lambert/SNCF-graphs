#include "Sommet.h"

Sommet::Sommet(int id, std::string nom, double x, double y)
        :m_id{id}, m_nom{nom}, m_x{x}, m_y{y}
{

}

Sommet::~Sommet()
{

}

void Sommet::affichage()const
{
    std::cout << m_id << " " << m_nom << " " << m_x << " " << m_y << " adj: ";

    for(auto &i : m_adjacents)
        std::cout << i.first->getId() << "=" << i.second << " ";
}

void Sommet::setPoidsAdjacent(double poids, Sommet* adjacent)
{
    for(size_t i = 0; i < m_adjacents.size() ; ++i)
        if(m_adjacents[i].first == adjacent)
        {
            m_adjacents[i].second = poids;
            break;
        }
}

void Sommet::dessiner(Svgfile&svgout)
{
    std::string couleur;
    switch((int)m_adjacents.size())
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
