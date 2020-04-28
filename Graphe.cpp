#include "Graphe.h"
#include <unordered_map>
#include <set>
#include <queue>

///Lecture du fichier via le constructeur de Graphe
Graphe::Graphe()
{
    std::cout<<"Quel fichier voulez-vous ouvrir ? ";
    std::string nom;
    std::ifstream ifs;
    do
    {
        std::cin>>nom;
        ifs.open(nom);
        if(!ifs)
            std::cout << "Impossible d'ouvrir le fichier " << nom << ", veuillez ressaisir un nom de fichier" << std::endl;
    }
    while(!ifs);

    ifs>>m_orientation;
    ///On r�cup�re l'odre
    ifs>>m_ordre;
    int indice;
    double x, y;
    ///On cr�e un nombre de sommet �gal � l'odre du graphe
    for(int i=0; i<m_ordre; ++i)
    {
        ifs>>indice>>nom>>x>>y;
        Sommet* nouv = new Sommet(indice, nom, x, y);
        m_sommets.push_back(nouv);
    }
    ///On r�cup�re la taille du graphe
    ifs>>m_taille;
    int extremite1, extremite2;
    for(int i=0; i<m_taille; ++i)
    {
        ifs>>indice>>extremite1>>extremite2;
        Arete* nouv = new Arete(indice, m_sommets[extremite1], m_sommets[extremite2]);
        m_aretes.push_back(nouv);
        m_sommets[extremite1]->ajouterAdjacent(m_sommets[extremite2]);
        if(!m_orientation)
            m_sommets[extremite2]->ajouterAdjacent(m_sommets[extremite1]);
    }
}

Graphe::~Graphe()
{
    for(auto &i: m_sommets)
        delete i;
    for(auto &i: m_aretes)
        delete i;
}

void Graphe::affichage()const
{
    std::cout<<std::endl<<m_orientation<<std::endl<<m_ordre<<std::endl;
    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        m_sommets[i]->affichage();
//            std::cout << "ajd: ";
//        for(auto j : m_sommets[i]->getAdjacents())
//            std::cout << j->getId() << " ";
//        std::cout << std::endl;
    }
    std::cout<<m_taille<<std::endl;
    for(size_t i=0; i<m_aretes.size(); ++i)
        m_aretes[i]->affichage();
}

void Graphe::ponderation()
{
    int taille;
    bool verif=false;
    std::cout<<std::endl<<"Quel fichier de ponderation voulez-vous ouvrir ? ";
    std::string nomFichier;
    std::cin.ignore();
    do
    {
        nomFichier="";
        std::getline(std::cin, nomFichier);
        std::ifstream ifs{nomFichier};
        if(ifs)
        {
            ifs>>taille;
            if(taille==m_taille)
            {
                verif=true;
                m_ponderation=true;
                int id;
                double poids;
                for(int i=0; i<taille; ++i)
                {
                    ifs>>id>>poids;
                    m_aretes[id]->setPoids(poids);
                }
            }
            else
                std::cout<<std::endl<<"Fichier est incompatible (nombre d'aretes different), veuillez ressaisir un nom de fichier "<<std::endl;
        }
        else if(nomFichier!="")
            std::cout<<std::endl<<"Impossible d'ouvrir le fichier " << nomFichier << ", veuillez ressaisir un nom de fichier "<<std::endl;
    }
    while(nomFichier!="" && !verif);
}

void Graphe::dessiner ()
{
    Svgfile svgout;
    svgout.addGrid();

    for(size_t i=0; i<m_sommets.size(); ++i)
        m_sommets[i]->dessiner(svgout);

    for(size_t j=0; j<m_aretes.size(); ++j)
        m_aretes[j]->dessiner(svgout);
}

