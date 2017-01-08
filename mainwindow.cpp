#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <QDebug>
#include "Vector2.h"
#include <algorithm>


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  // Reference

  abeceda = "a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,ž";
  cisla = "1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,x,y,z,ž";
  cisla0 = "0,1,2,3,4,5,6,7,8,9,a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,x,y,z,ž";

  addingVertexMode = false;
  renderUpdateTimer
      = new QTimer(this);
  renderUpdateTimer->start(2);
  connect(renderUpdateTimer,SIGNAL(timeout()),this,SLOT(RenderUpdate()));

  tabWidget = ui->tabWidget;

  i_chars = ui->i_chars;
  i_rels = ui->i_rels;
  i_charsA = ui->i_charsA;
  i_charsB = ui->i_charsB;
   i_charsC = ui->i_chars_C;
  i_zobrazeni = ui->i_zobrazeni;
  i_trojice = ui->i_trojice;
  i_vertex = ui->i_vertex;

  c_refl = ui->c_re;
  c_irefl = ui->c_ire;
  c_sym = ui->c_sym;
  c_asym = ui->c_asym;
  c_antisym = ui->c_anti;
  c_trans = ui->c_trans;
  c_ekv = ui->c_ekv;
  c_err = ui->c_err;

  c_funkce = ui->c_funkce;
  c_relace = ui->c_relace;
  c_bijekce = ui->c_bijekce;
  c_injekce = ui->c_injekce;
  c_surjekce = ui->c_surjekce;

  c_uzavrenost = ui->c_uzavrenost;
  c_asociativita = ui->c_asociativita;
  c_jednotkovyprvek = ui->c_jednotk;
  c_inverzniprvek = ui->c_inverz;
  c_komutativita = ui->c_komutativnost;



  g_log = ui->g_all;
  updateBTN = ui->updateBTN;

  zobrazeniView = ui->zobrazeniView;
  zobrazeniScene = new QGraphicsScene;
  zobrazeniView->setScene(zobrazeniScene);

  grafView = new CustomGraphicsView;
  grafView->setParent(ui->tab_2);
  grafView->setGeometry(QRect(20,60,531,271));
  grafView->setRenderHint(QPainter::RenderHint::Antialiasing,true);
  grafView->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing,true);
  grafScene = new CustomScene(0,this);
  grafView->setScene(grafScene);
  grafView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  grafView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  usporadaniView = new CustomGraphicsView;
  usporadaniView->setParent(ui->tab_5);
  usporadaniView->setGeometry(QRect(20,60,531,271));
  usporadaniView->setRenderHint(QPainter::RenderHint::Antialiasing,true);
  usporadaniView->setRenderHint(QPainter::RenderHint::HighQualityAntialiasing,true);
  usporadaniScene = new CustomScene(0,this);
  usporadaniView->setScene(usporadaniScene);
  usporadaniView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  usporadaniView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  ui->centralWidget->setMouseTracking(true);

  ui->c_err_3->setVisible(false);


  table = new CustomTableWidget;
  table->setParent(ui->tab_4);
  table->setGeometry(QRect(12,132,377,377));


  table->setRowCount(11);
  table->setColumnCount(11);
  QHeaderView *hView = new QHeaderView(Qt::Orientation::Horizontal,table);
  hView->setDefaultSectionSize(34);
  hView->setVisible(false);
   QHeaderView *vView = new QHeaderView(Qt::Orientation::Vertical,table);
   vView->setDefaultSectionSize(34);
   vView->setVisible(false);
  table->setHorizontalHeader(hView);
table->setVerticalHeader(vView);
table->setEditTriggers(QAbstractItemView::EditKeyPressed);

  currentIndex = 0;
  currentIndex2 = 0;
  addingEdgeMode = false;

  for(int y = 0; y <  11; y++)
    {
      for(int x= 0; x < 11; x++)
            table->setItem(x,y,new QTableWidgetItem(""));
    }

  for(int i = 0; i < 11; i++){
    table->item(i,0)->setBackgroundColor(qRgb(200,200,200));
    table->item(0,i)->setBackgroundColor(qRgb(200,200,200));
    }



  connect(updateBTN,SIGNAL(released()),this,SLOT(btnPushed()));
  connect(ui->updateBTN_3,SIGNAL(released()),this,SLOT(btnPushed()));
  connect(ui->updateBTN_6,SIGNAL(released()),this,SLOT(btnPushed()));
  connect(ui->updateBTN_7,SIGNAL(released()),this,SLOT(btnPushed()));
   connect(ui->updateBTN_8,SIGNAL(released()),this,SLOT(btnPushed()));
   connect(ui->updateBTN_9,SIGNAL(released()),this,SLOT(secondaryBtnPushed()));
   connect(ui->pushButton,SIGNAL(released()),this,SLOT(DoTabulkyPush()));


  connect(ui->addVertex,SIGNAL(clicked(bool)),this,SLOT(AddVertex()));
  connect(ui->addEdge,SIGNAL(clicked(bool)),this,SLOT(AddEdge()));
  connect(ui->reset,SIGNAL(clicked(bool)),this,SLOT(Reset()));


  connect(ui->addVertex_2,SIGNAL(clicked(bool)),this,SLOT(AddVertex()));
  connect(ui->addEdge_2,SIGNAL(clicked(bool)),this,SLOT(AddEdge()));
  connect(ui->reset_2,SIGNAL(clicked(bool)),this,SLOT(Reset()));

 connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(ListSelectionChange()));

  connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(forceStopPlacing()));

//  connect(ui->updateSupBTN,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
//  connect(ui->updateConfBTN,SIGNAL(clicked(bool)),this,SLOT(UpdateConfidence()));

  connect(ui->sup_i_1,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_2,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_3,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_4,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_5,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_6,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_7,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_8,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_9,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_10,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));
  connect(ui->sup_i_11,SIGNAL(clicked(bool)),this,SLOT(UpdateSupport()));


  connect(ui->confL_list,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateConfidence()));
  connect(ui->confR_list,SIGNAL(currentRowChanged(int)),this,SLOT(UpdateConfidence()));
  connect(ui->updateClosureBTN,SIGNAL(clicked(bool)),this,SLOT(NextClosure()));

  connect(ui->miEdit,SIGNAL(textChanged(QString)),this,SLOT(UpdateSupport()));
  connect(ui->miEdit,SIGNAL(textChanged(QString)),this,SLOT(UpdateConfidence()));

  ERROR = false;

  ui->removedSup->setVisible(false);
  ui->removedConf->setVisible(false);

  UpdateRelace();
}

MainWindow::~MainWindow()
{
  delete ui;
}

std::vector<podtabulka*> MergeTabulky(std::vector<podtabulka*> a, std::vector<podtabulka*> b)
{
  for(std::vector<podtabulka*>::iterator it = b.begin(); it != b.end(); ++it)
    a.push_back((*it));
  return a;
}

void MainWindow::actualizePodgrupyList()
{
  ui->listWidget->clear();
  QStringList items;
  items << "Nic";
  // ui->listWidget->addItem(QListWidgetItem("Nic"));
  for(std::vector<podtabulka*>::iterator it = podgrupy.begin(); it != podgrupy.end(); ++it)
    {
      std::stringstream ss;
      for(std::vector<char>::iterator jt = (*it)->char_c.begin(); jt != (*it)->char_c.end(); ++jt)
        ss << (*jt);
        //ui->listWidget->addItem(QListWidgetItem(ss.str().c_str()));
      items << ss.str().c_str();
    }
  ui->listWidget->addItems(items);
  ui->listWidget->setCurrentRow(0);
  table->setCurrentCell(0,0);
}

template <typename Iterator>
inline bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
   /* Credits: Thomas Draper */
   if ((first == last) || (first == k) || (last == k))
      return false;
   Iterator itr1 = first;
   Iterator itr2 = last;
   ++itr1;
   if (last == itr1)
      return false;
   itr1 = last;
   --itr1;
   itr1 = k;
   --itr2;
   while (first != itr1)
   {
      if (*--itr1 < *itr2)
      {
         Iterator j = k;
         while (!(*itr1 < *j)) ++j;
         std::iter_swap(itr1,j);
         ++itr1;
         ++j;
         itr2 = k;
         std::rotate(itr1,j,last);
         while (last != j)
         {
            ++j;
            ++itr2;
         }
         std::rotate(k,itr2,last);
         return true;
      }
   }
   std::rotate(first,k,last);
   return false;
}

bool intVecContains(int what, std::vector<int>  vec)
{
  for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
      if((*it) == what) return true;
    }
  return false;
}

bool MainWindow::vector35Contains(std::vector<char> name,std::vector<podtabulka*> checking)
{
  for(std::vector<podtabulka*>::iterator it = checking.begin(); it != checking.end(); ++it)
    {
      int i = 0;
      bool same = true;
      QString s1;
      for(std::vector<char>::iterator jt = (*it)->char_c.begin();  jt != (*it)->char_c.end(); ++jt)
        s1.append((*jt));
      QString s2;
      for(std::vector<char>::iterator jt = name.begin();  jt != name.end(); ++jt)
        s2.append((*jt));
      if(s1 == s2) return true;
  /*    for(std::vector<char>::iterator jt = (*it)->char_c.begin();  jt != (*it)->char_c.end(); ++jt)
        {
          if((*it)->char_c.at(i) != name.at(i)) same = false;
            i++;
        }*/
    }
      return false;
}

bool vector4Contains(std::vector<cell*> what, std::vector<podtabulka*> from)
{
  for(std::vector<podtabulka*>::iterator it = from.begin(); it != from.end();++it)
    {

    }
  return false;
}

std::vector<cell*> MainWindow::unionOfLines(std::vector<tabLine> rows, std::vector<tabLine> cols)
{
  std::vector<int> acceptableCoords;
  std::vector<char> acceptableCharsRows;
    std::vector<char> acceptableCharsCols;
 // qDebug() << "Union of :";
  for(std::vector<tabLine>::iterator it = rows.begin(); it != rows.end(); ++it){
    acceptableCoords.push_back((*it).pos);
    acceptableCharsRows.push_back((*it).name);
 //   qDebug() << (*it).name;
    }
  for(std::vector<tabLine>::iterator it = cols.begin(); it != cols.end(); ++it){
    acceptableCoords.push_back((*it).pos);
    acceptableCharsCols.push_back((*it).name);
 //   qDebug() << (*it).name;
    }
//qDebug() << " is ";
  std::vector<cell*> output;
  for(int y = 1; y < CHARS_C->size()+1; y++)
    {
      for(int x = 1; x < CHARS_C->size()+1; x++)
        {
          if(vectorContains(table->item(y,0)->text().toStdString()[0],&acceptableCharsRows)
             && vectorContains(table->item(0,x)->text().toStdString()[0],&acceptableCharsCols)){

 //         if (intVecContains(x,acceptableCoords) || intVecContains(y,acceptableCoords)){
            output.push_back(new cell(x,y,table->item(y,x)->text().toStdString()[0]));
       //     qDebug() << table->item(y,x)->text();
         }
        }
}
  return output;
}

std::vector<podtabulka*> MainWindow::GetPodgrupy(std::vector<char> chars, std::vector<std::vector<char>> ts)
{


  std::vector<podtabulka*> podtabulky;

  // Verze bez předkryvani
/*
  for(int size = CHARS_C->size()-2; size >=0; size--)
    {
      for(int x_base = 1; x_base + size <= CHARS_C->size(); x_base++)
        {
          for(int y_base = 1; y_base + size <= CHARS_C->size();y_base++)
            {
              std::vector<char> char_c;
              std::vector<std::vector<char>> trojice;
                std::vector<cell*> cells;

                qDebug() << "Podtabulka : ";
              for(int y = y_base; y <= y_base+size; y++)
                {
                  char A = table->item(y,0)->text().toStdString()[0];
                  char_c.push_back(A);
                  std::stringstream ss;
                  for(int x = x_base; x <= x_base+size ; x++)
                    {
                       std::vector<char> novaTrojce;
                       char B = table->item(0,x)->text().toStdString()[0];
                       char C = table->item(y,x)->text().toStdString()[0];
                       novaTrojce.push_back(A); novaTrojce.push_back(B); novaTrojce.push_back(C);
                       cells.push_back(new cell(x,y,C));
                       trojice.push_back(novaTrojce);

                       ss << C << " | ";
                    }
                    qDebug() << ss.str().c_str();
                }
              podtabulky.push_back(new podtabulka(trojice,char_c,cells));
            }
        }
    }
*/


  qDebug() << "Fine";
 //return podtabulky;
  // Verze s předkrýváním


    std::vector<std::vector<tabLine>> allMixed;

    std::size_t n = chars.size();



    for(std::size_t k = chars.size(); k > 0; k--)
    {
      do
      {
         if(k!=chars.size()){
           std::vector<tabLine> newMix;
           for (int i = 0; i < k; ++i)
               newMix.push_back(tabLine(i+1,chars.at(i)));
           std::sort (newMix.begin(), newMix.end(),
               [](tabLine  a, tabLine  b){return a.name < b.name;}   );
           allMixed.push_back(newMix);
          }
      }
      while(next_combination(chars.begin(),chars.begin() + k,chars.end()));
    }
              char JEDNOTKOVYMEM = jednotkovy;
  for(std::vector<std::vector<tabLine>>::iterator xt = allMixed.begin(); xt != allMixed.end(); ++xt)
    {
    for(std::vector<std::vector<tabLine>>::iterator it = allMixed.begin(); it != allMixed.end(); ++it)
      {
        if((*xt).size() == (*it).size())
          {
              std::vector<tabLine> rows = (*it);
              std::vector<tabLine> cols = (*xt);


          std::stringstream ss;
          std::vector<cell*> cells = unionOfLines(rows,cols);
          std::vector<cell*> cellsRevers = unionOfLines(cols,rows);
          std::vector<char> chC;
          std::vector<std::vector<char>> trojce;
          std::vector<std::vector<char>> trojceRevers;
          for(std::vector<cell*>::iterator jt = cells.begin(); jt != cells.end(); ++jt)
            {
              std::vector<char> novaTroj;
              std::vector<char> novaTrojReverse;
              novaTroj.push_back(table->item((*jt)->y,0)->text().toStdString()[0]);
              novaTrojReverse.push_back(table->item(0,(*jt)->x)->text().toStdString()[0]);
              novaTroj.push_back(table->item(0,(*jt)->x)->text().toStdString()[0]);
              novaTrojReverse.push_back(table->item((*jt)->y,0)->text().toStdString()[0]);
              novaTroj.push_back((*jt)->name);
              novaTrojReverse.push_back((*jt)->name);
              trojce.push_back(novaTroj);
              trojceRevers.push_back(novaTrojReverse);
            }
          for(std::vector<tabLine>::iterator jt = rows.begin(); jt !=rows.end(); ++jt)
            {
              ss << (*jt).name;
              chC.push_back((*jt).name);
            }
          for(std::vector<tabLine>::iterator jt = cols.begin(); jt !=cols.end(); ++jt)
            {
              ss << (*jt).name;
              if(!vectorContains((*jt).name,&chC))
               chC.push_back((*jt).name);
            }

          for(std::vector<cell*>::iterator kt = cells.begin(); kt != cells.end(); ++kt){
  //          qDebug()  << (*kt)->name;
              if(!vector35Contains(chC,podtabulky)){

          //       qDebug() << "a";
                  bool uz = thercialUzavrenostTest(chC,trojce);
              //    qDebug() << "b";

                  bool as = SecondaryAsociativitaTest(rows,cols,chC,trojce,false);
              //   qDebug() << "c";
                  bool jed =SecondaryJednotkovyTest(rows,cols,trojce,false);
            //     qDebug() << "d";
                  bool inv = SecondaryInverzniTest(rows,cols,trojce,false);
              //   qDebug() << "e";
                  if(uz &&as &&  jed && inv){// &&  jednotkovy == JEDNOTKOVYMEM){
            //     if(uz && as && jed && inv)// && jednotkovy == JEDNOTKOVYMEM)
                     podtabulky.push_back(new podtabulka(trojce,chC,cells));
                     podtabulky.push_back(new podtabulka(trojceRevers,chC,cellsRevers));
                   /*    std::vector<podtabulka*> subTabulky = GetPodgrupy(chC,trojce);
                       std::vector<podtabulka*> subTabulkyRev = GetPodgrupy(chC,trojceRevers);
                       if(subTabulky.size() > 0)
                      podtabulky = MergeTabulky(podtabulky,subTabulky);
                       if(subTabulkyRev.size() > 0)
                       podtabulky = MergeTabulky(podtabulky,subTabulkyRev);*/

                    }

                  }

    //            }
}
          }
        jednotkovy = JEDNOTKOVYMEM;
        //qDebug() << "Perm : " << ss.str().c_str();
      }
}

    qDebug() << "podgrupy done";
   return podtabulky;
}



Vector2 MainWindow::getTableCoords(char x,char y)
{
  Vector2 output;
  for(int ix = 1; ix <= CHARS_C->size();  ix++)
    {
      if(table->item(0,ix)->text().toStdString()[0] == x) output.x = ix;
    }
  for(int iy = 1; iy <= CHARS_C->size();  iy++)
    {
      if(table->item(iy,0)->text().toStdString()[0] == y) output.y = iy;
    }
  return output;
}

