#!/bin/bash
#docker exec database-tisig bash -c "cd /3DCityDB-Importer-Exporter/bin && ./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast /data/LYON_5EME_BATI_2015.gml"
docker exec database-tisig bash -c "cd /3DCityDB-Importer-Exporter/bin && ./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast "+$1;
