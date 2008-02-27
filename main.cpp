
#include "browser.h"
#include "teststate.h"
#define VERSION "0.5"
BrowserApp * browser;
static const char *description =
	I18N_NOOP("KWebTest: A simple testing tool for functional web testing");

	
static KCmdLineOptions options[] =
{
  { "+[File]", I18N_NOOP("file to open"), 0 },
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{
	KAboutData aboutData( "KWebTest", I18N_NOOP("KWebTest"),
			VERSION, description, KAboutData::License_GPL,
			"(c) 2006, Manish Chakravarty", 0, 0, "");
	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.
  
	KApplication app;


	if (app.isRestored())
	{
		RESTORE(BrowserApp);
	}
	else 
	{
		BrowserApp * brws = new BrowserApp;
		brws->show();
        browser=brws;
        
	}
	return app.exec();
}


