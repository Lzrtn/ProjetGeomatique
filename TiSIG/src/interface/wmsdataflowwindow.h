#ifndef WMSDATAFLOWWINDOW_H
#define WMSDATAFLOWWINDOW_H

#include <QDialog>
#include <QRectF>

namespace Ui {
class WMSDataFlowWindow;
}

class WMSDataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WMSDataFlowWindow(QWidget *parent = nullptr);
    ~WMSDataFlowWindow();
    std::string getUrl() { return url; }
    std::string getFilePath() { return filePath; }
    std::string getFlowName() { return flowName; }
    QRectF getViewProjExtent() { return viewProjExtent;}
    void setViewProjExtent(QRectF extent) {viewProjExtent = extent;}
    void setViewPxSize(QSize size) {viewPxSize = size;}

private:
    Ui::WMSDataFlowWindow *ui;
    std::string url;
    std::string filePath;
    std::string flowName;

    QRectF viewProjExtent;
    QSize viewPxSize;


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
