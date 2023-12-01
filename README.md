# TiSIG - Projet de création d'un mini sig TSI 2023

- [TiSIG - Projet de création d'un mini sig TSI 2023](#tisig---projet-de-création-dun-mini-sig-tsi-2023)
  - [Changement des droits administrateurs docker](#changement-des-droits-administrateurs-docker)
  - [Arborescence des fichiers](#arborescence-des-fichiers)
  - [Formatage Doxygen pour classes](#formatage-doxygen-pour-classes)
- [Formatage du code TSI](#formatage-du-code-tsi)
  - [Formatage global du code](#formatage-global-du-code)
  - [Formatage Doxygen pour fonctions](#formatage-doxygen-pour-fonctions)
  - [Formatage Doxygen pour classes](#formatage-doxygen-pour-classes)
- [Utilisation des tests](#utilisation-des-tests)
- [Docker et appel à la BDD](#docker-et-appel-à-la-bdd)
  - [Initialisation de la base de données (et du conteneur)](#initialisation-de-la-base-de-données-et-du-conteneur)
  - [Utilisation de la classe `DbManager`](#utilisation-de-la-classe-dbmanager)
- [Gestion des Shapefiles](#gestion-des-shapefiles)
  - [Import des shapefiles dans une base de données](#import-des-shapefiles-dans-une-base-de-données)
  - [Affichage des shapefiles depuis une base de données](#affichage-des-shapefiles-depuis-une-base-de-données)
- [Noms des variables des objets de l'interface](#noms-des-variables-des-objets-de-linterface)
  - [Barre d'outils](#barre-doutils)
  - [Fenêtre principale](#fenêtre-principale)
  - [Gestion des couches](#gestion-des-couches)
  - [Barre d'actions (menu TiSIG)](#barre-dactions-menu-tisig)
  - [Barre d'actions (menu Couche)](#barre-dactions-menu-couche)
  - [Fenêtre Ajouter couche Flux](#fenêtre-ajouter-couche-flux)
  - [Fenêtre Ajouter raster 3D](#fenêtre-ajouter-raster-3d)

## Changement des droits administrateurs docker

Pour avoir les droits administrateur et lancer des dockers sans problème, lancer ces deux commandes :

```
curl https://get.docker.com/ | sudo sh
sudo usermod -a -G docker $USER
```

Au besoin, fermez votre session et rouvrez-la.

## Arborescence des fichiers

L'application se trouve dans le dossier **TiSIG**.
Il faut inclure vos codes dans les différents sous-dossiers de TiSIG (2D, 3D, interface, outils pour le moment) et vérifier que les appels aux fichiers sources fonctionnent bien.

# Formatage du code TSI

## Formatage global du code
 - Produire des test unitaires dès l'implémentation d'une fonctionnalité
 - Mettre tous les attributs en *private* lors de la création d'une classe
 - Créer les Getters & les Setters pour les attributs des classes en *const*
 - Toutes les variables & attributs: 
   - En [camelCase](https://fr.wikipedia.org/wiki/Camel_case)
   - Doivent avoir un sens *en anglais*
 - Toutes les méthodes :
   - En [PascalCase](https://fr.wikipedia.org/wiki/Camel_case)
   - Doivent aussi avoir un sens
   - Les getters & setters sont quand même en camelCase (setValue, getValue...)
 - Tous les objets de l'interface:
   - En snake_camelCase (typeObj_nomObj)
   - [Voir tous les objets](./interface/nom_objets.md)

## Formatage Doxygen pour fonctions
```cpp
/**
 * @brief Function to sum two numbers
 *
 * This function takes two numbers and returns the resulting sum
 *
 * @param a Fisrt number
 * @param b Second number
 * @return Sum of a & b
 */
int Sum(int a, int b) {
    return a + b;
}
```
## Formatage Doxygen pour classes
```cpp
/**
 * @brief Class representing a student
 *
 * This class represents a student with it's name and student id.
 */
class Etudiant {
public:
    /**
     * @brief Student class constructor
     * @param nom Student name
     * @param id Student ID
     */
    Student(const std::string& name, int id);

    /**
     * @brief Get the student name
     * @return Student name
     */
    std::string getName() const;

    /**
     * @brief Get the student ID
     * @return Student ID
     */
    int getId() const;

private:
    std::string name; ///< Student Name
    int idNumber;    ///< Student ID
};
```

# Utilisation des tests 

Faire un projet qt, mettre le fichier .pro "test_subdirs" à la place du fichier .pro de votre nouveau projet et changez le nom du fichier "test_subdirs.pro" par le nom que vous avez donnée à votre nouveau projet. Mettre ensuite le nom de votre projet personnel dans le fichier .pro dans la section SUBDIRS. Copier le dossier de votre projet personnel à la racine du nouveau projet. Relancer Qt creator et vous verrez apparaître l'arborescence des dossiers dans Qt. 

![image](https://github.com/Lzrtn/ProjetGeomatique/assets/97800904/81d41e1d-4134-43de-9943-5592b734396f)

# Docker et appel à la BDD

## Initialisation de la base de données (et du conteneur)

Pour éviter d'installer une base de données sur chaque ordinateur, l'utilisation d'un docker est intéressante.
Le conteneur est construit à partir d'un docker-compose, et contient (pour le moment) uniquement une base de données postGIS.

Avant de se connecter à la base de données, il faut lancer / créer le conteneur.

Pour installer pqxx, une librairie permettant de faire des requêtes SQL, utilisez : 
```
sudo apt-get install libpqxx-dev
```

Il faut d'abord inclure les bons fichiers :
```cpp
#include <pqxx/pqxx> // Classe pour gérer PostgreSQL
#include "dbmanager.h" // Classe pour la base de données
#include "docker.h" // Classe de création du Docker
```

Pour créer le conteneur :

```cpp
// Creating container
std::string pathDockerFile = "../docker-compose.yml";
Docker docker(pathDockerFile);
```

Assurez vous de mettre le bon chemin pour le docker-compose.yml (relatif de préférence).
Cela va créer l'image si elle n'est pas encore créée et peut donc être assez long la toute première fois.
Ensuite, le conteneur sera créé (des logs sont affichés dans la console), même s'il a déjà été créé.

Pour créer une connection à la base de données, vous devez récupérer l'adresse ip du conteneur, et créer une instance de la classe `DbManager` :

```cpp
// Get the Ip Address
std::string ipAddress = docker.getIpAddress();

// Connection to the database "database2D"
DbManager dbManager("database2D", ipAddress);
```
La connexion est gérée directement par la classe `DbManager`

Pour information, les identifiants de connexion et le nom de la base de données initiale sont les suivants :

- **user** = `postgres`
- **password** = `postgres`
- **database** = `database2D`

## Utilisation de la classe `DbManager`

Lorsque vous aurez créé une connection, vous pourrez ensuite lancer des requêtes SQL, créer des tables et d'autres base de données.

Pour créer une **base de données**, utilisez la méthode `CreateDb` :

```cpp
// Create a second database
dbManager.CreateDb("database3D");
```

Vous devez créer une instance de la classe `DbManager` pour chaque base de données, donc :

```cpp
// Connection to the new database 
DbManager newDbManager("database3D", ipAddress);
```

Utilisez la méthode `CreateTable` pour créer une nouvelle table :

```cpp
// Drop table if exists
newDbManager.CreateTable("DROP TABLE IF EXISTS test_table;");
// Create a new table "test_table"
newDbManager.CreateTable("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");
```

**Attention**, il est important de gérer la gestion des tables en SQL (i.e. vérifiez qu'elle n'existe pas via une autre requête avant de la créer).

Enfin, pour toutes les autres requêtes (SELECT, INSERT, UPDATE...), utilisez la méthode `Request` :

```cpp
// Insert values into table "test_table"
newTest.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James Bond','37')");
newTest.Request("INSERT INTO test_table (id, nom, age) VALUES ('1654','Serge Botton','99')");
// Get values inserted
newTest.Request("SELECT * FROM test_table");
```

Pour visualiser le résultat, il faut utiliser la méthode `ParseResult` :

```cpp
// Get results
std::string results = newTest.ParseResult();
```

Pour récupérer le résultat sous forme de `std::vector<std::vector<std::string>> ` , il faut utiliser la méthode `ArrayParseResult` :

```cpp
// Get results
std::vector<std::vector<std::string>> resultArray = newTest.ArrayParseResult();
// Print data
std::cout << "result" << std::endl;
for (auto& line : resultArray)
{
    for (auto& feature : line)
    {
        std::cout << feature << std::endl;
    }
}
```

Attention, tous les features sont de type `std::string`, donc vous devrez ensuite les transformer dans le type qui vous arrange.

# Gestion des Shapefiles

## Import des shapefiles dans une base de données

**Prérequis** : Avoir une base de données existante, les tables correspondant aux shapefiles se créerons dans cette base de données.

**Librairies nécessaires** : postgis, lgdal et lpq
  
  
**postgis**
```
sudo apt-get install postgresql-[num_version]-postgis-3
```
**lgdal**
```
sudo apt-get install libgdal-dev
```
**lpq**
```
sudo apt-get install libpq-dev
```
## Affichage des shapefiles depuis une base de données

**Prérequis** : Avoir importé les shapefiles dans une bdd existante.

**Librairies nécessaires** : nlohmann/json, et pqxx.

**pqxx** 
```
sudo apt-get install libpqxx-dev
```
**nlohmann/json**
```
git clone https://github.com/nlohmann/json.git
```
Dans le dossier json-develop : 
```
mkdir build
cd build
cmake..
make
sudo make install
```

# Noms des variables des objets de l'interface

Les noms sont sous format *typeObj_nameObj*.

## Barre d'outils

- Bouton Switch 2D / 3D : *btn_switchMode2D3D*
- Bouton Zoom + : *btn_zoomIn*
- Bouton Zoom - : *btn_zoomOut*
- Bouton Zoom sur l'emprise totale : *btn_zoomFull*

## Fenêtre principale

- Fenêtre switch (pour passer d'une vue à l'autre) : *stackedWidget*
- Vue 2D :  
  - Fenêtre où se dessine la carte : *graphicsView_window2D*
  - Zone de texte de l'Epsg : *lineEdit_epsg2D*
  - Zone de texte de l'Échelle : *lineEdit_scale2D*
- Vue 3D :
  - Fenêtre où se dessine la carte : *openGLWidget_window3D*
  - Zone de texte de l'Epsg : *lineEdit_epsg3D*
  - Zone de texte du Zoom : *lineEdit_zoom3D*
  - Zone de texte des coordonnées de la caméra : *lineEdit_coords3D*

## Gestion des couches

- Outils de gestion des couches:
  - Bouton Information : *btn_layerInformation*
  - Bouton Monter la couche dans la liste : *btn_moveLayerUp*
  - Bouton Descendre la couche dans la liste : *btn_moveLayerDown*
  - Bouton Zoom sur l'emprise de la couche : *btn_zoomOnLayer*
  - Bouton Supprimer la couche : *btn_deleteLayer*

- Fenêtre de gestion des couches : *listeWidget_layersList*

- Fenêtre des informations attributaires : *tableWidget_layerAttributeInformation*

## Barre d'actions (menu TiSIG)

  - Aide : *action_help*

## Barre d'actions (menu Couche)

- 2D :
  - Ajouter couche Vecteur : *action_add2DVectorLayer*
  - Ajouter couche Rasteur : *action_add2DRastorLayer*
  - Ajouter couche Flux : *action_add2DDataFlow*

- 3D :
  - Ajouter couche Vecteur : *action_add3DVectorLayer*
  - Ajouter couche Rasteur : *action_add3DRastorLayer*
  - Ajouter couche Flux : *action_add3DModel*

## Fenêtre Ajouter couche Flux
 - Fenêtre flux pré-enregistré : *tab_dataFlowPreSaved*
 - Fenêtre flux Url : *tab_dataFlowUrl*
 - Liste des flux pré-enregistrés : *comboBox_dataFlowWindow*
 - Url d'un flux : *lineEdit_dataFlowWindow*
 - Bouton Valider Url : *btn_validateDataFlowUrl*
 - Bouton Valider Pré-enregistré : *btn_validateDataFlowPreSaved*
 
## Fenêtre Ajouter raster 3D
 - Chemin du fichier MNT : *lineEdit_DTMPath*
 - Chemin du fichier Orthoimage : *lineEdit_OrthoPath*
 - Ajouter un fichier MNT : *btn_DTMFile*
 - Ajouter un fichier Orthoimage : *btn_OrthoFile*
 - Label erreur : *label_Error*
 - Bouton Valider : *btn_Submit*
