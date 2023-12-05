#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <map>
#include <QMainWindow>
#include <QPushButton>
#include <QAction>
#include <QFileDialog>
#include <QGraphicsScene>

#include <QVector>

#include <iostream>
#include "../src/2D/layer.h"
#include "../src/2D/rasteritem.h"
#include "../src/3D/camera.h"
#include "../src/2D/shapefile.h"

#include "../src/3D/i_openglcityview.h"
#include "../src/3D/layer3d.h"

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
	 * @brief Get the current extent of the 2D graphics view in scene coordinates
	 * @return The extent of the view in scene coordinates
	 */
	QRectF get2DViewExtent();

	/**
	 * @brief Function to add Layer to the ListWidget
	 */
	void addLayerToListWidget(int, Layer &layer);


	int index = 0; // Temporaire
	void updateLayerOrderInGraphicsView();


	/**
	 * @brief Filter all events
	 * @param obj QObject to be filtered
	 * @param event QEvent to be filtered
	 * @return boolean
	 */
	bool eventFilter(QObject *obj, QEvent *event) override;

	/**
	 * @brief Function to get mouse pressed event on the scene
	 */
	void getAttributesLayer(QMouseEvent * event);


private:
	Ui::MainWindow *ui; ///< Ui::MainWindow ui
	bool mode; ///< bool mode

	QGraphicsScene *scene;
	std::map <int, Layer*> layerList;
	std::string ipAdress;
	std::map <int, Shapefile*> ShpList;

	Layer3D * layer3D;
	Object3DStorage * storage3D;

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
	* @brief Function to add Geotiff file
	*
	*
	*/
	void AddGeotiffFileClicked(std::string path);

	/**
	* @brief Function to show help window
	*
	*
	*/
	void OnActionHelpClicked();

	/**
	* @brief Function to show add2DWFSDataflow window
	*
	*
	*/
	void OnAction2DWFSDataFlowClicked();

	/**
	* @brief Function to show add2DWMSDataflow window
	*
	*
	*/
	void OnAction2DWMSDataFlowClicked();

	/**
	 * @brief Function to show add2DVectorLayer window
	 *
	 *
	 */
	std::string OnActionVector2DLayerClicked();

	/**
	 * @brief Function to show add3DVectorLayer window
	 *
	 *
	 */
	std::string OnActionVector3DLayerClicked();

	/**
	 * @brief Function to show add2DRastorLayer window
	 *
	 *
	 */
	std::string OnActionRastor2DLayerClicked();

	/**
	 * @brief Function to show add3DRastorLayer window
	 *
	 *
	 */
	void OnActionRastor3DLayerClicked();


	/**
	* @brief Function to show add3DModel window
	*

	*/
	std::string OnAction3DModelClicked();


	void moveItemUp( );
	void moveItemDown( );
	void onButtonClickedDeleteLayer();
	void onButtonClickedZoomOnLayer();

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
