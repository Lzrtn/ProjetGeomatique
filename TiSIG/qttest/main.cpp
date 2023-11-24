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
    Shapefile shapefile ("../src/data/DONNEES_BDTOPO/TronconRoute/TronconRoute_Lyon5eme.shp");
    DbManager db_manager("database2D", ipAdress);
    int res = shapefile.import_to_db(db_manager,  2154);
    QVERIFY(res == 0);
}

void TestQShapefile::ajout_db_bad()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/DONNEES_BDTOPO/TronconRoute/TronconRoute_Lyon5eme.shp");
    DbManager db_manager("database2D", ipAdress);
    int res = shapefile.import_to_db(db_manager,  2154);
    QVERIFY(res == 1);
}

void TestQShapefile::nom_table()
{
    const std::string ipAdress = docker.getIpAdress();
    Shapefile shapefile ("../src/data/DONNEES_BDTOPO/TronconRoute/TronconRoute_Lyon5eme.shp");
    DbManager db_manager("database2D", ipAdress);
    shapefile.import_to_db(db_manager,  2154);
    std::string nom = shapefile.getTableName();
    QVERIFY(nom == "TronconRoute_Lyon5eme");
}

QTEST_MAIN(TestQShapefile)
#include "main.moc"
