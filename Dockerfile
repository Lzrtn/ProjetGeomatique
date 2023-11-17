# Utilisez une image de base avec PostgreSQL
FROM postgres:15

# Installez PostGIS
RUN apt-get update \
    && apt-get install -y postgis postgresql-15-postgis-3 \
    && rm -rf /var/lib/apt/lists/*

# Exposez le port PostgreSQL
EXPOSE 5432
