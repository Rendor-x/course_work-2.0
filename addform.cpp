#include "addform.h"
#include "ui_addform.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>



AddForm::AddForm(MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent), ui(new Ui::AddForm), mainWindow(mainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Add Book");
}

AddForm::~AddForm()
{
    delete ui;
}

void AddForm::on_addButton_clicked()
{
    QString title = ui->lineEdit_title->text();
    QString genre = ui->lineEdit_genre->text();
    QString author = ui->lineEdit_author->text();
    int year = ui->lineEdit_year->text().toInt();
    int pages = ui->lineEdit_pages->text().toInt();
    QString presence = ui->lineEdit_presence->text();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Title cannot be empty");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO books (title, genre, author, year, pages, presence) "
                  "VALUES (:title, :genre, :author, :year, :pages, :presence)");
    query.bindValue(":title", title);
    query.bindValue(":genre", genre);
    query.bindValue(":author", author);
    query.bindValue(":year", year);
    query.bindValue(":pages", pages);
    query.bindValue(":presence", presence);

    if (query.exec()) {
        emit bookAdded();
        emit finished();
        this->hide();
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}


void AddForm::on_backButton_clicked()
{
    if (mainWindow){
        qDebug() << "Showing MainWindow";
        mainWindow->show();
    }
    qDebug() << "Hiding AddForm";
    this->hide();
}

