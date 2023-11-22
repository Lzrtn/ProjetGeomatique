# Utilisez une image de base avec PostgreSQL
FROM postgis/postgis:16-3.4

# Installez PostGIS
RUN apt-get update \
    && apt-get install -y postgis postgresql-15-postgis-3 \
    && rm -rf /var/lib/apt/lists/*

# Exposez le port PostgreSQL
EXPOSE 5432

# Environment variables
ENV POSTGRES_PASSWORD postgres
ENV POSTGRES_USER postgres
ENV POSTGRES_DB database2D
