#ifndef INTERACTIVEGRAPHICSVIEW
#define INTERACTIVEGRAPHICSVIEW

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QTableWidget>
#include <QKeyEvent>

class MainWindow;    // Forward declaration

class BinaryTableWidget : public QTableWidget
{
  Q_OBJECT
protected:
  void  keyPressEvent(QKeyEvent *e)
  {
    if(e->key() == Qt::Key_Up) {
        if(this->currentItem()->row() - 1 >= 0)
          this->setCurrentIndex(this->model()->index(this->currentItem()->row()-1,this->currentItem()->column()));
        return;
      }
    if(e->key() == Qt::Key_Left) {
        if(this->currentItem()->column()- 1 >= 0)
          this->setCurrentIndex(this->model()->index(this->currentItem()->row(),this->currentItem()->column()-1));
        return;
      }
    if(e->key() == Qt::Key_Down) {
        if(this->currentItem()->row() + 1 <= 10)
          this->setCurrentIndex(this->model()->index(this->currentItem()->row()+1,this->currentItem()->column()));
        return;
      }
    if(e->key() == Qt::Key_Right) {
        if(this->currentItem()->column() + 1 <= 10)
          this->setCurrentIndex(this->model()->index(this->currentItem()->row(),this->currentItem()->column()+1));
        return;
      }
    if(e->key() == Qt::Key_Delete)
      {
        QList<QTableWidgetItem*> selected = this->selectedItems();
        for(QList<QTableWidgetItem*>::iterator it = selected.begin(); it!=selected.end(); ++it)
          (*it)->setText("");
        return;
      }
    if(e->key() == Qt::Key_0 || e->key() == Qt::Key_1)
       this->currentItem()->setText(e->text());
    return;
  }
};





class CustomGraphicsView : public QGraphicsView
{
  Q_OBJECT
protected:
  void 	scrollContentsBy(int dx, int dy);
  void    wheelEvent ( QWheelEvent * event );

};

class CustomScene : public QGraphicsScene
{
Q_OBJECT
public:

explicit CustomScene(QObject *parent = 0,MainWindow * mw = 0);
  MainWindow * mainWin;

signals:

public slots:

protected:
void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
void  keyPressEvent(QKeyEvent *e);

};

#endif // CUSTOMSCENE_H

