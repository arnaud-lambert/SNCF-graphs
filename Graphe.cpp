#include "Graphe.h"

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
    ///On recupere l'odre
    ifs>>m_ordre;
    int indice;
    double x, y;
    ///On cree un nombre de sommet egal a l'odre du graphe
    for(int i=0; i<m_ordre; ++i)
    {
        ifs>>indice>>nom>>x>>y;
        Sommet* nouv = new Sommet(indice, nom, x, y);
        m_sommets.push_back(nouv);
    }
    ///On recupere la taille du graphe
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
        std::cout << std::endl;
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

std::vector<std::pair<Sommet*, double>> Graphe::vecteurPropre()
{
    std::vector<std::pair<Sommet*, double>> vecIndices;
    std::vector<double> vecSommeIndices;
    double lambda1=0, lambda2, sommeIndices=0, sommeSommeIndicesCarre=0;
    for(size_t i=0; i<m_sommets.size(); ++i)
        vecIndices.push_back({m_sommets[i], 1});
    do
    {
        lambda2=lambda1;
        for(size_t i=0; i<vecIndices.size(); ++i)
        {
            for(size_t j=0; j<vecIndices[i].first->getAdjacents().size(); ++j)
                sommeIndices+=vecIndices[vecIndices[i].first->getAdjacents()[j].first->getId()].second;

            vecSommeIndices.push_back(sommeIndices);
            sommeIndices=0;
        }
        for(size_t i=0; i<vecSommeIndices.size(); ++i)
            sommeSommeIndicesCarre+=pow(vecSommeIndices[i], 2);

        lambda1=sqrt(sommeSommeIndicesCarre);
        sommeSommeIndicesCarre=0;
        for(size_t i=0; i<vecIndices.size(); ++i)
            vecIndices[i].second=vecSommeIndices[i]/lambda1;
        vecSommeIndices.clear();
    }
    while(abs(lambda1-lambda2)>0.1);

    /*std::cout<<std::endl<<"Indices des sommets selon le vecteur propre"<<std::endl;
    for(size_t i=0; i<vecIndices.size(); i++)
            std::cout<<vecIndices[i].first->getId()<<" "<<vecIndices[i].second<<std::endl;*/
    return vecIndices;
}


std::vector<std::pair<int, float>> Graphe::centraliteDegre ()
{
   std::vector<std::pair<int, float>> centralite_degres;
   std::pair<int, float> degres;//first est degre non normalisé et second est degré normalisé

   for(auto i: m_sommets)
   {
       degres.first = i->getAdjacents().size();
       degres.second = ((float)i->getAdjacents().size())/((float)m_ordre-1);
       centralite_degres.push_back(degres);
   }

//   std::cout<<std::endl<<"Indices de centralite de degre: "<<std::endl;
//   for(auto j: centralite_degres)
//   {
//       std::cout<<"Non normalise: "<<j.first<<"  Normalise: "<<j.second<<std::endl;
//   }

   return centralite_degres;
}

void recursif (int &k, Sommet* i, Sommet* current, std::unordered_map<Sommet*, std::pair<std::vector<Sommet*>,int>> &predecesseurs)
{
    if (current == i)
    ++k;
    else if(predecesseurs.find(current) != predecesseurs.end())
       for(auto &j : predecesseurs[current].first)
            recursif(k,i,j,predecesseurs);
}

std::vector<double> Graphe::intermediarite()
{
    Sommet* courant;
    int longueur;
    std::vector<double> centralite(m_ordre,0.0);

    auto compare = [](const std::pair<Sommet*,int> s1, const std::pair<Sommet*,int> s2)
    {
        return s1.second > s2.second;
    };

    std::priority_queue<std::pair<Sommet*,int>, std::vector<std::pair<Sommet*,int>>, decltype(compare)> prio(compare);
    for(auto &j : m_sommets)
    {
        std::unordered_map<Sommet*, int> nombreChemins;
        std::unordered_map<Sommet*, std::pair<std::vector<Sommet*>,int>> predecesseurs;
        prio.push({j,0});

        while(!prio.empty())
        {
            courant = prio.top().first;
            longueur = prio.top().second;
            prio.pop();

            for(auto &i : courant->getAdjacents())
            {
                if(i.first != j)
                {
                    if(nombreChemins.find(i.first) == nombreChemins.end() || (longueur+i.second) < predecesseurs[i.first].second)//ecrase
                    {
                        prio.push({i.first,longueur+i.second});
                        predecesseurs[i.first] = {{courant},i.second+longueur};

                        if(courant == j)
                            nombreChemins[i.first] = 1;
                        else
                            nombreChemins[i.first] = nombreChemins[courant];
                    }
                    else if (longueur+i.second == predecesseurs[i.first].second)//autre chemin court
                    {
                        prio.push({i.first,longueur+i.second});

                        predecesseurs[i.first].second = i.second+longueur;
                        predecesseurs[i.first].first.push_back(courant);
                        nombreChemins[i.first] += nombreChemins[courant];
                    }
                }
            }
        }

        for(auto &l : m_sommets)
        {
            if(l != j)
                for(auto &i : nombreChemins)
                    if(i.first->getId() > j->getId() && i.first != l)
                    {
                        int k = 0;
                        for(auto &z : predecesseurs[i.first].first)
                            recursif(k,l,z,predecesseurs);

                        //std::cout << "i:" << l->getId() << " j:" << j->getId() << " k:" << i.first->getId()
                        //std::cout << " nbcheminsjk:"<< nombreChemins[i.first] <<" nbi:" << k << std::endl;
                        centralite[l->getId()]+=(k/nombreChemins[i.first]);
                    }
        }
    }

    for(auto &i : centralite)
    {
        i = 2*i/(m_ordre*m_ordre*m_ordre - 3*m_ordre + 2);
        std::cout << i << std::endl;
    }

    return centralite;
}
