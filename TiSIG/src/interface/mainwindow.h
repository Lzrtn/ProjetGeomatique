#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <map>
#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QVector3D>

#include <iostream>
#include "2D/layer.h"
#include "3D/camera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Class representing the main window
 *
 * This class represents the main window of TiSIG application.
 */
class MainWindow : public QMainWindow, public ICameraDisplayInfo
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
	*eturn path
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
	void AddShpFileClicked(std::string path);

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
   * @return path
   *
   */
  std::string OnActionVectorLayerClicked();

  /**
   * @brief Function to show add2DRastorLayer window
   * @return path
   *
   */
  std::string OnActionRastorLayerClicked();

  /**
   * @brief Function to show add3DModel window
   * @return path
   *
   */
  std::string OnAction3DModelClicked();

public:
  /**
   * @brief Function to display camera coordinates in 3D window
   * @param camera position
   *
   */
  void Display3DCameraCoordinates(QVector3D camPosition) override;

  /**
   * @brief Function to display level of zoom in 3D window
   * @param zoom
   *
   */
  void Display3DZoomLevel(float zoom) override;


};

#endif // MAINWINDOW_H
