#include <QObject>
#include <QGraphicsView>

/**
 * @brief Class representing the zoom
 *
 * This class represents the zoom, its level, and its modifications
 **/
class View_zoom : public QObject {
  Q_OBJECT

public:
    /**
    * @brief Construct a new View_zoom object
    *
    * @param view : the scene the zoom will be set on
    */
  View_zoom(QGraphicsView* view);

  /**
  * @brief Set the zoom
  *
  * @param factor : level of zoom the scene is currently on
  */
  void gentle_zoom(double factor);

  /**
  * @brief Adapt the modifier
  *
  * @param modifiers : wheel
  */
  void set_modifiers(Qt::KeyboardModifiers modifiers);

  /**
  * @brief Get the initial level of zoom
  *
  * @param value : value of the initial level of zoom
  */
  void set_zoom_factor_base(double value);

  /**
  * @brief Get the current zoom level
  *
  * @return The current zoom level
  */
  qreal getZoomLevel(){return zoom_level; };
  
  /**
  * @brief Set the current zoom level
  *
  * @param  qreal The new zoom level
  */
  void setZoomLevel(qreal zl){zoom_level=zl;};
  
private:
  QGraphicsView* view; ///< View on the interface
  Qt::KeyboardModifiers modifiers; ///< wheel
  double start_factor; ///< initial level of Zoom
  QPointF target_scene, target_view; ///< Points functions are going to target to zoom on
  int zoom_level; ///< Level of zoom
  bool eventFilter(QObject* object, QEvent* event); ///< mouse events
  

signals:
  void zoomed();
};
