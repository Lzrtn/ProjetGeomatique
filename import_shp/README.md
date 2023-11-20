### Import des shapefiles dans une base de données
**Prérequis** : avoir une base de données existante, les tables correspondant aux shapefiles se créerons dans cette base de données  
**Librairies nécessaires** : postgis, lgdal et lpq
**Commande pour lancer les fichiers** :  
dans le dossier import_shp
```
    g++ main.cpp shapefile.cpp -o main -lgdal -lpq  
    
    ./main
```