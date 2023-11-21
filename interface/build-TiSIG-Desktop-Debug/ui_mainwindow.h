/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_add2DVectorLayer;
    QAction *action_add2DRastorLayer;
    QAction *action_add2DDataStream;
    QAction *action_add3DVectorLayer;
    QAction *action_add3DRastorLayer;
    QAction *action_add3DModel;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QFrame *frame_4;
    QGridLayout *gridLayout_3;
    QListWidget *listeWidget_layersList;
    QLabel *label_3;
    QFrame *frame_5;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_layerInformation;
    QPushButton *btn_moveLayerUp;
    QPushButton *btn_moveLayerDown;
    QPushButton *btn_zoomOnLayer;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btn_deleteLayer;
    QLabel *label_attributeInformation;
    QTableWidget *tableWidget_layerAttributeInformation;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btn_switchMode2D3D;
    QPushButton *btn_zoomIn;
    QPushButton *btn_zoomOut;
    QPushButton *btn_zoomFull;
    QPushButton *btn_cameraRotation;
    QSpacerItem *horizontalSpacer;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page_2D;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *lineEdit_epsg2D;
    QLabel *label_5;
    QLineEdit *lineEdit_scale2D;
    QLabel *label_6;
    QGraphicsView *graphicsView_window2D;
    QWidget *page_3D;
    QGridLayout *gridLayout_5;
    QLineEdit *lineEdit_scale3D;
    QLabel *label_2;
    QLineEdit *lineEdit_epsg3D;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QOpenGLWidget *openGLWidget_window3D;
    QMenuBar *menubar;
    QMenu *menu_TiSIG;
    QMenu *menu_layers;
    QMenu *menu_addLayers;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(818, 558);
        action_add2DVectorLayer = new QAction(MainWindow);
        action_add2DVectorLayer->setObjectName(QString::fromUtf8("action_add2DVectorLayer"));
        action_add2DRastorLayer = new QAction(MainWindow);
        action_add2DRastorLayer->setObjectName(QString::fromUtf8("action_add2DRastorLayer"));
        action_add2DDataStream = new QAction(MainWindow);
        action_add2DDataStream->setObjectName(QString::fromUtf8("action_add2DDataStream"));
        action_add3DVectorLayer = new QAction(MainWindow);
        action_add3DVectorLayer->setObjectName(QString::fromUtf8("action_add3DVectorLayer"));
        action_add3DRastorLayer = new QAction(MainWindow);
        action_add3DRastorLayer->setObjectName(QString::fromUtf8("action_add3DRastorLayer"));
        action_add3DModel = new QAction(MainWindow);
        action_add3DModel->setObjectName(QString::fromUtf8("action_add3DModel"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        frame_4 = new QFrame(centralwidget);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame_4->sizePolicy().hasHeightForWidth());
        frame_4->setSizePolicy(sizePolicy);
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_4);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        listeWidget_layersList = new QListWidget(frame_4);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listeWidget_layersList);
        __qlistwidgetitem->setCheckState(Qt::Checked);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listeWidget_layersList);
        __qlistwidgetitem1->setCheckState(Qt::Checked);
        listeWidget_layersList->setObjectName(QString::fromUtf8("listeWidget_layersList"));
        listeWidget_layersList->setDragEnabled(true);
        listeWidget_layersList->setDragDropMode(QAbstractItemView::DragDrop);
        listeWidget_layersList->setDefaultDropAction(Qt::MoveAction);

        gridLayout_3->addWidget(listeWidget_layersList, 2, 0, 1, 1);

        label_3 = new QLabel(frame_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFrameShape(QFrame::NoFrame);

        gridLayout_3->addWidget(label_3, 0, 0, 1, 1);

        frame_5 = new QFrame(frame_4);
        frame_5->setObjectName(QString::fromUtf8("frame_5"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        horizontalLayout = new QHBoxLayout(frame_5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        btn_layerInformation = new QPushButton(frame_5);
        btn_layerInformation->setObjectName(QString::fromUtf8("btn_layerInformation"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("../icons/Information.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_layerInformation->setIcon(icon);
        btn_layerInformation->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(btn_layerInformation);

        btn_moveLayerUp = new QPushButton(frame_5);
        btn_moveLayerUp->setObjectName(QString::fromUtf8("btn_moveLayerUp"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("../icons/ArrowUp.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_moveLayerUp->setIcon(icon1);
        btn_moveLayerUp->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(btn_moveLayerUp);

        btn_moveLayerDown = new QPushButton(frame_5);
        btn_moveLayerDown->setObjectName(QString::fromUtf8("btn_moveLayerDown"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("../icons/ArrowDown.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_moveLayerDown->setIcon(icon2);
        btn_moveLayerDown->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(btn_moveLayerDown);

        btn_zoomOnLayer = new QPushButton(frame_5);
        btn_zoomOnLayer->setObjectName(QString::fromUtf8("btn_zoomOnLayer"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("../icons/Expand.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_zoomOnLayer->setIcon(icon3);
        btn_zoomOnLayer->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(btn_zoomOnLayer);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        btn_deleteLayer = new QPushButton(frame_5);
        btn_deleteLayer->setObjectName(QString::fromUtf8("btn_deleteLayer"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("../icons/Trash.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_deleteLayer->setIcon(icon4);
        btn_deleteLayer->setIconSize(QSize(20, 20));

        horizontalLayout->addWidget(btn_deleteLayer);


        gridLayout_3->addWidget(frame_5, 1, 0, 1, 1);

        label_attributeInformation = new QLabel(frame_4);
        label_attributeInformation->setObjectName(QString::fromUtf8("label_attributeInformation"));
        label_attributeInformation->setFrameShape(QFrame::NoFrame);
        label_attributeInformation->setFrameShadow(QFrame::Plain);

        gridLayout_3->addWidget(label_attributeInformation, 3, 0, 1, 1);

        tableWidget_layerAttributeInformation = new QTableWidget(frame_4);
        if (tableWidget_layerAttributeInformation->columnCount() < 2)
            tableWidget_layerAttributeInformation->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignCenter);
        tableWidget_layerAttributeInformation->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignCenter);
        tableWidget_layerAttributeInformation->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget_layerAttributeInformation->rowCount() < 1)
            tableWidget_layerAttributeInformation->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_layerAttributeInformation->setVerticalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        __qtablewidgetitem3->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget_layerAttributeInformation->setItem(0, 0, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_layerAttributeInformation->setItem(0, 1, __qtablewidgetitem4);
        tableWidget_layerAttributeInformation->setObjectName(QString::fromUtf8("tableWidget_layerAttributeInformation"));
        tableWidget_layerAttributeInformation->setFocusPolicy(Qt::StrongFocus);
        tableWidget_layerAttributeInformation->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        tableWidget_layerAttributeInformation->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_layerAttributeInformation->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget_layerAttributeInformation->setShowGrid(true);
        tableWidget_layerAttributeInformation->horizontalHeader()->setVisible(false);
        tableWidget_layerAttributeInformation->horizontalHeader()->setCascadingSectionResizes(true);
        tableWidget_layerAttributeInformation->horizontalHeader()->setDefaultSectionSize(126);

        gridLayout_3->addWidget(tableWidget_layerAttributeInformation, 4, 0, 1, 1);


        gridLayout->addWidget(frame_4, 0, 1, 2, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy1);
        frame->setMaximumSize(QSize(16777215, 46));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        btn_switchMode2D3D = new QPushButton(frame);
        btn_switchMode2D3D->setObjectName(QString::fromUtf8("btn_switchMode2D3D"));
        btn_switchMode2D3D->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(btn_switchMode2D3D);

        btn_zoomIn = new QPushButton(frame);
        btn_zoomIn->setObjectName(QString::fromUtf8("btn_zoomIn"));
        btn_zoomIn->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8("../icons/ZoomMore.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_zoomIn->setIcon(icon5);
        btn_zoomIn->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(btn_zoomIn);

        btn_zoomOut = new QPushButton(frame);
        btn_zoomOut->setObjectName(QString::fromUtf8("btn_zoomOut"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8("../icons/ZoomLess.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_zoomOut->setIcon(icon6);
        btn_zoomOut->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(btn_zoomOut);

        btn_zoomFull = new QPushButton(frame);
        btn_zoomFull->setObjectName(QString::fromUtf8("btn_zoomFull"));
        btn_zoomFull->setIcon(icon3);
        btn_zoomFull->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(btn_zoomFull);

        btn_cameraRotation = new QPushButton(frame);
        btn_cameraRotation->setObjectName(QString::fromUtf8("btn_cameraRotation"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8("../icons/Rotation.png"), QSize(), QIcon::Normal, QIcon::Off);
        btn_cameraRotation->setIcon(icon7);
        btn_cameraRotation->setIconSize(QSize(20, 20));

        horizontalLayout_2->addWidget(btn_cameraRotation);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_2->setVerticalSpacing(6);
        gridLayout_2->setContentsMargins(9, -1, -1, -1);
        stackedWidget = new QStackedWidget(frame_2);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page_2D = new QWidget();
        page_2D->setObjectName(QString::fromUtf8("page_2D"));
        gridLayout_4 = new QGridLayout(page_2D);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_4, 1, 2, 1, 1);

        lineEdit_epsg2D = new QLineEdit(page_2D);
        lineEdit_epsg2D->setObjectName(QString::fromUtf8("lineEdit_epsg2D"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEdit_epsg2D->sizePolicy().hasHeightForWidth());
        lineEdit_epsg2D->setSizePolicy(sizePolicy2);
        lineEdit_epsg2D->setFocusPolicy(Qt::NoFocus);
        lineEdit_epsg2D->setReadOnly(true);

        gridLayout_4->addWidget(lineEdit_epsg2D, 1, 1, 1, 1);

        label_5 = new QLabel(page_2D);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMaximumSize(QSize(16777215, 25));

        gridLayout_4->addWidget(label_5, 1, 3, 1, 1);

        lineEdit_scale2D = new QLineEdit(page_2D);
        lineEdit_scale2D->setObjectName(QString::fromUtf8("lineEdit_scale2D"));
        sizePolicy2.setHeightForWidth(lineEdit_scale2D->sizePolicy().hasHeightForWidth());
        lineEdit_scale2D->setSizePolicy(sizePolicy2);
        lineEdit_scale2D->setFocusPolicy(Qt::NoFocus);
        lineEdit_scale2D->setReadOnly(true);

        gridLayout_4->addWidget(lineEdit_scale2D, 1, 4, 1, 1);

        label_6 = new QLabel(page_2D);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 1, 0, 1, 1);

        graphicsView_window2D = new QGraphicsView(page_2D);
        graphicsView_window2D->setObjectName(QString::fromUtf8("graphicsView_window2D"));

        gridLayout_4->addWidget(graphicsView_window2D, 0, 0, 1, 5);

        stackedWidget->addWidget(page_2D);
        page_3D = new QWidget();
        page_3D->setObjectName(QString::fromUtf8("page_3D"));
        gridLayout_5 = new QGridLayout(page_3D);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        lineEdit_scale3D = new QLineEdit(page_3D);
        lineEdit_scale3D->setObjectName(QString::fromUtf8("lineEdit_scale3D"));
        lineEdit_scale3D->setEnabled(true);
        sizePolicy2.setHeightForWidth(lineEdit_scale3D->sizePolicy().hasHeightForWidth());
        lineEdit_scale3D->setSizePolicy(sizePolicy2);
        lineEdit_scale3D->setFocusPolicy(Qt::TabFocus);
        lineEdit_scale3D->setReadOnly(true);

        gridLayout_5->addWidget(lineEdit_scale3D, 1, 4, 1, 1);

        label_2 = new QLabel(page_3D);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_5->addWidget(label_2, 1, 3, 1, 1);

        lineEdit_epsg3D = new QLineEdit(page_3D);
        lineEdit_epsg3D->setObjectName(QString::fromUtf8("lineEdit_epsg3D"));
        lineEdit_epsg3D->setEnabled(true);
        sizePolicy2.setHeightForWidth(lineEdit_epsg3D->sizePolicy().hasHeightForWidth());
        lineEdit_epsg3D->setSizePolicy(sizePolicy2);
        lineEdit_epsg3D->setFocusPolicy(Qt::NoFocus);
        lineEdit_epsg3D->setReadOnly(true);

        gridLayout_5->addWidget(lineEdit_epsg3D, 1, 1, 1, 1);

        label = new QLabel(page_3D);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_5->addWidget(label, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        openGLWidget_window3D = new QOpenGLWidget(page_3D);
        openGLWidget_window3D->setObjectName(QString::fromUtf8("openGLWidget_window3D"));

        gridLayout_5->addWidget(openGLWidget_window3D, 0, 0, 1, 5);

        stackedWidget->addWidget(page_3D);

        gridLayout_2->addWidget(stackedWidget, 0, 1, 1, 2);


        gridLayout->addWidget(frame_2, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 818, 22));
        menu_TiSIG = new QMenu(menubar);
        menu_TiSIG->setObjectName(QString::fromUtf8("menu_TiSIG"));
        menu_layers = new QMenu(menubar);
        menu_layers->setObjectName(QString::fromUtf8("menu_layers"));
        menu_addLayers = new QMenu(menu_layers);
        menu_addLayers->setObjectName(QString::fromUtf8("menu_addLayers"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menu_TiSIG->menuAction());
        menubar->addAction(menu_layers->menuAction());
        menu_layers->addAction(menu_addLayers->menuAction());
        menu_addLayers->addAction(action_add2DVectorLayer);
        menu_addLayers->addAction(action_add2DRastorLayer);
        menu_addLayers->addAction(action_add2DDataStream);
        menu_addLayers->addSeparator();
        menu_addLayers->addAction(action_add3DVectorLayer);
        menu_addLayers->addAction(action_add3DRastorLayer);
        menu_addLayers->addAction(action_add3DModel);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "TiSIG", nullptr));
        action_add2DVectorLayer->setText(QCoreApplication::translate("MainWindow", "Ajouter une couche Vecteur", nullptr));
        action_add2DRastorLayer->setText(QCoreApplication::translate("MainWindow", "Ajouter une couche Raster", nullptr));
        action_add2DDataStream->setText(QCoreApplication::translate("MainWindow", "Ajouter un flux (WMS, WFS, WMTS)", nullptr));
        action_add3DVectorLayer->setText(QCoreApplication::translate("MainWindow", "Ajouter une couche Vecteur", nullptr));
        action_add3DRastorLayer->setText(QCoreApplication::translate("MainWindow", "Ajouter une couche Raster", nullptr));
        action_add3DModel->setText(QCoreApplication::translate("MainWindow", "Ajouter un mod\303\250le 3D", nullptr));

        const bool __sortingEnabled = listeWidget_layersList->isSortingEnabled();
        listeWidget_layersList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = listeWidget_layersList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MainWindow", "1\303\250re couche", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = listeWidget_layersList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MainWindow", "2\303\250me couche", nullptr));
        listeWidget_layersList->setSortingEnabled(__sortingEnabled);

        label_3->setText(QCoreApplication::translate("MainWindow", "Gestion des couches", nullptr));
#if QT_CONFIG(tooltip)
        btn_layerInformation->setToolTip(QCoreApplication::translate("MainWindow", "Information sur la couche", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_layerInformation->setText(QString());
#if QT_CONFIG(tooltip)
        btn_moveLayerUp->setToolTip(QCoreApplication::translate("MainWindow", "Monter la couche", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_moveLayerUp->setText(QString());
#if QT_CONFIG(tooltip)
        btn_moveLayerDown->setToolTip(QCoreApplication::translate("MainWindow", "Descendre la couche", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_moveLayerDown->setText(QString());
#if QT_CONFIG(tooltip)
        btn_zoomOnLayer->setToolTip(QCoreApplication::translate("MainWindow", "Zoom sur la couche", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_zoomOnLayer->setText(QString());
#if QT_CONFIG(tooltip)
        btn_deleteLayer->setToolTip(QCoreApplication::translate("MainWindow", "Supprimer la couche", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_deleteLayer->setText(QString());
        label_attributeInformation->setText(QCoreApplication::translate("MainWindow", "Informations attributaires", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_layerAttributeInformation->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_layerAttributeInformation->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Valeur", nullptr));

        const bool __sortingEnabled1 = tableWidget_layerAttributeInformation->isSortingEnabled();
        tableWidget_layerAttributeInformation->setSortingEnabled(false);
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_layerAttributeInformation->item(0, 0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "coucoucoucoucoucocuocuocu", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_layerAttributeInformation->item(0, 1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "coucoucoucoucoucou", nullptr));
        tableWidget_layerAttributeInformation->setSortingEnabled(__sortingEnabled1);

#if QT_CONFIG(tooltip)
        btn_switchMode2D3D->setToolTip(QCoreApplication::translate("MainWindow", "Changer de mode", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_switchMode2D3D->setText(QCoreApplication::translate("MainWindow", "2D / 3D", nullptr));
#if QT_CONFIG(tooltip)
        btn_zoomIn->setToolTip(QCoreApplication::translate("MainWindow", "Zoom avant", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_zoomIn->setText(QString());
#if QT_CONFIG(tooltip)
        btn_zoomOut->setToolTip(QCoreApplication::translate("MainWindow", "Zoom arri\303\250re", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_zoomOut->setText(QString());
#if QT_CONFIG(tooltip)
        btn_zoomFull->setToolTip(QCoreApplication::translate("MainWindow", "Zoom sur l'emprise totale", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_zoomFull->setText(QString());
#if QT_CONFIG(tooltip)
        btn_cameraRotation->setToolTip(QCoreApplication::translate("MainWindow", "Rotation", nullptr));
#endif // QT_CONFIG(tooltip)
        btn_cameraRotation->setText(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "\303\211chelle :", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "EPSG :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\303\211chelle :", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "EPSG :", nullptr));
        menu_TiSIG->setTitle(QCoreApplication::translate("MainWindow", "TiSIG", nullptr));
        menu_layers->setTitle(QCoreApplication::translate("MainWindow", "Couche", nullptr));
        menu_addLayers->setTitle(QCoreApplication::translate("MainWindow", "Ajouter une couche", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
