#include "DbManager.h"
#include "Maze.h"

QSqlDatabase DbManager::getDB()
{
   return db;
}

void DbManager::initialize()
{
   db = QSqlDatabase::addDatabase("QSQLITE");
   db.setDatabaseName("mazes.db");
   //QSqlDatabase::removeDatabase("mazes.db");
   //QSqlDatabase::removeDatabase("QSQLITE");
   if(true == db.open())
   {
      QSqlQuery q("", db);
      
      q.exec(R"(
CREATE TABLE IF NOT EXISTS Mazes
(
   uuid PRIMARY KEY,
   height INTEGER, 
   width INTEGER, 
   description TEXT, 
   array TEXT, 
   shortestWayArray TEXT
);
)");
      q.exec(R"(
CREATE TABLE IF NOT EXISTS Reports
(
   id INTEGER PRIMARY KEY AUTOINCREMENT, 
   maze_id INTEGER,
   ants_steps INTEGER,
   ants_counter INTEGER,
   
   FOREIGN KEY(maze_id) REFERENCES Mazes(id)
);
)");

      auto tables = db.tables();
   }
}

void DbManager::saveMaze(const Maze* maze, const QString& description)
{
   QSqlQuery q("", db);

   QString mazeArray;
   QString shortestWayArray;

   for(const auto& row : maze->mazeArray)
   {
      for(const auto& cell : row)
      {
         mazeArray += QString::number(cell) + " ";
      }
   }

   if(false == maze->shortestWayArray.empty())
   {
      for(const auto& row : maze->shortestWayArray)
      {
         for(const auto& cell : row)
         {
            shortestWayArray += QString::number(cell) + " ";
         }
      }
   }

   QString insertQuery = QString("INSERT INTO Mazes(uuid, height, width, description, array, shortestWayArray) VALUES (\"%1\", %2, %3, \"%4\", \"%5\", \"%6\")")
      .arg(maze->getUuid())
      .arg(QString::number(maze->getHeight()))
      .arg(QString::number(maze->getWidth()))
      .arg(description)
      .arg(mazeArray)
      .arg(shortestWayArray);

   bool ok = q.exec(insertQuery);
}
