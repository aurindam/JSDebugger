#ifndef DebuggerAgent_H
#define DebuggerAgent_H

#include <QObject>
#include "v8-debug.h"

class DebuggerAgent : public QObject
{
    Q_OBJECT
public:
    static DebuggerAgent* instance();

    void release();

signals:
    void response(QString);

public slots:
    void sendRequest(QString request);

private:
    DebuggerAgent(QObject *parent = 0);
    ~DebuggerAgent();
    void debuggerMessage(const v8::Debug::Message& message);

private:
    int m_refCount;
    static DebuggerAgent* m_agent;

    friend void DebuggerAgentMessageHandler(const v8::Debug::Message& message);
};

#endif // DebuggerAgent_H


