#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsScene>

#include "layer.h"


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

    /**
     * @brief Get the ui
     * @return Ui::MainWindow *
     */
    Ui::MainWindow * getUi() const;


    /**
     * @brief Function to add Layer to the ListWidget
     */
    void addLayerToListWidget(int, Layer &layer);

    int index = 0; // Temporaire

private:
    Ui::MainWindow *ui; ///< Ui::MainWindow ui
    QPushButton btn_switchMode2D3D; ///< QPushButton btn_switchMode2D3D
    bool mode; ///< bool mode

    QGraphicsScene *scene;
    std::map <int, Layer*> layerList;

private slots:

    /**
     * @brief Function to switch between 2D and 3D mode
     *
     *
     */
    void OnButtonSwitchTo2D3DClicked();

    /**
    * @brief Function to add SHP file
    *
    *
    */
    void OnActionAddShpFileClicked();

    /**
    * @brief Function to zoom in
    *
    */
    void OnButtonZoomIn();

    /**
    * @brief Function to zoom out
    *
    *
    */
    void OnButtonZoomOut();

    /**
    * @brief Function to zoom on full extent
    *
    *
    */
    void OnButtonZoomFull();


};
#endif // MAINWINDOW_H
