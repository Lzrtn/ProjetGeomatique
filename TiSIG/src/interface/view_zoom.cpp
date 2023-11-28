#include "view_zoom.h"
#include <QMouseEvent>
#include <QApplication>
#include <QScrollBar>
#include <qmath.h>

View_zoom::View_zoom(QGraphicsView* view)
  : QObject(view), view(view)
{
  view->viewport()->installEventFilter(this);
  view->setMouseTracking(true);
  modifiers = Qt::ControlModifier;
  start_factor = 1.0015;
}

void View_zoom::gentle_zoom(double factor) {
  view->scale(factor, factor);
  view->centerOn(target_scene);
  QPointF deltaviewport_pos = target_view - QPointF(view->viewport()->width() / 2.0,
                                                             view->viewport()->height() / 2.0);
  QPointF viewport_center = view->mapFromScene(target_scene) - deltaviewport_pos;
  view->centerOn(view->mapToScene(viewport_center.toPoint()));
  emit zoomed();
}

void View_zoom::set_modifiers(Qt::KeyboardModifiers modifier) {
  modifiers = modifier;

}

void View_zoom::set_zoom_factor_base(double value) {
  start_factor = value;
}

bool View_zoom::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::MouseMove) {
    QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
    QPointF delta = target_view - mouse_event->pos();
    if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
      target_view = mouse_event->pos();
      target_scene = view->mapToScene(mouse_event->pos());
    }
  } else if (event->type() == QEvent::Wheel) {
    QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
    if (QApplication::keyboardModifiers() == modifiers) {
      if (wheel_event->orientation() == Qt::Vertical) {
        double angle = wheel_event->angleDelta().y();
        double factor = qPow(start_factor, angle);
        gentle_zoom(factor);
        return true;
      }
    }
  }
  Q_UNUSED(object)
  return false;
}