QString MainWindow::printRady()
{
  QString output = "<font size=\"2\"><b>Řády prvků p(x) :</b><br>";
  bool tabed = false;
  for(std::vector<char>::iterator it = CHARS_C->begin(); it != CHARS_C->end(); ++it)
    {
      std::string tb = "<br>";
      if(tabed) tb = "<br>";
      int currRad = radTest(0,(*it),(*it));

      if(getResultCharOn((*it),(*it),*TROJICE) == jednotkovy) currRad = 0;
   if((*it) != jednotkovy && currRad != -1) currRad++;
      std::stringstream ss;
      if(currRad != -1)
        ss<<"p("<<(*it) << ") = " << currRad+1 << tb;
      else ss<<"p("<<(*it) << ") = " << "<span style=\"color:#aa0000;\">∞</span>" << tb;
      output.append(QString(ss.str().c_str()));
      tabed = !tabed;
    }
  output.append("</font>");
    return output;
}

int MainWindow::radTest(int curr_count, char x,char y)
{
  if((curr_count != 0 && x == y) || curr_count > 20)
   return -1;
  char z = getResultCharOn(x,y,*TROJICE);
  curr_count++;

  if(z == jednotkovy)
    return curr_count;

  return radTest(curr_count,x,z);
}

std::vector<vertex*> MainWindow:: inverseGraph(std::vector<vertex*> orig)
{
  std::vector<vertex*> output;
  for(std::vector<vertex*>::iterator it = orig.begin(); it != orig.end(); ++it)
    {
      vertex * nVert = new vertex((*it));
      nVert->derived = (*it)->inherited;
      nVert->inherited = (*it)->derived;
      output.push_back(nVert);
    }
  return output;
}

void MainWindow:: doOrdering(std::vector<vertex*> graph,vertex* v,std::vector<vertex*> *resultGraph)
{
   v->thisState = OPENED;
   std::vector<vertex*> getEdges;
   for(std::vector<vertex*>::iterator it = v->inherited.begin(); it != v->inherited.end(); it++){
      getEdges.push_back((*it));
     }

   std::sort (getEdges.begin(), getEdges.end(),
       [](vertex * a, vertex * b){return a->index < b->index;}
   );

    for(std::vector<vertex*>::iterator it = getEdges.begin(); it !=getEdges.end(); it++)
      {
        if((*it)->thisState == FRESH) doOrdering(graph,(*it),resultGraph);
          else if ((*it)->thisState == OPENED)
          {
            return;
          }
      }

      resultGraph->push_back((v));
      v->thisState = CLOSED;
}

std::vector<vertex*> MainWindow::topologyOrder(std::vector<vertex *> graf)
{
  std::vector<vertex*> output;




  std::vector<vertex*> roots;


  for(std::vector<vertex*>::iterator it = graf.begin(); it != graf.end(); ++it)
    {
      if((*it)->derived.size() == 0){
          roots.push_back((*it));
        }
    }

  std::vector<vertex*> inverted = inverseGraph(graf);
  for(std::vector<vertex*>::iterator it = graf.begin(); it != graf.end(); ++it){
    (*it)->thisState = FRESH;
    }

  for(std::vector<vertex*>::iterator it = roots.begin(); it != roots.end(); ++it)
    {
        doOrdering(inverted,(*it),&output);
    }
 return output;

}

bool MainWindow::regularityTest(std::vector<vertex*> graf)
{
  return true;
}


bool MainWindow::uplnyTest(std::vector<vertex *> graf)
{
/*  for(std::vector<vertex*>::iterator it = graf.begin(); it != graf.end(); ++it)
    {
      for(std::vector<vertex*>::iterator jt = graf.begin(); jt != graf.end(); ++jt)
        {
          if(!vectorContains3((*jt),(*it)->derived))
            {
              std::stringstream ss;
              ss << "Neexistuje hrana z "<< (*it)->name << " do " << (*jt)->name << " .";
              ui->c_uplnost->setToolTip(QString(ss.str().c_str()));
                  return false;
            }
            if(vectorContains3((*jt),(*it)->inherited) )
              {
                std::stringstream ss;
                ss << "Neexistuje hrana z "<< (*jt)->name << " do " << (*it)->name << " .";
                ui->c_uplnost->setToolTip(QString(ss.str().c_str()));
                    return false;
              }
        }
    }
  std::stringstream ss;
  ss << "Graf je úplný jelikož mezi každými dvěma uzly existuje právě jedna hrana.";
  ui->c_uplnost->setToolTip(QString(ss.str().c_str()));*/
      return false;
}

void MainWindow::cycleDebug(std::vector<vertex*>* pathToAdd,vertex* mainVert,std::vector<vertex*> workingVerts)
{

    for(std::vector<vertex*>::iterator it = workingVerts.begin(); it != workingVerts.end(); ++it)
      {
        if((*it)->thisState == CLOSED) continue;

        if(vectorContains3(mainVert,(*it)->derived))
          {
            pathToAdd->push_back(*it);
            pathToAdd->push_back(mainVert);
            ui->c_cykl->setToolTip(printVerticies("Graf obsahuje cyklus : ",*pathToAdd,false));
            return;
          }
         (*it)->thisState = CLOSED;
          pathToAdd->push_back((*it));
         cycleDebug(pathToAdd,mainVert,(*it)->derived);
      }
    pathToAdd->clear();
}

bool MainWindow::cycleTest(std::vector<vertex *> graf)
{
  for(std::vector<vertex*>::iterator it = graf.begin(); it != graf.end(); ++it) (*it)->thisState = OPENED;
  for(std::vector<vertex*>::iterator it = graf.begin(); it != graf.end(); ++it)
    {
      std::vector<vertex*> subGraph;
      getDerived((*it),&subGraph);
      if(vectorContains3((*it),subGraph)){
          std::vector<vertex*>debugVert;
          cycleDebug(&debugVert,(*it),(*it)->derived);



          return true;

        }
    }
  return false;

}

bool MainWindow::distributivniSvazTest(std::vector<vertex*> graf)
{
  double actualID = 0;
  double destination = graf.size() * graf.size() * graf.size();



  // all a,b,c : a inf (b sup c) = (a inf b) sup (a inf c)
  // all a,b,c : a sup (b inf c) = (a sup b) inf (a sup c)


  for(std::vector<vertex*>::iterator a = graf.begin(); a != graf.end(); ++a)
    {
      for(std::vector<vertex*>::iterator b = graf.begin(); b != graf.end(); ++b)
        {
          for(std::vector<vertex*>::iterator c = graf.begin(); c != graf.end(); ++c)
            {
              // First line

              actualID += 1;
              if((int)actualID % 100 == 0) QCoreApplication::processEvents();
             ui->progressBar->setValue(40 + int(double(actualID / destination) * 20));

              std::vector<vertex*> b_sup_c;  b_sup_c.push_back((*b)); b_sup_c.push_back((*c));
              std::vector<vertex*> a_inf_b; a_inf_b.push_back((*a)); a_inf_b.push_back((*b));
              std::vector<vertex*> a_inf_c; a_inf_c.push_back((*a)); a_inf_c.push_back((*c));

              supInfHolder b_sup_c_result, a_inf_b_result, a_inf_c_result, left_side_result, right_side_result;
              b_sup_c_result= getSUPINF(b_sup_c,&graf);
              a_inf_b_result = getSUPINF(a_inf_b,&graf);
              a_inf_c_result = getSUPINF(a_inf_c,&graf);

              std::vector<vertex*> left_side; left_side.push_back((*a)); left_side.push_back(b_sup_c_result.supremum);
              std::vector<vertex*> right_side; right_side.push_back(a_inf_b_result.infinum); right_side.push_back(a_inf_c_result.infinum);

              left_side_result = getSUPINF(left_side,&graf);
              right_side_result = getSUPINF(right_side,&graf);

              if(left_side_result.infinum->name != right_side_result.supremum->name)
                {
                  std::stringstream ss;
                  ss << "Selhalo jelikož neplatí rovnost  : \n";
                  ss << "\n Levá strana výsl. = " << (*a)->name << " ^ (" << (*b)->name << " v " << (*c)->name << ") = " << (*a)->name << " ^ " << b_sup_c_result.supremum->name << "  = " << left_side_result.infinum->name << "\n";
                  ss << "\n Pravá strana výsl. = (" << (*a)->name << " ^ " <<  (*b)->name << ") v (" << (*a)->name << " ^ " << (*c)->name << ") = " << a_inf_b_result.infinum->name << " v " << a_inf_c_result.infinum->name << "  = " << right_side_result.supremum->name;
                  ui->c_distrib->setToolTip(QString(ss.str().c_str()));
                   return false;
                }

              // Second line
              std::vector<vertex*> b_inf_c; b_inf_c.push_back((*b)); b_inf_c.push_back((*c));
              std::vector<vertex*> a_sup_b; a_sup_b.push_back((*a)); a_sup_b.push_back((*b));
              std::vector<vertex*> a_sup_c; a_sup_c.push_back((*a)); a_sup_c.push_back((*c));

              supInfHolder b_inf_c_result,a_sup_b_result,a_sup_c_result, left_side2_result, right_side2_result;
              b_inf_c_result = getSUPINF(b_inf_c,&graf);
              a_sup_b_result = getSUPINF(a_sup_b, &graf);
              a_sup_c_result = getSUPINF(a_sup_c,&graf);

              std::vector<vertex*> left_side2; left_side2.push_back((*a)); left_side2.push_back(b_inf_c_result.infinum);
              std::vector<vertex*> right_side2; right_side2.push_back(a_sup_b_result.supremum); right_side2.push_back(a_sup_c_result.supremum);

              left_side2_result = getSUPINF(left_side2,&graf);
              right_side2_result = getSUPINF(right_side2,&graf);

              if(left_side2_result.supremum->name != right_side2_result.infinum->name)
                {
                  std::stringstream ss;
                  ss << "Selhalo jelikož neplatí rovnost :\n";
                  ss << "\n Levá strana výsl. = " << (*a)->name << " v (" << (*b)->name << " ^ " << (*c)->name << ") = " << (*a)->name << " v " << b_inf_c_result.infinum->name << "  = " << left_side2_result.supremum->name << "\n";
                  ss << "\n Pravá strana výsl. = (" << (*a)->name << " v " <<  (*b)->name << ") ^ (" << (*a)->name << " v " << (*c)->name << ") = "  << a_sup_b_result.supremum->name << " v " << a_sup_c_result.supremum->name << "  = " << right_side2_result.infinum->name;
                  ui->c_distrib->setToolTip(QString(ss.str().c_str()));
                   return false;
                }
            }
        }
    }
  return true;
}

bool MainWindow::modularniSvazTest(std::vector<vertex*> graf)
{
  double actualID = 0;
  double destination = graf.size() * graf.size() * graf.size();



  // all a,b,c : a inf (b sup c) = (a inf b) sup c
  // all a,b,c : a sup (b inf c) = (a sup b) inf c


  for(std::vector<vertex*>::iterator a = graf.begin(); a != graf.end(); ++a)
    {
      for(std::vector<vertex*>::iterator b = graf.begin(); b != graf.end(); ++b)
        {
          for(std::vector<vertex*>::iterator c = graf.begin(); c != graf.end(); ++c)
            {
              // First line

              actualID += 1;
              if((int)actualID % 100 == 0) QCoreApplication::processEvents();
             ui->progressBar->setValue(40 + int(double(actualID / destination) * 20));

              std::vector<vertex*> b_sup_c;  b_sup_c.push_back((*b)); b_sup_c.push_back((*c));
              std::vector<vertex*> a_inf_b; a_inf_b.push_back((*a)); a_inf_b.push_back((*b));
              //std::vector<vertex*> a_inf_c; a_inf_c.push_back((*a)); a_inf_c.push_back((*c));

              supInfHolder b_sup_c_result, a_inf_b_result,  left_side_result, right_side_result;
              b_sup_c_result = getSUPINF(b_sup_c,&graf);
              a_inf_b_result = getSUPINF(a_inf_b,&graf);
             // a_inf_c_result = getSUPINF(a_inf_c,&graf);

              std::vector<vertex*> left_side; left_side.push_back((*a)); left_side.push_back(b_sup_c_result.supremum);
              std::vector<vertex*> right_side; right_side.push_back(a_inf_b_result.infinum); right_side.push_back((*c));

              left_side_result = getSUPINF(left_side,&graf);
              right_side_result = getSUPINF(right_side,&graf);

              if(left_side_result.infinum->name != right_side_result.supremum->name)
                {
                  std::stringstream ss;
                  ss << "Selhalo jelikož neplatí rovnost  : \n";
                  ss << "\n Levá strana výsl. = " << (*a)->name << " ^ (" << (*b)->name << " v " << (*c)->name << ") = " << (*a)->name << " ^ " << b_sup_c_result.supremum->name << "  = " << left_side_result.infinum->name << "\n";
                  ss << "\n Pravá strana výsl. = (" << (*a)->name << " ^ " <<  (*b)->name << ") v " << (*c)->name << " = " << a_inf_b_result.infinum->name << " v " << (*c)->name << "  = " << right_side_result.supremum->name;
                  ui->c_modul->setToolTip(QString(ss.str().c_str()));
                   return false;
                }

              // Second line
              std::vector<vertex*> b_inf_c; b_inf_c.push_back((*b)); b_inf_c.push_back((*c));
              std::vector<vertex*> a_sup_b; a_sup_b.push_back((*a)); a_sup_b.push_back((*b));
             // std::vector<vertex*> a_sup_c; a_sup_c.push_back((*a)); a_sup_c.push_back((*c));

              supInfHolder b_inf_c_result,a_sup_b_result, left_side2_result, right_side2_result;
              b_inf_c_result = getSUPINF(b_inf_c,&graf);
              a_sup_b_result = getSUPINF(a_sup_b, &graf);
             // a_sup_c_result = getSUPINF(a_sup_c,&graf);

              std::vector<vertex*> left_side2; left_side2.push_back((*a)); left_side2.push_back(b_inf_c_result.infinum);
              std::vector<vertex*> right_side2; right_side2.push_back(a_sup_b_result.supremum); right_side2.push_back((*c));

              left_side2_result = getSUPINF(left_side2,&graf);
              right_side2_result = getSUPINF(right_side2,&graf);

              if(left_side2_result.supremum->name != right_side2_result.infinum->name)
                {
                  std::stringstream ss;
                  ss << "Selhalo jelikož neplatí rovnost :\n";
                  ss << "\n Levá strana výsl. = " << (*a)->name << " v (" << (*b)->name << " ^ " << (*c)->name << ") = " << (*a)->name << " v " << b_inf_c_result.infinum->name << "  = " << left_side2_result.supremum->name << "\n";
                  ss << "\n Pravá strana výsl. = (" << (*a)->name << " v " <<  (*b)->name << ") ^ " << (*c)->name << " = "  << a_sup_b_result.supremum->name << " v " << (*c)->name << "  = " << right_side2_result.infinum->name;
                  ui->c_modul->setToolTip(QString(ss.str().c_str()));
                   return false;
                }
            }
        }
    }
  return true;
}

bool MainWindow::komplementarniSvazTest(std::vector<vertex*> graf)
{
  std::vector<vertex*> M = graf;

  vertex* min = getNejm(M,&graf);
  vertex* max = getNejv(M, &graf);

  if(min == NULL){ ui->c_komplem->setToolTip("Neexistuje svazová nula. (Neexistuje samotný nejspodnější prvek)"); return false; }
  if(max == NULL){ ui->c_komplem->setToolTip("Neexistuje svazová jedna. (Neexistuje samotný nejvyšší prvek)"); return false; }

  // Nalezení komplementů
  std::stringstream sucess;
  sucess << "Svaz. 0 = " << min->name << "\nSvaz 1 = "<<max->name<< "\nKomplementarní prvky : \n";
  for(std::vector<vertex*>::iterator a = graf.begin(); a != graf.end(); ++a)
    {
      vertex * komplement = NULL;
      sucess<<"komplement(" << (*a)->name << ") = ";
      for(std::vector<vertex*>::iterator b = graf.begin(); b != graf.end(); ++b)
       {
          std::vector<vertex*> tested; tested.push_back((*a)); tested.push_back((*b));
         supInfHolder s = getSUPINF(tested,&graf);
         if(s.infinum->name == min->name && s.supremum->name == max->name){
             komplement = (*b);
             sucess<< (*b)->name << ",";
            // break;
           }
       }
      sucess << "\n";
       if(komplement == NULL)
         {
           sucess << " Neexistuje komplement pro " << (*a)->name;
            ui->c_komplem->setToolTip(QString(sucess.str().c_str()));
            return false;
         }
    }
    ui->c_komplem->setToolTip(QString(sucess.str().c_str()));
  return true;
}

bool MainWindow::svazTest(std::vector<vertex*> graf)
{
 // qDebug() << "svaz test";
     std::vector<std::vector<vertex*>> dvojce;
     for(int i = 0; i < graf.size(); i++)
       {
         for(int j = 0; j < graf.size(); j++)
           {
              std::vector<vertex*> novaDvojce;
              novaDvojce.push_back(graf.at(i)); novaDvojce.push_back(graf.at(j));
              dvojce.push_back(novaDvojce);
           }
       }
     for(std::vector<std::vector<vertex*>> ::iterator it = dvojce.begin(); it != dvojce.end(); ++it)
       {
         supInfHolder supInf;
  //       qDebug() << "wana sup inf for " << (*it)[0]->name << " " << (*it)[1]->name;
         supInf = getSUPINF((*it),&graf);
         if(supInf.infinum == NULL)
           {
             std::stringstream ss;
             ss << "Nenalezeno INF pro M : " << printVerticies("",(*it)).toStdString();
             ui->c_svaz->setToolTip(QString(ss.str().c_str()));
             return false;
           }
           if(supInf.supremum == NULL)
             {
               std::stringstream ss;
               ss << "Nenalezeno SUP pro M : " << printVerticies("",(*it)).toStdString();
               ui->c_svaz->setToolTip(QString(ss.str().c_str()));
               return false;
             }
       }
     return true;

}

