// C++ Implementation: testwindow
//
// Description: 
//
//
// Author: Manish Chakravarty manishchaks@gmail.com, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "testWindow.h"
#include "JsRunner.h"
#include "browserview.h"
////////////////////////////////////////////////////////////////////////
TestWindow::TestWindow(QWidget *parent, JsRunner * runner)
    :QWidget (parent)
{
	m_runner=runner;
	int runCount=0;
	currentItem=0;
	isLoaded=true;
	//marker =0;
	m_testEditor = new TestEditor(this);
	loadButton = new QPushButton(this);
	saveButton = new QPushButton(this);
	playButton = new QPushButton (this);
	playAllButton = new QPushButton(this);
	deleteSelected = new QPushButton(this);
	deleteAll = new QPushButton(this);
	loadButton->setText("Load");
	saveButton->setText("Save");
	playButton->setText("Play");
	playAllButton->setText("Play All");
	deleteSelected->setText("Delete Selected");
	deleteAll->setText("Delete All!");
	layout = new QVBoxLayout(this);
	layout->add(m_testEditor);
	buttonLayout = new QGridLayout(this);
	buttonLayout->addWidget( loadButton, 0, 0);
	buttonLayout->addWidget (saveButton,0,1);
	buttonLayout->addWidget(playButton,1 ,0);
	buttonLayout->addWidget(playAllButton,1,1);
	buttonLayout->addWidget(deleteSelected,2,0);
	buttonLayout->addWidget(deleteAll,2,1);
	layout->addLayout(buttonLayout);
	connect(loadButton,SIGNAL(clicked()),this, SLOT(openLoadDialog()));
	connect(saveButton, SIGNAL(clicked()), this, SLOT(openSaveDialog()));
	connect(this, SIGNAL(load(const QString &)), m_testEditor, SLOT(readFile(const QString &)));
	connect(this, SIGNAL(save(const QString &)), m_testEditor, SLOT(writeFile(const QString &)));
	connect(playButton,SIGNAL(clicked()),this,SLOT(run()));
	connect(playAllButton,SIGNAL(clicked()),this,SLOT(runAll()));
	//connect(m_view,SIGNAL(loaded(BrowserView *, const KURL &url)), this, SLOT(pageLoaded()));
	connect(deleteSelected,SIGNAL(clicked()),this,SLOT(removeSelected()));
	connect(deleteAll,SIGNAL(clicked()),this,SLOT(removeAll()));
	isTestRunning = false;
	playAllClicked = false;
}

TestWindow::~TestWindow()
{
}

void TestWindow::removeAll()
{
    m_testEditor->clear();
}

void TestWindow::removeSelected()
{
    QListViewItem * item = m_testEditor->selectedItem();
    if(item)
    {
        m_testEditor->takeItem(item);
        //we have to re-number the first column
        QListViewItem *item = m_testEditor->firstChild();
        if(!item)
            return;
        int num=0;
        while(item)
        {
            QString numberText = QString::number(num);
            if(num<100)
                numberText.prepend("0");
            if(num<10)
                numberText.prepend("0");
            item->setText(0, numberText);
            item=item->nextSibling();
            num++;
        }
    }
    
}
  TestEditor * TestWindow::testEditor() const
{
    return m_testEditor;
}

void TestWindow::openLoadDialog()
{
	KFileDialog * fileDialog = new KFileDialog(".",QString::null, this,"File Open Dialog", true);
	fileDialog->setOperationMode(KFileDialog::Opening);
	connect(fileDialog, SIGNAL(fileSelected(const QString &)), this , SLOT(openFileSelected(const QString &)));
	fileDialog->show();
  
}
  
void TestWindow::openSaveDialog()
{
	KFileDialog * fileDialog = new KFileDialog(".",QString::null, this,"File Save Dialog", true);
	fileDialog->setOperationMode(KFileDialog::Saving);
	connect(fileDialog, SIGNAL(fileSelected(const QString &)), this , SLOT(saveFileSelected(const QString &)));
	fileDialog->show();
}

