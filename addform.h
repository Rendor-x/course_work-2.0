#ifndef ADDFORM_H
#define ADDFORM_H

#include <QWidget>

class MainWindow;

namespace Ui {
class AddForm;
}

class AddForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddForm(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~AddForm();

signals:
    void bookAdded();
    void finished();

private slots:
    void on_addButton_clicked();
    void on_backButton_clicked();

private:
    Ui::AddForm *ui;
    MainWindow *mainWindow;
};

#endif