int factorial(int n)
{
    if(n > 1)
        return n * factorial(n - 1);
    else
        return 1;
}

bool MainWindow::uplnySvazTest(std::vector<vertex*> graf)
{
  double actualID = 0;
  double destination = 0;
  for(int i = 0; i < graf.size()+1; i++)
    destination += factorial(i);

  for(int i =0; i < graf.size(); i++)
    {
      std::vector<vertex*> permutated;
      for(int j = 0; j < graf.size() - i; j++)
        permutated.push_back(graf.at(j));
      do{
            actualID += 1;
          if((int)actualID % 100 == 0) QCoreApplication::processEvents();
           ui->progressBar->setValue(10 + int(double(actualID / destination) * 30));
          supInfHolder supInf;
          supInf = getSUPINF(permutated,&graf);
          if(supInf.infinum == NULL)
            {
              std::stringstream ss;
              ss << "Nenalezeno INF pro M : " << printVerticies("",permutated).toStdString();
              ui->c_usvaz->setToolTip(QString(ss.str().c_str()));
              return false;
            }
            if(supInf.supremum == NULL)
              {
                std::stringstream ss;
                ss << "Nenalezeno SUP pro M : " << printVerticies("",permutated).toStdString();
                ui->c_usvaz->setToolTip(QString(ss.str().c_str()));
                return false;
              }

      }
       while(std::next_permutation(permutated.begin(),permutated.end()));
    }
  return true;
}

std::vector<vertex*> MainWindow::MtoVerticies(std::vector<char> * M,std::vector<vertex*>* graph)
{
  std::vector<vertex*> output;

   for(std::vector<char>::iterator it = M->begin(); it != M->end(); ++it)
     {
       bool found = false;
       for(std::vector<vertex*>::iterator jt = graph->begin(); jt!= graph->end(); ++jt  )
         {
           if((*jt)->name == (*it)){
               found = true;
               output.push_back((*jt));
             }
         }
       if(!found) ERROR = true;
     }
   //if(output.size() == 0) ERROR = true;
   return output;
}

QString MainWindow::printVerticies(QString header, std::vector<vertex*> verticies,bool sort,bool useDecimals)
{
  QString output = header;
 if(sort){
  std::sort (verticies.begin(), verticies.end(),
      [](vertex * a, vertex * b){return a->index < b->index;}
  );
}
 if(( tabWidget->currentIndex() == 2 && (ui->comboBox->currentIndex() == 1 || ui->comboBox->currentIndex() == 2))
    || ( tabWidget->currentIndex() == 3 && (ui->comboBox_2->currentIndex() == 1 || ui->comboBox_2->currentIndex() == 2) ))
   useDecimals = true;

  output.append("{");
  for(int i = 0; i < verticies.size(); i++)
    {
      if(useDecimals && verticies.at(i)->name >= 'a')
        {
              std::stringstream ss;
              ss << 10+(verticies.at(i)->name - 'a');
          output.append(ss.str().c_str());
        }
      else output.append(verticies.at(i)->name);
      if(i != verticies.size() - 1) output.append(",");
    }
  output.append("}");
  return output;
}

std::vector<vertex*> MainWindow::getMin(std::vector<vertex*> M,std::vector<vertex*> *graph)
{
  std::vector<vertex*> output;
   int lowestRank = 999;
   for(std::vector<vertex*>::iterator it = M.begin(); it != M.end(); ++it)
       if((*it)->rank < lowestRank) lowestRank = (*it)->rank;

     for(std::vector<vertex*>::iterator it = M.begin(); it != M.end(); ++it)
       {
         if((*it)->rank == lowestRank)
             output.push_back((*it));
       }
     return output;
}
std::vector<vertex*>MainWindow:: getMax(std::vector<vertex*> M,std::vector<vertex*> *graph)
{
  std::vector<vertex*> output;
   int highestRank = 0;
   for(std::vector<vertex*>::iterator it = M.begin(); it != M.end(); ++it)
       if((*it)->rank > highestRank) highestRank = (*it)->rank;

     for(std::vector<vertex*>::iterator it = M.begin(); it != M.end(); ++it)
       {
         if((*it)->rank == highestRank)
             output.push_back((*it));
       }
     return output;
}
vertex*  MainWindow::getNejm(std::vector<vertex*> M,std::vector<vertex*> *graph)
{
  std::vector<vertex*> min = getMin(M,graph);
  if(min.size() <= 0 || min.size() > 1) return NULL;
  return min[0];
}

vertex* MainWindow::getNejv(std::vector<vertex*> M,std::vector<vertex*> *graph)
{
  std::vector<vertex*> min = getMax(M,graph);
  if(min.size() <= 0 || min.size() > 1) return NULL;
  return min[0];
}

std::vector<vertex*> MainWindow::combineVectors (std::vector<vertex*> a,std::vector<vertex*> b )
{
  std::vector<vertex*> c = cloneGraph(&a);
  for(std::vector<vertex*>::iterator it = b.begin(); it != b.end(); ++it)
    { if(!vectorContains3((*it),c)) c.push_back((*it)); }
  return c;
}

std::vector<vertex*> MainWindow::subtractVectors(std::vector<vertex*> a,std::vector<vertex*> b )
{
  std::vector<vertex*> c;
  for(std::vector<vertex*>::iterator it = a.begin(); it != a.end(); ++it)
    {
      if(!vectorContains3((*it),b)) c.push_back((*it));
    }
  return c;
}

std::vector<vertex*> MainWindow::cleanDuplicates(std::vector<vertex*> vec)
{
  std::vector<vertex*> output;
  for(std::vector<vertex*>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
      if(!vectorContains3((*it),output)) output.push_back((*it));
    }
  qDebug() << "Cleaned from" << vec.size() << " to " <<  output.size();
  return output;
}

std::vector<vertex*> MainWindow::unionOfVectors(std::vector<vertex*> a,std::vector<vertex*> b )
{
  std::vector<vertex*> output;
  for(std::vector<vertex*>::iterator it = a.begin(); it != a.end(); ++it)
    {
      if(vectorContains3((*it),b)) output.push_back((*it));
    }
  return output;
}

supInfHolder MainWindow::getSUPINF(std::vector<vertex*> M, std::vector<vertex*>* graph)
{
  supInfHolder output;
  // DOLNI ZAVORA -> sebrat všechny společné předky pro všechny prvky množiny M
  // HORNI ZAVORA -> sebrat všechny společné potomky pro všechny prvky množiny M


  std::vector<vertex*> dolniZavora;
  std::vector<vertex*> horniZavora;
  vertex* inf = NULL;
  vertex* sup=NULL;
//qDebug() << "A";
  getInherited(M.at(0),&dolniZavora);    // naplění všech potomku (až na posledního)
  for(std::vector<vertex*>::iterator jt = M.begin(); jt != M.end(); ++jt){
      std::vector<vertex*> subGraphA;
      getInherited((*jt),&subGraphA);    // naplění všech potomku (až na posledního)

      dolniZavora =unionOfVectors(dolniZavora,subGraphA);
  }
  //dolniZavora = cleanDuplicates(dolniZavora);

//qDebug() << "A.5";
      int highestRank = 0;
      for(std::vector<vertex*>::iterator it = dolniZavora.begin(); it != dolniZavora.end(); ++it)
        {
          if((*it)->rank > highestRank) highestRank = (*it)->rank;
        }
      int count = 0;
      for(std::vector<vertex*>::iterator it = dolniZavora.begin(); it != dolniZavora.end(); ++it)
        {
          if((*it)->rank == highestRank)
            {
              inf = (*it);
              count++;
            }
        }
      if(count > 1) inf = NULL;
 //   qDebug() << "B";
      // Horni sup
      getDerived(M.at(0),&horniZavora);    // naplění všech potomku (až na posledního)
      for(std::vector<vertex*>::iterator jt = M.begin(); jt != M.end(); ++jt){
          std::vector<vertex*> subGraphA;
          getDerived((*jt),&subGraphA);    // naplění všech potomku (až na posledního)

          horniZavora =unionOfVectors(horniZavora,subGraphA);
      }
      //horniZavora = cleanDuplicates(horniZavora);

//qDebug() << "C";
      int lowestRank = 999;
      for(std::vector<vertex*>::iterator it = horniZavora.begin(); it != horniZavora.end(); ++it)
        {
          if((*it)->rank < lowestRank) lowestRank = (*it)->rank;
        }
      count = 0;
      for(std::vector<vertex*>::iterator it = horniZavora.begin(); it != horniZavora.end(); ++it)
        {
          if((*it)->rank ==  lowestRank)
            {
              sup = (*it);
              count++;
            }
        }

      if(count > 1) sup = NULL;

 output.dolniZavora = dolniZavora;
 output.horniZavora = horniZavora;
 output.supremum = sup;
 output.infinum = inf;
// qDebug() << "D";
      return output;
}


void MainWindow::raiseRank(vertex * v, int toRank)
{
   qDebug() << " seting rank of " << v->name << " to " << toRank;
  v->rank = toRank;
  for(std::vector<vertex*>::iterator jt = v->derived.begin(); jt != v->derived.end(); ++jt)
    {
      if((*jt)->name != v->name)
      raiseRank((*jt),toRank+1);

    }
}

void MainWindow::setRanksToVerticies(std::vector<vertex *> *graph)
{
  std::vector<vertex*> nonInherited;   // get verticies without parent -> rank 0
  for(std::vector<vertex*>::iterator it = graph->begin(); it != graph->end(); ++it)
    {
      if((*it)->inherited.size() == 0){
         raiseRank((*it),0);
        }
    }

}

std::vector<vertex*> MainWindow::convertGraphToHassDiagram(std::vector<vertex*>  graph)
{
  std::vector<vertex*>output = cloneGraph(&graph);
  // reference na podstromy
  for(std::vector<vertex*>::iterator it = output.begin(); it != output.end(); ++it)
    {
        std::vector<vertex*> subGraph;
        getDerived((*it),&subGraph);    // naplění všech potomků (až na posledního)

       for(std::vector<vertex*>::iterator jt = subGraph.begin(); jt != subGraph.end(); ++jt)
        {
            if(!vectorContains3((*jt),(*it)->derived)) (*it)->derived.push_back((*jt));
             if(!vectorContains3((*it),(*jt)->inherited)) (*jt)->inherited.push_back((*it));
        }
    }
  // reflexivita
   for(std::vector<vertex*>::iterator it = output.begin(); it != output.end(); ++it)
     {
       if(!vectorContains3((*it),(*it)->derived)) (*it)->derived.push_back(*it);
       if(!vectorContains3((*it),(*it)->inherited)) (*it)->inherited.push_back(*it);
     }
   return output;
}

