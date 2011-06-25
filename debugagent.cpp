#include <QTextStream>

#include "v8.h"
#include "debugagent.h"


void DebugAgentMessageHandler(const v8::Debug::Message& message) {
    static_cast<DebugAgent*>(QThread::currentThread())->debuggerMessage(message);
}

DebugAgent::DebugAgent(QObject *parent) :
    QThread(parent)
{
}

DebugAgent::~DebugAgent()
{
}

void DebugAgent::sendRequest(QString request)
{
    // Send the request received to the debugger.
    v8::Debug::SendCommand(request.utf16(), request.length());
}

void DebugAgent::run()
{
    v8::Debug::SetMessageHandler2(DebugAgentMessageHandler);
    exec();
}

void DebugAgent::debuggerMessage(const v8::Debug::Message& message)
{
    v8::String::Value val(message.GetJSON());

    QString result;
    QTextStream in(&result, QIODevice::WriteOnly);
    in << const_cast<uint16_t*>(*val);

    emit response(result);
}
