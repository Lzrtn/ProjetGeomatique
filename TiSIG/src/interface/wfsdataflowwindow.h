#ifndef WFSDATAFLOWWINDOW_H
#define WFSDATAFLOWWINDOW_H

#include <QDialog>

namespace Ui {
class WFSDataFlowWindow;
}

class WFSDataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WFSDataFlowWindow(QWidget *parent = nullptr);
    ~WFSDataFlowWindow();
    std::string getLien() { return lien; };

private:
    Ui::WFSDataFlowWindow *ui;
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

#endif // WFSDATAFLOWWINDOW_H
