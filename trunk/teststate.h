#include <qptrlist.h>
#include <qstringlist.h>
#include <qstring.h>
#include <qlabel.h>
#include <khtml_part.h>
#include <qlayout.h>
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qdom.h>
/**
\defgroup TestModule
\brief The classes in these modules perform the actual testing

*/
//Forward Declaration of all classes
class Test;
class TestCase;
class TestAction;
class ClickAction;
class FormSubmitAction;
class FormActionElement;
class TextElement;
class SelectElement;
class ChekBoxElement;
class RadioElement;
class ButtonElement;

class TestRunner; //should have the "load" method to run from file
class TestCaseRunner;
class TestActionRunner;
class ClickActionRunner;
class FormSubmitActionRunner; //the tough one

class TestRecorder; //should have the "save method" to save to file
class TestCaseRecorder;
class TestActionRecorder;
class ClickActionRecorder;
class FormSubmitActionRecorder;

class TestRunnerWidget;
class TestRecorderWidget;

/**
\ingroup TestModule
\class Test 
\brief Represents the entire test in memory
*/
class Test
{
  public:
    Test();
    ~Test();
    void addTestCase(TestCase *);
    int count() const;
    void setTestName(QString testName);
    QString testName() const;
  protected:
    QPtrList <TestCase > testCaseList;
    QString m_testName;
};

/**
\ingroup TestModule
\class  TestCase
\brief represents one test case
*/
class TestCase 
{
  public:
    TestCase( Test * parentTest);
    ~TestCase();
    Test * parent() const;
    void addTestAction(TestAction *);
    int count() const;
  protected:
    QPtrList<TestAction > testActionList;
  private:
    Test * m_parentTest;
    QString testCaseName;
};

/**
\ingroup TestModule
\class TestAction
\brief abstract class.Represents one test action

This action is later simulated by KWebTest
*/
class TestAction
{
  public:
    ~TestAction();
    TestCase * parent() const;
    enum TestActionType
    {
      ClkAction,
      FrmSubmitAction,
      Invalid
    };
  protected:
    TestAction(TestCase* parentTestCase, TestActionType type);		
  protected:
    TestCase * m_parent;
    TestActionType m_TestActionType;
    
};

/**
\ingroup TestModule
\class ClickAction
\brief Represents the "Click"
*/
class ClickAction: public TestAction
{
  public:
    ClickAction(TestCase* parentTestCase, QString link);
  public:
    ~ClickAction();	
    enum ClickActionType
    {
      ImgClick,
      SimpleClick,
      PostClick,
      Invalid
    };
    ClickAction::ClickActionType clickActionType() const;
        
  protected:
    QString m_link; //The actual link, JS in case of post, otherwise simple GET url
    void setClickActionType( ClickActionType type);
    ClickActionType m_type;

};

/**
\ingroup TestModule
\class FormSubmitAction
\brief Represents a form submission
*/

class FormSubmitAction: public TestAction
{
  public:
    FormSubmitAction(TestCase *parentTestCase);
    ~FormSubmitAction();
    void addFormActionElement(FormActionElement *);
    FormActionElement * elementAt(int index) const;
    int count() const;
    
    void setName( QString name);
    QString name() const;
  private:
    QPtrList<FormActionElement > formActionElementList;
    QString m_name;
};

/**
\ingroup TestModule
\class FormActionElement
\brief Represents an element in a form submission
*/
class FormActionElement
{
  public:
    FormActionElement( FormSubmitAction *action);
    ~FormActionElement();
    FormSubmitAction * parent() const;
    void setName(QString name);
    QString name() const;
  protected:
    FormSubmitAction *m_parentAction;
  private:
    QString m_name;
};

/**
\ingroug TestModule
\struct TextPair
\brief Stores val:attribute pair.Used for FormSubmitActions
*/
struct TextPair
{
  QString val;
  QString attr;
};
/**
\ingroup TestModule
\class TextElement
\brief Represents a user-editable Text Element
*/
class TextElement: public FormActionElement
{
  public: 
    TextElement(FormSubmitAction *action);	
    ~TextElement();
    void addPair(QString attr, QString val);
    TextPair * pair(int index);
    int count() const;
  private:
    QPtrList<TextPair> pairList;
    int m_count;
};

/**
\ingroup TestModule
\class SelectElement
\brief Represents a drop down menu on a form, that the user can select
*/
class SelectElement: public FormActionElement
{
  public: 
    SelectElement(FormSubmitAction * action);
    ~SelectElement();
    void addVal(QString val);
    int count () const;
  private:
    QStringList probableValues;
};

