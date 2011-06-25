#ifndef DEBUGAGENT_H
#define DEBUGAGENT_H

#include <QThread>
#include "v8.h"

class DebuggerAgent;
class DebugService : public QThread
{
    Q_OBJECT

public:
    explicit DebugService(QObject *parent = 0);
    ~DebugService();

signals:

public slots:
    bool setupExecutionEnvironment(QString& scriptSource);
    void response(QString result);

private:
    void run();
    void readSource(QString& scriptSource);
    void reportException(v8::TryCatch* try_catch);

private:
    v8::Handle<v8::String> m_scriptSource;
    v8::Handle<v8::Value> m_scriptName;
    v8::Persistent<v8::Context> m_debugContext;
    DebuggerAgent *m_agent;
};

#endif // DEBUGAGENT_H