void MainWindow::UpdateUsporadani()
{
  ERROR = false;
  ui->progressBar->setValue(0);
    ui->c_err_3->setVisible(false);
    bool sTest;
    bool uStest;
std::vector<vertex*> mem = cloneGraph(&usporadaniVerticies);
//setRanksToVerticies(&usporadaniVerticies);
std::vector<vertex*> hass =  convertGraphToHassDiagram(usporadaniVerticies);
      std::vector<vertex*> M = MtoVerticies(GetCharsFromInput(ui->mnozinaM->text()),&hass);

      ui->progressLab->setText("Vyhodnoceni Min(M)...");
      ui->minLab->setText("Min(M) : ");
      ui->maxLab->setText("Max(M) : ");
      ui->nejmLab->setText("Nejmen. (M) : ");
      ui->nejvLab->setText("Největší. (M) : ");
      ui->uLab->setText("L(M) - dolní závora : ");
      ui->uLab->setText("U(M) - horní závora : ");
      ui->infLab->setText("Inf(M) : ");
      ui->supLab->setText("Sup(M) : ");

        if(M.size() > 0){
      std::stringstream ss;
      ui->minLab->setText(printVerticies("Min(M) : ",getMin(M,&hass)));
      ui->progressLab->setText("Vyhodnoceni Max(M)...");
       ui->maxLab->setText(printVerticies("Max(M) : ",getMax(M,&hass)));
       if(getNejm(M,&hass) != NULL)
            ss << "Nejmen. (M) : " << getNejm(M,&hass)->name ;
       else ss << "Nejmen. (M) : -";
        ui->nejmLab->setText(QString(ss.str().c_str()));
        std::stringstream ss2;
        if(getNejv(M,&hass) != NULL)
             ss2 << "Největší. (M) : " << getNejv(M,&hass)->name;
        else ss2 << "Největší. (M) : -";
         ui->nejvLab->setText(QString(ss2.str().c_str()));

            supInfHolder h = getSUPINF(M,&hass);
            ui->progressLab->setText("Vyhodnoceni Závor(M)...");
           ui->lLab->setText(printVerticies("L(M) - dolní závora : ",h.dolniZavora));
           ui->uLab->setText(printVerticies("U(M) - horní závora : ",h.horniZavora));
           std::stringstream ss3;
           ui->progressLab->setText("Vyhodnoceni SupInf(M)...");
           if(h.infinum != NULL)
                ss3 << "Inf(M) : " << h.infinum->name ;
           else ss3 << "Inf(M) : -";
           ui->infLab->setText(QString(ss3.str().c_str()));

           std::stringstream ss4;
           if(h.supremum != NULL)
                ss4 << "Sup(M) : " << h.supremum->name ;
           else ss4 << "Sup(M) : -";
           ui->supLab->setText(QString(ss4.str().c_str()));


            ui->progressBar->setValue(5);
            ui->progressLab->setText("Svaz test....");

           sTest = svazTest(hass);
           ui->progressBar->setValue(10);
           if(ui->c_test_uplny->isChecked())
             {
           if(sTest){
               ui->progressLab->setText("Úplny svaz test...");
            uStest = uplnySvazTest(hass);
             }
           else uStest = false;

             }
           else uStest = false;
          }
        else {
            ui->progressLab->setText("Svaz test...");
            sTest = svazTest(hass);
            ui->progressBar->setValue(10);
            if(sTest){
                ui->progressLab->setText("Úplny svaz test...");
             uStest = uplnySvazTest(hass);
              }
            else uStest = false;
          }
        ui->progressBar->setValue(40);

        ui->c_distrib->setToolTip("");
        ui->c_modul->setToolTip("");
        ui->c_komplem->setToolTip("");
        ui->c_boole->setToolTip("");
        bool distributivni = false;
        bool modularni = false;
        bool komplementarni = false;
        bool booleovsky = false ;

        if(sTest){
        ui->progressLab->setText("Distributivní svaz test...");
         distributivni = distributivniSvazTest(hass);
         modularni = true;
        if(distributivni){
            ui->c_modul->setToolTip("Distributivní svaz je vždy zároveň modulární.");
            modularni = true;
         }
        else
          {
            ui->progressLab->setText("Modulární svaz test...");
            modularni = modularniSvazTest(hass);
            ui->progressBar->setValue(60);
          }

   ui->progressBar->setValue(80);
   ui->progressLab->setText("Komplementární svaz test...");
  komplementarni = komplementarniSvazTest(hass);


 ui->progressBar->setValue(99);
    booleovsky = distributivni && modularni && komplementarni;
     ui->c_boole->setToolTip("Booleovský svaz je takový svaz, který je zároveň Distributivní,Modulární a Komplementární");

          }
ui->c_modul->setChecked(modularni);
ui->c_komplem->setChecked(komplementarni);
 ui->c_boole->setChecked(booleovsky);
ui->c_distrib->setChecked(distributivni);
ui->progressBar->setValue(100);
ui->progressLab->setText("Hotovo!");

QRgb green = qRgb(0,230,0);
QRgb red = qRgb(230,0,0);

ui->c_distrib->setPalette(QPalette(distributivni  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
ui->c_komplem->setPalette(QPalette(komplementarni  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
ui->c_modul->setPalette(QPalette(modularni  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
ui->c_boole->setPalette(QPalette(booleovsky  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));

   std::vector<char> allUsedChars;
   std::vector<std::vector<char>> allRelations;
   std::stringstream ssused;
   std::stringstream ssrels;



   ssrels << "{";
   ssused << "{";
   for(std::vector<vertex*>::iterator it = hass.begin(); it != hass.end(); ++it)
     {
      allUsedChars.push_back((*it)->name);
      ssused << (*it)->name << ",";
      for(std::vector<vertex*>::iterator jt = (*it)->derived.begin(); jt != (*it)->derived.end(); ++jt)
        {
          std::vector<char> newRelation;
          newRelation.push_back((*it)->name);
          newRelation.push_back((*jt)->name);
          ssrels << "(" << (*it)->name << "," << (*jt)->name << "),";
          allRelations.push_back(newRelation);
        }
     }
   ssrels << "}";
   ssused << "}";

   c_refl->setToolTip("");
   c_irefl->setToolTip("");
   c_sym->setToolTip("");
   c_asym->setToolTip("");
   c_antisym->setToolTip("");
   c_trans->setToolTip("");
   c_ekv->setToolTip("");
   ui->c_uspo->setToolTip("");
   ui->c_toler_2->setToolTip("");
     ui->c_err_3->setVisible(ERROR);
       ui->c_err_3->setChecked(ERROR);

   CHARS = &allUsedChars;
   RELATIONS = &allRelations;




  ui->c_re_3->setPalette(QPalette(TestRelations(RE) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui->c_ire_3->setPalette(QPalette(TestRelations(IRE) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_sym_3->setPalette(QPalette(TestRelations(SY) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui-> c_asym_3->setPalette(QPalette(TestRelations(ASY) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_anti_3->setPalette(QPalette(TestRelations(AN) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_trans_3->setPalette(QPalette(TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui-> c_ekv_3->setPalette(QPalette(TestRelations(RE) && TestRelations(SY) && TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_uspo_3->setPalette(QPalette(TestRelations(RE) && TestRelations(AN) && TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));

//  ui-> c_toler_2->setPalette(QPalette(TestRelations(RE) && TestRelations(SY)  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));

  ui->c_svaz->setPalette(QPalette(sTest ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_usvaz->setPalette(QPalette(uStest ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));

 ui->c_re_3->setChecked(TestRelations(RE));
  ui->c_ire_3->setChecked(TestRelations(IRE));
  ui->c_sym_3->setChecked(TestRelations(SY));
 ui-> c_asym_3->setChecked(TestRelations(ASY));
 ui->c_anti_3->setChecked(TestRelations(AN));
 ui->c_trans_3->setChecked(TestRelations(TR));
 ui->c_ekv_3->setChecked(TestRelations(RE) && TestRelations(SY) && TestRelations(TR));
//  ui->c_toler_2->setChecked(TestRelations(RE) && TestRelations(SY));
 ui->c_svaz->setChecked(sTest );
 ui->c_usvaz->setChecked(uStest );

 bool toler = TestRelations(RE) && TestRelations(SY);
 bool kvazi = TestRelations(RE) && TestRelations(TR);
 bool neostre = TestRelations(RE) && TestRelations(AN) && TestRelations(TR);
 bool ostre = TestRelations(IRE) && TestRelations(TR);

   ui-> c_toler_3->setPalette(QPalette(toler  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
   ui->c_toler_3->setChecked(toler);
     ui->c_toler_3->setToolTip("Tolerance nastává tehdy pokud jsou relace zároveň: RE,SY");

     ui-> c_kvazi_3->setPalette(QPalette(kvazi  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
     ui->c_kvazi_3->setChecked(kvazi);
       ui->c_kvazi_3->setToolTip("Kvaziuspořádání nastává tehdy pokud jsou relace zároveň: RE,TRA");

       ui-> c_neostre_3->setPalette(QPalette(neostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
       ui->c_neostre_3->setChecked(neostre);
         ui->c_neostre_3->setToolTip("Neostré usp. nastává tehdy pokud jsou relace zároveň: RE,AN,TRA");

         ui-> c_ostre_3->setPalette(QPalette(ostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
         ui->c_ostre_3->setChecked(ostre);
           ui->c_ostre_3->setToolTip("Ostré usp. nastává tehdy pokud jsou relace zároveň: IRE,TRA");

           ui-> c_poset_3->setPalette(QPalette(neostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
           ui->c_poset_3->setChecked(neostre);
             ui->c_poset_3->setToolTip("Poset nastává tehdy pokud jsou relace zároveň: RE,AN,TRA");


  ui-> c_ekv_3->setChecked(TestRelations(RE) && TestRelations(SY) && TestRelations(TR));
 ui->c_uspo_3->setChecked(TestRelations(RE) && TestRelations(AN) && TestRelations(TR));
 ui->c_ekv_3->setToolTip("Ekvivalence nastává tehdy pokud jsou relace zároveň: RE,SY,TRA");
//  if(!ui-> c_toler_2->isChecked()) ui->c_ekv_3->setToolTip("Tolerance nastává tehdy pokud jsou relace zároveň: RE,SY");
 ui->c_uspo_3->setToolTip("Relace je uspořádáním, pokud zárověň platí: RE,AN,TRA");

 ui->c_re_3->setToolTip(c_refl->toolTip());
  ui->c_ire_3->setToolTip(c_irefl->toolTip());
  ui->c_sym_3->setToolTip(c_sym->toolTip());
 ui-> c_asym_3->setToolTip(c_asym->toolTip());
 ui->c_anti_3->setToolTip(c_antisym->toolTip());
 ui->c_trans_3->setToolTip(c_trans->toolTip());

ui->progressBar->setValue(100);

// ui->updateBTN_8->setEnabled(false);
}

std::vector<vertex*> MainWindow::preEndingVerticies(std::vector<vertex*> endingVerticies)
{
  std::vector<vertex*> output;
  for(std::vector<vertex*>::iterator it = endingVerticies.begin(); it != endingVerticies.end(); ++it)
    {
      for(std::vector<vertex*>::iterator jt = (*it)->inherited.begin(); jt != (*it)->inherited.end(); ++jt)
        {
            if(!vectorContains3((*jt),output))
                output.push_back((*jt));
        }
    }
  return output;
}

void MainWindow::constructCore(std::vector<vertex*>* workingGraph)
{
  std::vector<vertex*> ending = endingVerticies(workingGraph);
  if(ending.size() > 0)
    {
      for(std::vector<vertex*>::iterator it = ending.begin(); it != ending.end(); ++it)   //add to core
        {
        core.push_back((*it));
        qDebug() << "adding " << (*it)->name << " to core";
        }
    }
  else return;  // finish
  std::vector<vertex*> parents = preEndingVerticies(ending);
   removeVerticiesFromGraph(&ending,workingGraph);
   removeVerticiesFromGraph(&parents,workingGraph);
    constructCore(workingGraph);
}

std::vector<vertex*> MainWindow::endingVerticies(std::vector<vertex*> * workingGraph)
{
  std::vector<vertex*> output;
    for(std::vector<vertex*>::iterator it = workingGraph->begin(); it != workingGraph->end(); ++it)
      {
        if((*it)->derived.size() == 0  && !vectorContains3((*it),core)){
            qDebug() << "adding to ending = "<< (*it)->name;
            output.push_back((*it));
          }
      }
    return output;
}

void MainWindow::removeVerticiesFromGraph(std::vector<vertex*> * verticies,std::vector<vertex*> * workingGraph)
{
  std::vector<vertex*> newGraph;
  for(int i = 0; i < workingGraph->size(); i++)
    {
      bool found= false;
      for(std::vector<vertex*>::iterator it = verticies->begin(); it != verticies->end(); ++it)
        {
          if((*it) == workingGraph->at(i))
            {
               found = true;
               break;
            }
        }
      if(!found) newGraph.push_back(workingGraph->at(i));
    }

  // clean references
  for(std::vector<vertex*>::iterator it = newGraph.begin(); it != newGraph.end(); ++it)
    {
      std::vector<vertex*> newDerived;
      for(std::vector<vertex*>::iterator jt = (*it)->derived.begin(); jt != (*it)->derived.end(); ++jt)
        {
          bool found = false;
           for(std::vector<vertex*>::iterator kt = verticies->begin(); kt != verticies->end(); ++kt)
             {
               if((*kt) == (*jt))
                 {
                   found = true;
                 }
             }
           if(!found) newDerived.push_back((*jt));
        }
      (*it)->derived = newDerived;

      std::vector<vertex*> newInherited;
      for(std::vector<vertex*>::iterator jt = (*it)->inherited.begin(); jt != (*it)->inherited.end(); ++jt)
        {
          bool found = false;
           for(std::vector<vertex*>::iterator kt = verticies->begin(); kt != verticies->end(); ++kt)
             {
               if((*kt) == (*jt))
                 {
                   found = true;
                 }
             }
           if(!found) newInherited.push_back((*jt));
        }
      (*it)->inherited = newInherited;
    }

  workingGraph = &newGraph;
}

std::vector<vertex*> MainWindow::cloneGraph(std::vector<vertex*>* original)
{
  std::vector<vertex*>* output = new std::vector<vertex*>();
  for(std::vector<vertex*>::iterator it = original->begin(); it != original->end(); ++it){
      vertex* clonedVert = new vertex((*it)->name,(*it)->x,(*it)->y);
      clonedVert->index = (*it)->index;
      clonedVert->rank = (*it)->rank;
      output->push_back(clonedVert);
    }
  for(int i = 0; i < original->size(); i++){
      vertex* origVert = original->at(i);
      vertex* clonedVert = output->at(i);
      for(std::vector<vertex*>::iterator it = origVert->derived.begin(); it != origVert->derived.end(); ++it)
        {
          for(std::vector<vertex*>::iterator jt = output->begin(); jt != output->end(); ++jt)
            {
              if((*it)->name == (*jt)->name)
                {
                  clonedVert->derived.push_back((*jt));
                  break;
                }
            }
        }

      for(std::vector<vertex*>::iterator it = origVert->inherited.begin(); it != origVert->inherited.end(); ++it)
        {
          for(std::vector<vertex*>::iterator jt = output->begin(); jt != output->end(); ++jt)
            {
              if((*it)->name == (*jt)->name)
                {
                  clonedVert->inherited.push_back((*jt));
                  break;
                }
            }
        }
    }

  return*output;
}

void MainWindow::getDerived(vertex*from,std::vector<vertex*>* subGraph,vertex* starting)
{
    if(from->derived.size() == 0) return;
     for(std::vector<vertex*>::iterator it = from->derived.begin(); it != from->derived.end(); ++it)
       {
         if(!vectorContains3((*it),*subGraph))
           { subGraph->push_back((*it));
              getDerived((*it),subGraph);
           }
       }
}

void MainWindow::getInherited(vertex*from,std::vector<vertex*>* subGraph,vertex * starting)
{
    if(from->inherited.size() == 0) return;
     for(std::vector<vertex*>::iterator it = from->inherited.begin(); it != from->inherited.end(); ++it)
       {
         if(!vectorContains3((*it),*subGraph))
           { subGraph->push_back((*it));
              getInherited((*it),subGraph);
           }
       }
}





void MainWindow::UpdateGrafy()
{
   std::vector<char> allUsedChars;
   std::vector<std::vector<char>> allRelations;
   std::stringstream ssused;
   std::stringstream ssrels;


   ssrels << "{";
   ssused << "{";
   for(std::vector<vertex*>::iterator it = graphVerticies.begin(); it != graphVerticies.end(); ++it)
     {
      allUsedChars.push_back((*it)->name);
      ssused << (*it)->name << ",";
      for(std::vector<vertex*>::iterator jt = (*it)->derived.begin(); jt != (*it)->derived.end(); ++jt)
        {
          std::vector<char> newRelation;
          newRelation.push_back((*it)->name);
          newRelation.push_back((*jt)->name);
          ssrels << "(" << (*it)->name << "," << (*jt)->name << "),";
          allRelations.push_back(newRelation);
        }
     }
   ssrels << "}";
   ssused << "}";

   c_refl->setToolTip("");
   c_irefl->setToolTip("");
   c_sym->setToolTip("");
   c_asym->setToolTip("");
   c_antisym->setToolTip("");
   c_trans->setToolTip("");
   c_ekv->setToolTip("");
   ui->c_uspo->setToolTip("");
   ui->c_cykl->setToolTip("");

   CHARS = &allUsedChars;
   RELATIONS = &allRelations;

  QRgb green = qRgb(0,230,0);
  QRgb red = qRgb(230,0,0);

  bool cycle = cycleTest(graphVerticies);


  ui->c_re_2->setPalette(QPalette(TestRelations(RE) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui->c_ire_2->setPalette(QPalette(TestRelations(IRE) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_sym_2->setPalette(QPalette(TestRelations(SY) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui-> c_asym_2->setPalette(QPalette(TestRelations(ASY) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_anti_2->setPalette(QPalette(TestRelations(AN) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_trans_2->setPalette(QPalette(TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui-> c_ekv_2->setPalette(QPalette(TestRelations(RE) && TestRelations(SY) && TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_uspo_2->setPalette(QPalette(TestRelations(RE) && TestRelations(AN) && TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
 ui->c_cykl->setPalette(QPalette(cycle ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));

 bool toler = TestRelations(RE) && TestRelations(SY);
 bool kvazi = TestRelations(RE) && TestRelations(TR);
 bool neostre = TestRelations(RE) && TestRelations(AN) && TestRelations(TR);
 bool ostre = TestRelations(IRE) && TestRelations(TR);

   ui-> c_toler_2->setPalette(QPalette(toler  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
   ui->c_toler_2->setChecked(toler);
     ui->c_toler_2->setToolTip("Tolerance nastává tehdy pokud jsou relace zároveň: RE,SY");

     ui-> c_kvazi_2->setPalette(QPalette(kvazi  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
     ui->c_kvazi_2->setChecked(kvazi);
       ui->c_kvazi_2->setToolTip("Kvaziuspořádání nastává tehdy pokud jsou relace zároveň: RE,TRA");

       ui-> c_neostre_2->setPalette(QPalette(neostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
       ui->c_neostre_2->setChecked(neostre);
         ui->c_neostre_2->setToolTip("Neostré usp. nastává tehdy pokud jsou relace zároveň: RE,AN,TRA");

         ui-> c_ostre_2->setPalette(QPalette(ostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
         ui->c_ostre_2->setChecked(ostre);
           ui->c_ostre_2->setToolTip("Ostré usp. nastává tehdy pokud jsou relace zároveň: IRE,TRA");

           ui-> c_poset_2->setPalette(QPalette(neostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
           ui->c_poset_2->setChecked(neostre);
             ui->c_poset_2->setToolTip("Poset nastává tehdy pokud jsou relace zároveň: RE,AN,TRA");


 ui->c_re_2->setChecked(TestRelations(RE));
  ui->c_ire_2->setChecked(TestRelations(IRE));
  ui->c_sym_2->setChecked(TestRelations(SY));
 ui-> c_asym_2->setChecked(TestRelations(ASY));
 ui->c_anti_2->setChecked(TestRelations(AN));
 ui->c_trans_2->setChecked(TestRelations(TR));
  ui-> c_ekv_2->setChecked(TestRelations(RE) && TestRelations(SY) && TestRelations(TR));
 ui->c_uspo_2->setChecked(TestRelations(RE) && TestRelations(AN) && TestRelations(TR));
 ui->c_cykl->setChecked(cycle);
  ui->c_ekv_2->setToolTip("Ekvivalence nastává tehdy pokud jsou relace zároveň: RE,SY,TRA");
 ui->c_uspo_2->setToolTip("Relace je uspořádáním, pokud zárověň platí: RE,AN,TRA");

 ui->c_re_2->setToolTip(c_refl->toolTip());
  ui->c_ire_2->setToolTip(c_irefl->toolTip());
  ui->c_sym_2->setToolTip(c_sym->toolTip());
 ui-> c_asym_2->setToolTip(c_asym->toolTip());
 ui->c_anti_2->setToolTip(c_antisym->toolTip());
 ui->c_trans_2->setToolTip(c_trans->toolTip());



 core = *(new std::vector<vertex*>);
 std::vector<vertex*> cloned = *(new std::vector<vertex*>());
 for(std::vector<vertex*>::iterator it = graphVerticies.begin(); it != graphVerticies.end(); ++it)
   cloned.push_back(new vertex((*it)));
 constructCore(&cloned);

  std::vector<vertex*> topology = topologyOrder(graphVerticies);
  if(!cycle)
  ui->topologyLabel->setText(printVerticies("Topologické uspořádání : ",topology));
else ui->topologyLabel->setText("Topologické uspořádání : NEEXISTUJE KVULI CYKLU");


 ui->coreLabel->setText( printVerticies("Jádro grafu : ",core));
}

int MainWindow::charToIndex(char input,std::vector<char>chC)
{
  int index = 1;
  for(std::vector<char>::iterator it = chC.begin(); it != chC.end(); ++it,++index)
    {
      if((*it) == input) return index;
    }
  return index;
}

bool MainWindow::GrupoidyDoTabulky()
{
  bool uzavrenost = true;
  for(int y = 0; y < 11; y++)
    {
      for(int x = 0; x < 11; x++)
        table->item(x,y)->setText("");
    }



   for(int i = 0; i < CHARS_C->size(); i++)
     {
        table->item(0,i+1)->setText(QString(CHARS_C->at(i)));
        table->item(i+1,0)->setText(QString(CHARS_C->at(i)));
     }
  int countItems = 0;
   for(std::vector<std::vector<char>>::iterator it = TROJICE->begin(); it != TROJICE->end(); ++it)
     {
       int X = charToIndex((*it)[0],*CHARS_C);
       int Y = charToIndex((*it)[1],*CHARS_C);
       table->item(X,Y)->setText(QString((*it)[2]));
       if(!vectorContains((*it)[0],CHARS_C)) uzavrenost = false;
       if(!vectorContains((*it)[1],CHARS_C)) uzavrenost = false;
       if(!vectorContains((*it)[2],CHARS_C)) uzavrenost = false;

       countItems++;
     }

   if(countItems != CHARS_C->size() * CHARS_C->size()){
     ERROR = true;
     uzavrenost = false;
     }


   return uzavrenost;

}

char MainWindow::getResultCharOn(char x,char y,std::vector<std::vector<char>> troj)
{
  /*int iX = charToIndex(x);
  int iY = charToIndex(y);

  return table->item(iX,iY)->text().at(0).toLatin1();*/

  for(std::vector<std::vector<char>>::iterator it = troj.begin(); it != troj.end(); ++it)
    {
      char ret = (*it)[2];
      if((*it)[0] == x && (*it)[1] == y){
          return ret;

        }
    }
  return 'q';
}

bool MainWindow::SecondaryAsociativitaTest(std::vector<tabLine> rows,std::vector<tabLine> cols,std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug)
{
//  qDebug() << "Im here";
  for(std::vector<tabLine>::iterator it = rows.begin(); it != rows.end(); ++it)
    {
      char X = (*it).name;
      for(std::vector<tabLine>::iterator jt = cols.begin(); jt != cols.end(); ++jt)
        {
          char Y = (*jt).name;
          for(std::vector<char>::iterator kt = chC.begin(); kt != chC.end(); ++kt)
            {
              char Z = (*kt);

            //  if(X == Y || Y == Z || X == Z)
             //   continue;

           //   qDebug() << "Ass test for " << X << ";" << Y << ";" << Z;

              char XY = getResultCharOn(X,Y,troj);
//qDebug() << "XY is " << XY;
              char result1 = getResultCharOn(XY,Z,troj);

              // X * (Y * Z)
              char YZ =   getResultCharOn(Y,Z,troj);
              char result2 =  getResultCharOn(X,YZ,troj);

              if(result1 != result2){
                  std::stringstream ss;

                  ss << "Asociativita nesplněna při : \n";
                  ss << "(" << X << " * " << Y << ") * " << Z << " = " << result1 << "\n";
                  ss << X << " * (" << Y << " * " << Z << ") = " << result2 << "\n";
                  if(debug) c_asociativita->setToolTip(QString(ss.str().c_str()));
                  return false;
                }

            }
        }
    }
  return true;
}

bool MainWindow::AsociativitaTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug)
{
//  qDebug() << "Im here";
  for(std::vector<char>::iterator it = chC.begin(); it != chC.end(); ++it)
    {
      char X = (*it);
      for(std::vector<char>::iterator jt = chC.begin(); jt != chC.end(); ++jt)
        {
          char Y = (*jt);
          for(std::vector<char>::iterator kt = chC.begin(); kt != chC.end(); ++kt)
            {
              char Z = (*kt);

            //  if(X == Y || Y == Z || X == Z)
             //   continue;

           //   qDebug() << "Ass test for " << X << ";" << Y << ";" << Z;

              char XY = getResultCharOn(X,Y,troj);
//qDebug() << "XY is " << XY;
              char result1 = getResultCharOn(XY,Z,troj);

              // X * (Y * Z)
              char YZ =   getResultCharOn(Y,Z,troj);
              char result2 =  getResultCharOn(X,YZ,troj);

              if(result1 != result2){
                  std::stringstream ss;

                  ss << "Asociativita nesplněna při : \n";
                  ss << "(" << X << " * " << Y << ") * " << Z << " = " << result1 << "\n";
                  ss << X << " * (" << Y << " * " << Z << ") = " << result2 << "\n";
                  if(debug) c_asociativita->setToolTip(QString(ss.str().c_str()));
                  return false;
                }

            }
        }
    }
  return true;
}

bool MainWindow::SecondaryJednotkovyTest(std::vector<tabLine> rows,std::vector<tabLine> cols, std::vector<std::vector<char>>troj,bool debug)
{
  for(std::vector<tabLine>::iterator it = cols.begin(); it != cols.end(); ++it)
    {
      // X * Y == Y * X == Y  pro všechny případy Y
      char X = (*it).name;
      bool success = true;
      for(std::vector<tabLine>::iterator jt = rows.begin(); jt != rows.end(); ++jt)
        {
          char Y = (*jt).name;
          char Z =getResultCharOn(X,Y,troj);     // výsledek X*Y
          char Z2 =getResultCharOn(Y,X,troj);  // výsledek Y*X

          if(!(Y == Z && Y == Z2 && Z == Z2)) {
              success = false;
              break;
            }
        }
      if(success)
        {
          // byl nalezen
          std::stringstream ss;
          ss<< "<b>Jedn. prvek e(x) =</b> <span style=\" color:#00aa00;\">" << X << "</span>";
          if(debug)ui->jednLab->setText(QString(ss.str().c_str()));
          jednotkovy = X;
          if(debug)c_jednotkovyprvek->setToolTip(ss.str().c_str());
          return true;
        }
    }
  std::stringstream ss;
  ss<< "<b>Jedn. prvek e(x) =</b> <span style=\" color:#aa0000;\">-</span>";
  if(debug)ui->jednLab->setText(QString(ss.str().c_str()));
  if(debug)c_jednotkovyprvek->setToolTip(ss.str().c_str());
  return false;
}

bool MainWindow::JednotkovyTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug)
{
  for(std::vector<char>::iterator it = chC.begin(); it != chC.end(); ++it)
    {
      // X * Y == Y * X == Y  pro všechny případy Y
      char X = (*it);
      bool success = true;
      for(std::vector<char>::iterator jt = chC.begin(); jt != chC.end(); ++jt)
        {
          char Y = (*jt);

          int iX = charToIndex(X,chC);
          int iY = charToIndex(Y,chC);
          char Z =getResultCharOn(X,Y,troj);     // výsledek X*Y
          char Z2 =getResultCharOn(Y,X,troj);  // výsledek Y*X

          if(!(Y == Z && Y == Z2 && Z == Z2)) {
              success = false;
              break;
            }
        }
      if(success)
        {
          // byl nalezen
          std::stringstream ss;
          ss<< "<b>Jedn. prvek e(x) =</b> <span style=\" color:#00aa00;\">" << X << "</span>";
          if(debug)ui->jednLab->setText(QString(ss.str().c_str()));
          jednotkovy = X;
          if(debug)c_jednotkovyprvek->setToolTip(ss.str().c_str());
          return true;
        }
    }
  std::stringstream ss;
  ss<< "<b>Jedn. prvek e(x) =</b> <span style=\" color:#aa0000;\">-</span>";
  if(debug)ui->jednLab->setText(QString(ss.str().c_str()));
  if(debug)c_jednotkovyprvek->setToolTip(ss.str().c_str());
  return false;
}

bool MainWindow::SecondaryInverzniTest(std::vector<tabLine> rows,std::vector<tabLine> cols, std::vector<std::vector<char>>troj,bool debug)
{
  std::stringstream scs;
  bool tabed =false;
  bool totalSucess = true;
  for(std::vector<tabLine>::iterator it = cols.begin(); it != cols.end(); ++it)
    {
      // X * Y == Y * X == JEDNOTKOVY                pro každý prvek grupy

      char a = (*it).name;
      bool success = false;
      for(std::vector<tabLine>::iterator jt = rows.begin(); jt != rows.end(); ++jt)
        {
          char b = (*jt).name;

          char Z = getResultCharOn(a,b,troj);    // výsledek A*B
          char Z2 = getResultCharOn(b,a,troj);  // výsledek B*A

          if(Z == Z2 && Z == jednotkovy && Z2 == jednotkovy)
            {success = true;
              //&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
              std::string tb = "<br>";
              if(tabed) tb = "<br>";
              scs << "i(" << a << ") = " << b <<tb;
              tabed = !tabed;
              break;
            }
        }
      if(!success)
        {
            std::stringstream ss;
            std::string tb = "<br>";
            if(tabed) tb = "<br>";
            scs << "i(" << a << ") = " << "<span style=\" color:#aa0000;\">NEEXISTUJE</span>" <<tb;
             totalSucess = false;
        }
    }
  // byl nalezen
  std::stringstream ss;
  ss<< "<font size=\"2\"><b>Inverzní prvky i(x) :</b><br>";
  ss<<scs.str();
  ss << "</font>";
if(debug)  c_inverzniprvek->setToolTip(ss.str().c_str());
 if(debug) ui->invLabel->setText(QString(ss.str().c_str()));
  return totalSucess;
}

bool MainWindow::InverzniTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug)
{
  std::stringstream scs;
  bool tabed =false;
  bool totalSucess = true;
  for(std::vector<char>::iterator it = chC.begin(); it != chC.end(); ++it)
    {
      // X * Y == Y * X == JEDNOTKOVY                pro každý prvek grupy

      char a = (*it);
      bool success = false;
      for(std::vector<char>::iterator jt = chC.begin(); jt != chC.end(); ++jt)
        {
          char b = (*jt);

          int iA = charToIndex(a,chC);
          int iB = charToIndex(b,chC);
          char Z = getResultCharOn(a,b,troj);    // výsledek A*B
          char Z2 = getResultCharOn(b,a,troj);  // výsledek B*A

          if(Z == Z2 && Z == jednotkovy && Z2 == jednotkovy)
            {success = true;
              //&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
              std::string tb = "<br>";
              if(tabed) tb = "<br>";
              scs << "i(" << a << ") = " << b <<tb;
              tabed = !tabed;
              break;
            }
        }
      if(!success)
        {
            std::stringstream ss;
            std::string tb = "<br>";
            if(tabed) tb = "<br>";
            scs << "i(" << a << ") = " << "<span style=\" color:#aa0000;\">NEEXISTUJE</span>" <<tb;
             totalSucess = false;
        }
    }
  // byl nalezen
  std::stringstream ss;
  ss<< "<font size=\"2\"><b>Inverzní prvky i(x) :</b><br>";
  ss<<scs.str();
  ss << "</font>";
if(debug)  c_inverzniprvek->setToolTip(ss.str().c_str());
 if(debug) ui->invLabel->setText(QString(ss.str().c_str()));
  return totalSucess;
}

bool MainWindow::KomutativitaTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug)
{
  for(std::vector<char>::iterator it = chC.begin(); it != chC.end(); ++it)
    {
      // X * Y == Y * X                pro každý prvek grupy

      char a = (*it);
      bool success = false;
      for(std::vector<char>::iterator jt = chC.begin(); jt != chC.end(); ++jt)
        {
          char b = (*jt);

          char Z =getResultCharOn(a,b,troj);     // výsledek A*B
          char Z2 = getResultCharOn(b,a,troj);  // výsledek B*A
          if(Z != Z2)
            {
                std::stringstream ss;
                ss << a <<" * " <<b << " = " << Z << "\n" << b << " * " << a << " = " << Z2 << "\n Komutativita selhala při porovnání " << Z << " = " << Z2 << " .";
               if(debug) c_komutativita->setToolTip(ss.str().c_str());
                return false;
            }
        }
    }

      return true;
}

 std::vector<char>* MainWindow::GetCharsFromTable()
 {
   std::vector<char>* output = new  std::vector<char>();
    for(int i=1; i< 10; i++)
      {
        QString curText = table->item(0,i)->text();
        if(curText == "") break;
        else
          {
            if(vectorContains(curText.toStdString().c_str()[0],output))
              {
                ERROR = true;
                return output;
              }
             qDebug() << "Adding " << curText.toStdString().c_str()[0] << " to chars table";
            output->push_back(curText.toStdString().c_str()[0]);
          }
      }
    if(output->size() == 0) ERROR = true;
    return output;
 }

 bool MainWindow::thercialUzavrenostTest(std::vector<char> chC, std::vector<std::vector<char>>troj)
 {
   for(std::vector<std::vector<char>>::iterator it = troj.begin(); it != troj.end(); ++it)
     {
       if(!vectorContains((*it)[2],&chC)) return false;
     }
   return true;
 }

 bool MainWindow::secondaryUzavrenostTest(std::vector<char> chC, std::vector<std::vector<char>>troj)
 {
   for(int y= 0; y < 11; y++)
     {
       for(int x = 0; x < 11; x++)
         {
           QString curText = table->item(x,y)->text();
           if(curText == "") break;
           if(!vectorContains(curText.toStdString().c_str()[0],CHARS_C))
             return false;
         }
     }
   return true;
 }

 std::vector<std::vector<char>>* MainWindow::GetTrojiceFromTable()
 {

   std::vector<std::vector<char>>* output = new std::vector<std::vector<char>>();
   for(int y = 1; y < 11; y++)
     {

       QString X = table->item(y,0)->text();
       if(X.size() == 0) return output;
       for(int x = 1; x < 11; x++)
         {
           std::vector<char> unit;
           QString Y = table->item(0,x)->text();
            if(Y.size() == 0)break;
         //   table->item(x,y)->setBackgroundColor(qRgb(((x+y)/16.0)*255,((x+y)/16.0)*255,((x+y)/16.0)*255));
           QString Z = table->item(y,x)->text();
           if(Y.size() == 0 || Z.size() == 0)
             { ERROR = true; return output; }
           unit.push_back(X.at(0).toLatin1());
           unit.push_back(Y.at(0).toLatin1());
           unit.push_back(Z.at(0).toLatin1());
          // qDebug() << "newRel : " << unit.at(0) << unit.at(1) << unit.at(2);
           output->push_back(unit);
         }
     }
   return output;
 }

void MainWindow::UpdateGrupoidy(bool secondary)
{

  ERROR = false;
  qDebug() << "A";
  if(!secondary)
    CHARS_C = GetCharsFromInput(i_charsC->text());
  else CHARS_C = GetCharsFromTable();
  qDebug() << "B";
  if(!secondary)
  TROJICE = GetRelationsFromInput(i_trojice,3);
  else TROJICE = GetTrojiceFromTable();
  jednotkovy = CHARS_C->at(0);
  bool uzavrenost;
  if(!secondary)
    uzavrenost = GrupoidyDoTabulky();
  else uzavrenost =secondaryUzavrenostTest(*CHARS_C,*TROJICE);
qDebug() << "D";
  c_asociativita->setToolTip("");
  c_jednotkovyprvek->setToolTip("");
  c_inverzniprvek->setToolTip("");
  c_komutativita->setToolTip("");

 // return;

  QRgb green = qRgb(0,230,0);
  QRgb red = qRgb(230,0,0);

    ui->c_err_4->setVisible(ERROR);
    ui->c_err_4->setChecked(ERROR);

  c_uzavrenost->setPalette(QPalette(uzavrenost ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_uzavrenost->setChecked(uzavrenost);
  qDebug() << "AS";
  bool asociativita = AsociativitaTest(*CHARS_C,*TROJICE,true);
  qDebug() << "JED";
  bool jednotkovy = JednotkovyTest(*CHARS_C,*TROJICE,true);
  qDebug() << "UZ";
  bool inverzni = jednotkovy && InverzniTest(*CHARS_C,*TROJICE,true);
  if(!jednotkovy)
    {
      ui->invLabel->setText( "<b>Inverzní prvky i(x) :</b><br><span style=\" color:#aa0000;\">NEEXISTUJE JEDNOTKOVY</span>");
    }
  qDebug() << "KOM";
  bool komutativni = KomutativitaTest(*CHARS_C,*TROJICE,true);

  c_asociativita->setPalette(QPalette(asociativita ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_asociativita->setChecked(asociativita);
  c_jednotkovyprvek->setPalette(QPalette(jednotkovy ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_jednotkovyprvek->setChecked(jednotkovy);
  c_inverzniprvek->setPalette(QPalette(inverzni ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_inverzniprvek->setChecked(inverzni);
  c_komutativita->setPalette(QPalette(komutativni ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_komutativita->setChecked(komutativni);



  QString output = "Výsledné struktury :\n";
  if(uzavrenost) output.append(" Grupoid\n");
  if(uzavrenost && asociativita) output.append(" Pologrupa\n");
  if(uzavrenost && asociativita && jednotkovy) output.append(" Monoid\n");
  if(uzavrenost && asociativita && jednotkovy && inverzni) output.append(" Grupa\n");
  if(uzavrenost && !asociativita &&  jednotkovy && inverzni ) output.append(" Lupa\n");
   if(uzavrenost && !asociativita &&  !jednotkovy && inverzni ) output.append(" Kvazigrupa\n");
   if(uzavrenost && asociativita && jednotkovy && inverzni && komutativni) output.append(" Abelova grupa\n");


  qDebug() << "act;";

   ui->vysledekGrup->setText(output);
   if(jednotkovy)
   ui->rady->setText(printRady());
   else ui->rady->setText("<b>Inverzní prvky i(x) :</b><br><span style=\" color:#aa0000;\">NEEXISTUJE JEDNOTKOVY</span>");

   podgrupy =  GetPodgrupy(*CHARS_C,*TROJICE);
   actualizePodgrupyList();
}



vertex* MainWindow::GetVertex(char name,std::vector<vertex>*vec)
{
  for(std::vector<vertex>::iterator it = vec->begin(); it != vec->end(); ++it)
    {
      if((*it).name == name) return &(*it);
    }
  return NULL;
}

void MainWindow::RenderArrow(int x1,int y1,int x2, int y2, QGraphicsScene * scene,bool noArrow)
{
  Vector2 end = Vector2(x2,y2);
  Vector2 back = Vector2(x1,y1) - Vector2(x2,y2);
  back.normalize();
  end = end - (back * -12);
  Vector2 start = Vector2(x1,y1);
  start = start - (back * 12);

  scene->addLine((int)start.x,(int)start.y,(int)end.x,(int)end.y,QPen(QBrush(qRgb(0,0,25)),2.1));

  if(noArrow) return;

  Vector2 firstLine = end - start;
  firstLine.normalize();
  firstLine.rotate(150);
  firstLine = end + firstLine * 12;
  Vector2 secondLine  = end - start;
  secondLine.normalize();
  secondLine.rotate(210);
  secondLine = end + secondLine * 12;





  scene->addLine((int)end.x,(int)end.y,(int)firstLine.x,(int)firstLine.y,QPen(QBrush(qRgb(0,0,25)),2.1));
  scene->addLine((int)end.x,(int)end.y,(int)secondLine.x,(int)secondLine.y,QPen(QBrush(qRgb(0,0,25)),2.1));
  end = end - (back * -4);
  scene->addLine((int)end.x,(int)end.y,(int)firstLine.x,(int)firstLine.y,QPen(QBrush(qRgb(0,0,25)),2.1));
  scene->addLine((int)end.x,(int)end.y,(int)secondLine.x,(int)secondLine.y,QPen(QBrush(qRgb(0,0,25)),2.1));
  end = end - (back * -4);
  scene->addLine((int)end.x,(int)end.y,(int)firstLine.x,(int)firstLine.y,QPen(QBrush(qRgb(0,0,25)),2.1));
  scene->addLine((int)end.x,(int)end.y,(int)secondLine.x,(int)secondLine.y,QPen(QBrush(qRgb(0,0,25)),2.1));
}

void MainWindow::RenderZobrazeni()
{
  zobrazeniScene->clear();



  std::vector<vertex> aVerticies;
  std::vector<vertex> bVerticies;

  int y = -150;
  for(std::vector<char>::iterator it = CHARS_A->begin(); it != CHARS_A->end(); ++it,y+= 30)
    aVerticies.push_back(vertex((*it),50,y));
  y = -150;
  for(std::vector<char>::iterator it = CHARS_B->begin(); it != CHARS_B->end(); ++it,y+= 30)
    bVerticies.push_back(vertex((*it),250,y));

  for(std::vector<vertex>::iterator it = aVerticies.begin(); it != aVerticies.end(); ++it)
    {
      zobrazeniScene->addEllipse((*it).x - 4,(*it).y - 4,8,8,QPen(qRgb(230,150,0)),QBrush(qRgb(230,152,0),Qt::BrushStyle::SolidPattern ));
        zobrazeniScene->addEllipse((*it).x - 4,(*it).y - 4,8,8,QPen(qRgb(0,0,0)));
      QGraphicsTextItem * t = zobrazeniScene->addText(QString((*it).name));
      t->setPos((*it).x - 20,(*it).y-12);
    }
  for(std::vector<vertex>::iterator it = bVerticies.begin(); it != bVerticies.end(); ++it)
    {
      zobrazeniScene->addEllipse((*it).x -4,(*it).y - 4,8,8,QPen(qRgb(230,150,0)),QBrush(qRgb(230,152,0),Qt::BrushStyle::SolidPattern ));
            zobrazeniScene->addEllipse((*it).x -4,(*it).y - 4,8,8,QPen(qRgb(0,0,0)));
      QGraphicsTextItem * t = zobrazeniScene->addText(QString((*it).name));
      t->setPos((*it).x + 10,(*it).y-12);
    }

  for(std::vector<std::vector<char>>::iterator it = ZOBRAZENI->begin(); it != ZOBRAZENI->end(); ++it)
    {
      vertex * A = GetVertex((*it)[0],&aVerticies);
      if(A == NULL) { ERROR = true; return; }
      vertex * B = GetVertex((*it)[1],&bVerticies);
      if(B == NULL) { ERROR = true; return; }

      RenderArrow(A->x,A->y,B->x,B->y,zobrazeniScene);
    }
   zobrazeniScene->update();
}

void MainWindow::UpdateZobrazeni()
{
  ERROR = false;

  CHARS_A = GetCharsFromInput(i_charsA->text());
  CHARS_B = GetCharsFromInput(i_charsB->text());
    ZOBRAZENI = GetRelationsFromInput(i_zobrazeni,2);

  c_funkce->setToolTip("");
  c_injekce->setToolTip("");
  c_surjekce->setToolTip("");
  c_bijekce->setToolTip("");


  QRgb green = qRgb(0,230,0);
  QRgb red = qRgb(230,0,0);

  c_funkce->setPalette(QPalette(functionTest() ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_funkce->setChecked(functionTest());
  c_injekce->setPalette(QPalette(InjekceTest() ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_injekce->setChecked(InjekceTest());
  c_surjekce->setPalette(QPalette(SurjekceTest() ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_surjekce->setChecked(SurjekceTest());
  c_bijekce->setPalette(QPalette(InjekceTest() && SurjekceTest() ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_bijekce->setChecked(InjekceTest() && SurjekceTest());
  if(!c_bijekce->isChecked()) c_bijekce->setToolTip("Bijekce nastává pokud jsou zároveň splněny Injekce a Surjekce");

  c_relace->setPalette(QPalette(!ERROR ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_relace->setChecked(!ERROR);

  RenderZobrazeni();
  ui->c_err_2->setVisible(ERROR);
  ui->c_err_2->setChecked(ERROR);
}

void MainWindow::UpdateRelace()
{
  ERROR = false;
  CHARS = GetCharsFromInput(i_chars->text());


  RELATIONS = GetRelationsFromInput(i_rels,2);

  c_refl->setToolTip("");
  c_irefl->setToolTip("");
  c_sym->setToolTip("");
  c_asym->setToolTip("");
  c_antisym->setToolTip("");
  c_trans->setToolTip("");
  c_ekv->setToolTip("");
  ui->c_uspo->setToolTip("");

  QRgb green = qRgb(0,230,0);
  QRgb red = qRgb(230,0,0);


  c_refl->setPalette(QPalette(TestRelations(RE) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_irefl->setPalette(QPalette(TestRelations(IRE) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_sym->setPalette(QPalette(TestRelations(SY) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_asym->setPalette(QPalette(TestRelations(ASY) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_antisym->setPalette(QPalette(TestRelations(AN) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_trans->setPalette(QPalette(TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_ekv->setPalette(QPalette(TestRelations(RE) && TestRelations(SY) && TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_uspo->setPalette(QPalette(TestRelations(RE) && TestRelations(AN) && TestRelations(TR) ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  c_err->setVisible(ERROR);

  c_refl->setChecked(TestRelations(RE));
  c_irefl->setChecked(TestRelations(IRE));
  c_sym->setChecked(TestRelations(SY));
  c_asym->setChecked(TestRelations(ASY));
  c_antisym->setChecked(TestRelations(AN));
  c_trans->setChecked(TestRelations(TR));
  c_ekv->setChecked(TestRelations(RE) && TestRelations(SY) && TestRelations(TR));
  ui->c_uspo->setChecked(TestRelations(RE) && TestRelations(AN) && TestRelations(TR));
  c_ekv->setToolTip("Ekvivalence nastává tehdy pokud jsou relace zároveň: RE,SY,TRA");
   ui->c_uspo->setToolTip("Relace je uspořádáním, pokud zárověň platí: RE,AN,TRA");
  c_err->setChecked(ERROR);


bool toler = TestRelations(RE) && TestRelations(SY);
bool kvazi = TestRelations(RE) && TestRelations(TR);
bool neostre = TestRelations(RE) && TestRelations(AN) && TestRelations(TR);
bool ostre = TestRelations(IRE) && TestRelations(TR);

  ui-> c_toler->setPalette(QPalette(toler  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
  ui->c_toler->setChecked(toler);
    ui->c_toler->setToolTip("Tolerance nastává tehdy pokud jsou relace zároveň: RE,SY");

    ui-> c_kvazi->setPalette(QPalette(kvazi  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
    ui->c_kvazi->setChecked(kvazi);
      ui->c_kvazi->setToolTip("Kvaziuspořádání nastává tehdy pokud jsou relace zároveň: RE,TRA");

      ui-> c_neostre->setPalette(QPalette(neostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
      ui->c_neostre->setChecked(neostre);
        ui->c_neostre->setToolTip("Neostré usp. nastává tehdy pokud jsou relace zároveň: RE,AN,TRA");

        ui-> c_ostre->setPalette(QPalette(ostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
        ui->c_ostre->setChecked(ostre);
          ui->c_ostre->setToolTip("Ostré usp. nastává tehdy pokud jsou relace zároveň: IRE,TRA");

          ui-> c_poset->setPalette(QPalette(neostre  ? green : red,qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(230,0,0),qRgb(0,0,0),qRgb(128,127,128)));
          ui->c_poset->setChecked(neostre);
            ui->c_poset->setToolTip("Poset nastává tehdy pokud jsou relace zároveň: RE,AN,TRA");

    ui->c_toler_2->setVisible(!ERROR);
  c_refl->setVisible(!ERROR);
  c_irefl->setVisible(!ERROR);
  c_sym->setVisible(!ERROR);
  c_asym->setVisible(!ERROR);
  c_antisym->setVisible(!ERROR);
  c_trans->setVisible(!ERROR);
  c_ekv->setVisible(!ERROR);
  ui->c_uspo->setVisible(!ERROR);

  mainLog.clear();
  mainLog.append(QString("Relace log\n-------------------\n - Vygenerované množiny:\n    - Kartézský součin:\t {"));

  mainLog.append(QString(GenerateRelations(ALL).c_str()));
  mainLog.append(QString("}\n    - RE:\t\t {"));
  mainLog.append(QString(GenerateRelations(RE).c_str()));
  mainLog.append(QString("}"));

  g_log->setPlainText(mainLog);
}

std::vector<std::vector<char>>* MainWindow::GetRelationsFromInput(QLineEdit * ledit,int sizeOfRelations)
{
  // Získá znaky ze stringu
  std::vector< std::vector<char>> * newRels = new std::vector< std::vector<char>>();
  std::string inputText = ledit->text().toStdString();
  string buf;
  stringstream ss(inputText);
  while(std::getline(ss, buf, ','))
    {
      if(buf.size() != sizeOfRelations)
        {
          ERROR = true;
          return newRels;
        }
      std::vector<char> newRelation;

      for(int i = 0; i < buf.size(); i++)
        {
          newRelation.push_back(buf.c_str()[i]);
        }

      if(!vectorContains2(newRelation,newRels))
        newRels->push_back(newRelation);
    }
  return newRels;

}

std::vector<char>* MainWindow::GetCharsFromInput(QString  input)
{
  // Získá znaky ze stringu
  std::vector<char> * newChars = new std::vector<char>();
  std::string inputText = input.toStdString();
  string buf;
  stringstream ss(inputText);
  while(std::getline(ss, buf, ','))
    {
      if(buf.size() > 1 || buf.size() <= 0)
        {
          ERROR = true;
          return newChars;
        }
      if(!vectorContains(buf.c_str()[0],newChars)){
        newChars->push_back(buf.c_str()[0]);

        }
    }
  return newChars;
}

bool MainWindow::SurjekceTest()
{
  // test pokrytí def. oboru
 for(std::vector<char>::iterator it = CHARS_B->begin(); it != CHARS_B->end(); ++it)
   {
     bool found = false;
      for(std::vector<std::vector<char>>::iterator jt = ZOBRAZENI->begin(); jt != ZOBRAZENI->end(); ++jt)
        {
            if((*jt)[1] == (*it)){ found = true; break; }
        }
      if(!found){
          c_surjekce->setToolTip("V oboru hodnot (napravo) existuje nepokrytý prvek.");
          return false;  // existuje napravo  prvek bez relace
        }
   }
    return true;
}

bool MainWindow::InjekceTest()
{
  // test pokrytí def. oboru
 for(std::vector<char>::iterator it = CHARS_A->begin(); it != CHARS_A->end(); ++it)
   {
     bool found = false;
      for(std::vector<std::vector<char>>::iterator jt = ZOBRAZENI->begin(); jt != ZOBRAZENI->end(); ++jt)
        {
            if((*jt)[0] == (*it)){ found = true; break; }
        }
      if(!found){
          c_injekce->setToolTip("V definičním oboru (nalevo) existuje nepokrytý prvek.");
          return false;  // existuje nalevo prvek bez relace
       }
   }

   // test ukazatele na stejný prvek
  for(std::vector<std::vector<char>>::iterator it = ZOBRAZENI->begin(); it != ZOBRAZENI->end(); ++it)
    {
      char A = (*it) [0];
      char B = (*it) [1];
      std::stringstream sucess;
      int bCount = 0;
      for(std::vector<std::vector<char>>::iterator jt = ZOBRAZENI->begin(); jt != ZOBRAZENI->end(); ++jt)
        {
            char _A = (*jt) [0];
            char _B = (*jt) [1];
            if(B == _B) bCount++;
        }
        if(bCount > 1){
            std::stringstream ss;
            ss<<"Na prvek" << B << " jež je v oboru hodnot (napravo) , ukazuje více než jeden prvek z definičního oboru (nalevo). Tudíž se injekce zamítá.";
            c_injekce->setToolTip(ss.str().c_str());
            return false;
          }
    }
    return true;
}

bool MainWindow::functionTest()
{
   for(std::vector<std::vector<char>>::iterator it = ZOBRAZENI->begin(); it != ZOBRAZENI->end(); ++it)
     {
       char A = (*it)[0];
       char B = (*it)[1];
       for(std::vector<std::vector<char>>::iterator jt = ZOBRAZENI->begin(); jt != ZOBRAZENI->end(); ++jt)
         {
           char _A = (*jt)[0];
           char _B = (*jt)[1];
           if(A == _A)
             {
               if(B != _B){
                    std::stringstream ss;
                    ss<< "Tohle zobrazení není funkcí jelikož prvek " << A << " z definičního oboru (nalevo), ukazuje na více prvků v oboru hodnot (napravo).\n Přesněji na prvky " << B << " a " << _B << ".";
                    c_funkce->setToolTip(ss.str().c_str());
                   return false;
                    }
                }
             }
     }
   return true;
}

bool MainWindow:: ReflectionTest(char _A,char _B)
{
	return _A == _B;
}

bool MainWindow:: AntisymetryTest()
{

    // Antisymtry test
    for(std::vector<vector<char>>::iterator it = RELATIONS->begin(); it != RELATIONS->end(); ++it)
      {
        char A = (*it)[0];
        char B = (*it)[1];

        for(std::vector<vector<char>>::iterator jt = RELATIONS->begin(); jt != RELATIONS->end(); ++jt)
          {
            char _A = (*jt)[0];
            char _B = (*jt)[1];

            if(A == _B  && B == _A && A != B){
                std::stringstream ss; ss << "Antisimetrie nesplněna pro relace :\n (" << A << "," << B << ") a (" << _A << "," << _B << ").";
                ss <<"\nJelikož (" << A << " je v relaci s " << _B << ")  ^ (" << B << " je v relaci s "  << _A << ") \n";
                ss << " tudíž pro splnění antisymetrie by se měli " << A << " a " << B << " rovnat.";
                c_antisym->setToolTip(QString(ss.str().c_str()));
                return false;
              }
          }
      }
    return true;
}

bool MainWindow:: arrayContains(char ch, char * arr)
{
	for(int i = 0; i < (sizeof(arr)/sizeof(*arr)) ; i++)
		if(arr[i] == ch) return true;
	return false;
}

bool MainWindow:: vectorContains2(std::vector<char> ch,std::vector< vector<char>>* vec)
{
	for(std::vector< vector<char>>::iterator it = vec->begin(); it != vec->end(); ++it)
		if((*it) == ch) return true;
	return false;
}

bool MainWindow:: vectorContains(char ch, vector<char> *vec)
{
	for(vector<char>::iterator it = vec->begin(); it != vec->end(); ++it)
		if((*it) == ch) return true;
	return false;
}

bool MainWindow:: TransitivityTest()
{
	for(vector<vector<char>>::iterator it = RELATIONS->begin(); it != RELATIONS->end(); ++it)
	{
		char A = (*it)[0];    //1
		char B = (*it)[1];    //2
		vector<vector<char>>* all_b_related = new vector<vector<char>>();

		for(vector<vector<char>>::iterator jt = RELATIONS->begin(); jt != RELATIONS->end(); ++jt)
		{
		    if((*jt)[0] == B) all_b_related->push_back((*jt));	//2,3
		}
		bool found = true;
		for(vector<vector<char>>::iterator kt = all_b_related->begin(); kt != all_b_related->end(); ++kt)
		  {
		    if(!FindRelation(A,(*kt)[1]))
		    {
			std::stringstream ss; ss << "Transitivita nesplněna protože nebylo nalezeno : \n" <<  "(" << A << ";" << B << ")" << " ^ " << "(" << (*kt)[0] << ";" << (*kt)[1] << ")  -> (" << A << ";" << (*kt)[1] << ")";
			c_trans->setToolTip(QString(ss.str().c_str()));
			found = false;
			break;
		    }
		  }
		if(!found) return false;
	}
	return true;
}

bool MainWindow::SymetryTest(bool Asymetry)
{
  std::stringstream sucess;
  for(std::vector<std::vector<char>>::iterator it = RELATIONS->begin(); it != RELATIONS->end(); ++it)
    {
      char A = (*it)[0];
      char B = (*it)[1];

      if(!FindRelation(B,A) && !Asymetry){
         std::stringstream ss; ss << "Symetrie nesplněna protože nebyla nalezena relace (" << B << "," << A << ") k původní relaci (" << A << "," << B << ").";
         c_sym->setToolTip(QString(ss.str().c_str()));
          return false;}
      else
          sucess << "(" << A << "," << B << ") -> (" << B << "," << A << ")\n";
      if(FindRelation(B,A) && Asymetry){
          std::stringstream ss; ss << "Asymetrie  nesplněna protože byla nalezena relace (" << B << "," << A << ") k původní relaci (" << A << "," << B << ").";
          c_asym->setToolTip(QString(ss.str().c_str()));
          return false;
        }
    }
  if(!Asymetry)
    {
      std::stringstream ss; ss << "Symetrie splněna díky všem dvojcím :\n" << sucess.str();
      c_sym->setToolTip(QString(ss.str().c_str()));
    }
  else
    {
      c_asym->setToolTip("Asymetrie splněna jelikož neexistuje ");
    }
  return true;
}



bool MainWindow::FindRelation(char _A,char _B)
{
  for(std::vector<std::vector<char>>::iterator it = RELATIONS->begin(); it != RELATIONS->end(); ++it)
  {
      if(_A == (*it)[0] && _B == (*it)[1]) return true;
   }
  return false;
}


bool MainWindow::TestRelations(generationPreset preset)
{
  vector<vector<char>> AN_MEMORY;

  if(preset == TR) return TransitivityTest();
  if(preset == SY) return SymetryTest(false);
  if(preset == ASY) return SymetryTest(true);
  if(preset == AN) return AntisymetryTest();

  std::stringstream succesRefl;
  int RE_num = 0;
   for(std::vector<std::vector<char>>::iterator it = RELATIONS->begin(); it != RELATIONS->end(); ++it)
     {
        char A = (*it)[0];
        char B = (*it)[1];

        switch(preset)
        {
                case RE:
                case IRE:
                        if(ReflectionTest(A,B)){
                          RE_num++;
                          succesRefl << "(" <<A << "," << B << ")";
                          }
                        break;
                default:break;
	}
     }
   if(preset == RE && RE_num == CHARS->size())
      return true;
   else if(preset == RE){
       std::stringstream unsucessfull;
       for(int i = 0; i < CHARS->size(); i++)
         {
           std::vector<char> tested;
           tested.push_back(CHARS->at(i));
           tested.push_back(CHARS->at(i));
           if(!vectorContains2(tested,RELATIONS)) unsucessfull << "(" << CHARS->at(i) << "," << CHARS->at(i) << ")";
         }
       std::stringstream ss; ss << "Reflexivita nesplněna jelikož chybí relace :" << unsucessfull.str();
       c_refl->setToolTip(QString(ss.str().c_str()));
       return false;
     }
   if(preset == IRE && RE_num == 0) return true;
   else if (preset == IRE){
       std::stringstream ss; ss << "Ireflexivita nesplněna kvůli přítomnosti relací :" << succesRefl.str();
       c_irefl->setToolTip(QString(ss.str().c_str()));
       return false;
     }
   return true;
}

string MainWindow:: GenerateRelations(generationPreset preset)
{
	stringstream output;
	vector<vector<char>> AN_MEMORY;
	bool first = true;

	for(int i = 0; i < CHARS->size() ; i++)
	{
		for(int j = 0; j < CHARS->size() ; j++)
		{
			char A = CHARS->at(i);
			char B = CHARS->at(j);
			bool belongs = false;
			switch(preset)
			{
				case RE:
					belongs = ReflectionTest(A,B);
					break;
				default:
					belongs = true;
			}
			if(belongs && !first) output <<  "," << A << B;
			else if (belongs && first)
			  {	output << A << B;
				 first = false;
			  }
		}
	}	
	if(preset == ALL || preset == RE || preset == AN) return output.str();

	// SY, AN, TRA
	 return output.str();
}


void MainWindow::btnPushed()
{
  switch(tabWidget->currentIndex())
    {
      case 0:  UpdateRelace(); break;
    case 1: UpdateZobrazeni(); break;
    case 2: UpdateGrafy(); break;
case 3: UpdateUsporadani(); break;
    case 4: UpdateGrupoidy(false);break;
    default: UpdateRelace(); break;
    }
}

void MainWindow::secondaryBtnPushed()
{
  UpdateGrupoidy(true);
}

void MainWindow::RenderGraph(QGraphicsScene *scene, std::vector<vertex*> *verticies)
{
  scene->clear();
  selected = NULL;
  // render ranks
  if(tabWidget->currentIndex() == 3)
    {
      QPoint z;
      QPointF p;
      z = usporadaniView->mapFromGlobal(QCursor::pos());
      p = usporadaniView->mapToScene(z);
      int i = 0;
      for(int y =280; y >= 0; y-= 40,i++)
        {
           scene->addLine(-500,y,500,y,QPen(qRgb(150,150,150)));
          if(p.y() <= y && p.y() > y-40){

                  selectedRank = i;
            }
        }
      if( addingVertexMode)
      scene->addRect(-500,280 - ((selectedRank+1)*40),1200,40,QPen(qRgb(230,245,230)),QBrush(qRgb(230,245,230),Qt::SolidPattern));
    }


  for(std::vector<vertex*>::iterator it = verticies->begin(); it != verticies->end(); ++it)
    {
      QRgb color = qRgb(250,220,0);
      if(vectorContains3((*it),core)) color = qRgb(250,100,0);
      if(tabWidget->currentIndex() == 3)
        color = qRgb(0,220,250);



      if(addingEdgeMode || deletingVertexMode)
        {
          QPoint z;
          QPointF p;
          if(tabWidget->currentIndex() == 2)
            {
              z = grafView->mapFromGlobal(QCursor::pos());
              p = grafView->mapToScene(z);
            }
          else
            {
              z = usporadaniView->mapFromGlobal(QCursor::pos());
              p = usporadaniView->mapToScene(z);
            }

          if(p.x() >= (*it)->x - 12 && p.x() <= (*it)->x + 12 && p.y() >= (*it)->y - 12 && p.y() <= (*it)->y + 12
              ){
              if((edgeA != NULL && !vectorContains3((*it),edgeA->derived)) || edgeA == NULL)
                {
                  if(tabWidget->currentIndex() == 3)
                    {
                      if((*it) != edgeA)
                        {
                          if(edgeA != NULL)
                            {
                              if((*it)->rank > edgeA->rank){
                                color = qRgb(80,250,0);
                                    selected = (*it);
                                }
                            }
                          else
                            {
                              color = qRgb(80,250,0);
                                  selected = (*it);
                            }
                        }
                    }
                  else
                    {
                      color = qRgb(80,250,0);
                      selected = (*it);
                    }

                }
            }
        }
      else
        {
          QPoint z;
          QPointF p;
          if(tabWidget->currentIndex() == 2)
            {
              z = grafView->mapFromGlobal(QCursor::pos());
              p = grafView->mapToScene(z);
            }
          else
            {
              z = usporadaniView->mapFromGlobal(QCursor::pos());
              p = usporadaniView->mapToScene(z);
            }
          if(p.x() >= (*it)->x - 12 && p.x() <= (*it)->x + 12 && p.y() >= (*it)->y - 12 && p.y() <= (*it)->y + 12
              ) selected = (*it);

        }

        scene->addEllipse((*it)->x - 12,(*it)->y - 12,24,24,QPen(color),QBrush(color,Qt::BrushStyle::SolidPattern ));
        scene->addEllipse((*it)->x - 12,(*it)->y - 12,24,24,QPen(qRgb(0,0,0)));
        QString txt = (*it)->name;
        int xpos = 9;
        if(((*it)->name >= 'a' && tabWidget->currentIndex() == 2 && (ui->comboBox->currentIndex() == 1 || ui->comboBox->currentIndex() == 2))
           || ((*it)->name >= 'a' && tabWidget->currentIndex() == 3 && (ui->comboBox_2->currentIndex() == 1 || ui->comboBox_2->currentIndex() == 2) ))
          {
            std::stringstream ss;
            ss << 10 + ((*it)->name - 'a');
            txt = QString(ss.str().c_str());
            xpos = 14;
          }
        QGraphicsTextItem * t = scene->addText(QString(txt),QFont("Verdana",12,12,false));
        t->setPos((*it)->x - xpos,(*it)->y-14);


        for(std::vector<vertex*>::iterator jt = (*it)->derived.begin(); jt != (*it)->derived.end(); ++jt )
          {
            if((*it) == (*jt))RenderArrow((*it)->x-20,(*it)->y-20,(*jt)->x+5,(*jt)->y+5,scene);
            else if(tabWidget->currentIndex() == 3 && (*jt) != (*it))
              RenderArrow((*it)->x,(*it)->y,(*jt)->x,(*jt)->y,scene);//,tabWidget->currentIndex() == 3);
             else
                 RenderArrow((*it)->x,(*it)->y,(*jt)->x,(*jt)->y,scene);//,tabWidget->currentIndex() == 3);
          }


    }
  if(addingEdgeMode && edgeA != NULL)
    {
      QPoint z;
      QPointF p;
      if(tabWidget->currentIndex() == 2)
        {
          z = grafView->mapFromGlobal(QCursor::pos());
          p = grafView->mapToScene(z);
        }
      else
        {
          z = usporadaniView->mapFromGlobal(QCursor::pos());
          p = usporadaniView->mapToScene(z);
        }

      RenderArrow(edgeA->x,edgeA->y,p.x(),p.y(),scene);//,tabWidget->currentIndex() == 3);
    }
   if(addingVertexMode)
     {
       QPoint z;
       QPointF p;
       if(tabWidget->currentIndex() == 2)
         {
           z = grafView->mapFromGlobal(QCursor::pos());
           p = grafView->mapToScene(z);
         }
       else
         {
           z = usporadaniView->mapFromGlobal(QCursor::pos());
           p = usporadaniView->mapToScene(z);
         }


       addingVertex->x = p.x();
       addingVertex->y = p.y();

       QRgb color = qRgba(250,220,0,128);
       if(tabWidget->currentIndex() == 3) color = qRgba(0,220,250,128);
       scene->addEllipse(addingVertex->x - 12,addingVertex->y - 12,24,24,QPen(color),QBrush(color,Qt::BrushStyle::SolidPattern ));
       scene->addEllipse(addingVertex->x - 12,addingVertex->y - 12,24,24,QPen(qRgba(0,0,0,128)));


       QString txt = addingVertex->name;
       int xpos = 9;
       if((addingVertex->name >= 'a' && tabWidget->currentIndex() == 2 && (ui->comboBox->currentIndex() == 1 || ui->comboBox->currentIndex() == 2))
           || (addingVertex->name >= 'a' && tabWidget->currentIndex() == 3 && (ui->comboBox_2->currentIndex() == 1 || ui->comboBox_2->currentIndex() == 2) ))
         {
           std::stringstream ss;
           ss << 10 + (addingVertex->name - 'a');
           txt = QString(ss.str().c_str());
           xpos = 14;
         }

       QGraphicsTextItem * t = scene->addText(txt,QFont("Verdana",12,12,false));
       t->setPos(addingVertex->x - xpos,addingVertex->y-14);

     }


}

void MainWindow::StopPlacing()
{
  addingVertexMode = false;
  addingEdgeMode = false;
}

void MainWindow::PlaceVertex()
{
  if(deletingVertexMode)
    {
      DeleteActiveVertex();
      deletingVertexMode = false;
      return;
    }

  if(addingEdgeMode)
    {

      if(edgeA == NULL && selected != NULL){ edgeA = selected; return;}
      if(edgeB == NULL && selected != NULL)
        {
          edgeA->derived.push_back(selected);
          selected->inherited.push_back(edgeA);


          addingEdgeMode = false;
          AddEdge();
          return;
        }
    }

  if(!addingVertexMode) return;
  if(selected != NULL) return;
  if( tabWidget->currentIndex() == 2)
   {
      addingVertex->index = currentIndex;
      graphVerticies.push_back(addingVertex);
      currentIndex++;
    }
  else
    {
      addingVertex->index = currentIndex2;
      usporadaniVerticies.push_back(addingVertex);
      currentIndex2++;
      addingVertex->rank = selectedRank;
    }

    addingVertexMode = false;
  AddVertex();
}

void MainWindow::AddVertex()
{
  addingEdgeMode = false;
  deletingVertexMode = false;
  if(addingVertexMode) return;

QString inputstring = QString(abeceda.c_str());
QComboBox *tested = ui->comboBox;
if(tabWidget->currentIndex() == 3) tested = ui->comboBox_2;
if(tested->currentIndex() == 1) inputstring = QString(cisla.c_str());
if(tested->currentIndex() == 2) inputstring = QString(cisla0.c_str());
if(tested->currentIndex() == 3 && tabWidget->currentIndex() == 2) inputstring = i_vertex->text();
if(tested->currentIndex() == 3 && tabWidget->currentIndex() == 3) inputstring = ui->i_vertex_2->text();

if( tabWidget->currentIndex() == 2)
 {
    CHARS_VERTEX = GetCharsFromInput(inputstring);

    if(currentIndex >= CHARS_VERTEX->size()){StopPlacing(); return; }
    addingVertex = new vertex(CHARS_VERTEX->at(currentIndex),0,0);
  }
else
  {
    CHARS_VERTEX2 = GetCharsFromInput(inputstring);

    if(currentIndex2 >= CHARS_VERTEX2->size()){StopPlacing(); return; }
    addingVertex = new vertex(CHARS_VERTEX2->at(currentIndex2),0,0);
  }

  addingVertexMode = true;
}

void MainWindow::DeleteVertex()
{
  addingVertexMode = false;
  addingEdgeMode = false;
  if(deletingVertexMode) return;

  edgeA = NULL;
  edgeB = NULL;
  deletingVertexMode = true;
}

void MainWindow::AddEdge()
{
    addingVertexMode = false;
    deletingVertexMode = false;
    if(addingEdgeMode) return;

    edgeA = NULL;
    edgeB = NULL;
    addingEdgeMode = true;

}

void MainWindow::RenderUpdate()
{
  if(tabWidget->currentIndex() < 2 && tabWidget->currentIndex() > 3 ) return;
  ui->addVertex->setCheckable(addingVertexMode);
  ui->addVertex->setChecked(addingVertexMode);
    ui->addVertex->update();

    ui->addEdge->setCheckable(addingEdgeMode);
    ui->addEdge->setChecked(addingEdgeMode);
      ui->addEdge->update();

      ui->addVertex_2->setCheckable(addingVertexMode);
      ui->addVertex_2->setChecked(addingVertexMode);
        ui->addVertex_2->update();

        ui->addEdge_2->setCheckable(addingEdgeMode);
        ui->addEdge_2->setChecked(addingEdgeMode);
          ui->addEdge_2->update();


 if(tabWidget->currentIndex() == 2)
    RenderGraph(grafScene,&graphVerticies);
 else RenderGraph(usporadaniScene,&usporadaniVerticies);
  renderUpdateTimer->start(5);
}

void MainWindow::Reset()
{
   if(tabWidget->currentIndex() == 2)
     {
         currentIndex = 0;
          graphVerticies.clear();
     }
 else
     {
         currentIndex2 = 0;
          usporadaniVerticies.clear();
     }
  StopPlacing();
  RenderUpdate();
  ui->updateBTN_8->setEnabled(true);
}

bool MainWindow::vectorContains3(vertex * v, std::vector<vertex*> vs)
{
  for(std::vector<vertex*>::iterator it = vs.begin(); it != vs.end(); ++it)
    {
      if((*it)->name == v->name) return true;
    }
  return false;
}

void MainWindow::forceStopPlacing()
{
  StopPlacing();
}

void MainWindow::DeleteActiveVertex()
{
  vertex * toDelete = selected;
  std::vector<vertex*> deleteFrom;
  if(tabWidget->currentIndex() == 3)
     deleteFrom = usporadaniVerticies;
  else if(tabWidget->currentIndex() == 2)
    deleteFrom = graphVerticies;
  else return;

  if(toDelete == NULL) return;

  for(std::vector<vertex*>::iterator it  = deleteFrom.begin(); it!=deleteFrom.end(); ++it)
    {
      std::vector<vertex*> newInherited;
      for(int i = (*it)->inherited.size()-1; i >= 0; i--)
        {
          if(!((*it)->inherited.at(i)->name == toDelete->name))
            newInherited.push_back((*it)->inherited.at(i));
        }
      std::vector<vertex*> newDerived;
      for(int i = (*it)->derived.size()-1; i >= 0; i--)
        {
          if(!((*it)->derived.at(i)->name == toDelete->name))
            newDerived.push_back((*it)->derived.at(i));
        }
    }
  std::vector<vertex*> newVerticies;
  for(int i = deleteFrom.size()-1; i >= 0; i--)
    {
      if(!(deleteFrom.at(i)->name == toDelete->name))
        newVerticies.push_back(deleteFrom.at(i));
    }
  if(tabWidget->currentIndex() == 3)
     usporadaniVerticies = deleteFrom;
  else if(tabWidget->currentIndex() == 2)
     graphVerticies = deleteFrom;
}

void MainWindow::ListSelectionChange()
{
  // clear
  for (int y = 1; y<11; y++)
    {
      for(int x= 1; x < 11; x++)
        {
          table->item(y,x)->setBackgroundColor(qRgb(255,255,255));
        }
    }
  int index = ui->listWidget->currentRow()-1;
  if(ui->listWidget->currentRow() > 0)
    {
      for(std::vector<cell*>::iterator it = podgrupy.at(index)->cells.begin(); it != podgrupy.at(index)->cells.end(); ++it)
        {
          table->item((*it)->y,(*it)->x)->setBackgroundColor(qRgb(255,120,120));
        }
    }
}

Vector2 toLowestZlom(Vector2 input)
{
  int testedDivisor = input.x > input.y ? input.x : input.y;
  for(;testedDivisor >= 1;testedDivisor--)
    {
      if(((int)input.x) % testedDivisor == 0 && ((int)input.y) % testedDivisor == 0)
        break;
    }
   return Vector2(input.x / testedDivisor, input.y / testedDivisor);
}

void MainWindow::UpdateSupport()
{
  determineUsedIs(ui->asTable);
  float mi = std::stof(ui->miEdit->text().toStdString());

  std::vector<int> allCheckedIs;
  if(ui->sup_i_1->isChecked()) allCheckedIs.push_back(1);
  if(ui->sup_i_2->isChecked()) allCheckedIs.push_back(2);
  if(ui->sup_i_3->isChecked()) allCheckedIs.push_back(3);
  if(ui->sup_i_4->isChecked()) allCheckedIs.push_back(4);
  if(ui->sup_i_5->isChecked()) allCheckedIs.push_back(5);
  if(ui->sup_i_6->isChecked()) allCheckedIs.push_back(6);
  if(ui->sup_i_7->isChecked()) allCheckedIs.push_back(7);
  if(ui->sup_i_8->isChecked()) allCheckedIs.push_back(8);
  if(ui->sup_i_9->isChecked()) allCheckedIs.push_back(9);
  if(ui->sup_i_10->isChecked()) allCheckedIs.push_back(10);
  if(ui->sup_i_11->isChecked()) allCheckedIs.push_back(11);

  float unionSum = 0;
  for(int y = 0; y < asRowCount; y++)
    {
      bool addToSupport = true;
      for(int i = 0; i < allCheckedIs.size(); i++)
      {
        int x = allCheckedIs.at(i)-1;
        if(ui->asTable->item(y,x)->text() == "0") addToSupport = false;
       }
      if(addToSupport) unionSum++;
    }
  QRgb col = qRgb(0,210,0);
  if(unionSum / (asRowCount*1.0) > mi) col = qRgb(210,0,0);

  std::stringstream ss;

  Vector2 result = toLowestZlom(Vector2(unionSum,asRowCount));
  ss << "Výsledek = " << result.x << "/" << result.y;
  ui->supResLabel->setText(QString(ss.str().c_str()));


  if((unionSum*1.0) / (asRowCount*1.0) < std::stof(ui->miEdit->text().toStdString()))
    ui->removedSup->setVisible(true);
  else ui->removedSup->setVisible(false);
}

void MainWindow::UpdateConfidence()
{
  determineUsedIs(ui->asTable);
  int Asum = 0;
  int Bsum= 0;
  int Csum= 0;
  int Dsum= 0;

  for(int y = 0; y < asRowCount; y++)
    {
        bool A = ui->asTable->item(y,ui->confL_list->currentIndex().row())->text() == "1";
        bool B = ui->asTable->item(y,ui->confR_list->currentIndex().row())->text() == "1";
        if(A && B) Asum++;
        if(A && !B) Bsum++;
        if(!A && B) Csum++;
        if(!A && !B) Dsum++;
    }
  ui->debugTabl->item(0,0)->setText(QString(std::to_string(Asum).c_str()));
  ui->debugTabl->item(0,1)->setText(QString(std::to_string(Bsum).c_str()));
  ui->debugTabl->item(1,0)->setText(QString(std::to_string(Csum).c_str()));
  ui->debugTabl->item(1,1)->setText(QString(std::to_string(Dsum).c_str()));


  std::stringstream ss;
  qDebug() << Asum << " / (" << Asum << " + " << Bsum << ")";
  Vector2 result = toLowestZlom(Vector2(Asum,Asum+Bsum));
  ss << "Výsledek = " << result.x << "/" << result.y;
  ui->confResLabel->setText(QString(ss.str().c_str()));


  if((Asum*1.0) / (Asum*1.0 + Bsum*1.0) < std::stof(ui->miEdit->text().toStdString()))
    ui->removedConf->setVisible(true);
  else ui->removedConf->setVisible(false);
}





void MainWindow::determineUsedIs(QTableWidget* tab)
{
  IsCount = 0;
  asRowCount = 0;
  for(int i = 0; i < tab->columnCount(); i++)
{
  if( tab->item(0,i)->text() == "0" || tab->item(0,i)->text() == "1")
    IsCount++;
}
  for(int i = 0; i < tab->rowCount(); i++)
{
  if( tab->item(i,0)->text() == "0" || tab->item(i,0)->text() == "1")
    asRowCount++;
}
qDebug() << "Used cols = " << IsCount << "   " << " used rows = " << asRowCount;
}

std::vector<int> cloneVector(std::vector<int> * original)
{
  std::vector<int> output;
  for(std::vector<int>::iterator it = original->begin(); it != original->end(); ++it)
    output.push_back((*it));
  return output;
}

bool vector5Contains(std::vector<int> vec, int what)
{
  for(std::vector<int>::iterator it = vec.begin(); it != vec.end(); ++it)
    {
      if((*it) == what) return true;
    }
  return false;
}

std::vector<int> MainWindow::getPrunik(std::vector<int> A, int i)
{
  std::vector<int> output;
  for(std::vector<int>::iterator it = A.begin(); it != A.end(); ++it)
  {
      if((*it) < i) output.push_back((*it));
  }
  if(!vector5Contains(output,i)) output.push_back(i);
  return output;
}

std::vector<int> MainWindow::getUzaver(std::vector<int> prunik, int i)
{
  std::vector<int> output;
  std::vector<int> progressed1;
  for(int y = 0; y < asRowCount; y++)
    {
      bool unionOk = true;
     for(std::vector<int>::iterator it = prunik.begin(); it != prunik.end(); ++it)
       {
        if(ui->binTableClosure->item(y,(*it)-1)->text() == "0") unionOk = false;
       }
      if (unionOk){ progressed1.push_back(y);  }
    }
  for(int x = 0;x < IsCount; x++)
    {
      bool unionOk = true;
      for(std::vector<int>::iterator it = progressed1.begin(); it != progressed1.end(); ++it)
        {
           if(ui->binTableClosure->item((*it),x)->text() == "0") { unionOk = false; break; }
        }
      if(unionOk) output.push_back(x+1);
    }
/*  for(std::vector<int>::iterator it = prunik.begin(); it != prunik.end(); ++it)
    {
      if(!vector5Contains(output,(*it))) output.push_back((*it));
    }
*/
  return output;
}
std::vector<int>MainWindow::getBwithoutA(std::vector<int> uzaver,std::vector<int> A)
{
  std::vector<int> output;
  for(std::vector<int>::iterator it = uzaver.begin(); it != uzaver.end(); ++it)
    {
      bool contains = false;
      for(std::vector<int>::iterator jt = A.begin(); jt != A.end(); ++jt)
        {
          if((*it) == (*jt)) contains = true;
        }
      if(!contains) output.push_back((*it));
    }
  return output;
}
bool MainWindow::bEqualsItest(std::vector<int>B, int i)
{
  for(std::vector<int>::iterator it = B.begin(); it != B.end(); ++it)
    {
      if((*it) < i) return false;
    }
  return true;
}

QString intVectorToString(std::vector<int> input)
{
  std::stringstream ss;
  ss<<"{";
  for(int i = 0; i < input.size(); i++)
    {
      ss << input.at(i);
      if(i != input.size() - 1) ss << ",";
    }
  ss<<"}";
   return QString(ss.str().c_str());
}
struct closureRow
{
public:
  std::vector<int> A;
  int i;
  std::vector<int> prunik;
  std::vector<int> uzaver;
  std::vector<int> bWithoutA;
  bool ba_equalsI;
};
void MainWindow::NextClosure()
{
  determineUsedIs(ui->binTableClosure);

  ui->ieLog->clear();
//  <font size=\"2\"><b>Řády prvků p(x) :</b><br>
  //<span style=\"color:#aa0000;\">∞</span>
  ui->ieLog->append("<font size=\"3\"><b><u>EXTENTY :</u></b>");
  std::vector<tentHolder> extents = getExtents();
  ui->ieLog->append("<br><font size=\"3\"><b><u>INTENTY :</u></b>");
  std::vector<tentHolder> intents = getIntents(extents);



  //ui->closureTable->clear();
  ui->closureTable->setRowCount(0);
qDebug() << "ok";


  std::vector<closureRow> closureProcess;

  std::vector<int> workingA;
  int workingI = IsCount;
  int s = 0;
  while(true)
    {
      if(workingA.size() >= IsCount){
          closureRow endRow;
          endRow.A = cloneVector(&workingA);
          endRow.ba_equalsI = true;
          closureProcess.push_back(endRow);
          break;
        }

      closureRow newRow;
      newRow.A = cloneVector(&workingA);
      newRow.i = workingI;
      newRow.prunik = getPrunik(workingA,workingI);
      newRow.uzaver = getUzaver(newRow.prunik,workingI);
      newRow.bWithoutA = getBwithoutA(newRow.uzaver,workingA);
      newRow.ba_equalsI = bEqualsItest(newRow.bWithoutA,workingI);

      closureProcess.push_back(newRow);

      if(newRow.ba_equalsI)
        {
          workingA = cloneVector(&newRow.uzaver);
          for(int i = IsCount; i > 0; i--)
            {
              if(!vector5Contains(workingA,i)){ workingI = i; break; }
            }
        }
      else {
          for(int i = workingI-1; i > 0; i--)
            {
              if(!vector5Contains(workingA,i)){ workingI = i; break;}
            }
        }

      s++;
    }

  // print into table
  for(std::vector<closureRow>::iterator it = closureProcess.begin(); it != closureProcess.end(); ++it)
    {
      // add row
      ui->closureTable->setRowCount(ui->closureTable->rowCount()+1);
      bool equaled = false;
      for(int x = 0; x < 6; x++)
        {
          QString textToAdd;
          switch(x)
            {
              case 0 : textToAdd = intVectorToString((*it).A); break;
            case 1 : textToAdd = QString(std::to_string((*it).i).c_str()); break;
              case 2 : textToAdd = intVectorToString((*it).prunik); break;
              case 3 : textToAdd = intVectorToString((*it).uzaver); break;
              case 4 : textToAdd = intVectorToString((*it).bWithoutA); break;
            default: if((*it).ba_equalsI){
                  equaled = true;
                  textToAdd = "✓";
                }
                               else textToAdd = "×"; break;

            }

         ui->closureTable->setItem(ui->closureTable->rowCount()-1,x,new QTableWidgetItem(textToAdd));

        }
      if(equaled)
        {
          for(int x = 0; x < 6; x++)
              ui->closureTable->item(ui->closureTable->rowCount()-1,x)->setBackgroundColor(qRgb(210,245,210));

        }
    }
 int intent = 1;
  for(int i = closureProcess.size()-1; i >= 0; i --)
    {
      if(closureProcess.at(i).ba_equalsI)
        {
          stringstream ss;
          ss << "i" << intent;
          ui->closureTable->setVerticalHeaderItem(i,new QTableWidgetItem(ss.str().c_str()));
          if(intent == 1) {ui->closureTable->item(i,1)->setText(""); ui->closureTable->item(i,2)->setText(""); ui->closureTable->item(i,3)->setText(""); ui->closureTable->item(i,4)->setText(""); }
          intent++;
        }
      else ui->closureTable->setVerticalHeaderItem(i,new QTableWidgetItem(""));
    }

}


std::vector<tentHolder> MainWindow::getExtents()
{
 std::vector<tentHolder> output;
  for(int x = 0; x < 10; x++)
    {
      std::stringstream ss;
      tentHolder newHolder = tentHolder(cols,rows,'e','x','y',x+1);
      newHolder.preReq.push_back(x+1);
      ss<< "<b><br><font size=\"2\">e"<< x+1 <<" = </b>" <<"y" << x+1 << "<font size=\"4\">↓" << "<font size=\"2\"> = <span style=\" color:#005500;\">";
      if(ui->binTableClosure->item(0,x)->text() != "")
      {
          for(int y = 0; y < 10; y++)
            {
              if(ui->binTableClosure->item(y,x)->text() == "1")
              {
                   ss << "x" << y+1 << ",";
                   newHolder.result.push_back(y+1);
               }

            }
             ui->ieLog->append(ss.str().substr(0,ss.str().size()-1).c_str());
             output.push_back(newHolder);
        }
        else break;

    }
//  for(int i = 0; i < 2; i++)
//    {
      std::vector<tentHolder> unions = getUnions(output);
      for(std::vector<tentHolder>::iterator it = unions.begin(); it!= unions.end(); ++it)
        output.push_back((*it));
//    }

  return output;
}

bool duplicityTest(std::vector<tentHolder> vec, tentHolder hold)
{
  for(std::vector<tentHolder>::iterator it = vec.begin(); it != vec.end(); ++it)
    {

      bool same = true;
      for(std::vector<int>::iterator jt = hold.result.begin(); jt!= hold.result.end(); ++jt)
        {
          if(!vector5Contains((*it).result,(*jt))) { same = false; break; }
        }
      if((*it).result.size() != hold.result.size()) same = false;
      if(same) return true;
    }
  return false;
}

std::vector<tentHolder> MainWindow::getUnions(std::vector<tentHolder> extents)
{
  std::vector<tentHolder> output;
  std::vector<tentHolder*> ptrs;
  for(std::vector<tentHolder>::iterator it = extents.begin(); it != extents.end(); ++it)
    ptrs.push_back(&(*it));
 std::vector<std::vector<tentHolder>> allMixed;
  do
  {
     if(2!=ptrs.size()){
       std::vector<tentHolder> newMix;
       for (int i = 0; i < 2; ++i){
           newMix.push_back(*ptrs.at(i));
         }
       allMixed.push_back(newMix);
      }
  }
  while(next_combination(ptrs.begin(),ptrs.begin() + 2,ptrs.end()));

  int i = extents.size()+1;
  for(std::vector<std::vector<tentHolder>>::iterator it = allMixed.begin(); it != allMixed.end(); ++it)
    {
      tentHolder a = (*it)[0];
      tentHolder b = (*it)[1];
     std::stringstream ss;
     tentHolder bigger = a;
     tentHolder lesser = b;
     tentHolder newHolder = tentHolder(cols,rows,'e','x','y',i);
      if(b.result.size()>a.result.size()){
          lesser = a;
          bigger = b;
        }
      ss<< "<b><br><font size=\"2\">e"<< i<<" = </b>" <<"e" << a.mainIndex << " ∩ e" << b.mainIndex << " = "<< "<font size=\"2\"> = <span style=\" color:#005500;\">";
      for(std::vector<int>::iterator jt = bigger.result.begin(); jt != bigger.result.end(); ++jt)
        {
          for(std::vector<int>::iterator kt = lesser.result.begin(); kt!=lesser.result.end(); ++kt)
            {
              if((*kt) == (*jt))
                {
                   ss << "x" << (*jt) << ",";
                    newHolder.result.push_back((*jt));
                }
            }
        }
      if(!duplicityTest(output,newHolder) && !duplicityTest(extents,newHolder)){
        ui->ieLog->append(ss.str().substr(0,ss.str().size()-1).c_str());
        output.push_back(newHolder);
        i++;
        }
    }

  std::vector<tentHolder> outputMem = output;
 for(std::vector<tentHolder>::iterator it = extents.begin(); it != extents.end(); ++it)
   {
     for(std::vector<tentHolder>::iterator jt = outputMem.begin();  jt != outputMem.end(); ++jt)
       {
         std::stringstream ss;
         tentHolder newHolder = tentHolder(cols,rows,'e','x','y',i);
         ss<< "<b><br><font size=\"2\">e"<< i<<" = </b>" <<"e" << (*it).mainIndex << " ∩ e" << (*jt).mainIndex << " = "<< "<font size=\"2\"> = <span style=\" color:#005500;\">";
         for(std::vector<int>::iterator xt = (*it).result.begin(); xt != (*it).result.end(); ++xt)
           {
             for(std::vector<int>::iterator yt =  (*jt).result.begin(); yt!= (*jt).result.end(); ++yt)
               {
                 if((*xt) == (*yt))
                   {
                      ss << "x" << (*yt) << ",";
                       newHolder.result.push_back((*yt));
                   }
               }
           }
          if(!duplicityTest(output,newHolder) && !duplicityTest(extents,newHolder)){
           ui->ieLog->append(ss.str().substr(0,ss.str().size()-1).c_str());
           output.push_back(newHolder);
           i++;
           }
       }
   }
 std::stringstream ss;
  tentHolder newHolder = tentHolder(cols,rows,'e','x','y',i);
   ss<< "<br><b><font size=\"2\">e"<< i<<" =</b> "<< "<font size=\"2\"> = <span style=\" color:#005500;\">";;
  for(int y = 0; y < asRowCount; y++)
    {
          ss << "x" << y << ",";
          newHolder.result.push_back(y+1);
    }
   if(!duplicityTest(output,newHolder) && !duplicityTest(extents,newHolder)){
    ui->ieLog->append(ss.str().substr(0,ss.str().size()-1).c_str());
    output.push_back(newHolder);
    i++;
    }
   return output;
}
std::vector<tentHolder> MainWindow::getIntents(std::vector<tentHolder> extents)
{
  std::vector<tentHolder> output;
  int i = 0;
  for(std::vector<tentHolder>::iterator it = extents.begin(); it != extents.end(); ++it,i++)
    {
      std::stringstream ss;
      tentHolder newHolder = tentHolder(rows,cols,'i','y','e',i);
      ss<< "<br><b><font size=\"2\">i"<< i+1<<" =</b> " <<"e" << i << "<font size=\"4\">↑" << "<font size=\"2\"> = "<< "<font size=\"2\"> = <span style=\" color:#005500;\">";
      for(int x = 0; x < IsCount; x++)
        {
          bool unionCom = true;
          for(std::vector<int>::iterator jt = (*it).result.begin(); jt != (*it).result.end(); ++jt)
            {
                 if(ui->binTableClosure->item((*jt)-1,x)->text() != "1") unionCom = false;
            }
          if(unionCom){
               ss << "y" << x+1 << ",";
              newHolder.result.push_back(x+1);
            }
        }

      ui->ieLog->append(ss.str().substr(0,ss.str().size()-1).c_str());
     output.push_back(newHolder);
    }
  return output;
}


void MainWindow::DoTabulkyPush()
{

}
