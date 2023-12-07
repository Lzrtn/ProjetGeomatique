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
     *
     */
    explicit MntWindow(QWidget *parent = nullptr);

    /**
     * @brief DTM Window destructor
     */
    ~MntWindow();

    /**
     * @brief getter of m_pathObj attribute
     *
     * @return  m_pathObj : attribute value of the path of the .obj  file
     */
    std::string getObj() { return m_pathObj; };

    /**
     * @brief getter of m_pathTexture attribute
     *
     * @return  m_pathTexture : attribute value of the path of the texture file
     */
    std::string getTexture() { return m_pathTexture; };

    /**
     * @brief getter of m_xtranslate attribute
     *
     * @return  m_xtranslate : attribute value of the translation in x
     */
    double getXTranslate() { return m_xtranslate; };

    /**
     * @brief getter of m_ytranslate attribute
     *
     * @return  m_ytranslate : attribute value of the translation in y
     */
    double getYTranslate() { return m_ytranslate; };


private:
    Ui::MntWindow *ui; ///< Ui::MntWindow ui
    std::string m_pathObj;
    std::string m_pathTexture;
    double m_xtranslate;
    double m_ytranslate;

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


private slots:
    /**
     * @brief Function Submit
     *
     */
    void OnButtonSubmitClicked();

};

#endif // MNTWINDOW_H
