#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <map>
#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QFileDialog>

#include <iostream>

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
    QAction action_help; ///< QAction action_help
    QAction action_add2DDataFlow; ///< QAction action_help
    QAction action_add2DVectorLayer; ///< QAction action_add2DVectorLayer
    QAction action_add2DRastorLayer; ///< QAction action_add2DRastorLayer
    QAction action_add3DVectorLayer; ///< QAction action_add3DVectorLayer
    QAction action_add3DRastorLayer; ///< QAction action_add3DRastorLayer
    QAction action_add3DModel; ///< QAction action_add3DModel
	bool mode; ///< bool mode

    QGraphicsScene *scene;
    std::map <int, Layer*> layerList;
    std::string ipAdress;


private slots:

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
   * @brief Function to show help window
   *
   *
   */
  void OnActionHelpClicked();

  /**
   * @brief Function to show add2DDataflow window
   *
   *
   */
  void OnAction2DDataFlowClicked();

  /**
   * @brief Function to show add2DVectorLayer window
   *
   *
   */
  std::string OnActionVectorLayerClicked();

  /**
   * @brief Function to show add2DRastorLayer window
   *
   *
   */
  std::string OnActionRastorLayerClicked();

  /**
   * @brief Function to show add3DModel window
   *
   *
   */
  std::string OnAction3DModelClicked();

};

#endif // MAINWINDOW_H
