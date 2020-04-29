#include "Graphe.h"
#include <set>

///Lecture du fichier via le constructeur de Graphe
Graphe::Graphe(std::string& nomFichier)
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
    nomFichier=nom;
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
    if(nomFichier == "")
    {
        m_ponderation = false;
        for (auto &i : m_aretes)
            i->setPoids(1.0);
    }
}

void Graphe::dessiner ()
{
    Svgfile svgout;
    svgout.addGrid();

    for(size_t i=0; i<m_sommets.size(); ++i)
        m_sommets[i]->dessiner(svgout);

    for(size_t j=0; j<m_aretes.size(); ++j)
        m_aretes[j]->dessiner(svgout);

    ///legende
    svgout.addRectangle(870, 600, 120, 130, 5, 5, "white");
    svgout.addText(880, 620, "indice arete", "darkorchid");
    svgout.addText(880, 640, "poids arete", "darkgrey");
    svgout.addDisk(885, 655, 5, "cyan");
    svgout.addText(895, 660, "C_degre = 1", "black");
    svgout.addDisk(885, 675, 5, "green");
    svgout.addText(895, 680, "C_degre = 2", "black");
    svgout.addDisk(885, 695, 5, "blue");
    svgout.addText(895, 700, "C_degre = 3", "black");
    svgout.addDisk(885, 715, 5, "red");
    svgout.addText(895, 720, "C_degre = 4", "black");

}

