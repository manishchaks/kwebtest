#ifndef BROWSERVIEW_H
#define BROWSERVIEW_H
#include "kwebtestincludes.h"
#include "JsRunner.h"
class BrowserDoc;
class BrowserApp;

/**
\ingroup MainModule
\class BrowserApp
\brief Provides the view for the browser 
*/
class BrowserView : public QWidget
{
Q_OBJECT

	KHTMLPart *htmlPart;
	KURL at;

public:
	BrowserView(QWidget *parent = 0, const char *name=0);
	~BrowserView();
	KHTMLPart * part();
	BrowserApp* parentApp();
	void setParentApp( BrowserApp * app);
	JsRunner * runner;

public slots:
	void openURL(const KURL &url);

signals:
	void progress(BrowserView *, int percent);
	void loading(BrowserView *, const KURL &url);
	void loaded(BrowserView *, const KURL &url);

private slots:
	void openURLRequestDelayed( const KURL&, const KParts::URLArgs &);
	void done();
	void progressChanged(int);
private:
	void atUrl(const KURL &url);


BrowserApp * m_parentApp;
};

#endif // BROWSERVIEW_H

