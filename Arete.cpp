#include "Arete.h"

Arete::Arete(int id, Sommet* s1, Sommet* s2)
        :m_id{id}, m_extremites{s1, s2}
{

}

Arete::~Arete()
{
    //dtor
}

void Arete::affichage()const
{
    std::cout<<m_id<<" "<<m_extremites.first->getId()<<" "<<m_extremites.second->getId()<<std::endl;
}

void Arete::dessiner(Svgfile&svgout)
{
    svgout.addLine( (m_extremites.first)->getX()*100, (m_extremites.first)->getY()*100, (m_extremites.second)->getX()*100, (m_extremites.second)->getY()*100, "black");
}
