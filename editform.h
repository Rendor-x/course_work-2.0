#ifndef EDITFORM_H
#define EDITFORM_H

#include <QWidget>

namespace Ui {
class EditForm;
}

class MainWindow;

class EditForm : public QWidget
{
    Q_OBJECT

public:
    explicit EditForm(QWidget *parent = nullptr, MainWindow *mainWin = nullptr);
    ~EditForm();

    void setBookData(int id, const QString &title, const QString &genre,
                     const QString &author, int year, int pages, const QString &presence);

signals:
    void bookEdited();
    void finished();

private slots:
    void on_saveButton_clicked();
    void on_backButton_clicked();

private:
    Ui::EditForm *ui;
    int bookId;
    MainWindow *mainWindow;
};

#endif
