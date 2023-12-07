#ifndef LAYERMANAGER3D_H
#define LAYERMANAGER3D_H

#include <map>

#include "../3D/layer3d.h"
#include "../3D/exempleobject3dstorage.h"

#include "ui_mainwindow.h"




/**
 * @brief The LayerManager3D class represents a manager for 3D layers in the application.
 * 
 * This class provides functionality to add, retrieve, and manage 3D layers in the application.
 * It interacts with the UI and maintains a list of Layer3D objects.
 */
class LayerManager3D
{
public:
    /**
     * @brief Constructs a LayerManager3D object.
     * @param ui The UI object associated with the main window.
     */
    LayerManager3D(Ui::MainWindow *ui);

    /**
     * @brief Destroys the LayerManager3D object.
     */
    ~LayerManager3D();

    /**
     * @brief Adds a 3D layer to the OpenGL widget and list widget.
     * @param storage3D The Object3DStorage object representing the 3D layer to be added.
     */
    void addLayer3DtoOpenGLWidgetAndListWidget(Object3DStorage *storage3D);

    /**
     * @brief Retrieves a 3D layer by its ID.
     * @param id The ID of the 3D layer to retrieve.
     * @return A pointer to the Layer3D object, or nullptr if not found.
     */
    Layer3D *getLayer3D(int id);

    /**
     * @brief Retrieves the selected layer.
     * @return A pointer to the Layer3D object, or nullptr if not found.
     */
    Layer3D* getSelectedLayer3D();

    int index = 0;

private:
    std::map<int, Layer3D*> layerList3D; ///< The list of 3D layers.
    Ui::MainWindow* ui; ///< The UI object associated with the main window.
};

#endif // LAYERMANAGER3D_H
