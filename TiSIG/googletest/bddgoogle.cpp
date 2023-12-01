#include "gtest/gtest.h"
#include "../src/outils/dbmanager.h"
#include "../src/outils/docker.h"
#include "../src/outils/executor.h"

TEST(DbManagerTest, constructor_Default){
	DbManager manager;
	EXPECT_EQ(manager.getString(),"dbname=postgres user=postgres password=postgres host=172.17.0.2 port=5432");
}
TEST(DbManagerTest, constructor_String){
	DbManager manager("zoebug");
	EXPECT_EQ(manager.getString(),"dbname=zoebug user=postgres password=postgres host=172.17.0.2 port=5432");
}
TEST(DbManagerTest, constructor_StringIp){
	DbManager manager("zpieng","468.2.6.45");
	EXPECT_EQ(manager.getString(),"dbname=zpieng user=postgres password=postgres host=468.2.6.45 port=5432");
}
TEST(DbManagerTest, test_Connection){
	std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
	Docker docker(pathDockerFile);
	DbManager manager("database2D",docker.getIpAdress());
	pqxx::connection newConn(manager.getString());
	EXPECT_EQ(newConn.is_open(),1);
}
TEST(DbManagerTest, test_InsertSelect){
	std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
	Docker docker(pathDockerFile);
	DbManager manager("database2D",docker.getIpAdress());    manager.CreateTable("DROP TABLE IF EXISTS test_table");
	manager.CreateTable("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");
	manager.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James Bond','37')");
	manager.Request("SELECT * FROM test_table");
	std::string result = manager.ParseResult();
	EXPECT_EQ(result,"id = 7, nom = James Bond, age = 37, \n");
}
TEST(DbManagerTest, test_ArrayParse){
	std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
	Docker docker(pathDockerFile);
	DbManager manager("database2D",docker.getIpAdress());
	manager.CreateTable("DROP TABLE IF EXISTS test_table");
	manager.CreateTable("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");
	manager.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James Bond','37')");
	manager.Request("INSERT INTO test_table (id, nom, age) VALUES ('1654','Serge Botton','99')");
	manager.Request("SELECT * FROM test_table");
	std::vector<std::vector<std::string>> result = manager.ArrayParseResult();
	std::vector<std::vector<std::string>> expected = {{"7","James Bond","37"},{"1654","Serge Botton","99"}};
	EXPECT_EQ(result,expected);
}
TEST(DbManagerTest, test_getResult){
	std::string pathDockerFile = "../src/data/Docker/docker-compose.yml";
	Docker docker(pathDockerFile);
	DbManager manager("database2D",docker.getIpAdress());    manager.CreateTable("DROP TABLE IF EXISTS test_table");
	manager.CreateTable("CREATE TABLE test_table (id SERIAL PRIMARY KEY, nom VARCHAR(100), age INT)");
	manager.Request("INSERT INTO test_table (id ,nom, age) VALUES ('007','James Bond','37')");
	manager.Request("SELECT * FROM test_table");
	pqxx::result result = manager.getResult();
	std::string stringResult;
	std::string expectedResult = "7, James Bond, 37, \n";
	for  (const auto &row : result){
		for (const auto &field : row){
			stringResult += field.c_str();
			stringResult += ", ";
		}
		stringResult += "\n";
	}
	EXPECT_EQ(stringResult,expectedResult);
}
