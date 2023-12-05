#!/bin/bash

# Function to display an error message and stop the script in case of failure
function exit_with_error {
    zenity --error --text="Erreur: $1"
    exit 1
}

# Project directory
current_directory="$PWD"

# Installation of dependencies

#sudo apt-get update || exit_with_error "Échec de la mise à jour des paquets"
sudo apt-get install -y gdal-bin libgdal-dev libpq-dev libpqxx-dev libgtest-dev xvfb || exit_with_error "Failed to install dependencies"
sudo apt-get install build-essential qt5-qmake qtbase5-dev qtchooser qtbase5-dev-tools

#Installation of postgres/postgis

sudo apt-get install postgresql-14-postgis-3
sudo apt-get install postgresql-15-postgis-3
sudo apt-get install postgresql-16-postgis-3

sudo apt-get install curl

sudo apt-get install -y docker.io

sudo apt-get install apt-transport-https ca-certificates curl software-properties-common
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg -y --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
sudo echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null

# Granting usermod access to docker
sudo usermod -a -G docker $USER || exit_with_error "Failed to add user to the docker group"
sudo apt-get install docker-ce docker-ce-cli containerd.io
sudo apt-get install zenity

mkdir ./TiSIG/src/data/Docker/LYON_5EME_2015
mkdir ./TiSIG/src/data/Docker/zip

#Download of necessary files

cd ./TiSIG/src/data/Docker/LYON_5EME_2015
wget -O LYON_5EME_BATI_2015.gml https://www.dropbox.com/scl/fi/d0sybsln4o483fr5d9j17/LYON_5EME_BATI_2015.gml?rlkey=6m51ytawrfq5l18fiugb2h2ur&dl=0
wait
cd ../zip
wget -O 3dcitydb.zip https://www.dropbox.com/scl/fi/giksp7fbzvkbxv9zydl9e/3dcitydb.zip?rlkey=uqncjb068s7ct242gpjgc6efw&dl=0
wait
cd ../../../../..

# Build of the docker and creation of important tables

sudo docker build -t tisig_database_img ./TiSIG/src/data/Docker/.
sudo docker run -d --name database-tisig tisig_database_img
sleep 30
sudo docker exec database-tisig chmod u+x CREATE_DB.sh
sudo docker exec database-tisig ./CREATE_DB.sh 
sudo docker exec database-tisig bash -c "psql -h localhost -U postgres -p 5432 -d CityGML -c 'CREATE DATABASE \"database2D\";'"
sudo docker exec database-tisig bash -c "psql -h localhost -U postgres -p 5432 -d database2D -c 'CREATE EXTENSION postgis;'"
sudo docker exec database-tisig bash -c "psql -h localhost -U postgres -p 5432 -d database2D -c 'CREATE TABLE symbologie (id int,name character varying,red int,green int,blue int,alpha int);'"
sudo docker exec database-tisig bash -c "cd /3DCityDB-Importer-Exporter/bin && ./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast /data/LYON_5EME_BATI_2015.gml"
sudo docker stop database-tisig

# # Change to the project directory
# cd "$PROJET_DIR" || exit_with_error "Échec du changement de répertoire vers $PROJET_DIR"

#Build
mkdir -p build 

# Change directory to build directory
cd build || exit_with_error "Failed to change directory to build"

# Execute qmake for the TiSIG/src project
qmake ../TiSIG || exit_with_error "Failed to execute qmake for the TiSIG/src project"

# Compile the TiSIG/src project
make  || exit_with_error "Failed to compile TiSIG/src project"

# Change directory to src
cd src || exit_with_error "Failed to change directory to src"

# Creation of a .desktop file in /usr/share/applications
sudo bash -c 'cat <<EOL > /usr/share/applications/TiSIG.desktop
[Desktop Entry]
Name=TiSIG
Comment=MiniSIG
Path='$current_directory'/build/src
Exec='$current_directory'/build/src/src
Icon='$current_directory'/logotisig.png
Terminal=False
Type=Application
Categories=Application;
EOL'

# Display a message when the installation is completed
zenity --info --text="Configuration & compilation completed ! Please close and reopen your session to apply some changes."

