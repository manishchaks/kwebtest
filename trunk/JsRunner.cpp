#include "JsRunner.h"
#include "testWindow.h"
JsRunner::JsRunner( )

{
	layout=0;
	m_part=0;
	m_TestEditor=0;
	recMode=true;
	layout = new QVBoxLayout(this);
	linkName = new QLabel(this);
	linkNameLineEdit = new QLineEdit(this);
	formPushButton = new QPushButton(this);
	linkPushButton = new QPushButton(this);
	helpLabel = new QLabel(this);
	helpLabel ->setText("Welcome To KWebTest!\n Use the buttons above to record\n clicks or form submissions");
	linkName->setText( "Enter Link Name above");
	formPushButton->setText( "Record Form");
	//formPushButton->setToggleButton( true);
	linkPushButton->setText( "Record Click");
	//linkPushButton->setToggleButton(true);
	formTree = new QListView(this);
	assertionLabel = new QLabel(this);
	assertionLineEdit = new QLineEdit(this);
	assertionLabel->setText("Enter Assertion Text Below");
	layout->addWidget( formPushButton);
	layout->addWidget( linkPushButton);
	layout->addWidget( helpLabel);
	layout->addWidget( linkNameLineEdit);
	layout->addWidget (formTree);
	layout->addWidget( linkName);
	layout->addWidget(assertionLabel);
	layout->addWidget(assertionLineEdit);
	
	linkNameLineEdit->hide();
	assertionLabel->hide();
	assertionLineEdit->hide();
	
	connect(formPushButton,SIGNAL(clicked()),this, SLOT(executeForms()));
	connect(linkPushButton,SIGNAL(clicked()),this, SLOT(executeLinks()));
	formTree->addColumn("Form Name");	
	formTree->addColumn("Form Type");
	formTree->addColumn("Attribute");
	formTree->addColumn("Value:");
	
	formTree->setRootIsDecorated(true);
	formTree->hide();
	
	console = new QWidget(this);
	consoleLayout = new QVBoxLayout(console);
	console->hide();
	layout->addWidget(console);
	this->setCaption("JavaScript Debugger - KWebTest");
	initialWidgetState();

}

void JsRunner::initialWidgetState()
{
	console->hide();
	linkName->hide();
	formTree->hide();
	helpLabel->show();
	assertionLabel->hide();
	assertionLineEdit->hide();
}

JsRunner::~ JsRunner()
{
	delete layout;
	delete linkName;
	delete linkNameLineEdit;
	delete formPushButton;
	delete linkPushButton;
	delete formTree;
}

bool JsRunner::isRecording() const
{
	return recMode;
}

void JsRunner::setIsRecording(bool val)
{
    recMode = val;
}

void JsRunner::setPart(KHTMLPart *part)
{
    m_part = part;
}

