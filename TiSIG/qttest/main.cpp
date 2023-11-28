#include <QTest>
#include "../src/2D/shapefile.h"
#include "../src/outils/docker.h"

/*class TestQString: public QObject
{
	Q_OBJECT
private slots:
	void toUpper();
};

void TestQString::toUpper()
{
	QString str = "Hello";
	QVERIFY(str.toUpper() == "HELLO");
}
*/
class TestQShapefile: public QObject
{
    Q_OBJECT
private slots:
    void constructeur();
    void ajout_db();
    void ajout_db_bad();
    void remplissage();
    void nom_table_espace();
    void nom_table_point();
    void ouverture_bis();
};

std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
Docker docker(pathDockerFile);

void TestQShapefile::constructeur()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile shapefile ("hello", db_manager);
    QVERIFY(shapefile.getPath() == "hello");
}

void TestQShapefile::ajout_db()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile* shapefile = new Shapefile("../src/data/Tests/test.shp", db_manager);
    std::cout<<shapefile->getPath()<<std::endl;
    int res = shapefile->import_to_db(2154);
    std::string nom = shapefile->getTableName();
    shapefile->~Shapefile();
    QVERIFY(res == 0 && nom == "test");
}

void TestQShapefile::ajout_db_bad()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile shapefile ("../src/Tests/test.shp", db_manager);
    int res = shapefile.import_to_db(2154);
    QVERIFY(res == 1);
}

void TestQShapefile::remplissage()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile shapefile ("../src/data/Tests/test.shp", db_manager);
    shapefile.import_to_db(2154);
    std::string nom = shapefile.getTableName();
    std::string requete_SQL="SELECT hauteur FROM "+nom+" WHERE id='BATIMENT0000000240870596';";
    db_manager.Request(requete_SQL);
    pqxx::result resultat_SQL =db_manager.getResult();
    double res = resultat_SQL[0][0].as<double>();
    QVERIFY(res == 28.4);
}

void TestQShapefile::nom_table_espace()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile shapefile ("../src/data/Tests/test espace.shp", db_manager);
    shapefile.import_to_db(2154);
    std::string nom = shapefile.getTableName();
    QVERIFY(nom == "test_espace");
}

void TestQShapefile::nom_table_point()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile shapefile ("../src/data/Tests/test.point.shp", db_manager);
    shapefile.import_to_db(2154);
    std::string nom = shapefile.getTableName();
    QVERIFY(nom == "test_point");
}

void TestQShapefile::ouverture_bis()
{
    std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
    Docker docker(pathDockerFile);
    const std::string ipAdress = docker.getIpAdress();
    DbManager db_manager("database2D", ipAdress);
    Shapefile shapefile1 ("../src/data/Tests/test.shp", db_manager);
    shapefile1.import_to_db(2154);
    Shapefile shapefile2 ("../src/data/Tests/test.shp", db_manager);
    shapefile2.import_to_db(2154);
    std::string nom = shapefile2.getTableName();
    QVERIFY(nom == "test_1");
}

QTEST_MAIN(TestQShapefile)
#include "main.moc"
