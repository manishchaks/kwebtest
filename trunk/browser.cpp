#include "kwebtestincludes.h"
#include "browser.h"
#include "browserview.h"
#include "browser.h"
#include "JsRunner.h"
#include "testWindow.h"
#include <qpixmap.h>

BrowserApp::BrowserApp(const char* name)
	: KDockMainWindow(0, name)
{
	
    	initActions();
   	m_runner=0;
    //qWarning("In constructor ");
	KDockWidget * mainWidget;
	mainWidget = createDockWidget("Main Widget",QPixmap(),0L, "contains a layout");
	QVBox *layout = new QVBox(mainWidget);
	mainWidget->setWidget(layout);
        mainWidget->setDockSite(KDockWidget::DockCorner);
  	// forbit docking abilities of mainDock itself
	mainWidget->setEnableDocking(KDockWidget::DockNone);
	setView( mainWidget); // central widget in a KDE mainwindow
	setMainDockWidget( mainWidget); // master dockwidget

	m_runner = new JsRunner();

	urlBox = new KLineEdit(layout);
	connect(urlBox, SIGNAL(returnPressed()), SLOT(urlEntered()));
	
	tabs = new QTabWidget(layout);
	progressBar = new QProgressBar(100, statusBar());
	progressBar->hide();
	progressBar->setFixedWidth(100);
	statusBar()->addWidget(progressBar, 0, true);
 	createTab();

	KDockWidget * dockLog = createDockWidget("Log Window Dock Widget",QPixmap(),0L, "Log Window dock widget");
	textEdit = new QTextEdit(dockLog);
	dockLog->setWidget(textEdit);
	dockLog->manualDock( mainWidget,              // dock target
			      KDockWidget::DockLeft, // dock site
			      20 );			
	
	KDockWidget * dockJS = createDockWidget("JS Window Dock Widget",QPixmap(),0L, "JS Window dock widget");
	dockJS->setWidget(m_runner);
	dockJS->manualDock( dockLog,              // dock target
			     KDockWidget::DockBottom, // dock site
			     20 );
   	textEdit->setText( "JS Window : KWebTest");
	post=false;
	
	KDockWidget * dockTest = createDockWidget("Test Window",QPixmap(),0L, "TestWindow");
	m_TestWindow= new TestWindow(dockTest, m_runner);
	dockTest->setWidget(m_TestWindow);
	dockTest->manualDock( dockJS,KDockWidget::DockBottom);
	m_runner->setTestEditor( m_TestWindow->testEditor());



	slotConnected =false;
	
}


BrowserApp::~BrowserApp()
{
	//delete textEdit;
}
void BrowserApp::initActions()
{
	new KAction(
			i18n("New &Tab"), "tabnew", KShortcut(),
	this, SLOT(createTab()),
	actionCollection(), "file_new_tab"
		   );
	KStdAction::quit(kapp, SLOT(quit()), actionCollection());
	createGUI();
    
}

void BrowserApp::urlEntered()
{
	KURL url(urlBox->text());
	if (url.protocol().length() == 0)
	{
		url = KURL("http://" + urlBox->text());
	}
	
	current()->openURL(url);
}

BrowserView *BrowserApp::current()
{
	return static_cast<BrowserView*>(tabs->currentPage());
}

void BrowserApp::createTab()
{

    //qWarning("Creating tabs");
    BrowserView *view = new BrowserView(this);
	view->setParentApp( this);
	connect(view, SIGNAL(progress(BrowserView *, int)), SLOT(progress(BrowserView *, int)));
	connect(view, SIGNAL(loading(BrowserView *, const KURL &)), SLOT(loading(BrowserView *, const KURL&)));
	connect(view, SIGNAL(loaded(BrowserView *, const KURL&)), SLOT(loaded(BrowserView *, const KURL&)));
    KHTMLPart *hPart = view->part();
    connect(hPart, SIGNAL(formSubmitNotification(   const char *,
		const QString &, 
		const QByteArray &, 
		const QString &, 
		const QString &, 
		const QString &)
			     ),
		this ,
		SLOT(formSubmitted(   const char *, 
		     const QString &, 
		     const QByteArray &, 
		     const QString &, 
		     const QString &, 
		     const QString &)
		    ));
	tabs->addTab(view, i18n("Untitled"));
}

void BrowserApp::progress(BrowserView *view, int percent)
{
	if (current() == view)
	{
		progressBar->setProgress(percent);
	}
}

void BrowserApp::loading(BrowserView *view, const KURL &url)
{

	if (current() == view)
	{
		progressBar->show();
		statusBar()->message(i18n("Loading from host %1").arg(url.host()));
		urlBox->setText(url.prettyURL());
        //m_runner->setPart( view->part());
	}
}

void BrowserApp::loaded(BrowserView *view, const KURL &url)
{
	tabs->changeTab(view, url.prettyURL());
	addLog(url.url());
	
	if (current() == view)
	{
		setCaption(url.prettyURL());
		statusBar()->message(i18n("Done."));
		progressBar->hide();
        m_runner->setPart(current()->part());
        if(!slotConnected)
        {
            connect(current(),SIGNAL(loaded( BrowserView*, const KURL& )),m_TestWindow,SLOT(pageLoaded()));
            slotConnected=true;
        }
		qWarning ("loaded");
	}
	post = false;
	urlBox->setText(url.url());

}


void BrowserApp::addLog(const QString & string)
{
	this->logString += "\n";
	this->logString += "URL:";
	this->logString += string;
	textEdit->setText(this->logString);
	progressBar->setFixedWidth(100);    
	progressBar->setFixedWidth(100);
}   



void BrowserApp::formSubmitted(   const char *   action, 
				  const QString &   url, 
				  const QByteArray &   formData, 
				  const QString &   target, 
				  const QString &   contentType, 
				  const QString &   boundary)
{
	post = true;
	//qWarning( "formSubmitNotification");
	this->logString += "\n";
	this->logString += "Action:";
	this->logString += QString(action);
	this->logString += "\n";
	this->logString += "URL:";
	this->logString += url;
	this->logString += "\n";
	this->logString += "FormData";
	QString fData = QString (formData);
	fData.replace('&','\n');

	this->logString += "\n====BEGIN FORM DATA==\n";
	this->logString +=  fData;
	this->logString += "\n====END FORM DATA==\n";
	this->logString += "\n";
	this->logString += "Target";
	this->logString += target;
	this->logString += "\n";
	this->logString += "contentType:";
	this->logString += contentType;
	this->logString += "\n";
	this->logString += "boundary";
	this->logString += boundary;
}

void BrowserApp::urlStatusNotification(const QString & url)
{
	loaded (0, KURL (url));
}
bool BrowserApp::doingPostRequest()
{
	return post;
}

JsRunner * BrowserApp::runner() const
{
    return m_runner;
}