/**
\ingroup TestModule
\class CheckBoxElement
\brief Represents a checkbox on a form
*/
class CheckBoxElement: public FormActionElement
{
  public:
    CheckBoxElement(FormSubmitAction *action);
    ~CheckBoxElement();
    void setState(bool checked);
    bool isChecked() const;
  private:
    bool m_checked;

};

/**
\ingroup TestModule
\class RadioElement
\brief Represents a radio button on a form
*/
class RadioElement: public FormActionElement
{
  public:
    RadioElement(FormSubmitAction *action);
    ~RadioElement();
  private:
    QString value;
};

/**
\ingroup TestModule
\class ButtonElement
\brief Represents a push button on a form

Eg "Submit" or "Cancel Buttons"
*/
class ButtonElement: public FormActionElement
{
  public:
    ButtonElement(FormSubmitAction *action);
    ~ButtonElement();
  private:
    QString value;
};
    
/**
\ingroup TestModule
\class TestRunner
\brief Given an instance of a \ref Test Class, this class runs the entire test.
*/
class TestRunner: public QObject
{
  Q_OBJECT
  public:
    TestRunner(Test * test, KHTMLPart *part);
    TestRunner(Test * test); //runs test in "dummy" mode, ie performs no actual HTTP transactions
    ~TestRunner();
    bool run();
    bool isDummyMode();
    QString report() const;
  private:
    Test *m_test;
    KHTMLPart *m_part;
    bool hasRun;
    bool dummyMode;
};

/**
\ingroup TestModule
\class TestRunnerWidget
\brief Visually "runs" and already recorded test for the user.

This class holds all the test-running widgets. When instantiated, it spawns a new instance of  \ref TestRunner to run the test
*/
class TestRunnerWidget:public QWidget
{
  Q_OBJECT
  public:
    TestRunnerWidget(QWidget *parent, TestRunner * runner);
    ~TestRunnerWidget();
  public slots:
    void savePage(QString &pageHtml);
  private:
    QLabel * testRunnerLabel;
    QPtrList <QString> menuList;
    QPushButton * runButton;
    QPushButton *next;
    QPushButton *finish;
    QPushButton *loadTestCase;
    QPtrList <QWidget> listWidget;
    QLabel *runStatus;
    QVBoxLayout * layout;

    TestRunner * m_testRunner;
        //The actual methods needed to run the test case
  private slots:
    bool runTest();
    bool runTestCase();
    bool runAction();
    bool runClickAction();
    bool runFormAction();
};

/**
\ingroup TestModule
\class TestRecorderWidget
\brief This class holds all the test-recording widgets

When instantiated, it spawns a new instance of \ref TestRecorderWidget to run the test cases
*/
class TestRecorderWidget : public QWidget
{
  Q_OBJECT
  public:
    TestRecorderWidget(QWidget *parent, TestRecorder * testRecorder);
    ~TestRecorderWidget();
  public slots:
    void createTest();
    void createTestCase();
    void createAction();

  public:
    int testCount() const;
    int testCaseCount() const;

  private:
        //The Widgets to show the TestRecorder
    QLabel *mainLabel;
    QLabel *recorderStatus;
    QPushButton * startRecording;
    QPushButton * stopRecording;
    QListBox * TestCaseMenu;
    TestRecorder * m_TestRecorder;
    QVBoxLayout *layout;
        //the layout is complete
        
        //The methods which do the actual recording
    bool recordTest();
    bool recordTestCase();
    bool recordTestAction();
    bool recordClickAction();
    bool recordFormAction();			
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
\ingroup TestModule
\class TestReader
\brief This class reads a test from disk

When instantiated, it spawns a new instance of \ref TestRecorderWidget to run the test cases
*/
class TestReader
{
  public:
    TestReader(QString fileName);
    ~TestReader();
        
    Test * read();
  private:
    QString m_fileName;
    QDomDocument *m_doc;
    Test *m_test;
        
        //xml reading functions
    void readTest(QDomElement roomElement);
    void readTestCase(QDomElement testElement);
    void readTestAction(QDomElement testCaseElement, TestCase * testCase);
    void readLink(QDomElement testActionElement, TestCase * testCase);
    void readFormSubmitAction(QDomElement testActionElement, TestCase * testCase);
    void readField(QDomElement formSubmitActionElemement, FormSubmitAction *frmSubmitAction);
    void readPossibleValue(QDomElement formSubmitActionElemement, FormSubmitAction *frmSubmitAction);
    void readCheckBox(QDomElement formSubmitActionElemement, FormSubmitAction *frmSubmitAction);
    void readRadioValue(QDomElement formSubmitActionElemement, FormSubmitAction *frmSubmitAction);
    void readButton(QDomElement formSubmitActionElemement, FormSubmitAction *frmSubmitAction);
    
};
