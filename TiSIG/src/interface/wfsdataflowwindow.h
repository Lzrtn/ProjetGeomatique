#ifndef WFSDATAFLOWWINDOW_H
#define WFSDATAFLOWWINDOW_H

#include <QDialog>


namespace Ui {
class WFSDataFlowWindow;
}

/**
 * @brief Class representing the data flow window
 *
 * This class represents the WFS flow window of TiSIG application.
 */
class WFSDataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief WFS DataFlow Window constructor
     * @param QWidget parent
     */
    explicit WFSDataFlowWindow(QWidget *parent = nullptr);

    /**
     * @brief WFS DataFlow Window destructor
     */
    ~WFSDataFlowWindow();

    std::string getURL() { return url; };


private:
    Ui::WFSDataFlowWindow *ui;

    std::string url;


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

#endif // WFSDATAFLOWWINDOW_H
