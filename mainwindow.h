#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class AddForm;
class EditForm;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void refreshTable();

private:
    Ui::MainWindow *ui;
    AddForm *addForm;
    EditForm *editForm;
    QSqlDatabase db;
    QSqlTableModel *model;

    void setupDatabase();
    void setupModel();
};

#endif
