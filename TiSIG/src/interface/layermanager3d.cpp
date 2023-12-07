#include "layermanager3d.h"
#include "ui_mainwindow.h"

LayerManager3D::LayerManager3D(Ui::MainWindow *ui)
	:ui(ui)
{
	ui->openGLWidget_window3D->setLayerManager(this);
}

LayerManager3D::~LayerManager3D()
{
	for(auto pair: layerList3D)
	{
		delete pair.second;
	}
}

Layer3D* LayerManager3D::getLayer3D(int id)
{
	return layerList3D[id];
}

Layer3D* LayerManager3D::getSelectedLayer3D()
{
	QListWidgetItem *item = ui->listeWidget_layersList3D->currentItem();
	if (item)
	{
		int currentId = item->data(Qt::UserRole).toInt();
		return layerList3D[currentId];
	}
	return nullptr;
}

void LayerManager3D::addLayer3DtoOpenGLWidgetAndListWidget(Object3DStorage *storage3D)
{
		// Adding layer to openGLWidget and to map LayerList3D
		QString name = "Layer " + QString::number(index);
		Layer3D *layer3D = new Layer3D(storage3D, index, name);
		layerList3D[layer3D->getLayerId()] = layer3D; //index devra être changer par l'ID de la couche
		ui->openGLWidget_window3D->addLayer(0, layer3D);
		index++; // Temporaire


		// Adding ListWidgetItem to QListWidget with layer name and visibility checkbox
		layer3D->setLayerItem(new QListWidgetItem());
		layer3D->getLayerItem()->setData(Qt::UserRole, layer3D->getLayerId()); // Donne un ID à l'item

		layer3D->setLayerWidget(new QWidget());
		layer3D->setLayout(new QHBoxLayout(layer3D->getLayerWidget()));

		layer3D->setVisibilityCheckbox(new QCheckBox(""));
		layer3D->getVisibilityCheckbox()->setChecked(layer3D->isVisible());

		layer3D->setLayerLabel(new QLabel(layer3D->getLayerName()));

		QObject::connect(layer3D->getVisibilityCheckbox(), &QCheckBox::toggled, [=](bool checked) {
			layer3D->setVisible(checked); //Idem index -> ID
			ui->openGLWidget_window3D->update();
		});

		layer3D->getLayout()->addWidget(layer3D->getVisibilityCheckbox());
		layer3D->getLayout()->addWidget(layer3D->getLayerLabel());
		layer3D->getLayout()->setAlignment(Qt::AlignLeft);

		layer3D->getLayout()->setContentsMargins(2, 0, 2, 0);
		layer3D->getLayout()->setSpacing(10);

		layer3D->getLayerWidget()->setLayout(layer3D->getLayout());
		layer3D->getLayerItem()->setSizeHint(layer3D->getLayerWidget()->sizeHint());

		ui->listeWidget_layersList3D->insertItem(0, layer3D->getLayerItem());
		ui->listeWidget_layersList3D->setItemWidget(layer3D->getLayerItem(), layer3D->getLayerWidget());
}

