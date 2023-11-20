#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Class representing the main window
 *
 * This class represents the main window of TiSIG application.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Main Window constructor
     * @param QWidget parent
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Main Window destructor
     */
    ~MainWindow();

    /**
     * @brief Get the mode
     * @return bool mode
     */
    bool getMode() const;

private:
    Ui::MainWindow *ui; ///< Ui::MainWindow ui
    QPushButton btn_switchMode2D3D; ///< QPushButton btn_switchMode2D3D
    bool mode; ///< bool mode
    QGraphicsScene *scene;

private slots:

    /**
     * @brief Function to switch between 2D and 3D mode
     *
     *
     */
  void OnButtonSwitchTo2D3DClicked();
  void OnActionAddShpFileClicked();
  void OnButtonZoomIn();
  void OnButtonZoomOut();

};
#endif // MAINWINDOW_H