void Graphe::vecteurPropre()
{
    std::vector<std::pair<Sommet*, double>> vecIndices;
    std::vector<double> vecSommeIndices;
    double lambda1=0, lambda2, sommeIndices=0, sommeSommeIndicesCarre=0;
    for(size_t i=0; i<m_sommets.size(); ++i)
        vecIndices.push_back({m_sommets[i], 1});
    do
    {
        lambda2=lambda1;
        lambda1=0;
        for(size_t i=0; i<vecIndices.size(); ++i)
        {
            for(size_t j=0; j<vecIndices[i].first->getAdjacents().size(); ++j)
                sommeIndices+=vecIndices[vecIndices[i].first->getAdjacents()[j]->getId()].second;

            vecSommeIndices.push_back(sommeIndices);
            sommeIndices=0;
        }
        for(size_t i=0; i<vecSommeIndices.size(); ++i)
            sommeSommeIndicesCarre+=pow(vecSommeIndices[i], 2);

        lambda1=sqrt(sommeSommeIndicesCarre);
        sommeSommeIndicesCarre=0;
        for(size_t i=0; i<vecIndices.size(); i++)
            vecIndices[i].second=vecSommeIndices[i]/lambda1;
        vecSommeIndices.clear();
    }
    while(lambda1>lambda2+0.1 || lambda1<lambda2-0.1);
}


std::vector<std::pair<int, double>> Graphe::centraliteDegre ()
{
    std::vector<std::pair<int, double>> centralite_degres;
    std::pair<int, double> degres;//first est degre non normalisé et second est degré normalisé

    for(auto i: m_sommets)
    {
        degres.first = i->getAdjacents().size();
        degres.second = ((double)i->getAdjacents().size())/((double)m_ordre-1);
        centralite_degres.push_back(degres);
    }

//   std::cout<<std::endl<<"Indices de centralite de degre: "<<std::endl;
//   for(auto j: centralite_degres)
//   {
//       std::cout<<"Non normalise: "<<j.first<<"  Normalise: "<<j.second<<std::endl;
//   }

    return centralite_degres;
}


///version modifs
void Graphe::rechercheCC ()
{
    std::queue<Sommet*> file;
    std::set<Sommet*> marques;
    std::vector<Sommet*> sommets_isoles;
    int compteur=0;

    Sommet*parcours=m_sommets[0];
    do
    {
        file.push(parcours);
        marques.insert(parcours);
        ++compteur;

        while(!file.empty())
        {
            file.pop();

            for(auto i: parcours->getAdjacents())
            {
                if(marques.find(i)==marques.end())
                {
                    marques.insert(i);
                    file.push(i);
                }
            }
            if(!file.empty())
                parcours=file.front();
        }

        if((int)marques.size()!=m_ordre)
        {
            for(auto s: m_sommets)
            {
                if(marques.find(s)==marques.end())
                    parcours=s;
            }
        }

    }
    while((int)marques.size()!=m_ordre);

    if(compteur==1)
        std::cout<<"Graphe connexe"<<std::endl;
    else
        std::cout<<"Graphe non connexe. Il contient "<<compteur<<" composantes connexes"<<std::endl;

    for(auto s: m_sommets)
    {
        if((s->getAdjacents()).size()==0)
            std::cout<<s->getNom()<<" est un sommet isole"<<std::endl;
    }


}



void Graphe::supprimerArete ()
{
    int indice=0;

    do
    {
        std::cout<<"Saisissez l'indice de l'arete a supprimer svp: ";
        std::cin>>indice;
    }
    while((indice<0)||(indice>(int)m_aretes.size()-1));

    std::pair<Sommet*, Sommet*> extremites = m_aretes[indice]->getExtremites();
    for(auto s: m_sommets)
    {
        if(s==extremites.first)
            s->suppAdjacent(extremites.second);

        if (s==extremites.second)
            s->suppAdjacent(extremites.first);

    }
    m_aretes.erase(m_aretes.begin()+indice);
    --m_taille;

}


void Graphe::testConnexite ()
{
    int nb=0;
    do{
    std::cout<<std::endl<<"Combien d'aretes voulez vous supprimer ? ";
    std::cin>>nb;
    }while((nb<0)||(nb>(int)m_taille));

    for(int i=0; i<nb; ++i)
    {
        this->supprimerArete();
    }

    this->rechercheCC();
}






