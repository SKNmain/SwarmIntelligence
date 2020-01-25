#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QObject>


class Maze;

class DbManager : public QObject
{
   Q_OBJECT;

public:
   DbManager() = delete;

   static QSqlDatabase getDB();
   static void initialize();
   static void saveMaze(const Maze* maze, const QString& description = "");

private:
   static inline QSqlDatabase db;
};

