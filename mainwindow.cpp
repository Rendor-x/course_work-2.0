#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addform.h"
#include "editform.h"
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , addForm(nullptr)
    , editForm(nullptr)
{
    ui->setupUi(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Library Management System");
    setupDatabase();
    setupModel();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete addForm;
    delete editForm;
}

void MainWindow::setupDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("library.db");

    if (!db.open()) {
        QMessageBox::critical(this, "Error", db.lastError().text());
        return;
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS books ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "title TEXT NOT NULL, "
               "genre TEXT, "
               "author TEXT, "
               "year INTEGER, "
               "pages INTEGER)");
    QSqlQuery alterQuery;
    // Попробуем добавить колонку 'presence', если её ещё нет:
    alterQuery.exec("ALTER TABLE books ADD COLUMN presence TEXT");

}

void MainWindow::setupModel()
{
    model = new QSqlTableModel(this, db);
    model->setTable("books");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(1, Qt::Horizontal, tr("Title"));
    model->setHeaderData(2, Qt::Horizontal, tr("Genre"));
    model->setHeaderData(3, Qt::Horizontal, tr("Author"));
    model->setHeaderData(4, Qt::Horizontal, tr("Year"));
    model->setHeaderData(5, Qt::Horizontal, tr("Pages"));
    model->setHeaderData(6, Qt::Horizontal, tr("Presence"));

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::on_addButton_clicked()
{
    if (!addForm) {
        addForm = new AddForm(this);  // Передаем только один указатель на MainWindow
        connect(addForm, &AddForm::bookAdded, this, &MainWindow::refreshTable);
        connect(addForm, &AddForm::finished, this, [this]() {
            this->show();  // Показываем MainWindow при закрытии AddForm
            addForm->deleteLater();
            addForm = nullptr;
        });
    }

    addForm->show();
    addForm->activateWindow();
    this->hide();




}
void MainWindow::on_editButton_clicked()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a book to edit");
        return;
    }

    int row = selected.first().row();
    int id = model->data(model->index(row, 0)).toInt();
    QString title = model->data(model->index(row, 1)).toString();
    QString genre = model->data(model->index(row, 2)).toString();
    QString author = model->data(model->index(row, 3)).toString();
    int year = model->data(model->index(row, 4)).toInt();
    int pages = model->data(model->index(row, 5)).toInt();
    QString presence = model->data(model->index(row, 6)).toString();

    if (editForm) {
        editForm->close();
        editForm->deleteLater();
    }

    editForm = new EditForm(nullptr, this);

    editForm->setBookData(id, title, genre, author, year, pages, presence);

    connect(editForm, &EditForm::bookEdited, this, &MainWindow::refreshTable);
    connect(editForm, &EditForm::finished, this, [this]() {
        this->show();
        editForm->deleteLater();
        editForm = nullptr;
    });

    this->hide();
    editForm->show();
    editForm->raise();
    editForm->activateWindow();
}



void MainWindow::on_deleteButton_clicked()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a book to delete");
        return;
    }

    if (QMessageBox::question(this, "Confirm", "Are you sure you want to delete this book?") == QMessageBox::Yes) {
        int row = selected.first().row();
        model->removeRow(row);
        model->submitAll();
        refreshTable();
    }
}

void MainWindow::refreshTable()
{
    model->select();
    ui->tableView->resizeColumnsToContents();
}
