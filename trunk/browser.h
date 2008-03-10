#ifndef BROWSER_H
#define BROWSER_H
#include "kwebtestincludes.h"
#include "JsRunner.h"
#include "testWindow.h"

class BrowserView;

/**
\ingroup MainModule
\class BrowserApp
\brief Represents the browser running within KWebTest

Each instance of this class represents a running browser within KWebTest
*/
class BrowserApp : public KDockMainWindow
{
Q_OBJECT

QTabWidget *tabs;
KLineEdit *urlBox;
QProgressBar *progressBar;

public:
	BrowserApp(const char* name=0);
	~BrowserApp();

	BrowserView *current();
	bool doingPostRequest();

	void addLog(const QString & string);
	JsRunner * runner() const;
public slots:
	void createTab();
	void urlStatusNotification(const QString &);

private slots:
	void progress(BrowserView *, int percent);
	void loading(BrowserView *, const KURL &url);
	void loaded(BrowserView *, const KURL &url);
	void urlEntered();
	void formSubmitted(   const char *   action, 
				const QString &   url, 
				const QByteArray &   formData, 
				const QString &   target, 
				const QString &   contentType, 
				const QString &   boundary);

private:
	void initActions();
	QTextEdit *textEdit;
	QString logString;
	bool post;
	JsRunner *m_runner;
	TestWindow *m_TestWindow;
	bool slotConnected;
};
#endif // BROWSER_H