void TestWindow::openFileSelected(const QString & fileName)
{
	qWarning("Open File selected");
	QString newString = fileName;
	newString.remove("file://");
	emit load(newString);
}


void TestWindow::saveFileSelected(const QString & fileName)
{
	qWarning("Save File selected");
	QString newString = fileName;
	newString.remove("file://");
	emit save(newString);
}



void TestWindow::putFormValue( QListViewItem *listViewItem)
{
    
    //Find out the HTML input element corresponding to the given item and the put the value in it
   // DOM::HTMLInputEle inputEle
    KHTMLPart * part = m_runner->part();
    if(!part)
    {
        return;
    }
  
    DOM::HTMLDocument * doc = new DOM::HTMLDocument(part->htmlDocument());
    DOM::HTMLCollection  collection = doc->forms();
    for(unsigned long i = 0; i< collection.length(); i ++)
    {
      
        DOM::Node node = collection.item( i);
        DOM::DOMString domStringName = node.nodeName();
        QString stringName = domStringName.string();
        
        DOM::HTMLFormElement * htmlFormEle = (DOM::HTMLFormElement*)&(node);
        DOM::HTMLCollection formColln = htmlFormEle->elements();
        for(unsigned long j =0; j< formColln.length(); j ++)
        {
            DOM::Node formNode = formColln.item(j);
            DOM::HTMLInputElement element(formNode);
            DOM::DOMString formDomName = formNode.nodeName();
            QString formString = formDomName.string();
            DOM::Element newEle(formNode);
            if( newEle.getAttribute("type").string()!="hidden")
            {
                QString cName=element.name().string();
                QString cValue = element.value().string();
                QString cType = newEle.getAttribute("type").string();
                //if(cType == "text" || cType== "password"|| cType=="file")
                {
                    //we must make sure that we are setting the values of the right elements, before we proceed
                    if(cName==listViewItem->text(2))
                        //we have a match!!
                    {
                        element.setValue( listViewItem->text(3));
                        m_runner->part()->executeScript( element.getAttribute("onchange").string());
                    }
                }
            }
        }
    }
}
void TestWindow::clickButton( QListViewItem *listViewItem)
{
    //Find out the HTML input element (with type submit) and then "click" it    
    KHTMLPart * part = m_runner->part();
    if(!part)
    {
        return;
    }
  
    DOM::HTMLDocument * doc = new DOM::HTMLDocument(part->htmlDocument());
    DOM::HTMLCollection  collection = doc->forms();
    for(unsigned long i = 0; i< collection.length(); i ++)
    {
        DOM::Node node = collection.item( i);
        DOM::DOMString domStringName = node.nodeName();
        QString stringName = domStringName.string();
        DOM::HTMLFormElement * htmlFormEle = (DOM::HTMLFormElement*)&(node);
        DOM::HTMLCollection formColln = htmlFormEle->elements();
        for(unsigned long j =0; j< formColln.length(); j ++)
        {
            DOM::Node formNode = formColln.item(j);
            DOM::HTMLInputElement element(formNode);
            DOM::DOMString formDomName = formNode.nodeName();
            QString formString = formDomName.string();
            DOM::Element newEle(formNode);
            if( newEle.getAttribute("type").string()!="hidden")
            {
                QString cName=element.name().string();
                QString cValue = element.value().string();
                QString cType = newEle.getAttribute("type").string();
                if(cType == "submit")
                {
                    qWarning( "Button Name %s, Read Name: %s", cValue.ascii(), listViewItem->text(3).ascii());
                    //we must make sure that we are setting the values of the right elements, before we proceed
                    if(cValue==listViewItem->text(3))
                        //we have a match!!
                    {
                        element.click();
                    }
                }
            }
        }
    }
}

