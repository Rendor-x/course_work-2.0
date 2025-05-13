#include "editform.h"
#include "ui_editform.h"
#include "mainwindow.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

EditForm::EditForm(QWidget *parent, MainWindow *mainWin) :
    QWidget(parent),
    ui(new Ui::EditForm),
    mainWindow(mainWin)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Edit Book");
}

EditForm::~EditForm()
{
    delete ui;
}

void EditForm::setBookData(int id, const QString &title, const QString &genre,
                           const QString &author, int year, int pages, const QString &presence)
{
    bookId = id;
    ui->lineEdit_title->setText(title);
    ui->lineEdit_genre->setText(genre);
    ui->lineEdit_author->setText(author);
    ui->lineEdit_year->setText(QString::number(year));
    ui->lineEdit_pages->setText(QString::number(pages));
    ui->lineEdit_presence->setText(presence);
}

void EditForm::on_saveButton_clicked()
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
    query.prepare("UPDATE books SET title = :title, genre = :genre, author = :author, year = :year, pages = :pages, presence = :presence WHERE id = :id");
    query.bindValue(":title", title);
    query.bindValue(":genre", genre);
    query.bindValue(":author", author);
    query.bindValue(":year", year);
    query.bindValue(":pages", pages);
    query.bindValue(":presence", presence);
    query.bindValue(":id", bookId);

    if (query.exec()) {
        emit bookEdited();
        emit finished();
        this->hide();
        if (mainWindow) {
            mainWindow->show();
        }
    } else {
        QMessageBox::critical(this, "Error", query.lastError().text());
    }
}

void EditForm::on_backButton_clicked()
{
    emit finished();
    this->hide();
    if (mainWindow) {
        mainWindow->show();
    }
}
