#include "Graphe.h"
#include <set>
#include <algorithm>

///Lecture du fichier via le constructeur de Graphe
Graphe::Graphe(std::string& nomFichier)
{
    std::cout<<"Quel ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<"fichier";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" voulez-vous ouvrir ? ";
    std::string nom;
    std::ifstream ifs;
    do
    {
        SetConsoleTextAttribute(texteConsole, 10);
        std::cin>>nom;
        SetConsoleTextAttribute(texteConsole, 15);
        ifs.open(nom + ".txt");
        if(!ifs)
        {
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<std::endl<<"Fichier inexistant";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<< ", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" un nom : ";
        }
    }
    while(!ifs);
    std::cout<<std::endl<<"Lecture du fichier ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<nom;
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<"..."<<std::endl;
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
        m_sommets[extremite1]->ajouterAdjacent(m_sommets[extremite2],nouv);
        if(!m_orientation)
            m_sommets[extremite2]->ajouterAdjacent(m_sommets[extremite1],nouv);
    }
}

//constructeur copie
Graphe::Graphe(const Graphe&source): m_sommets{source.m_sommets}, m_aretes{source.m_aretes}, m_orientation{source.m_orientation},
    m_ponderation{source.m_ponderation}, m_ordre{source.m_ordre}, m_taille{source.m_taille}
{
    std::map<Sommet*,Sommet*>transpose;
    std::map<Arete*, Arete*>trans_aretes;

    for(size_t i=0; i<source.m_sommets.size(); ++i)
    {
        m_sommets[i]=new Sommet(*source.m_sommets[i]);
        transpose[source.m_sommets[i]]=m_sommets[i];//associe nvelle adresse de chaque sommet à l'ancienne
    }

    for(size_t i=0; i<source.m_aretes.size(); ++i)
    {
        m_aretes[i]=new Arete(*source.m_aretes[i]);
        trans_aretes[source.m_aretes[i]]=m_aretes[i];
        m_aretes[i]->setExtremites(transpose[source.m_aretes[i]->getExtremites().first], transpose[source.m_aretes[i]->getExtremites().second]);
    }

    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        for(auto j: source.m_sommets[i]->getAdjacents())
        {
            m_sommets[i]->ajouterAdjacent(transpose[j.first], trans_aretes[j.second]);
        }
    }

}


Graphe::~Graphe()
{
    for(auto &i: m_sommets)
        delete i;
    for(auto &i: m_aretes)
        delete i;
}

/*void Graphe::affichage()const
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
}*/