void TestWindow::runItem( QListViewItem * item, bool step)
{
   // playButton->setEnabled( false);
    if(!item)
        if(step==true)
            item=m_testEditor->firstChild();
    else
        return;
    runCount ++;
    //playAllButton->setEnabled( false);
    m_runner->setIsRecording(false);
    m_testEditor->setSelected(item, true);
    if(item->text(4) == "NULL")
    {
        //test pass
        //TODO Put the "N/A" in the report generation code
        item->setText(5, "N/A");
    }
    else
    {
        QString docString = m_runner->part()->documentSource();
        if(docString.contains(item->text(4)))
        {
            //assertion pass
            item->setText(5, "PASS");
        }
        else
        {
            //assertion failed
            item->setText(5, "FAIL");
        }
    }
    if(item->text(1)=="URL")
    {
        QString docString = m_runner->part()->documentSource();
        if(docString.contains(item->text(2)))
            m_runner->click( item->text( 2));
        else
        {
            item->setText(5,"FAIL");
            currentItem=currentItem->nextSibling();
            pageLoaded();
        }
    }
      
    else  if (item->text(1)=="FORM")
        putFormValue(item);
    else if(item->text(1)=="BUTTON")
        clickButton(item);
    QListViewItem  *tempItem = item->nextSibling();
    if(tempItem&& step)
        if(currentItem->text(1)=="FORM" && (tempItem->text(1)=="BUTTON" || tempItem->text(1)=="FORM") )
        {
            currentItem=tempItem;
            runItem(tempItem, step);
        }
    
    m_runner->setIsRecording(true);
}

void TestWindow::run()
{
    //first check for the current item
    if(!currentItem)
        currentItem=m_testEditor->firstChild();
    else
        currentItem=currentItem->nextSibling();
    //run the given item
    m_testEditor->setSelected(currentItem, true);
    runItem(currentItem, false);
}
void TestWindow::runAll()
{
    playAllClicked=true;
    runCount=0;
    //runItem(0,true);
    currentItem= m_testEditor->firstChild();
    pageLoaded();
    
}

void TestWindow::pageLoaded()
{
    //playButton->setEnabled( true);
    playAllButton->setEnabled( true);
    qWarning( "Run count %d", runCount);
    if(playAllClicked==true)
    {
        runItem(currentItem, true);
        if(!currentItem->nextSibling())
            playAllClicked=false;
        currentItem=currentItem->nextSibling();
    }
}


void TestWindow::runStep()
{
}
///////////////////////////////////////////////////////////////////////

TestEditor::TestEditor(QWidget *parent)
    :QListView(parent)

{
	isForm=false;
	addColumn("Serial No");
	addColumn( "Type");
	addColumn ("Value");
	addColumn ("Attribute");
	addColumn ("Assertion");
	addColumn("Assertion Results");
}

TestEditor::~TestEditor()
{

}

void TestEditor::readFile(const QString & fileName)
{
	qWarning("Read File: %s", fileName.ascii());
	isForm=false;
	QFile file(fileName);
	if(!file.open(IO_ReadOnly))
	{
	qWarning("File open failed");
	return;
	}
	clear();
	m_fileString = QString(file.readAll());
	m_StringList = QStringList::split("\n",m_fileString);
	parse();
	file.close();
}

void TestEditor::writeFile(const QString & fileName)
{
	qWarning("Write file: %s", fileName.ascii());
	isForm=false;
	m_fileString=QString::null;
	QFile file(fileName);
	file.open(IO_WriteOnly);
	QTextStream ts (&file);
	QListViewItem * myChild= this->firstChild();
	while(myChild)
	{
	m_fileString.append(writeStringFromNode(myChild));
	myChild = myChild->nextSibling();
	};
	endFile();
	ts << m_fileString;
	file.close();
  
};

