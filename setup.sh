#!/bin/bash

# Fonction pour afficher un message d'erreur et quitter le script en cas d'échec
function exit_with_error {
    zenity --error --text="Erreur: $1"
    exit 1
}

# Dossier du projet
current_directory="$PWD"



# Installer les dépendances nécessaires
#sudo apt-get update || exit_with_error "Échec de la mise à jour des paquets"
sudo apt-get install -y gdal-bin libgdal-dev libpq-dev libpqxx-dev libgtest-dev xvfb || exit_with_error "Échec de l'installation des dépendances"
sudo apt-get install build-essential qt5-qmake qtbase5-dev qtchooser qtbase5-dev-tools
sudo apt-get install postgresql-14-postgis-3
sudo apt-get install postgresql-15-postgis-3
sudo apt-get install postgresql-16-postgis-3

sudo apt-get install curl

sudo apt-get install -y docker.io

sudo apt-get install apt-transport-https ca-certificates curl software-properties-common
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg -y --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
sudo echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

sudo usermod -a -G docker $USER || exit_with_error "Échec de l'ajout de l'utilisateur au groupe docker"
sudo apt-get install docker-ce docker-ce-cli containerd.io
sudo apt-get install zenity

mkdir ./TiSIG/src/data/Docker/LYON_5EME_2015
mkdir ./TiSIG/src/data/Docker/zip


#Installation des fichiers
cd ./TiSIG/src/data/Docker/LYON_5EME_2015
wget -O LYON_5EME_BATI_2015.gml https://www.dropbox.com/scl/fi/d0sybsln4o483fr5d9j17/LYON_5EME_BATI_2015.gml?rlkey=6m51ytawrfq5l18fiugb2h2ur&dl=0
wait
cd ../zip
wget -O 3dcitydb.zip https://www.dropbox.com/scl/fi/giksp7fbzvkbxv9zydl9e/3dcitydb.zip?rlkey=uqncjb068s7ct242gpjgc6efw&dl=0
wait
cd ../../../../..
# Build du docker et création des tables

sudo docker build -t tisig_database_img ./TiSIG/src/data/Docker/.
sudo docker run -d --name database-tisig tisig_database_img
sleep 30
sudo docker exec database-tisig chmod u+x CREATE_DB.sh
sudo docker exec database-tisig ./CREATE_DB.sh 
sudo docker exec database-tisig bash -c "psql -h localhost -U postgres -p 5432 -d CityGML -c 'CREATE DATABASE \"database2D\";'"
sudo docker exec database-tisig bash -c "psql -h localhost -U postgres -p 5432 -d database2D -c 'CREATE EXTENSION postgis;'"
sudo docker exec database-tisig bash -c "cd /3DCityDB-Importer-Exporter/bin && ./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast /data/LYON_5EME_BATI_2015.gml"
sudo docker stop database-tisig

#Build
mkdir -p build 

# Se déplacer dans le répertoire build
cd build || exit_with_error "Échec du changement de répertoire vers build"

# Exécuter qmake pour le projet TiSIG/src
qmake ../TiSIG || exit_with_error "Échec de l'exécution de qmake pour le projet TiSIG/src"

# Compiler le projet TiSIG/src
make  || exit_with_error "Échec de la compilation du projet TiSIG/src"

# Se déplacer dans le répertoire src
cd src || exit_with_error "Échec du changement de répertoire vers src"

# Créer un fichier .desktop directement dans /usr/share/applications
sudo bash -c 'cat <<EOL > /usr/share/applications/TiSIG.desktop
[Desktop Entry]
Name=TiSIG
Comment=MiniSIG
Path='$current_directory'/build/src
Exec='$current_directory'/ProjetGeomatique/build/src/src
Icon='$current_directory'/ProjetGeomatique/logotisig.png
Terminal=False
Type=Application
Categories=Application;
EOL'

# Afficher un message de réussite
zenity --info --text="Configuration et compilation réussies! Vous pouvez maintenant fermer et rouvrir votre session."