KHTMLPart * JsRunner::part()
{
    return m_part;
}
void JsRunner::executeForms()
{
	initialWidgetState();
	linkName->hide();
	linkNameLineEdit->hide();
	helpLabel->hide();
	assertionLabel->show();
	assertionLineEdit->show();
	assertionLineEdit->setText("NULL");
	//formPushButton->setOn( true);
	//linkPushButton->setOn(false);
	delete consoleLayout;
	consoleLayout = new QVBoxLayout(console);
	
	formTree->show();
	formTree->clear();
  if(!m_part)
  {
    initialWidgetState();
    return;
  }
  
  DOM::HTMLDocument * doc = new DOM::HTMLDocument(m_part->htmlDocument());
  DOM::HTMLCollection  collection = doc->forms();
  for(unsigned long i = 0; i< collection.length(); i ++)
  {
      
    DOM::Node node = collection.item( i);
    DOM::DOMString domStringName = node.nodeName();
    QString stringName = domStringName.string();
    //qWarning("%d : Form Name: %s", (int)(i), stringName.ascii());
    DOM::HTMLFormElement * htmlFormEle = (DOM::HTMLFormElement*)&(node);
    QListViewItem * formNameItem = new QListViewItem(formTree);
    formNameItem->setText(0,htmlFormEle->name().string());	
    formTree->setOpen(formNameItem, true);
        
    DOM::HTMLCollection formColln = htmlFormEle->elements();
    for(unsigned long j =0; j< formColln.length(); j ++)
    {
      DOM::Node formNode = formColln.item(j);
            
      DOM::HTMLInputElement element(formNode);
      DOM::DOMString formDomName = formNode.nodeName();
      QString formString = formDomName.string();
      //qWarning("Form Element name: %s", element.name().string().ascii());
      //qWarning("Form Value name: %s", element.value().string().ascii());
      //qWarning("Form Type: %s", element.type().string().ascii());
      DOM::Element newEle(formNode);
      if( newEle.getAttribute("type").string()!="hidden")
      {
          QString attrString = element.name().string();
          if(attrString.contains("browse:search") || attrString.contains("dashboard")|| attrString.contains("search"))
             continue;
        QListViewItem * nodeNameItem = new QListViewItem(formNameItem);
        formNameItem->setText(1, formString);
        QListViewItem * elementNameItem = new QListViewItem(nodeNameItem);
        elementNameItem->setText(2,"Element Name");
        QString cName=element.name().string();
        elementNameItem->setText(3,element.name().string());
        
        QListViewItem * elementValueItem = new QListViewItem(nodeNameItem);
        elementValueItem->setText(2,"Element Value");
        elementValueItem->setText(3,element.value().string());
        QString cValue = element.value().string();
        
        QListViewItem * typeItem = new QListViewItem(nodeNameItem);
        typeItem->setText(2,"Type");
        typeItem->setText(3,newEle.getAttribute("type").string());
        QString cType = newEle.getAttribute("type").string();
        if(cType=="submit")
        {
          DOM::HTMLInputElement inputEle(newEle);
          MagicButton* button = new MagicButton(console,cValue,inputEle,m_part,this);
          button->setText(cValue);
          consoleLayout->add( button);
        }
        else if (cType =="radio")
        {
          QRadioButton * radioButton = new QRadioButton(cName,console);
          consoleLayout->add(radioButton);
        }
        else 
        {
            TestEditor * editor = m_TestEditor;
            QListViewItem * item = new QListViewItem(editor);
            QString numberText;
            int count = editor->childCount();
            if(count < 10)
                numberText.append("0");
            if(count<100)
                numberText.append("0");
            numberText.append(QString::number(editor->childCount()));
            item->setText(0, numberText );
            item->setText(2,element.name().string());
            item->setText(3,element.value().string());
            item->setText(4, "NULL");
            QLabel * label = new QLabel(cName,console,cName.ascii());
	    MagicLineEdit * lineEdit = new MagicLineEdit(console,DOM::HTMLInputElement(newEle),m_part,this,item);
	    consoleLayout->add(label);
	    consoleLayout->add(lineEdit);
	    item->setText(1, "FORM");
        }
      }
    }
    formTree->triggerUpdate();
  }
  consoleLayout->activate();
  console->show();
  consoleLayout->activate();
}
    

void JsRunner::executeLinks()
{
	initialWidgetState();
	linkNameLineEdit->show();
	linkName->show();
	helpLabel->hide();
	assertionLabel->show();
	assertionLineEdit->show();
	//assertionLineEdit->setText("NULL");
	//linkPushButton->setOn( true);
	//formPushButton->setOn(false);
	console->hide();
	formTree->hide();
	repeat = true;
	if(!m_part)
	{
	////qWarning("invalid part");
	return;
	}
	QString * text = new QString();
	
	//////////LINK RECORDER CODE///////////////////////////
	if(linkNameLineEdit->text()!="" && recMode)
	{
	
	TestEditor * editor = m_TestEditor;
	QListViewItem * item = new QListViewItem(editor);
	QString numberText;
	int count = editor->childCount();
	if(count < 10)
		numberText.append("0");
	if(count<100)
		numberText.append("0");
	numberText.append(QString::number(editor->childCount()));
	item->setText(0, numberText );
	item->setText(1, "URL");
	item->setText(2,linkNameLineEdit->text());
	item->setText(4,assertionLineEdit->text());

  }
  /////////////////END LINK RECORDER///////////////////////////////
  text->append(linkNameLineEdit->text());    
  DOM::HTMLDocument * doc = new DOM::HTMLDocument(m_part->htmlDocument());
  DOM::HTMLCollection  collection = doc->links();

  text->append( QString("\n") + QString("Collection Length:") + QString::number(collection.length()));
  for(unsigned long i = 0; i< collection.length(); i ++)
  {
    DOM::Node node = collection.item( i);
    DOM::DOMString domStringName = node.nodeName();
    QString stringName = domStringName.string();
    DOM::HTMLLinkElement * htmlLinkEle = (DOM::HTMLLinkElement*)&(node);

    if(htmlLinkEle->href().string().contains("#"))
    {
      QString jText = htmlLinkEle->getAttribute(DOM::DOMString(QString("onclick"))).string();
    }
    exploreDomNode(node, text);
  }
  DOM::HTMLCollection  collectionImg = doc->images();
  ////qWarning("Number of images: %d", (int)collectionImg.length());
  for(unsigned long i = 0; i< collectionImg.length(); i ++)
  {
    DOM::Node nodeImg = collection.item( i);
    if(nodeImg.isNull())
    {	
      ////qWarning("Invalid node # %d",(int)i);
      continue;
    }
    DOM::Node actualNode = nodeImg.firstChild();
    ////qWarning("Child Node Name %s", actualNode.nodeName().string().ascii());
    if(actualNode.nodeName().string()=="IMG")
    {
      DOM::Element targetElement(actualNode);
      ////qWarning("title: %s", targetElement.getAttribute("title").string().ascii());
      if(targetElement.getAttribute("title").string()==linkNameLineEdit->text())
      {
        DOM::Element nodeImgEle(nodeImg);
        if(nodeImgEle.hasAttribute("onclick"))
          m_part->executeScript(nodeImgEle.getAttribute("onclick").string());
      }
    }
  }
  delete text;
  linkNameLineEdit->setText("");
}

