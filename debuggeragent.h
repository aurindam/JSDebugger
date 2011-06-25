#ifndef DebuggerAgent_H
#define DebuggerAgent_H

#include <QThread>
#include "v8-debug.h"

class DebuggerAgent : public QThread
{
    Q_OBJECT
public:
    explicit DebuggerAgent(QObject *parent = 0);
    ~DebuggerAgent();

signals:
    void response(QString);

public slots:
    void sendRequest(QString request);

private:
    virtual void run();
    void debuggerMessage(const v8::Debug::Message& message);

private:
    v8::internal::Isolate *m_isolate;

    friend void DebuggerAgentMessageHandler(const v8::Debug::Message& message);
};

#endif // DebuggerAgent_H
