#!/bin/bash
docker build -t tisig_database_img .

docker run -d --name database-tisig tisig_database_img
sleep 10
docker exec database-tisig chmod u+x CREATE_DB.sh

docker exec database-tisig ./CREATE_DB.sh 

#docker exec database-tisig bash -c "cd /3DCityDB-Importer-Exporter/bin && ./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast /data/LYON_5EME_BATI_2015.gml"

docker exec database-tisig bash -c "psql -h localhost -U postgres -p 5432 -d CityGML -c 'CREATE DATABASE database2D;'"

docker container stop tisig_database
