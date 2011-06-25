#ifndef DEBUGAGENT_H
#define DEBUGAGENT_H

#include <QThread>
#include "v8-debug.h"

class DebugAgent : public QThread
{
    Q_OBJECT
public:
    explicit DebugAgent(QObject *parent = 0);
    ~DebugAgent();

signals:
    void response(QString);

public slots:
    void sendRequest(QString request);

private:
    virtual void run();
    void debuggerMessage(const v8::Debug::Message& message);

private:
    v8::internal::Isolate *m_isolate;

    friend void DebugAgentMessageHandler(const v8::Debug::Message& message);
};

#endif // DEBUGAGENT_H
