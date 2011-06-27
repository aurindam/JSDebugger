#include <QTextStream>
#include <QDebug>

#include "v8.h"
#include "debuggeragent.h"

DebuggerAgent *DebuggerAgent::m_agent = 0;

void DebuggerAgentMessageHandler(const v8::Debug::Message& message) {
    DebuggerAgent* agent = DebuggerAgent::instance();
    Q_ASSERT(agent != NULL);
    agent->debuggerMessage(message);
}

DebuggerAgent* DebuggerAgent::instance()
{
    if (!m_agent) {
        m_agent = new DebuggerAgent;
    }
    m_agent->m_refCount++;
    return m_agent;
}

void DebuggerAgent::release()
{
    m_refCount--;
    if (!m_refCount)
        delete this;
}

DebuggerAgent::DebuggerAgent(QObject *parent) :
    QObject(parent), m_refCount(0)
{
    v8::Debug::SetMessageHandler2(DebuggerAgentMessageHandler);
}

DebuggerAgent::~DebuggerAgent()
{
}

void DebuggerAgent::sendRequest(QString request)
{
    // Send the request received to the debugger.
    v8::Debug::SendCommand(request.utf16(), request.length());
}

void DebuggerAgent::debuggerMessage(const v8::Debug::Message& message)
{
    v8::Handle<v8::String> val(message.GetJSON());

    QString result;
    QTextStream in(&result, QIODevice::WriteOnly);
    in << *(v8::String::AsciiValue(val));/*const_cast<uint16_t*>(*val);*/

    qDebug() << result;
    emit response(result);
}
