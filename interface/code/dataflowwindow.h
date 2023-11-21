#ifndef DATAFLOWWINDOW_H
#define DATAFLOWWINDOW_H

#include <QDialog>

namespace Ui {
class DataFlowWindow;
}

class DataFlowWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataFlowWindow(QWidget *parent = nullptr);
    ~DataFlowWindow();

private:
    Ui::DataFlowWindow *ui;
};

#endif // DATAFLOWWINDOW_H
