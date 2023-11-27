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
    void nom_table();
    void remplissage();
    void nom_table_espace();
    void nom_table_point();
};

std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
Docker docker(pathDockerFile);

void TestQShapefile::constructeur()
{
    Shapefile shapefile ("hello");
    QVERIFY(shapefile.getPath() == "hello");
}

void TestQShapefile::ajout_db()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/data/Tests/test.shp");
    DbManager db_manager("database2D", ipAdress);
    int res = shapefile.import_to_db(db_manager,  2154);
    QVERIFY(res == 0);
}

void TestQShapefile::ajout_db_bad()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/Tests/test.shp");
    DbManager db_manager("database2D", ipAdress);
    int res = shapefile.import_to_db(db_manager,  2154);
    QVERIFY(res == 1);
}

void TestQShapefile::nom_table()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/data/Tests/test.shp");
    DbManager db_manager("database2D", ipAdress);
    shapefile.import_to_db(db_manager,  2154);
    std::string nom = shapefile.getTableName();
    QVERIFY(nom == "test");
}

void TestQShapefile::remplissage()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/data/Tests/test.shp");
    DbManager db_manager("database2D", ipAdress);
    shapefile.import_to_db(db_manager,  2154);
    std::string nom = shapefile.getTableName();
    std::string requete_SQL="SELECT hauteur FROM "+nom+" WHERE id='BATIMENT0000000240870596';";
    db_manager.Request(requete_SQL);
    pqxx::result resultat_SQL =db_manager.getResult();
    double res = resultat_SQL[0][0].as<double>();
    QVERIFY(res == 28.4);
}

void TestQShapefile::nom_table_espace()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/data/Tests/test espace.shp");
    DbManager db_manager("database2D", ipAdress);
    shapefile.import_to_db(db_manager,  2154);
    std::string nom = shapefile.getTableName();
    QVERIFY(nom == "test_espace");
}

void TestQShapefile::nom_table_point()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/data/Tests/test.point.shp");
    DbManager db_manager("database2D", ipAdress);
    shapefile.import_to_db(db_manager,  2154);
    std::string nom = shapefile.getTableName();
    QVERIFY(nom == "test_point");
}

QTEST_MAIN(TestQShapefile)
#include "main.moc"
