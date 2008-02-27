#ifndef TEST_WINDOW_H_INCLUDED
#define TEST_WINDOW_H_INCLUDED
//
// C++ Interface: testWindow
//
// Description: 
//
//
// Author: Manish Chakravarty <mchakravarty@spikesource.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "kwebtestincludes.h"
class JsRunner;
class BrowserView;
class TestEditor;
class TestWindow: public QWidget
{
  Q_OBJECT
  public: 
    TestWindow(QWidget * parent, JsRunner * runner);
    ~TestWindow();
    TestEditor * testEditor() const;
   
  signals:
    void load(const QString & fileName);
    void save(const QString & fileName);
    void click(QString linkToClick);
    void loadCompleted();
  public slots:
    void openSaveDialog();
    void openLoadDialog();
    void openFileSelected(const QString & fileName);
    void saveFileSelected(const QString & fileName);
    void pageLoaded();
    private slots:
    void run();
    void runAll();
    void runStep();
    void runItem(QListViewItem * item, bool step);
    void putFormValue(QListViewItem * item);
    void clickButton(QListViewItem *item);

    void removeSelected();
    void removeAll();
  private:
      
    QVBoxLayout *layout;
    QGridLayout * buttonLayout;
    TestEditor * m_testEditor;
    QPushButton * loadButton;
    QPushButton * saveButton;
    QPushButton * playButton;
    QPushButton * playAllButton;
    QPushButton * deleteSelected;
    QPushButton * deleteAll;
    JsRunner * m_runner;
    BrowserView *m_view;
    QListViewItem *currentItem;
    int runCount;
    bool isTestRunning;
    bool isLoaded;
    bool playAllClicked;
};

class TestEditor : public QListView
{
  Q_OBJECT
  public:
    TestEditor(QWidget *parent);
    ~TestEditor();
    
  public slots:
    void readFile(const QString & fileName);
    void writeFile(const QString & fileName);
  
  private:
    void addUrl(QString url, QString assertion= QString::null);
    void addFormData(QString attr, QString val);
    void addButtonAssertion(QString buttonName, QString assertion=QString::null);
    QListViewItem * prevItem;
    QString m_fileString;
    QStringList m_StringList;
    void parse();
    void endFile();
    bool isForm;
    int counter;
    QString writeStringFromNode(QListViewItem * item);
    void endfile();
        
};

#endif
