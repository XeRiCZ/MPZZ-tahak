#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Vector2.h"

#include <vector>
#include <QMainWindow>
#include <QSlider>
#include <QMainWindow>
#include <QProgressBar>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTabWidget>
#include <QGraphicsTextItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolTip>
#include <QChar>
#include <QTimer>
#include <QComboBox>
#include "interactivegraphicsview.h"
#include <QHeaderView>
#include <QAbstractItemView>
#include <QProgressBar>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QListWidget>
#include <QStringList>
#include <QGroupBox>
#include <QVBoxLayout>
#include <math.h>
#include "customtablewidget.h"


using namespace std;

namespace Ui {
  class MainWindow;
}


enum dimension
{
  rows,
  cols
};

struct tentHolder
{
public:


  dimension prereqDimension;
  dimension resultDimension;
  char preReqSign,resultSign,mainSign;

  int mainIndex;
  std::vector<int> preReq;
  std::vector<int> result;

  tentHolder(dimension pre_dim,dimension res_dim, char mainSi,char resSi, char preReqSi, int mainId) :
    prereqDimension(pre_dim), resultDimension(res_dim), mainSign(mainSi), resultSign(resSi),preReqSign(preReqSi),mainIndex(mainId) {}
};


struct cell
{
public:
  int x;
  int y;
  char name;
  cell (int _x, int _y, char _n) : x(_x), y(_y), name(_n) {}
};

struct podtabulka
{
public:
  std::vector<std::vector<char>> trojce;
  std::vector<char> char_c;
  std::vector<cell*> cells;
  podtabulka(std::vector<std::vector<char>> t, std::vector<char> c,std::vector<cell*> ce) : trojce(t), char_c(c), cells(ce) {}
};



enum vertexState
{
  FRESH,
  OPENED,
  CLOSED
};

struct tabLine
{
public:
  int pos;
  char name;
  tabLine(int p,char n) : pos(p), name(n) {}
};

struct vertex
{
public:
  char name;
  int x;
  int y;
  int index;
  int rank;
  vertexState thisState;
  std::vector<vertex*> derived;
  std::vector<vertex*> inherited;
  vertex (char _n,int _x,int _y) : name(_n),x(_x),y(_y) {}
  vertex(){x = 0; y = 0; name = 'p';}
  vertex(vertex*c) : name(c->name),x(c->x),y(c->y),derived(c->derived),inherited(c->inherited){};
};

struct supInfHolder
{
  std::vector<vertex*> dolniZavora;
  std::vector<vertex*> horniZavora;
  vertex* infinum;
  vertex* supremum;
};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void PlaceVertex();
  void StopPlacing();

std::vector<cell*> unionOfLines(std::vector<tabLine> rows, std::vector<tabLine> cols);

  QLineEdit * i_chars;
  QLineEdit * i_rels;
  QLineEdit * i_charsA;
  QLineEdit * i_charsB;
   QLineEdit * i_charsC;
  QLineEdit * i_zobrazeni;
  QLineEdit * i_trojice;
  QLineEdit * i_vertex;

  string abeceda;
  string cisla;
  string cisla0;

  void DeleteActiveVertex();

 CustomTableWidget * table;

  QPlainTextEdit * g_log;
  QPlainTextEdit * graph_log;

  QString mainLog;

  QCheckBox * c_refl;
  QCheckBox * c_irefl;
  QCheckBox * c_sym;
  QCheckBox * c_asym;
  QCheckBox * c_antisym;
  QCheckBox * c_trans;
  QCheckBox * c_ekv;
  QCheckBox * c_err;
  QCheckBox * c_kvaz;

  QCheckBox * c_funkce;
  QCheckBox * c_relace;
  QCheckBox * c_injekce;
  QCheckBox * c_surjekce;
  QCheckBox * c_bijekce;

  QCheckBox * c_uzavrenost;
  QCheckBox * c_asociativita;
  QCheckBox * c_jednotkovyprvek;
  QCheckBox * c_inverzniprvek;
  QCheckBox * c_komutativita;

  QTabWidget * tabWidget;

  QGraphicsView * zobrazeniView;
  QGraphicsScene * zobrazeniScene;

  CustomGraphicsView * grafView;
  CustomScene * grafScene;

  CustomGraphicsView * usporadaniView;
  CustomScene * usporadaniScene;

  QPushButton * updateBTN;
  QTimer * renderUpdateTimer;

  void UpdateRelace();
  void UpdateZobrazeni();
  void UpdateGrafy();
  void UpdateGrupoidy(bool secondary = false);
  void UpdateUsporadani();

 void determineUsedIs(QTableWidget* tab);
  int IsCount;
  int asRowCount;

  Vector2 getTableCoords(char x,char y);

  std::vector<char>* GetCharsFromTable();
  std::vector<std::vector<char>>* GetTrojiceFromTable();
  bool secondaryUzavrenostTest(std::vector<char> chC, std::vector<std::vector<char>>troj);
   bool MainWindow::thercialUzavrenostTest(std::vector<char> chC, std::vector<std::vector<char>>troj);

  vertex* GetVertex(char name,std::vector<vertex>*vec);
  void RenderZobrazeni();
  void RenderArrow(int x1,int y1,int x2, int y2, QGraphicsScene * scene,bool noArrow = false);

  bool ERROR;
