#include "Sommet.h"

Sommet::Sommet(int indice, char nom, int x, int y)
        :m_indice{indice}, m_nom{nom}, m_coordonnees{x, y}
{

}

Sommet::~Sommet()
{
    //dtor
}

void Sommet::affichage()const
{
    std::cout<<std::endl<<m_indice<<" "<<m_nom<<" "<<m_coordonnees.first<<" "<<m_coordonnees.second;
}

int Sommet::getId()
{
    return m_indice;
}
