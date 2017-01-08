#ifndef CUSTOMTABLEWIDGET
#define CUSTOMTABLEWIDGET

#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include <QGraphicsView>
#include <QTableWidget>
#include <QKeyEvent>
class CustomTableWidget : public QTableWidget
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
    if(!e->text().isSimpleText()) return;
       // Do something
     if(this->currentItem()->row() == 0 || this->currentItem()->column() == 0)
       this->item(this->currentItem()->column(),this->currentItem()->row())->setText(e->text());

       this->currentItem()->setText(e->text());
    return;
  }
};
#endif // CUSTOMTABLEWIDGET

