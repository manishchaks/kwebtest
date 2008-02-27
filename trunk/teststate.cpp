#include "teststate.h"
#include "qfile.h"
#include "kfiledialog.h"	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The Test class
Test::Test()
{
    //empy constructor
};

Test::~Test()
{
  for(unsigned int i =0; i<testCaseList.count(); i++)
    delete testCaseList.at(i);
  qWarning("Destroying Test");
};
void Test::addTestCase(TestCase *s)
{
  if(!s)
    return;
  testCaseList.append(s);
}
int Test::count() const
{
  return testCaseList.count();
}

void Test::setTestName( QString name)
{
  m_testName = name;
  qWarning("Setting test name : %s", name.ascii());
}

QString Test::testName() const 
{
  return m_testName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class TestCase
TestCase::TestCase( Test * parentTest)
{
  if(!parentTest)
    return;
  m_parentTest = parentTest;
  m_parentTest->addTestCase( this);
  qWarning("constructing test case");
    
    
} 
TestCase::~TestCase()
{
  for(uint i =0; i< testActionList.count() ; i++)
    delete testActionList.at(i);
  qWarning("Destroying TestCase");
}

Test * TestCase::parent() const
{
  return m_parentTest;
}

void TestCase::addTestAction(TestAction *ta)
{
  testActionList.append(ta);
}

int TestCase::count() const
{
  return testActionList.count();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class TestAction

TestAction::TestAction(TestCase* parentTestCase, TestActionType type)
{
  if(!parentTestCase)
    return;
  m_parent= parentTestCase;
  m_TestActionType = type;	
  m_parent->addTestAction( this);
}

TestAction::~TestAction()
{
  qWarning("Destroying TestAction");
}

TestCase *TestAction:: parent() const
{
  return m_parent;
    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class ClickAction

ClickAction::ClickAction(TestCase *parentTestCase,QString link)
  : TestAction(parentTestCase, TestAction::ClkAction)
{
  this->m_link= link;
  qWarning("Constructing click action ");
}

ClickAction::~ ClickAction()
{
  qWarning("Destroying ClickAction");

};

ClickAction::ClickActionType ClickAction::clickActionType() const
{
  return m_type;
}

void ClickAction::setClickActionType( ClickActionType type)
{
  m_type = type;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class FormSubmitAction

FormSubmitAction::FormSubmitAction(TestCase *parentTestCase)
  : TestAction(parentTestCase,TestAction::FrmSubmitAction)
{
  qWarning("Constructing FormSubmitAction");
}

FormSubmitAction::~ FormSubmitAction()
{
  qWarning("Destroying FormSubmitAction");
};

void FormSubmitAction::addFormActionElement( FormActionElement *element)
{
  formActionElementList.append(element);
}

void FormSubmitAction::setName( QString name)
{
  m_name = name;
  qWarning("Setting name (FormSubmitAction) to %s", name.ascii());
}

QString FormSubmitAction::name() const 
{
  return m_name;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class 

FormActionElement::FormActionElement( FormSubmitAction *action)
{
  if(!action)
    return;
  m_parentAction=action;
  m_parentAction->addFormActionElement( this);
  qWarning("Constructing FormActionElement");
}

FormActionElement::~FormActionElement()
{
  qWarning("Destroying FormActionElement");
}

FormSubmitAction * FormActionElement::parent() const
{
  return m_parentAction;
}

void FormActionElement::setName( QString name)
{
  m_name=name;
  qWarning("Setting FormActionElement name to %s", name.ascii());
}

QString FormActionElement::name() const 
{
  return m_name;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


TextElement::TextElement( FormSubmitAction * action)
  :FormActionElement(action)
{
  m_count=0;
}

TextElement::~ TextElement()
{
  for(uint i=0; i< pairList.count(); i++)
  {
    delete pairList.at(i);
  }
  
  qWarning("Destroying TextElement");
    
};

void TextElement::addPair(QString attr, QString val)
{
  TextPair * pair = new TextPair();
  pair->attr=attr;
  pair->val=val;
  pairList.append(pair);
  qWarning("Added %s , %s to testpair", attr.ascii(), val.ascii());
};

int TextElement::count() const 
{
  return pairList.count();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class

SelectElement::SelectElement(FormSubmitAction *action)
  :FormActionElement(action)
{
  qWarning("Constructing SelectElement");
};

SelectElement::~ SelectElement()
{

  qWarning("Destroying SelectElement");
};

void SelectElement::addVal( QString val)
{
  probableValues.append(val);
  qWarning("Appending %s to SelectElement", val.ascii());
}

int SelectElement::count() const 
{
  return probableValues.count();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class

CheckBoxElement::CheckBoxElement(FormSubmitAction *action)
  :FormActionElement(action)
{
  qWarning("Constructing CheckBoxElement");
};

CheckBoxElement::~ CheckBoxElement()
{
  qWarning("Destroying CheckBoxElement");
}

void CheckBoxElement::setState(bool checked)
{
  m_checked = checked;
}

bool CheckBoxElement::isChecked() const 
{
  return m_checked;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class

RadioElement::RadioElement( FormSubmitAction *action)
  :FormActionElement(action)
{
  qWarning("Constructing RadioElement");
}

RadioElement::~RadioElement()
{
  qWarning("Destroying RadioElement");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ButtonElement::ButtonElement( FormSubmitAction *action)
  :FormActionElement(action)
{
  qWarning("Constructing ButtonElement");
}

ButtonElement::~ButtonElement()
{
  qWarning("Destroying ButtonElement");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class TestRunner


TestRunner::TestRunner(Test * test, KHTMLPart *part)
{
  if(!test || !part)
    return;
  m_test=test;
  m_part=part;
  dummyMode=false;
};
TestRunner::TestRunner(Test * test)
{
  if(!test)
    return;
    
  m_test = test;
  m_part = 0;
  dummyMode=true;
}

TestRunner::~TestRunner()
{
    //do nothing, for nowBrowserAppBrowserAppBrowserApp
}

bool TestRunner::run()
{
  qWarning("Run begun");
    //TODO complete run code
  return false;
}

bool TestRunner::isDummyMode()
{
  return dummyMode;
}

QString TestRunner::report() const
{
  return QString("Report Generation not ready yet");
    //TODO complete report generation mode
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class TestRunnerWidget


TestRunnerWidget::TestRunnerWidget(QWidget *parent, TestRunner * runner)
  :QWidget(parent)
{
  if(!runner)
    m_testRunner=0;
  else
    m_testRunner=runner;
    
  testRunnerLabel= new QLabel(this);
  runButton = new QPushButton(this);
  next = new QPushButton(this);
  finish= new QPushButton(this);
  loadTestCase = new QPushButton(this);
  runStatus = new QLabel(this);
  layout = new QVBoxLayout(this);
    
  testRunnerLabel->setText("Test Runner");
  runButton->setText("Run");
  next->setText("Next");
  finish->setText("Finish");
  loadTestCase->setText("Load Test Case");
  runStatus->setText("Run Status");
    
  layout->add(testRunnerLabel);
  layout->add(runButton);
  layout->add(next);
  layout->add(finish);
  layout->add(loadTestCase);
  layout->add(runStatus);
    
    //connect the signals and the slots
  connect(runButton, SIGNAL(clicked()),this, SLOT(runTest()));
    
}

TestRunnerWidget::~TestRunnerWidget()
{
  delete layout;
}

void TestRunnerWidget::savePage(QString &pageHtml)
{
    //TODO: save the given QString to a filename
  qWarning("Simulate saving file");
};


bool TestRunnerWidget::runTest()
{
  qWarning("Run Test");
  return true;
}

bool TestRunnerWidget::runTestCase()
{
  qWarning("Run TestCase");
  return true;
}

bool TestRunnerWidget::runAction()
{
  qWarning("Run TestAction");
  return true;
}

bool TestRunnerWidget::runClickAction()
{
  qWarning("Run ClickAction");
  return true;
}

bool TestRunnerWidget::runFormAction()
{
  qWarning("run form action");
  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class TestRunnerWidget



TestRecorderWidget::TestRecorderWidget(QWidget *parent, TestRecorder * testRecorder)
  :QWidget(parent)
{
  m_TestRecorder= testRecorder;
    
  mainLabel = new QLabel(this);
  startRecording  = new QPushButton (this);
  stopRecording = new QPushButton(this);
  TestCaseMenu = new QListBox(this);
  layout = new QVBoxLayout(this);
  recorderStatus = new QLabel(this);
    
  mainLabel->setText("Test Recorder");
  startRecording->setText("Start Recording");
  stopRecording->setText("Stop Recording");
  recorderStatus->setText("Recorder Status");
    
  layout->add(mainLabel);
  layout->add(TestCaseMenu);
  layout->add(startRecording);
  layout->add(stopRecording);
  layout->add(recorderStatus);
}

TestRecorderWidget::~TestRecorderWidget()
{
    //do nothing
}

void TestRecorderWidget::createTest()
{
    //TODO
}

void TestRecorderWidget::createTestCase()
{
    //TODO
}

void TestRecorderWidget::createAction()
{
    //TODO
}


int TestRecorderWidget::testCount() const
{
    //TODO
  return 0;
}
int TestRecorderWidget::testCaseCount() const
{
    //TODO
  return 0;
}



//The methods which do the actual recording
bool TestRecorderWidget::recordTest()
{
    //TODO
  return false;
}

bool TestRecorderWidget::recordTestCase()
{
    //TODO
  return false;
}

bool TestRecorderWidget::recordTestAction()
{
    //TODO
  return false;
}

bool TestRecorderWidget::recordClickAction()
{
    //TODO
  return false;
}

bool TestRecorderWidget::recordFormAction()
{
    //TODO
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class TestReader 

TestReader::TestReader( QString fileName)
{
  m_fileName = fileName;
  m_test = 0;
  m_doc =0;
    
  m_test = new Test();
}

TestReader::~ TestReader()
{
  delete m_doc;
}

Test * TestReader::read()
{
  QFile file(m_fileName);
  if(!file.open(IO_ReadOnly))
    return 0;
  qWarning("Begun reading test");
  m_doc = new QDomDocument();
  m_doc->setContent(&file);
  if(m_doc->isNull())
  {
    qWarning("Error: Null Document");
    return 0;
  }
  QDomNodeList topLevelNodes = m_doc->childNodes();
  QDomElement rootElement = topLevelNodes.item(1).toElement();
  qWarning("Root Tag: %s",rootElement.tagName().ascii());
  //prints out "root"
  m_test = new Test();
  readTest(rootElement);

  return m_test;
};
void TestReader::readTest(QDomElement rootElement)
{;
  QDomNodeList testNodes = rootElement.childNodes();
  QDomElement testElement=testNodes.item(0).toElement();
  qWarning("Test Element tag %s",testElement.tagName().ascii());
  if(testElement.tagName()=="test")
  {
  //Make the Test Class
    m_test->setTestName(testElement.tagName());
   readTestCase(testElement);
  }
  
}

void TestReader::readTestCase(QDomElement e)
{
  QDomNodeList testCaseNodeList = e.childNodes();
  for(uint i=0; i< testCaseNodeList.count(); i++)
  {
    QDomElement testCaseElement = testCaseNodeList.item(i).toElement();
    if(testCaseElement.tagName()=="testcase")
    {
      qWarning("Found test case with name %s",testCaseElement.attribute("name").ascii());
      // make the test case class 
      TestCase * testCase = new TestCase(m_test);
      readTestAction(testCaseElement,testCase);
    }
  }
}


void TestReader::readTestAction(QDomElement e, TestCase * testCase)
{
  QDomNodeList testActionList = e.childNodes();
  for(unsigned int i=0; i<testActionList.count(); i++)
  {
    QDomElement testActionElement = testActionList.item(i).toElement();
    if(testActionElement.tagName() == "testaction")
    {
      //we have a match 
      qWarning("Test Action: %s, Type %s", testActionElement.attribute("name").ascii(),
               testActionElement.attribute("type").ascii());
      QDomNodeList childNodes = testActionElement.childNodes();
      for(int j=0; j< childNodes.count(); j++)
      {
        QDomElement childElement = childNodes.item(j).toElement();
        if(childElement.tagName() =="Link")
          readLink(childElement, testCase);
        else if(childElement.tagName() == "formsubmitaction")
          readFormSubmitAction(childElement, testCase);
        
      } 
    }
  }
}

void TestReader::readLink( QDomElement e, TestCase *testCase)
{
  //we get the element directly, no need to go to it's childnodes
  qWarning("Link Name: %s", e.attribute("Name").ascii());
  QString linkName = e.attribute("Name");
  ClickAction * clickAction = new ClickAction(testCase,linkName);
  
}

void TestReader::readFormSubmitAction(QDomElement e, TestCase * testCase)
{
  //we get the element directly, no need to go to it's childnodes
  qWarning("Form Submit Action Name: %s, Type: %s",e.attribute("name").ascii(), e.attribute("type").ascii());
  //TODO make the FormSubmitAction class
  FormSubmitAction *frmSubmitAction = new FormSubmitAction(testCase);
  frmSubmitAction->setName(e.attribute("name"));
  //now we need to get into the formsubmitaction and find out the types of class possible 
  
  QDomNodeList childNodes = e.childNodes();
  for(unsigned int i=0; i< childNodes.count(); i++)
  {
    QDomElement childElement = childNodes.item(i).toElement();
    //1. Field <field name="somename" attr ="someattr">
    QString tName = childElement.tagName();
    if (tName=="field")
      readField(e, frmSubmitAction);
    //2. <possiblevalue name="value1"/>
    else if(tName =="possiblevalue")
      readPossibleValue(e, frmSubmitAction);
    //3. <checkbox name="checkboxName" state="checked"/>
    else if(tName == "checkbox")
      readCheckBox(childElement,frmSubmitAction);
    //4. <radiovalue name ="value1"/>
    else if(tName == "radiovalue")
      readRadioValue(childElement,frmSubmitAction);
    //5.  <button name="button name">
    else if(tName == "button")
      readButton(childElement, frmSubmitAction);
  }
}

void TestReader::readField(QDomElement e,FormSubmitAction *frmSubmitAction)
{
  //1. Field <field name="somename" attr ="someattr">
  qWarning("Field Name: %s, Attr: %s", e.attribute("name").ascii(),e.attribute("attr").ascii());
   //Read all the fields, parse them and put them into ONE textElement
  QDomNodeList fieldNodes = e.childNodes();
  TextElement *te = new TextElement(frmSubmitAction);
  te->setName(e.attribute("name"));
  for(uint i=0; i<fieldNodes.count(); i++)
  {
    QDomElement fieldElement = fieldNodes.item(i).toElement();
    if(fieldElement.tagName()=="field")
    {
      //we have a match, put it in the te class
      QString attr = fieldElement.attribute("attr");
      qWarning("attr %s", attr.ascii());
      QString val = fieldElement.attribute("val");
      qWarning("val %s", val.ascii());
      te->addPair( attr,val);
    }
  }
  

}

void TestReader::readPossibleValue(QDomElement e, FormSubmitAction *frmSubmitAction)
{
  //2. <possiblevalue name="value1"/>
 
  qWarning("Possible Value Name: %s",e.attribute("name").ascii());
  SelectElement * element = new SelectElement(frmSubmitAction);
  QDomNodeList valueList = e.childNodes();
  
  for(uint i=0; i< valueList.count(); i++)
  {
    QDomElement sElement = valueList.item(i).toElement();
    if(sElement.tagName()=="possiblevalue")
    {
      //we have a match
      element->addVal( sElement.attribute("name"));
    }
  }
}

void TestReader::readCheckBox(QDomElement e, FormSubmitAction *frmSubmitAction)
{
  //3. <checkbox name="checkboxName" state="checked"/>
  qWarning("Check Box Name: %s, State: %s",e.attribute("name").ascii(), e.attribute("state").ascii());
  CheckBoxElement * element = new CheckBoxElement(frmSubmitAction);
  if(e.attribute("state")=="checked")
    element->setState(true);
  else if(e.attribute("state")=="unchecked")
    element->setState(false);
  
}

void TestReader::readRadioValue(QDomElement e, FormSubmitAction *frmSubmitAction)
{
  //4. <radiovalue name ="value1"/>
  qWarning("Radio Value Name: %s",e.attribute("name").ascii());
  RadioElement *re = new RadioElement(frmSubmitAction);
  re->setName(e.attribute("name"));
  

}

void TestReader::readButton(QDomElement e, FormSubmitAction *frmSubmitAction)
{
  //5.  <button name="button name">
  qWarning("Button Name: %s",e.attribute("name").ascii());
  ButtonElement *be = new ButtonElement(frmSubmitAction);
  be->setName(e.attribute("name"));
}