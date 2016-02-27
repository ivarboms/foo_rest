#include "stdafx.h"
#include "foobar2000/SDK/foobar2000.h"

#include "ServerProxy.h"

DECLARE_COMPONENT_VERSION(
"foo_rest",
"0.1",
"REST interface for foobar.\nhttps://github.com/ivarboms/foo_rest"
)

class InitQuit : public initquit
{
public:
	virtual void on_init();
	virtual void on_quit();
};

std::unique_ptr<ServerProxy> serverProxy;

void InitQuit::on_init()
{
	serverProxy = std::make_unique<ServerProxy>();
}

void InitQuit::on_quit()
{
	serverProxy.release();
}

initquit_factory_t<InitQuit> initQuitFactory;