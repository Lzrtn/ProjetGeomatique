# Utilisez une image de base avec PostgreSQL
FROM postgres:15

# Installez PostGIS
RUN apt-get update \
    && apt-get install -y postgis postgresql-15-postgis-3 \
    && rm -rf /var/lib/apt/lists/*

# Copiez le script Bash dans le conteneur
COPY create_db.sh /create_db.sh

# Donnez les droits d'exécution au script
RUN chmod +x /create_db.sh 
RUN ./create_db.sh

# Exposez le port PostgreSQL
EXPOSE 5432
