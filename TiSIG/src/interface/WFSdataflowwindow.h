#ifndef WFSDATAFLOWWINDOW_H
#define WFSDATAFLOWWINDOW_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class WFSDataFlowWindow;
}

/**
 * @brief Class representing the data flow window
 *
 * This class represents the main window of TiSIG application.
 */
class WFSDataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DataFlow Window constructor
     * @param QWidget parent
     */
    explicit WFSDataFlowWindow(QWidget *parent = nullptr);

    /**
     * @brief DataFlow Window destructor
     */
    ~WFSDataFlowWindow();

private:
    Ui::WFSDataFlowWindow *ui; ///< Ui::DataFlowWindow ui
    QComboBox comboBox_dataFlowWindow; ///< QComboBox comboBox_dataFlowWindow
    QLineEdit lineEdit_dataFlowWindow; ///< QLineEdit lineEdit_dataFlowWindow
    QPushButton btn_validateDataFlowUrl; ///< QPushButton btn_validateDataFlowUrl
    QPushButton btn_validateDataFlowPreSaved; ///< QPushButton btn_validateDataFlowPreSaved

private slots:

    /**
     * @brief Function to return data flow url entered in lineEdit
     *
     * @return Url
     */
  const char* OnButtonValidateDataFlowUrlClicked();

    /**
     * @brief Function to return pre-saved data flow choosed in comboBox
     *
     * @return which dataflow was choosed
     */
  const char* OnButtonValidateDataFlowPreSavedlClicked();
};

#endif // WFSDATAFLOWWINDOW_H
