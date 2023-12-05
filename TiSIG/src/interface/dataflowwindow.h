#ifndef DATAFLOWWINDOW_H
#define DATAFLOWWINDOW_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class DataFlowWindow;
}

/**
 * @brief Class representing the data flow window
 *
 * This class represents the main window of TiSIG application.
 */
class DataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief DataFlow Window constructor
     * @param QWidget parent
     */
    explicit DataFlowWindow(QWidget *parent = nullptr);

    /**
     * @brief DataFlow Window destructor
     */
    ~DataFlowWindow();

    std::string getLien() { return lien; };

private:
    Ui::DataFlowWindow *ui; ///< Ui::DataFlowWindow ui
    QComboBox comboBox_dataFlowWindow; ///< QComboBox comboBox_dataFlowWindow
    QLineEdit lineEdit_dataFlowWindow; ///< QLineEdit lineEdit_dataFlowWindow
    QPushButton btn_validateDataFlowUrl; ///< QPushButton btn_validateDataFlowUrl
    QPushButton btn_validateDataFlowPreSaved; ///< QPushButton btn_validateDataFlowPreSaved
    std::string lien;

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
  void OnButtonValidateDataFlowPreSavedlClicked();
};

#endif // DATAFLOWWINDOW_H
