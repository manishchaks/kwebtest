
#include "browserview.h"
#include "JsRunner.h"
/**
* @brief Constructor
* @param parent The Parent of the current widget
* @param name The name of the widget
* @return none
*/
BrowserView::BrowserView(QWidget *parent, const char *name)
	: QWidget(parent, name)
{

	(new QVBoxLayout(this))->setAutoAdd(true);

	htmlPart = new KHTMLPart(this);
    //htmlPart->setPluginsEnabled( false);
	KURL url = "http://google.com";

//set the properties for the HTML Part
// we want form notifications to occur after the form has been submitted
	htmlPart->setFormNotification(KHTMLPart::Before);

	connect(
			htmlPart->browserExtension(), SIGNAL(loadingProgress(int)),
	SLOT(progressChanged(int))
	);

	connect(
			htmlPart->browserExtension(),
	SIGNAL(openURLRequestDelayed( const KURL&, const KParts::URLArgs&)),
	SLOT(openURLRequestDelayed( const KURL&, const KParts::URLArgs&))
	);
	connect(htmlPart, SIGNAL(completed()), SLOT(done()));

	this->setCaption( "Main Window : KWebTest");
	htmlPart->openURL(url);

}

BrowserView::~BrowserView()
{
}

void BrowserView::openURL(const KURL &url)
{
	at = url;
	loading(this, url);
	
	htmlPart->openURL( url);
	//qWarning(url.url().data());
}


void BrowserView::done()
{   
	emit loaded(this, at);

}


void BrowserView::openURLRequestDelayed(
		const KURL &url, const KParts::URLArgs &args
				)
{
	htmlPart->browserExtension()->setURLArgs(args);
	openURL(url);


}

void BrowserView::progressChanged(int percent)
{
	emit progress(this, percent);


}

KHTMLPart * BrowserView::part()
{
	return this->htmlPart;
}

BrowserApp * BrowserView::parentApp()
{
	return m_parentApp;
}

void BrowserView::setParentApp( BrowserApp *p)
{
	m_parentApp = p;
}