QString TestEditor::writeStringFromNode( QListViewItem *item)
{
    QString type = item->text(1);
    QString value = item->text(2);
    QString attribute = item->text(3);
    QString assertion = item->text(4);
    bool prevForm = isForm;
    if(type =="FORM"|| type=="BUTTON" )
        isForm=true;
    else isForm=false;
    if(prevForm != isForm)
    {
        if(prevForm==true && isForm ==false)
        {
            qWarning("Form Over");
            QString string = QString("==END_FORM_DATA==")+QString("\n");
            m_fileString.append(string);
        }
        else if(prevForm==false && isForm == true)
        {
            qWarning("Form begin");
            QString string = QString("==BEGIN_FORM_DATA==")+QString("\n");
            m_fileString.append(string);
        }
    }
    if(isForm)
    {
        if(type=="BUTTON")
        {
            return  attribute + QString("|") + assertion + QString("\n");
        }
        else
        {
            return attribute + QString("@") + value+ QString("\n");
        }
    }
    else
    {
        return value + QString("|") + assertion + QString("\n");
    }
    return QString::null;
}

/*******************************************
URL | Assertion
==BEGIN_FORM_DATA==
attr@value
attr2@value
button_to_Click | assertion
==END_FORM_DATA==
*******************************************/

/***************************************
Type Value  Attribute   Assertion
URL url             assertion
FORMattr        value   
FORMattr2   value2  
BUTTON
*************************************/

void TestEditor::addUrl(QString url, QString assertion)
{
    QListViewItem * item = new QListViewItem(this);
    QString numberText;
    int count = childCount();
    if(count < 10)
        numberText.append("0");
    if(count<100)
        numberText.append("0");
    numberText.append(QString::number(childCount()));
    item->setText(0,numberText);
    item->setText(1,"URL");
    item->setText(2,url);
    item->setText(3,"NULL");
    item->setText(4,assertion);
}


void TestEditor::addFormData(QString attr, QString val)
{
    QListViewItem * item = new QListViewItem(this);
    QString numberText;
    int count = childCount();
    if(count < 10)
        numberText.append("0");
    if(count<100)
        numberText.append("0");
    numberText.append(QString::number(childCount()));
    item->setText(0,numberText);
    item->setText(1,"FORM");
    item->setText(2,val);
    item->setText(3,attr);
    item->setText(4,"NULL");
}

void TestEditor::addButtonAssertion(QString buttonName, QString assertion)
{
    QListViewItem * item = new QListViewItem (this);
    QString numberText;
    int count = childCount();
    if(count < 10)
        numberText.append("0");
    if(count<100)
        numberText.append("0");
    numberText.append(QString::number(childCount()));
    item->setText(0,numberText);
    item->setText(1,"BUTTON");
    item->setText(2,"NULL");
    item->setText(3,buttonName);
    item->setText(4,assertion);
}

void TestEditor::parse()
{
    for(uint i=0; i< m_StringList.count() ; i++)
    {
        QString currString = m_StringList[i];
        if(currString=="==BEGIN_FORM_DATA==")
        {
            isForm=true;
            continue;
        }
        else if(currString =="==END_FORM_DATA==")
        {
            isForm=false;
            continue;
        }
        else
        {
            if(isForm)
            {
                if(currString.contains("@"))
                {
                    QStringList valAttr = QStringList::split("@",currString);
                    addFormData(valAttr[0], valAttr[1]);
                    continue;
                }
                else if(currString.contains("|"))
                {
                    QStringList buttonAssertion = QStringList::split("|",currString);
                    addButtonAssertion(buttonAssertion[0], buttonAssertion[1]);
                    continue;
                }
            }
            else
            {
                QStringList urlAssertion = QStringList::split("|",currString);
                addUrl(urlAssertion[0], urlAssertion[1]);
            }
        }
    }
}

void TestEditor::endFile()
{
    if(isForm)
        m_fileString.append("==END_FORM_DATA");
    m_fileString.append("\n");
}
