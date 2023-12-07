#ifndef WMSDATAFLOWWINDOW_H
#define WMSDATAFLOWWINDOW_H

#include <QDialog>

namespace Ui {
class WMSDataFlowWindow;
}

class WMSDataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WMSDataFlowWindow(QWidget *parent = nullptr);
    ~WMSDataFlowWindow();
    std::string getLien() { return lien; };

private:
    Ui::WMSDataFlowWindow *ui;
    std::string lien;

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
