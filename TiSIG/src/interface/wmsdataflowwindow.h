#ifndef WMSDATAFLOWWINDOW_H
#define WMSDATAFLOWWINDOW_H

#include <QDialog>


namespace Ui {
class WMSDataFlowWindow;
}

/**
 * @brief Class representing the data flow window
 *
 * This class represents the WMS flow window of TiSIG application.
 */
class WMSDataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief WMS DataFlow Window constructor
     * @param QWidget parent
     */
    explicit WMSDataFlowWindow(QWidget *parent = nullptr);

    /**
     * @brief WMS DataFlow Window destructor
     */
    ~WMSDataFlowWindow();

    std::string getURL() { return url; };
    std::string getFlowName() { return flowName; };


private:
    Ui::WMSDataFlowWindow *ui;

    std::string url;
    std::string flowName;


private slots:
    /**
     * @brief Function to return data flow url entered in lineEdit
     *
     * @return Url
     */
  void OnButtonValidateDataFlowUrlClicked();

    /**
     * @brief Function to return pre-saved data flow choosed in comboBox
     *
     * @return which dataflow was choosed
     */
  void OnButtonValidateDataFlowPreSavedlClicked();
};

#endif // WMSDATAFLOWWINDOW_H