TestEditor * JsRunner::testEditor() const
{
    return m_TestEditor;
}

void JsRunner::exploreDomNode( DOM::Node node , QString * text)
{
  if(node.isNull())
  {
    return;
  }
  repeat = false;
  DOM::NodeList childNodeList = node.childNodes();
  if(childNodeList.length()==0)
    return;
  for( unsigned long i = 0 ; i < childNodeList.length() ; i ++)
  {
    QString childString;
    DOM::Node childNode = childNodeList.item(i);
    if(childNode.nodeValue().string() == linkNameLineEdit->text())
    {
      DOM::HTMLLinkElement * htmlLinkEle = (DOM::HTMLLinkElement*)&(node);
      QString jText = htmlLinkEle->getAttribute(DOM::DOMString(QString("onclick"))).string();
      if(jText!= QString::null)
      {
        m_part->executeScript( jText);
      }
      else
      {
        QString href = htmlLinkEle->href().string();
        m_part->openURL(href);
      }
      text->append("\nSuccessful click on:") + linkNameLineEdit->text();
      return;
    }
    else
    {
      repeat = true;
    }
    exploreDomNode(childNode, text);
  }
}

void JsRunner::executeScriptFile()
{
}

void JsRunner::executeClick()
{
    if(!m_part)
    {
        ////qWarning("Invalid part: Aborting");
        return;
    }
  m_part->executeScript( buttonOnClick);
}

void JsRunner::click( QString linkToClick)
{
	linkNameLineEdit->setText(linkToClick);
	executeLinks();  
  
}

void JsRunner::setTestEditor( TestEditor * editor)
{
	m_TestEditor=editor;
}

MagicButton::MagicButton(QWidget *parent, QString name, DOM::HTMLInputElement inputEle, KHTMLPart *part, JsRunner * runner)
  :QPushButton(parent)
{
	m_part = part;
	m_inputEle = inputEle;
	m_parent=parent;
	m_runner=runner;
	connect(this,SIGNAL(clicked()),this,SLOT(run()));
	setText(name);
  
}

MagicButton::~ MagicButton()
{
  
}
void MagicButton::run()
{
	////qWarning("Clicking on %s", m_inputEle.name().string().ascii());
	//we must add the the button to the TestEditor
	TestEditor * editor = m_runner->testEditor();
	QString value = m_inputEle.value().string();
	QListViewItem * item = new QListViewItem(editor);
	QString numberText;
	int count = editor->childCount();
	if(count < 10)
	numberText.append("0");
	if(count<100)
	numberText.append("0");
	numberText.append(QString::number(editor->childCount()));
	item->setText(0, numberText );
	item->setText(1, "BUTTON");
	item->setText(2,"NULL");
	item->setText(3,value);
	item->setText(4,m_runner->assertionLineEdit->text());
	
	
	m_inputEle.click(); 
	m_parent->hide();
}

MagicLineEdit::MagicLineEdit(QWidget *parent, DOM::HTMLInputElement inputEle, KHTMLPart * part, JsRunner *runner, QListViewItem *item)
  :QLineEdit(parent)
{
	m_inputEle=inputEle;
	m_part=part;
	m_runner=runner;
	m_Item=item;
	connect(this, SIGNAL(textChanged( const QString& )), this, SLOT(change( const QString& )));
	connect(m_part,SIGNAL(nodeActivated( const DOM::Node& )),this,SLOT(checkForChanges( const DOM::Node& )));
	setText(inputEle.value().string());
	QString onChangeJS = inputEle.getAttribute("onchange").string();
	part->executeScript(onChangeJS);
  //qWarning("JS Code: %s", onChangeJS.ascii());
}

MagicLineEdit::~ MagicLineEdit()
{
  
}

void MagicLineEdit::change(const QString & text)
{
  m_inputEle.setValue( text);
  if(!m_Item)
  {
      qWarning("illegal m_item");
      return;
  }
  QString onChangeJS = m_inputEle.getAttribute("onchange").string();
  m_part->executeScript(onChangeJS);
  //qWarning("JS Code: %s", onChangeJS.ascii());
  m_Item->setText(3,text);
}

void MagicLineEdit::checkForChanges( const DOM::Node & )
{
 	setText(m_inputEle.value().string());
}
