#ifndef DEBUGAGENT_H
#define DEBUGAGENT_H

#include <QThread>
#include <QVariantMap>
#include <QTextStream>
#include <QMutex>

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
    void setScriptPath(QString scriptPath);
    bool runScript();
    void response(QString result);
    void executeCommand(QVariantMap args);

    bool runMain(/*v8::Handle<v8::Script> script, v8::Local<v8::Context> context,
                 bool report_exceptions*/);
private:
    virtual void run();

    void reportException(v8::TryCatch* try_catch);
    void createMessage(QTextStream& params, QVariantMap args);

private:
    v8::Persistent<v8::Context> m_debugContext;
    DebuggerAgent *m_agent;
    int m_sequenceNumber;
    QString m_scriptPath;
    QMutex mutex;
};

#endif // DEBUGAGENT_H
