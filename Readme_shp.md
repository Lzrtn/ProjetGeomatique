### Import des shapefiles dans une base de données
#### Prérequis : 
Avoir une base de données existante, les tables correspondant aux shapefiles se créerons dans cette base de données  
#### Librairies nécessaires : 
postgis, lgdal et lpq  
### Import des librairies :  
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
### Affichage des shapefiles depuis une base de données
### Prérequis : 
avoir importé les shapefiles dans une bdd existante
### Librairies nécessaires : 
nlohmann/json, et pqxx  
### Import : 
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
