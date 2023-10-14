#include "errorhelper.h"

void SendErrorEvent(std::string error)
{
	std::shared_ptr<ErrorEventArgs> args = std::make_shared<ErrorEventArgs>();
	args->message = "[-]: "+error;
	EventHandler::FireEvent(EventType::ERROR_GOT, args);
}