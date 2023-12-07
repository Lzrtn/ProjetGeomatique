#include <QObject>
#include <QGraphicsView>

class View_zoom : public QObject {
  Q_OBJECT
public:
  View_zoom(QGraphicsView* view);
  void gentle_zoom(double factor);
  void set_modifiers(Qt::KeyboardModifiers modifiers);
  void set_zoom_factor_base(double value);
  int getZoomLevel(){return zoom_level; };

private:
  QGraphicsView* view;
  Qt::KeyboardModifiers modifiers;
  double start_factor;
  QPointF target_scene, target_view;
  int zoom_level;
  bool eventFilter(QObject* object, QEvent* event);

signals:
  void zoomed();
};
