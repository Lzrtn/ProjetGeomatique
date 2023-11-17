#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool getMode() const;

private:
    Ui::MainWindow *ui;
    QPushButton btn_switchMode2D3D;
    bool mode;

private slots:

  void OnButtonSwitchTo2D3DClicked();

};
#endif // MAINWINDOW_H
