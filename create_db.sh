#!/bin/bash

# Paramètres de connexion à PostgreSQL
DB_USER="postgres"
DB_PASSWORD="postgres"
DB_NAME="docker_db"

# Vérifier si la base de données existe déjà
if psql -lqt | cut -d \| -f 1 | grep -qw $DB_NAME; then
    echo "La base de données $DB_NAME existe déjà. Abandon."
    exit 1
fi

# Trouver le chemin de l'extension PostGIS
POSTGIS_EXTENSION_PATH=$(pg_config --sharedir)/extension/postgis.control

# Vérifier si l'extension PostGIS est installée
if [ ! -f "$POSTGIS_EXTENSION_PATH" ]; then
    echo "L'extension PostGIS n'est pas installée. Veuillez installer postgis et réessayer."
    exit 1
fi

# Créer la base de données
echo "Création de la base de données $DB_NAME..."
createdb -U $DB_USER $DB_NAME

# Activer l'extension PostGIS
echo "Activation de l'extension PostGIS..."
psql -U $DB_USER -d $DB_NAME -c "CREATE EXTENSION postgis;"

echo "La base de données $DB_NAME avec l'extension PostGIS a été créée avec succès."
