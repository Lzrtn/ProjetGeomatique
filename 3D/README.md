FAIT PAR HANNICK ET MATHEO

# Flex
![image](https://github.com/Lzrtn/ProjetGeomatique/assets/121936719/30d5537a-0336-4274-ae42-bbab7856bf0c)
![image](https://github.com/Lzrtn/ProjetGeomatique/assets/121936719/d52f8d60-aa6c-4533-991b-dec7c7849015)


## Données Maquette 3D disponible ici :
https://data.grandlyon.com/portail/fr/jeux-de-donnees/maquettes-3d-texturees-2015-communes-metropole-lyon/donnees

## PostgreSQL

### Installation
- installer postgresql avec :
```shell
sudo apt-get install postgresql-15
```
- et postgis avec : 
```shell
sudo apt-get install postgresql-15-postgis-3
```

## PgAdmin4

- créer un server avec Register > Server
  - General > Nom : à votre guise
  - Connection > Host name/adress : localhost
  - Port : 5432
  - Username : postgres
  - Password : postgres
  - Save
- créer une database avec Create > Database
  - General > Database : CityGML
  - Save
- Ouvrir Query Tools de CityGML
- Lancer ces requêtes :
```sql
CREATE EXTENSION postgis;
CREATE EXTENSION postgis_raster;
CREATE EXTENSION postgis_sfcgal;
```

### Erreurs rencontrées
- ERROR:  n'a pas pu accéder au fichier « $libdir/postgis-3 » : Aucun fichier ou dossier de ce type
- ERROR:  n'a pas pu ouvrir le fichier de contrôle d'extension « /usr/share/postgresql/14/extension/postgis.control » : Aucun fichier ou dossier de ce type 

## 3DCityDB
- installation : 
```shell
sudo java -jar <path3D.jar>
```
- changer le ./3dcitydb/postgresql/ShellScripts/Unix/CONNECTION_DETAILS.sh : 
```shell
#!/bin/bash
# Provide your database details here ------------------------------------------
export PGBIN=/home/share/postgresql-common/pg_wrapper
export PGHOST=localhost
export PGPORT=5432
export CITYDB=CityGML
export PGUSER=postgres
#------------------------------------------------------------------------------
```
selon les droits, il faut : 
```shell
sudo gedit CONNECTION_DETAILS.sh
```
- pour créer les tables dans la database :
```shell
sudo chmod u+x CREATE_DB.sh
sudo ./CREATE_DB.sh
```
    - enter a valid SRID : 4171
    - EPSG code of the height system : 0
    - gml:srsName : EPSG::4171
    - Mot de passe pour l'utilisateur postgres : postgres 

- pour les remplir :
documentation ici : https://3dcitydb-docs.readthedocs.io/en/latest/impexp/cli/import.html

```shell
./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast /home/formation/Téléchargements/LYON_5EME_2015/LYON_5EME_BATI_2015.gml
```
le no-fail-fast sert à passer les exceptions.

## Démo sur QGIS
![image](https://github.com/Lzrtn/ProjetGeomatique/assets/61098254/9bc964ee-a261-4956-9649-1488de4b81d4)

## DONNEES DANS LES TABLES :
```sql
SELECT ST_ASGEOJSON((ST_DUMP(solid_geometry)).geom)
FROM surface_geometry;
```
Cette requête explose les géométries de `<CompositeSurface>` en polygones qui sont les mêmes que la colonne geometry des `<surfaceMember>`.
Ainsi, la requête : 
```sql
SELECT ST_ASGEOJSON(geometry)
FROM surface_geometry;
```
renvoie  les mêmes polygones mais celle-ci nous permet d'accéder aux ids de chaque surfaceMember.
Ces ids sont primordiaux car ils font la jointure avec la table textureparam qui contient les coordonnées uv.

```sql
SELECT id, ST_ASGEOJSON(geometry), ST_ASGEOJSON(textureparam.texture_coordinates) FROM surface_geometry
JOIN textureparam ON surface_geometry.id = textureparam.surface_geometry_id;
```

(A CHANGE DEPUIS)














