#include "Arete.h"

Arete::Arete(int id, Sommet* s1, Sommet* s2)
        :m_id{id}, m_extremites{s1, s2}
{

}

Arete::Arete(Arete&source) : m_id{source.m_id}, m_poids{source.m_poids}
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

void Arete::setExtremites (Sommet*s1, Sommet*s2)
{
    m_extremites.first=s1;
    m_extremites.second=s2;
}

void Arete::dessiner(Svgfile&svgout)
{
    svgout.addLine( (m_extremites.first)->getX()*100, (m_extremites.first)->getY()*100, (m_extremites.second)->getX()*100, (m_extremites.second)->getY()*100, "black");
    svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)+15, m_id, "darkorchid");
    if(m_poids!=0)//si fichier avec pond�ration
        svgout.addText( (m_extremites.first->getX() + m_extremites.second->getX())*50, ((m_extremites.first->getY()+ m_extremites.second->getY())*50)-5, m_poids, "grey" );
}
