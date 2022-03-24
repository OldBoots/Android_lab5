#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QRandomGenerator>
#include <QScroller>
#include <QSqlRecord>
#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow  *ui;
    /* В проекте используются объекты для взаимодействия с информацией в базе данных
     * и моделью представления таблицы базы данных
     * */
    DataBase        *db;
    QSqlTableModel  *model;
    QStringList list_init;
    QStringList list_name;
    QModelIndex index_;

private:
    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView
     * */
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
private slots:
    void slot_butt_add();
    void slot_butt_del();
};

int Rand(int min, int max);

#endif // MAINWINDOW_H
