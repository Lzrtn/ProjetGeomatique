# Documentation d'utilisation : Docker et appel à la BDD

- [Initialisation de la base de données (et du conteneur)](#initialisation-de-la-base-de-données-et-du-conteneur)
- [Utilisation de la classe `DbManager`](#utilisation-de-la-classe-dbmanager)

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