void Graphe::ponderation()
{
    int taille;
    bool verif=false;
    std::cout<<"Quel ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<"fichier";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de ";
    SetConsoleTextAttribute(texteConsole, 10);
    std::cout<<"ponderation";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" voulez-vous ouvrir ? ";
    std::string nomFichier;
    std::cin.ignore();
    do
    {
        nomFichier="";
        SetConsoleTextAttribute(texteConsole, 10);
        std::getline(std::cin, nomFichier);
        SetConsoleTextAttribute(texteConsole, 15);
        std::ifstream ifs{nomFichier + ".txt"};
        if(ifs)
        {
            ifs>>taille;
            if(taille==m_taille)
            {
                verif=true;
                std::cout<<std::endl<<"Lecture du fichier ";
                SetConsoleTextAttribute(texteConsole, 10);
                std::cout<<nomFichier;
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<"..."<<std::endl;
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
            {
                SetConsoleTextAttribute(texteConsole, 12);
                std::cout<<std::endl<<"Fichier incompatible";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" (nombre d'aretes different), veuillez ";
                SetConsoleTextAttribute(texteConsole, 9);
                std::cout<<"ressaisir";
                SetConsoleTextAttribute(texteConsole, 15);
                std::cout<<" un nom : ";
            }
        }
        else if(nomFichier!="")
        {
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<std::endl<<"Fichier inexistant";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout << ", veuillez ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" un nom : ";
        }
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
    bool even=true;
    Svgfile svgout("Dessin_graphe.svg", 1500, 2000);
    svgout.addGrid();

    for(size_t i=0; i<m_sommets.size(); ++i)
        m_sommets[i]->dessiner(svgout);

    for(size_t j=0; j<m_aretes.size(); ++j)
    {
        m_aretes[j]->dessiner(svgout, m_orientation, even);
        if(even==true)
            even=false;
        else
            even=true;
    }

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

    /*std::cout<<std::endl<<"Indices des sommets selon le vecteur propre"<<std::endl;
    for(size_t i=0; i<m_sommets.size(); i++)
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



int Graphe::rechercheCC ()
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

        if((int)marques.size()<m_ordre)
        {
            for(auto s: m_sommets)
            {
                if(marques.find(s)==marques.end())
                    parcours=s;
            }
        }
    }
    while((int)marques.size()<m_ordre);

    return compteur;
}

void Graphe::testConnexite (int nb)
{
    Graphe copie=*this;
    int cc=0;

    for(int i=0; i<nb; ++i)
        copie.supprimerArete();

    cc= copie.rechercheCC();

    std::cout<<std::endl<<"Le graphe est ";
    SetConsoleTextAttribute(texteConsole, 14);

    if(cc==1)
        std::cout<<"connexe"<<std::endl;

    else
    {
        std::cout<<"non connexe";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<". Il contient ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<cc<<" "<<"composantes connexes"<<std::endl;
    }

    SetConsoleTextAttribute(texteConsole, 15);

    for(auto s: copie.m_sommets)
    {
        if((s->getAdjacents()).size()==0)
        {
            SetConsoleTextAttribute(texteConsole, 14);
            std::cout<<std::endl<<s->getNom();
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" est un sommet ";
            SetConsoleTextAttribute(texteConsole, 14);
            std::cout<<"isole"<<std::endl;
            SetConsoleTextAttribute(texteConsole, 15);
        }
    }

}

void recursifIntermediarite(std::pair<std::unordered_map<Sommet*,unsigned int> ,std::unordered_map<Arete*,unsigned int>> &compt, std::pair<Sommet*,Arete*> current,
        std::unordered_map<Sommet*, std::pair<std::vector<std::pair<Sommet*,Arete*>>,double>> &predecesseurs, std::unordered_map<Sommet*, int> &nombreChemins)
{
    if (predecesseurs.find(current.first) != predecesseurs.end())
    {
        compt.first[current.first]+=nombreChemins[current.first];
        compt.second[current.second]+=nombreChemins[current.first];
        for(auto &j : predecesseurs[current.first].first)
            recursifIntermediarite(compt,j,predecesseurs,nombreChemins);
    }
    else
        ++compt.second[current.second];

}

std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> Graphe::intermediarite()
{
    Sommet* courant;
    double longueur;
    std::vector<std::pair<double,double>> centraliteSommets(m_ordre,{0.0,0.0});
    std::vector<std::pair<Arete*,std::pair<double,double>>> centraliteAretes;
    std::unordered_map<Arete*,double> mapCentraliteAretes;

    auto compare = [](const std::pair<Sommet*,double> s1, const std::pair<Sommet*,double> s2)
    {
        return s1.second > s2.second;
    };

    std::priority_queue<std::pair<Sommet*,double>, std::vector<std::pair<Sommet*,double>>, decltype(compare)> prio(compare);
    for(auto &j : m_sommets)
    {
        std::unordered_map<Sommet*, int> nombreChemins;
        std::unordered_map<Sommet*, std::pair<std::vector<std::pair<Sommet*,Arete*>>,double>> predecesseurs;
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
                    if(nombreChemins.find(i.first) == nombreChemins.end() || (longueur+i.second->getPoids()) < predecesseurs[i.first].second)//ecrase
                    {
                        prio.push({i.first,longueur+i.second->getPoids()});
                        predecesseurs[i.first] = {{{courant,i.second}} ,i.second->getPoids()+longueur};

                        if(courant == j)
                            nombreChemins[i.first] = 1;
                        else
                            nombreChemins[i.first] = nombreChemins[courant];
                    }
                    else if (longueur+i.second->getPoids() == predecesseurs[i.first].second)//autre chemin court
                    {
                        predecesseurs[i.first].second = i.second->getPoids()+longueur;
                        predecesseurs[i.first].first.push_back({courant,i.second});
                        nombreChemins[i.first] += nombreChemins[courant];
                    }
                }
            }
        }


        for(auto &k : predecesseurs)
            if(m_orientation || k.first->getId() > j->getId())
                for(auto &z : predecesseurs[k.first].first)
                {
                    std::pair<std::unordered_map<Sommet*,unsigned int> ,std::unordered_map<Arete*,unsigned int>> compt;

                    recursifIntermediarite(compt,z,predecesseurs,nombreChemins);
                    for(auto &i : compt.first)
                        centraliteSommets[i.first->getId()].first += (double) i.second/nombreChemins[k.first];

                    for(auto &i : compt.second)
                        mapCentraliteAretes[i.first] += (double) i.second/nombreChemins[k.first];
                }
    }

    for(auto &i : centraliteSommets)
    {
        if(m_orientation)
            i.first /= 2.0;

         i.second =i.first *2.0/(double)((m_ordre-1)*(m_ordre-2));
        //std::cout << i.first  << " " << i.second<< std::endl;
    }

    for(auto &i : mapCentraliteAretes)
    {
        centraliteAretes.push_back({i.first,{i.second,i.second/(double)((m_taille - 1)/(m_taille))}});
        //std::cout << "ID " << i.first->getId() << ": " << i.second <<" "<< i.second*(double)2.0/((m_taille-1)*(m_taille-2)) << std::endl;
    }
    for(auto &i : m_aretes)
        if(mapCentraliteAretes.find(i) == mapCentraliteAretes.end())
            centraliteAretes.push_back({i,{0.0,0.0}});

     std::sort(centraliteAretes.begin(), centraliteAretes.end(), [](std::pair<Arete*,std::pair<double,double>> a1, std::pair<Arete*,std::pair<double,double>> a2)
        { return a1.first->getId() < a2.first->getId(); });

    return {centraliteSommets,centraliteAretes};
}

std::vector<std::pair<double, double>> Graphe::vecteurProximite()
{
    std::vector<double> distance(m_ordre, RAND_MAX);
    Sommet* sommetCourant;

    std::map<Sommet*, std::pair<Sommet*, double>> predecesseur;
    std::vector<std::pair<double, double>> indiceSommets;

    auto comparaison=[](const std::pair<Sommet*, double> s1, const std::pair<Sommet*, double> s2)
    {
        return s1.second > s2.second;
    };

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
                if(distance[sommetCourant->getAdjacents()[j].first->getId()] > distance[sommetCourant->getId()] + sommetCourant->getAdjacents()[j].second->getPoids())
                {
                    distance[sommetCourant->getAdjacents()[j].first->getId()] = distance[sommetCourant->getId()] + sommetCourant->getAdjacents()[j].second->getPoids();
                    file.push({sommetCourant->getAdjacents()[j].first, distance[sommetCourant->getAdjacents()[j].first->getId()]});
                    predecesseur[sommetCourant->getAdjacents()[j].first]= {sommetCourant, distance[sommetCourant->getAdjacents()[j].first->getId()]};
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

void Graphe::sauvegarder(std::vector<std::pair<int, double>> centralite_degres, std::vector<std::pair<double, double>> vecteurPropre, std::vector<std::pair<double, double>> vecteurProximite, std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite, std::string nomFichier)
{
    bool verif=false;
    int occurence=0;
    std::string fichierSauvegarde;
    while(!verif)
    {
        fichierSauvegarde=nomFichier + "_save" + std::to_string(occurence);
        std::ifstream ifs{fichierSauvegarde + ".txt"};
        if(!ifs)
            verif=true;
        occurence++;
    }
    std::ofstream ofs{fichierSauvegarde + ".txt"};
    if(!ofs)
    {
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<std::endl<<"Ouverture impossible";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", la sauvegarde ne peut avoir lieu"<<std::endl;
    }
    else
    {
        std::cout<<"Sauvegarde du fichier ";
        SetConsoleTextAttribute(texteConsole, 10);
        std::cout<<nomFichier;
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<"..."<<std::endl;
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<std::endl<<"Indice";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" de ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"centralite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" des sommets : ";
        SetConsoleTextAttribute(texteConsole, 3);
        std::cout<<"degre";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<"vecteur propre";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 11);
        std::cout<<"proximite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<", ";
        SetConsoleTextAttribute(texteConsole, 13);
        std::cout<<"intermediarite";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" :"<<std::endl;

        for(size_t i=0; i<m_sommets.size(); i++)
        {
            ofs<<m_sommets[i]->getId()<<" "<<centralite_degres[i].first<<" "<<centralite_degres[i].second<<" "
               <<vecteurPropre[i].first<<" "<<vecteurPropre[i].second<<" "
               <<vecteurProximite[i].first<<" "<<vecteurProximite[i].second<<" "
               <<intermediarite.first[i].first<<" "<<intermediarite.first[i].second<<std::endl;

            std::cout<<std::endl<<"Sommet "<<m_sommets[i]->getId()<<" : "<<std::endl;
            SetConsoleTextAttribute(texteConsole, 3);
            std::cout<<"   "<<centralite_degres[i].first<<" "<<centralite_degres[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"   "<<vecteurPropre[i].first<<" "<<vecteurPropre[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 11);
            std::cout<<"   "<<vecteurProximite[i].first<<" "<<vecteurProximite[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 13);
            std::cout<<"   "<<intermediarite.first[i].first<<" "<<intermediarite.first[i].second<<std::endl;
            SetConsoleTextAttribute(texteConsole, 15);
        }
        std::cout << std::endl << "Indice de ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"centralite intermediaire";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" des ";
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<"aretes";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<" : "<< std::endl << std::endl;
        ofs << std::endl << "aretes" <<std::endl;
        for(auto &i : intermediarite.second)
        {
            ofs << i.first->getId() << " " << i.second.first << " " << i.second.second << std::endl;
            std::cout << "Arete "<<i.first->getId() << " : " << " " << i.second.first << " " << i.second.second << std::endl;
        }
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

    std::cout<<std::endl<<"Saisissez l'";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"indice";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de l'";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"arete";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" a ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"supprimer";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" : ";

    do
    {
        std::cin>>indice;
        if(indices.find(indice)==indices.end())
        {
            std::cout<<std::endl<<"Cet ";
            SetConsoleTextAttribute(texteConsole, 14);
            std::cout<<"indice";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" est ";
            SetConsoleTextAttribute(texteConsole, 12);
            std::cout<<"invalide";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<", veuillez le ";
            SetConsoleTextAttribute(texteConsole, 9);
            std::cout<<"ressaisir";
            SetConsoleTextAttribute(texteConsole, 15);
            std::cout<<" : ";
        }
    }
    while(indices.find(indice)==indices.end());

    this->SuppAreteI(indice);

}

void Graphe::SuppAreteI (int indice)
{
    std::pair<Sommet*, Sommet*> extremites;

    for(auto i: m_aretes)
    {
        if(i->getId()==indice)
            extremites = i->getExtremites();
    }

    extremites.first->suppAdjacent(extremites.second);

    if(!m_orientation)
        extremites.second->suppAdjacent(extremites.first);

    for(size_t i=0; i<m_aretes.size(); ++i)
    {
        if(m_aretes[i]->getId()==indice)
        {
            delete m_aretes[i];
            m_aretes.erase(m_aretes.begin()+i);
        }

    }
    --m_taille;
}

void Graphe::supprimerSommet (Sommet*s)
{

    for(auto i: s->getAdjacents())
    {
        for(auto j: m_sommets)
        {
            if(i.first == j)
            {
                j->suppAdjacent(s);
            }
        }
    }



    int i=0;
    do
    {
        for(size_t j=0; j<m_aretes.size(); ++j)
        {
            ///
            if((m_aretes[j]->getExtremites().first->getId()==s->getId())||(m_aretes[j]->getExtremites().second->getId()==s->getId()))
            {
                m_aretes.erase(m_aretes.begin()+j);
                --m_taille;
                ++i;
                break;
            }
        }

    }
    while(i<(int)s->getAdjacents().size());


    for(size_t i=0; i<m_sommets.size(); ++i)
    {
        ///
        if(m_sommets[i]->getId()==s->getId())
        {
            m_sommets.erase(m_sommets.begin()+i);
            --m_ordre;
        }
    }
}

void Graphe::kSommetsConnexite ()
{
    std::vector<int>nb;
    std::vector<std::unordered_set<int>> chemins_p;
    int compteur=1, h=0;
    std::vector <int> tempo;
    std::unordered_set<int>actuel;
    std::unordered_set<int> tempo2;
    std::unordered_set<int> suivant;
    std::vector<int> reference_s;

    std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> chemins = tousLesChemins();
    for(auto&i: m_sommets)
    {
        for(auto&j: m_sommets)
        {
            if((m_orientation && j!=i) || j->getId() > i->getId())
            {
                chemins_p=chemins[{i,j}];
                //std::cout<<"PAIRE"<<std::endl;
                std::sort(chemins_p.begin(), chemins_p.end(), [](std::unordered_set<int> a, std::unordered_set<int> b) { return a.size()<b.size();} );
                for(size_t y=0; y<chemins_p.size(); ++y)
                {
                    //std::cout<<"Comparaison"<<std::endl;
                    actuel=chemins_p[y];
                    if(y+1<chemins_p.size())
                    {
                        suivant=chemins_p[y+1];
                        for(auto a: m_aretes)
                        {
                            if(suivant.find(a->getId())!=suivant.end())
                            {
                                if(a->getExtremites().first!=i && a->getExtremites().first!=j)
                                        tempo.push_back(a->getExtremites().first->getId());
                                if(a->getExtremites().second!=i && a->getExtremites().second!=j)
                                        tempo.push_back(a->getExtremites().second->getId());

                            }
                            if(actuel.find(a->getId())!=actuel.end())
                            {
                                if(a->getExtremites().first!=i && a->getExtremites().first!=j)
                                        tempo2.insert(a->getExtremites().first->getId());
                                if(a->getExtremites().second!=i && a->getExtremites().second!=j)
                                        tempo2.insert(a->getExtremites().second->getId());

                            }
                        }

                        for(auto&l: tempo)
                        {
//                                std::cout<<l<<"  ";
                                if(tempo2.find(l)!=tempo2.end())
                                    {
                                        h=1;
                                        chemins_p[y+1]=chemins_p[y];
                                    }
                        }
//                        std::cout<<std::endl;
//                        for(auto&l: tempo2)
//                        {
//                                std::cout<<l<<"  ";
//                        }
//                        std::cout<<std::endl;

                        if(h==0)
                        {
                            ++compteur;
                            //tempo2.insert(tempo.begin(),tempo.end());
                        }

                        h=0;
                        tempo.clear();
                        tempo2.clear();
                    }

                }
                //std::cout<<"cpt: "<<compteur<<std::endl;
                nb.push_back(compteur);
                compteur=1;
                chemins_p.clear();
            }

        }
    }

    std::sort(nb.begin(), nb.end(), [](int a, int b)
    {
        return a < b;
    });
    std::cout<<std::endl<<"Ce graphe est "<<nb[0]<<"-sommet(s) connexe"<<std::endl;
}


void Graphe::comparaisonIndices(int nb)
{
    Graphe copie=*this;

    std::vector<std::pair<int, double>> centralite_degres1 = centraliteDegre ();
    std::vector<std::pair<double, double>> vecteurPropre1=vecteurPropre();
    std::vector<std::pair<double, double>> vecteurProximite1=vecteurProximite();
    std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite1=intermediarite();

    for(int i=0; i<nb; ++i)
        copie.supprimerArete();

    std::vector<std::pair<int, double>> centralite_degres2 = copie.centraliteDegre ();
    std::vector<std::pair<double, double>> vecteurPropre2=copie.vecteurPropre();
    std::vector<std::pair<double, double>> vecteurProximite2=copie.vecteurProximite();
    std::pair<std::vector<std::pair<double,double>>,std::vector<std::pair<Arete*,std::pair<double,double>>>> intermediarite2=copie.intermediarite();

    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<std::endl<<"Difference";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" des ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"indices";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" de ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"centralite";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<", ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"avant";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" et ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<"apres";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" la ";
    SetConsoleTextAttribute(texteConsole, 12);
    std::cout<<"suppression";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" d'arete(s)"<<std::endl;

    for(size_t i=0; i<copie.m_sommets.size(); ++i)
    {
        std::cout<<std::endl<<"Sommet "<<m_sommets[i]->getId()<<" : "<<std::endl;

        SetConsoleTextAttribute(texteConsole, 3);
        std::cout<<"   Degre: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<centralite_degres1[i].first-centralite_degres2[i].first<<" ";
        std::cout<<centralite_degres1[i].second-centralite_degres2[i].second<<" ";
        SetConsoleTextAttribute(texteConsole, 12);
        std::cout<<std::endl<<"   Vecteur Propre: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<vecteurPropre1[i].first-vecteurPropre2[i].first<<" ";
        std::cout<<vecteurPropre1[i].second-vecteurPropre2[i].second<<" ";
        SetConsoleTextAttribute(texteConsole, 11);
        std::cout<<std::endl<<"   Proximite: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<vecteurProximite1[i].first-vecteurProximite2[i].first<<" ";
        std::cout<<vecteurProximite1[i].second-vecteurProximite2[i].second<<" ";
        SetConsoleTextAttribute(texteConsole, 13);
        std::cout<<std::endl<<"   Intemediarite: ";
        SetConsoleTextAttribute(texteConsole, 15);
        std::cout<<intermediarite1.first[i].first-intermediarite2.first[i].first<<" ";
        std::cout<<intermediarite1.first[i].second-intermediarite2.first[i].second<<std::endl;
    }
}

void recursifTousLesChemins (std::vector<std::unordered_set<int>> &commun,std::pair<std::unordered_set<Sommet*>,std::unordered_set<int>> cheminUnique, std::pair<Sommet*,Arete*> current, std::pair<Sommet*,Sommet*> &debFin)
{
    if(current.first != debFin.first && cheminUnique.first.find(current.first) == cheminUnique.first.end() && cheminUnique.second.find(current.second->getId()) == cheminUnique.second.end() )
    {
        cheminUnique.second.insert(current.second->getId());
        cheminUnique.first.insert(current.first);
        if (current.first == debFin.second)
            commun.push_back(cheminUnique.second);
        else

            for (auto i : current.first->getAdjacents())
                recursifTousLesChemins(commun, cheminUnique,i,debFin);
    }
}

std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> Graphe::tousLesChemins()
{
    std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> chemins;
    for(auto &i : m_sommets)
        for(auto &j : m_sommets)
            if((m_orientation && j!=i)|| j->getId() > i->getId())
            {
                std::pair<Sommet*,Sommet*> debFin = {i,j};
                std::vector<std::unordered_set<int>> commun;

                for(auto &l : i->getAdjacents())
                {
                    std::pair<std::unordered_set<Sommet*>,std::unordered_set<int>> cheminUnique;
                    recursifTousLesChemins(commun,cheminUnique,l,debFin);
                }
                chemins[ {i,j}] = commun;
                //std::cout << "(" << i->getNom() << "," << j->getNom() << ") " << chemins[ {i,j}].size() << " chemins" << std::endl;
            }

    return chemins;
}



void Graphe::kAretesConnexe ()
{
    std::vector<int>nb;
    std::vector<std::unordered_set<int>> chemins_p;
    int compteur=1, h=0;
    std::vector <int> tempo;
    //std::vector <int> tempo2;
    std::unordered_set<int> suivant;

    std::map<std::pair<Sommet*,Sommet*>,std::vector<std::unordered_set<int>>> chemins = tousLesChemins();
    for(auto&i: m_sommets)
    {
        for(auto&j: m_sommets)
        {
            if((m_orientation && j!=i) || j->getId() > i->getId())
            {
                chemins_p=chemins[{i,j}];
                //std::cout<<"PAIRE"<<std::endl;
                std::sort(chemins_p.begin(), chemins_p.end(), [](std::unordered_set<int> a, std::unordered_set<int> b) { return a.size()<b.size();} );
                for(size_t y=0; y<chemins_p.size(); ++y)
                {
                    //std::cout<<"Comparaison"<<std::endl;
                    tempo={chemins_p[y].begin(), chemins_p[y].end()};
                    if(y+1<chemins_p.size())
                    {
                        suivant={chemins_p[y+1].begin(), chemins_p[y+1].end()};
                        //tempo2={chemins_p[y+1].begin(), chemins_p[y+1].end()};
                        for(auto&l: tempo)
                        {
                                //std::cout<<l<<"  ";
                                if(suivant.find(l)!=suivant.end())
                                    {
                                        h=1;
                                        chemins_p[y+1]=chemins_p[y];
                                    }
                        }
//                        std::cout<<std::endl;
//                        for(auto&l: tempo2)
//                        {
//                                std::cout<<l<<"  ";
//                        }
//                        std::cout<<std::endl;

                        if(h==0)
                        {
                            ++compteur;
                            chemins_p[y+1].insert(chemins_p[y].begin(),chemins_p[y].end());
                        }
                        h=0;
                    }

                }
                //std::cout<<"cpt: "<<compteur<<std::endl;
                nb.push_back(compteur);
                compteur=1;
                chemins_p.clear();
            }

        }
    }

    std::sort(nb.begin(), nb.end(), [](int a, int b)
    {
        return a < b;
    });
    std::cout<<std::endl<<"Ce graphe est "<<nb[0]<<"-arete(s) connexe"<<std::endl;
}




void Graphe::testForteConnexite()
{
    std::vector<bool> sommetCouleur(m_ordre, false);
    std::vector<int> ordreSommet;

    for(size_t i=0; i<m_sommets.size(); i++)
    {
        if(sommetCouleur[i]==false)
            m_sommets[i]->dfs(sommetCouleur, ordreSommet);
    }

    std::reverse(ordreSommet.begin(), ordreSommet.end());
    sommetCouleur.clear();
    sommetCouleur.resize(m_ordre, false);

    std::vector<std::vector<int>> reverseAdjacents(m_ordre);

    for(size_t i=0; i<m_sommets.size(); i++)
    {
        for(size_t j=0; j<m_sommets[i]->getAdjacents().size(); j++)
            reverseAdjacents[m_sommets[i]->getAdjacents()[j].first->getId()].push_back(m_sommets[i]->getId());
    }

    std::vector<int> composanteFortementConnexe;
    std::vector<std::vector<int>> composantesFortementConnexes;

    for(size_t i=0; i<ordreSommet.size(); i++)
    {
        if(sommetCouleur[ordreSommet[i]]==false)
        {
            m_sommets[ordreSommet[i]]->dfsReverse(sommetCouleur, reverseAdjacents, composanteFortementConnexe, getSommets());
            composantesFortementConnexes.push_back(composanteFortementConnexe);
            composanteFortementConnexe.clear();
        }
    }

    std::cout<<"Il y a ";
    SetConsoleTextAttribute(texteConsole, 14);
    std::cout<<(int)composantesFortementConnexes.size()<<" composante(s) fortement connexe(s)";
    SetConsoleTextAttribute(texteConsole, 15);
    std::cout<<" : "<<std::endl;
    for(size_t i=0; i<composantesFortementConnexes.size(); i++)
    {
        SetConsoleTextAttribute(texteConsole, 14);
        std::cout<<std::endl<<i+1<<" : ";
        SetConsoleTextAttribute(texteConsole, 15);
        for(size_t j=0; j<composantesFortementConnexes[i].size(); j++)
            std::cout<<composantesFortementConnexes[i][j]<<" ";
    }
    std::cout<<std::endl;

}



std::vector<std::vector<int>> Graphe::creationMatriceAdjacence()
{
    std::vector<std::vector<int>> matriceAdjacence(m_ordre, std::vector<int>(m_ordre, 0));
    for(size_t i=0; i<m_aretes.size(); i++)
        matriceAdjacence[m_aretes[i]->getExtremites().first->getId()][m_aretes[i]->getExtremites().second->getId()]=m_aretes[i]->getPoids();

    return matriceAdjacence;
}
