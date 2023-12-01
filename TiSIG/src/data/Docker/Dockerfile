FROM postgis/postgis:16-3.4

COPY ./zip /zip

WORKDIR zip

RUN service postgresql start

RUN apt-get update && apt-get install unzip && unzip 3dcitydb.zip -d /

WORKDIR /3DCityDB-Importer-Exporter/3dcitydb/postgresql/ShellScripts/Unix

COPY setup.sql /docker-entrypoint-initdb.d/

RUN sed -i '2,$d' CONNECTION_DETAILS.sh && echo export PGBIN=/home/share/postgresql-common/pg_wrapper >> CONNECTION_DETAILS.sh && echo export PGHOST=localhost >> CONNECTION_DETAILS.sh && echo export PGPORT=5432 >> CONNECTION_DETAILS.sh && echo export CITYDB=CityGML >> CONNECTION_DETAILS.sh && echo export PGUSER=postgres >> CONNECTION_DETAILS.sh 

#&& chmod u+x CREATE_DB.shRUN echo -ne '\n' | bash ./CREATE_DB.sh 

RUN apt-get install -y default-jre && echo 'export JAVA_HOME=/usr/lib/jvm/default-java' >> /root/.bashrc && echo 'export PATH=$PATH:$JAVA_HOME/bin' >> /root/.bashrc && . /root/.bashrc


#RUN cd /3DCityDB-Importer-Exporter/bin && ./impexp import -T=postgresql -H=localhost -P=5432 -d=CityGML -u=postgres -p=postgres --no-fail-fast /data/LYON_5EME_BATI_2015.gml



# Copiez le contenu du répertoire LYON_5EME_2015 dans /data

COPY ./LYON_5EME_2015 /data

ENV POSTGRES_HOST localhost
ENV POSTGRES_PORT 5432
ENV POSTGRES_PASSWORD postgres
ENV POSTGRES_USER postgres
ENV POSTGRES_DB CityGML

