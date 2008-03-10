#ifndef JS_RUNNER_H_INCLUDED
#define JS_RUNNER_H_INCLUDED

#include "kwebtestincludes.h"
class TestEditor;

/**
\ingroup MainModule
\class BrowserApp
\brief This class runs the test

This is the most important class in KWebTest. This class runs and records tests.
*/
class JsRunner : public QWidget

{
  Q_OBJECT
  public:
	JsRunner();
	~JsRunner();
	
	void setPart( KHTMLPart * partToWatch);
	void setTestEditor(TestEditor *editor);
	TestEditor * testEditor() const;
	bool isRecording() const;
	void setIsRecording ( bool value) ;
	KHTMLPart * part();
	QLineEdit * assertionLineEdit;
public slots:
	void executeForms();
	void executeLinks();
	void executeScriptFile();
	void executeClick();
	void initialWidgetState();
	void click(QString linkToClick);
  private:
	void exploreDomNode( DOM::Node node , QString * text);
	QLabel * linkName;
	QLineEdit * linkNameLineEdit;
	
	QLabel *assertionLabel;
	QPushButton * formPushButton;
	QPushButton * linkPushButton;
	QLabel * executedJavaScript;
	QListView * formTree;
	KHTMLPart * m_part;
	QVBoxLayout * layout;
	bool repeat;
	bool clickSuccess;
	QWidget * console;
	QVBoxLayout * consoleLayout;
	QLabel * helpLabel;
	QString buttonOnClick;
	TestEditor *m_TestEditor;
	bool recMode;
};

class MagicButton : public QPushButton
{
  Q_OBJECT
  public:
	MagicButton(QWidget *parent, QString name, DOM::HTMLInputElement inputEle, KHTMLPart *part, JsRunner * runner);
	~MagicButton();
  public slots:
	void run();
  private:
	KHTMLPart *m_part;
	DOM::HTMLInputElement m_inputEle;
	QWidget * m_parent;
	QListViewItem *m_Item;
	JsRunner *m_runner;
};

class MagicLineEdit: public QLineEdit
{
  Q_OBJECT
  public:
	MagicLineEdit(QWidget *parent, DOM::HTMLInputElement, KHTMLPart * part, JsRunner * runner, QListViewItem *item);
	~MagicLineEdit();
    
  public slots:
	void change(const QString &text);
	void checkForChanges(const DOM::Node &);
    
  private:
	DOM::HTMLInputElement m_inputEle;
	KHTMLPart * m_part;
	QListViewItem * m_Item;
	JsRunner *m_runner;
};


#endif