private:
  Ui::MainWindow *ui;
  std::vector<char>*                               CHARS;
  std::vector<std::vector<char>>*         RELATIONS;

  std::vector<char>*                               CHARS_A;
  std::vector<char>*                               CHARS_B;
  std::vector<char>*                               CHARS_C;
  std::vector<char>*                               CHARS_VERTEX;
    std::vector<char>*                               CHARS_VERTEX2;
     std::vector<char>*                               CHARS_M;
  std::vector<std::vector<char>>*         ZOBRAZENI;
  std::vector<std::vector<char>>*         TROJICE;


  std::vector<podtabulka*> GetPodgrupy(std::vector<char> chars, std::vector<std::vector<char>> ts);
  void actualizePodgrupyList();
  std::vector<podtabulka*> podgrupy;
  std::vector<char>* GetCharsFromInput(QString  input);
  std::vector<std::vector<char>>* GetRelationsFromInput(QLineEdit * ledit,int sizeOfRelations);

  enum generationPreset
  {
          ALL,
          RE,
          IRE,
          SY,
          ASY,
          AN,
          TR
  };


  bool arrayContains(char ch, char * arr);
  bool vectorContains(char ch, vector<char> *vec);
  bool vectorContains2(std::vector<char>,std::vector< vector<char>>* vec);
  bool vectorContains3(vertex * v, std::vector<vertex*> vs);

  string GenerateRelations(generationPreset preset);
  bool TestRelations(generationPreset preset);
  bool FindRelation(char _A,char _B);
  bool SymetryTest(bool Asymetry);
  bool ReflectionTest(char _A,char _B);
  bool AntisymetryTest();
  bool TransitivityTest();

  bool functionTest();
  bool relaceTest();
  bool InjekceTest();
  bool SurjekceTest();

  bool AsociativitaTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug = false);
  bool SecondaryAsociativitaTest(std::vector<tabLine> rows,std::vector<tabLine> cols,std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug);
  bool JednotkovyTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug = false);
  bool InverzniTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug = false);
  bool SecondaryInverzniTest(std::vector<tabLine> rows,std::vector<tabLine> cols, std::vector<std::vector<char>>troj,bool debug);
  bool SecondaryJednotkovyTest(std::vector<tabLine> rows,std::vector<tabLine> cols, std::vector<std::vector<char>>troj,bool debug);
  bool KomutativitaTest(std::vector<char> chC, std::vector<std::vector<char>>troj,bool debug = false);

  int charToIndex(char input,std::vector<char> chC);
  bool GrupoidyDoTabulky();
  char getResultCharOn(char x,char y, std::vector<std::vector<char>>troj);
char jednotkovy;


bool addingVertexMode;
bool addingEdgeMode;
bool deletingVertexMode;
void RenderGraph(QGraphicsScene * scene,std::vector<vertex*>*  verticies);
void ConnectVerticies(vertex * parent,vertex * child);
std::vector<vertex*> graphVerticies;
std::vector<vertex*> usporadaniVerticies;
vertex* addingVertex;
int currentIndex;
int currentIndex2;

