#ifndef MNTWINDOW_H
#define MNTWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class MntWindow;
}


/**
 * @brief Class representing the DTM window
 *
 * This class represents the main window of TiSIG application.
 */
class MntWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DTM Window constructor
     * @param QWidget parent
     */
    explicit MntWindow(QWidget *parent = nullptr);

    /**
     * @brief DTM Window destructor
     */
    ~MntWindow();

private:
    Ui::MntWindow *ui; ///< Ui::MntWindow ui

//    QLineEdit lineEdit_DTMPath; ///< QLineEdit lineEdit_DTMPath
//    QLineEdit lineEdit_OrthoPath; ///< QLineEdit lineEdit_Path
//    QPushButton btn_validateDataFlowUrl; ///< QPushButton btn_validateDataFlowUrl
//    QPushButton btn_validateDataFlowPreSaved; ///< QPushButton btn_validateDataFlowPreSaved

    /**
     * @brief Function to choose a DTM file
     *
     */
  void OnButtonAddDTMClicked();

  /**
   * @brief Function to choose an Orthoimage file
   *
   */
void OnButtonAddOrthoClicked();

/**
 * @brief Function Submit
 *
 */
void OnButtonSubmitClicked();

};

#endif // MNTWINDOW_H