std::vector<std::pair<double, double>> Graphe::vecteurPropre()
{
    std::vector<std::pair<double, double>> vecIndices;
    std::vector<double> vecSommeIndices;
    double lambda1=0, lambda2, sommeIndices=0, sommeSommeIndicesCarre=0;
    for(size_t i=0; i<m_sommets.size(); ++i)
        vecIndices.push_back({0, 1});
    do
    {
        lambda2=lambda1;
        for(size_t i=0; i<m_sommets.size(); ++i)
        {
            for(size_t j=0; j<m_sommets[i]->getAdjacents().size(); ++j)
                sommeIndices+=vecIndices[m_sommets[i]->getAdjacents()[j].first->getId()].second;

            vecSommeIndices.push_back(sommeIndices);
            sommeIndices=0;
        }
        for(size_t i=0; i<vecSommeIndices.size(); ++i)
            sommeSommeIndicesCarre+=pow(vecSommeIndices[i], 2);

        lambda1=sqrt(sommeSommeIndicesCarre);
        sommeSommeIndicesCarre=0;
        for(size_t i=0; i<vecIndices.size(); ++i)
        {
            vecIndices[i].first=vecSommeIndices[i];
            if(lambda1!=0)
                vecIndices[i].second=vecIndices[i].first/lambda1;
            else
                vecIndices[i].second=0;
        }
        vecSommeIndices.clear();
    }
    while(abs(lambda1-lambda2)>0.01);

    std::cout<<std::endl<<"Indices des sommets selon le vecteur propre"<<std::endl;
    /*for(size_t i=0; i<m_sommets.size(); i++)
            std::cout<<m_sommets[i]->getId()<<" "<<vecIndices[i].first<<" "<<vecIndices[i].second<<std::endl;*/
    return vecIndices;
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
                if(marques.find(i.first)==marques.end())
                {
                    marques.insert(i.first);
                    file.push(i.first);
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
    std::set<int> indices;
    for(auto a: m_aretes)
    {
        indices.insert(a->getId());
    }

    do
    {
        std::cout<<"Saisissez l'indice de l'arete a supprimer svp: ";
        std::cin>>indice;
    }
    while(indices.find(indice)==indices.end());

    std::pair<Sommet*, Sommet*> extremites = m_aretes[indice]->getExtremites();
    for(auto s: m_sommets)
    {
        if(s==extremites.first)
            s->suppAdjacent(extremites.second);

        if (s==extremites.second)
            s->suppAdjacent(extremites.first);

    }

    for(size_t i=0; i<m_aretes.size(); ++i)
    {
        if(m_aretes[i]->getId()==indice)
            m_aretes.erase(m_aretes.begin()+i);
    }
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


void recursif (std::unordered_map<Sommet*,unsigned int> &compt, Sommet* current, std::unordered_map<Sommet*, std::pair<std::vector<Sommet*>,double>> &predecesseurs)
{
    if ( predecesseurs.find(current) != predecesseurs.end())
    {
       ++compt[current];
       for(auto &j : predecesseurs[current].first)
            recursif(compt,j,predecesseurs);
    }
}

std::vector<std::pair<double,double>> Graphe::intermediarite()
{
    Sommet* courant;
    double longueur;
    std::vector<std::pair<double,double>> centralite(m_ordre,{0.0,0.0});

    auto compare = [](const std::pair<Sommet*,double> s1, const std::pair<Sommet*,double> s2)
    { return s1.second > s2.second; };

    std::priority_queue<std::pair<Sommet*,double>, std::vector<std::pair<Sommet*,double>>, decltype(compare)> prio(compare);
    for(auto &j : m_sommets)
    {
        std::unordered_map<Sommet*, int> nombreChemins;
        std::unordered_map<Sommet*, std::pair<std::vector<Sommet*>,double>> predecesseurs;
        prio.push({j,0.0});

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

        for(auto &k : predecesseurs)
            if(m_orientation || k.first->getId() > j->getId())
                for(auto &z : predecesseurs[k.first].first)
                {
                    std::unordered_map<Sommet*,unsigned int> compt;
                    recursif(compt,z,predecesseurs);
                    for(auto &i : compt)
                        centralite[i.first->getId()].first += (float) i.second/nombreChemins[k.first];
                }

    }

    for(auto &i : centralite)
    {
        if(m_orientation)
            i.first /= 2.0;

         i.second = i.first *2.0/(float)(m_ordre*m_ordre - 3.0*m_ordre + 2.0);
        std::cout << i.first  << " " << i.second<< std::endl;
    }

    return centralite;
}

std::vector<std::pair<double, double>> Graphe::vecteurProximite()
{
    std::vector<double> distance(m_ordre, RAND_MAX);
    Sommet* sommetCourant;

    std::map<Sommet*, std::pair<Sommet*, double>> predecesseur;
    std::vector<std::pair<double, double>> indiceSommets;

    auto comparaison=[](const std::pair<Sommet*, double> s1, const std::pair<Sommet*, double> s2)
    { return s1.second > s2.second; };

    std::priority_queue<std::pair<Sommet*, double>, std::vector<std::pair<Sommet*, double>>, decltype(comparaison)> file(comparaison);

    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        indiceSommets.push_back({0, 0});
        file.push({m_sommets[i], 0});
        distance[m_sommets[i]->getId()]=0;
        while(!file.empty())
        {
            sommetCourant=file.top().first;
            file.pop();
            for(size_t j=0; j<sommetCourant->getAdjacents().size(); j++)
            {
                if(distance[sommetCourant->getAdjacents()[j].first->getId()] > distance[sommetCourant->getId()] + sommetCourant->getAdjacents()[j].second)
                {
                    distance[sommetCourant->getAdjacents()[j].first->getId()] = distance[sommetCourant->getId()] + sommetCourant->getAdjacents()[j].second;
                    file.push({sommetCourant->getAdjacents()[j].first, distance[sommetCourant->getAdjacents()[j].first->getId()]});
                    predecesseur[sommetCourant->getAdjacents()[j].first]={sommetCourant, distance[sommetCourant->getAdjacents()[j].first->getId()]};
                }
            }
        }
        for(size_t j=0; j<m_sommets.size(); j++)
            indiceSommets[i].first+=distance[j];

        if(indiceSommets[i].first!=0)
            indiceSommets[i].first=1/indiceSommets[i].first;

        else
            indiceSommets[i].first=0;

        indiceSommets[i].second=indiceSommets[i].first*(m_ordre-1);
        predecesseur.clear();
        distance.clear();
        distance.resize(m_ordre, RAND_MAX);
    }
    /*for(size_t i=0; i<m_sommets.size(); i++)
        std::cout<<m_sommets[i]->getId()<<" "<<indiceSommets[i].first<<" "<<indiceSommets[i].second<<std::endl;*/
    return indiceSommets;
}

void Graphe::sauvegarder(std::vector<std::pair<int, double>> centralite_degres, std::vector<std::pair<double, double>> vecteurPropre, std::vector<std::pair<double, double>> vecteurProximite, std::vector<std::pair<double, double>> intermediarite, std::string nomFichier)
{
    bool verif=false;
    int occurence=0;
    std::string fichierSauvegarde;
    while(!verif)
    {
        fichierSauvegarde=nomFichier + "_save" + std::to_string(occurence) + ".txt";
        std::ifstream ifs{fichierSauvegarde};
        if(!ifs)
            verif=true;
        occurence++;
    }
    std::ofstream ofs{fichierSauvegarde};
    if(!ofs)
        std::cout<<"Ouverture impossible"<<std::endl;
    else
    {
        for(size_t i=0; i<m_sommets.size(); i++)
            ofs<<i<<" "<<centralite_degres[i].first<<" "<<centralite_degres[i].second<<" "
                       <<vecteurPropre[i].first<<" "<<vecteurPropre[i].second<<" "
                       <<vecteurProximite[i].first<<" "<<vecteurProximite[i].second<<" "
                       <<intermediarite[i].first<<" "<<intermediarite[i].second;
    }
}
