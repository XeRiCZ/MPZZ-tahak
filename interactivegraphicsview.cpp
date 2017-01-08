
#include "interactivegraphicsview.h"
#include "mainwindow.h"

#include <QMessageBox>



CustomScene::CustomScene(QObject *parent,MainWindow * mw) :
QGraphicsScene(parent),mainWin(mw)
{

}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
  if(mouseEvent->button() == Qt::RightButton)
    mainWin->StopPlacing();
    else
mainWin->PlaceVertex();
}

void CustomGraphicsView::scrollContentsBy(int, int)
{
    //don't do anything hah!
}

void CustomGraphicsView:: wheelEvent ( QWheelEvent * event )
{

}

void  CustomScene::keyPressEvent(QKeyEvent *e)
{
  if(e->key() == Qt::Key_Delete)
    {
      mainWin->DeleteActiveVertex();
    }
}