vertex * edgeA;
vertex * edgeB;
vertex * selected;

void cycleDebug(std::vector<vertex*>* pathToAdd,vertex* mainVert,std::vector<vertex*> workingVerts);

std::vector<vertex*> cloneGraph(std::vector<vertex*>* original);
void constructCore(std::vector<vertex*>* workingGraph);
std::vector<vertex*> core;
std::vector<vertex*> preEndingVerticies(std::vector<vertex*> endingVerticies);
std::vector<vertex*> endingVerticies(std::vector<vertex*> * workingGraph);
void removeVerticiesFromGraph(std::vector<vertex*> * verticies,std::vector<vertex*> * workingGraph);

std::vector<vertex*> convertGraphToHassDiagram(std::vector<vertex*>  graph);
void getDerived(vertex*from,std::vector<vertex*>* subGraph,vertex* starting = NULL);
void getInherited(vertex*from,std::vector<vertex*> * subGraph,vertex * starting = NULL);
void setRanksToVerticies(std::vector<vertex*> *graph);
void raiseRank(vertex *v, int toRank);

std::vector<vertex*> MtoVerticies(std::vector<char>*  M,std::vector<vertex*> *graph);
std::vector<vertex*> getMin(std::vector<vertex*> M,std::vector<vertex*> *graph);
std::vector<vertex*> getMax(std::vector<vertex*> M,std::vector<vertex*> *graph);
vertex* getNejm(std::vector<vertex*> M,std::vector<vertex*> *graph);
vertex* getNejv(std::vector<vertex*> M,std::vector<vertex*> *graph);

std::vector<vertex*> combineVectors (std::vector<vertex*> a,std::vector<vertex*> b );
std::vector<vertex*> subtractVectors(std::vector<vertex*> a,std::vector<vertex*> b );
std::vector<vertex*> unionOfVectors(std::vector<vertex*> a,std::vector<vertex*> b );
supInfHolder getSUPINF(std::vector<vertex*> M, std::vector<vertex*>* graph);
std::vector<vertex*> cleanDuplicates(std::vector<vertex*> vec);

bool svazTest(std::vector<vertex*> graf);
bool uplnySvazTest(std::vector<vertex*> graf);
bool distributivniSvazTest(std::vector<vertex*> graf);
bool modularniSvazTest(std::vector<vertex*> graf);
bool komplementarniSvazTest(std::vector<vertex*> graf);


bool cycleTest(std::vector<vertex*> graf);
bool regularityTest(std::vector<vertex*> graf);
std::vector<vertex*> topologyOrder(std::vector<vertex*> graf);
std::vector<vertex*> inverseGraph(std::vector<vertex*> orig);
void doOrdering(std::vector<vertex*> graph,vertex* v,std::vector<vertex*> *resultGraph);
bool uplnyTest(std::vector<vertex*> graf);

int radTest(int curr_count, char x,char y);
QString printRady();


int selectedRank;

QGroupBox * gBox_L;
QGroupBox * gBox_R;
QVBoxLayout * bLayout_L;
QVBoxLayout * bLayout_R;


bool vector35Contains(std::vector<char> name,std::vector<podtabulka*> checking);
QString printVerticies(QString header, std::vector<vertex*> verticies,bool sort=false,bool useDecimals = false);

std::vector<int> getPrunik(std::vector<int> A, int i);
std::vector<int> getUzaver(std::vector<int> prunik, int i);
std::vector<int> getBwithoutA(std::vector<int> uzaver,std::vector<int> A);
bool bEqualsItest(std::vector<int>B, int i);

std::vector<tentHolder> getExtents();
std::vector<tentHolder> getUnions(std::vector<tentHolder> extents);
std::vector<tentHolder> getIntents(std::vector<tentHolder> extents);


private slots:
    // Metody určující chování programu
    void btnPushed();
    void secondaryBtnPushed();
    void RenderUpdate();
    void AddVertex();
    void AddEdge();
    void DeleteVertex();
    void Reset();
    void forceStopPlacing();
    void ListSelectionChange();
    void DoTabulkyPush();


    void UpdateSupport();
    void UpdateConfidence();
    void NextClosure();
};

#endif // MAINWINDOW_H
