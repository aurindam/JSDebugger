#include <QTextStream>

#include "v8.h"
#include "debuggeragent.h"


void DebuggerAgentMessageHandler(const v8::Debug::Message& message) {
    static_cast<DebuggerAgent*>(QThread::currentThread())->debuggerMessage(message);
}

DebuggerAgent::DebuggerAgent(QObject *parent) :
    QThread(parent)
{

}

DebuggerAgent::~DebuggerAgent()
{
}

void DebuggerAgent::sendRequest(QString request)
{
    // Send the request received to the debugger.
    v8::Debug::SendCommand(request.utf16(), request.length());
}

void DebuggerAgent::run()
{
    v8::Debug::SetMessageHandler2(DebuggerAgentMessageHandler);
    exec();
}

void DebuggerAgent::debuggerMessage(const v8::Debug::Message& message)
{
    v8::String::Value val(message.GetJSON());

    QString result;
    QTextStream in(&result, QIODevice::WriteOnly);
    in << const_cast<uint16_t*>(*val);

    emit response(result);
}
