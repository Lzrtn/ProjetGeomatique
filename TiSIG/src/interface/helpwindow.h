#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
class HelpWindow;
}

/**
 * @brief Class representing the help window
 *
 * This class represents the help window of TiSIG application.
 */
class HelpWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Help Window constructor
     * @param QWidget parent
     */
    explicit HelpWindow(QWidget *parent = nullptr);

    /**
     * @brief Help Window destructor
     */
    ~HelpWindow();

private:
    Ui::HelpWindow *ui; ///< Ui::HelpWindow ui
};

#endif // HELPWINDOW_H
