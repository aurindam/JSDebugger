#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "debugservice.h"
#include "debuggeragent.h"
#include "v8.h"

const char* ToCString(const v8::String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}

v8::Handle<v8::Value> print(const v8::Arguments& args) {
    bool first = true;
    for (int i = 0; i < args.Length(); i++) {
        v8::HandleScope handle_scope;
        if (first) {
            first = false;
        } else {
            qDebug() << " ";
        }
        v8::String::Utf8Value str(args[i]);
        qDebug() << ToCString(str);
    }
    return v8::Undefined();
}

DebugService::DebugService(QObject *parent):
    QThread(parent), m_sequenceNumber(1)
{
    m_agent = DebuggerAgent::instance();
    connect(m_agent, SIGNAL(response(QString)), this, SLOT(response(QString)));
}

DebugService::~DebugService()
{
    v8::V8::Dispose();
}

void DebugService::setScriptPath(QString scriptPath)
{
    m_scriptPath = scriptPath;
}

bool DebugService::runScript()
{

    QFile file(m_scriptPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray contents(file.readAll());

    v8::Isolate *isolate = v8::Isolate::New();
    isolate->Enter();

    v8::HandleScope handleScope;

    v8::Handle<v8::String> scriptSource = v8::String::New(contents.data(), contents.length());

    if (scriptSource.IsEmpty())
        return false;

    v8::Handle<v8::Value> scriptName = v8::String::New(QFileInfo(m_scriptPath).fileName().toUtf8().constData());

    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    global->Set(v8::String::New("print"), v8::FunctionTemplate::New(print));

    // Create a new context.
    m_debugContext = v8::Context::New();

    // Enter the created context for compiling and
    // running the hello world script.
    v8::Context::Scope context_scope(m_debugContext);

//    v8::Handle<v8::Context> context = v8::Context::New(NULL, global);
//    // Enter the newly created execution environment.
//    v8::Context::Scope context_scope(context);

//    m_debugContext = v8::Persistent<v8::Context>::New(context);

  //  v8::Locker locker;

    bool report_exceptions = true;

    v8::Handle<v8::Script> script;
    {
        // Compile script in try/catch context.
     //   v8::TryCatch try_catch;
        script = v8::Script::Compile(scriptSource, scriptName);
        if (script.IsEmpty()) {
            // Print errors that happened during compilation.
//            if (report_exceptions)
//                reportException(&try_catch);
            return false;
        }
    }

    {
        v8::TryCatch try_catch;

        script->Run();
        if (try_catch.HasCaught()) {
            if (report_exceptions)
                reportException(&try_catch);
            return false;
        }
    }

    //v8::Debug::DebugBreak();

    runMain(/*script,v8::Context::GetCurrent(),report_exceptions*/);

    v8::Debug::CancelDebugBreak();

    //isolate->Exit();
    //isolate->Dispose();

}

bool DebugService::runMain(/*v8::Handle<v8::Script> script, v8::Local<v8::Context> context,
                           bool report_exceptions*/) {
    //v8::Locker lock;

    v8::Handle<v8::String> fun_name = v8::String::New("main");
    v8::Handle<v8::Value> process_val =
            m_debugContext->Global()->Get(fun_name);

    if (!process_val->IsFunction()) {
        qDebug() << "Error: Script does not declare 'ProcessLine' global function.\n";
        return false;
    }

    // It is a function; cast it to a Function
    v8::Handle<v8::Function> process_fun =
            v8::Handle<v8::Function>::Cast(process_val);

    v8::HandleScope handle_scope;

    const int argc = 0;
    v8::Handle<v8::Value> argv[argc];

    v8::Handle<v8::Value> result;
    {
        v8::TryCatch try_catch;
        result = process_fun->Call(m_debugContext->Global(),
                                   argc, argv);
        if (try_catch.HasCaught()) {
//            if (report_exceptions)
//                reportException(&try_catch);
            return false;
        }
    }
    v8::String::Utf8Value str(result);
    qDebug() << ToCString(str);

    return true;
}

void DebugService::reportException(v8::TryCatch* try_catch) {
        v8::HandleScope handle_scope;
        v8::String::Utf8Value exception(try_catch->Exception());
        const char* exception_string = ToCString(exception);
        v8::Handle<v8::Message> message = try_catch->Message();
        if (message.IsEmpty()) {
            // V8 didn't provide any extra information about this error; just
            // print the exception.
            qDebug() << exception_string;
        } else {
            // Print (filename):(line number): (message).
            v8::String::Utf8Value filename(message->GetScriptResourceName());
            const char* filename_string = ToCString(filename);
            int linenum = message->GetLineNumber();
            qDebug() << filename_string << linenum << exception_string;

            // Print line of source code.
            v8::String::Utf8Value sourceline(message->GetSourceLine());

            qDebug() << ToCString(sourceline);
//            printf("%s\n", sourceline_string);
//            // Print wavy underline (GetUnderline is deprecated).
//            int start = message->GetStartColumn();
//            for (int i = 0; i < start; i++) {
//                printf(" ");
//            }
//            int end = message->GetEndColumn();
//            for (int i = start; i < end; i++) {
//                printf("^");
//            }
//            printf("\n");
        }
}

void DebugService::response(QString result)
{

}

void DebugService::executeCommand(QVariantMap args)
{
    QString params;
    QTextStream paramsStream( &params, QIODevice::WriteOnly);

    paramsStream << QString("{\"seq\":") << m_sequenceNumber << QString(",\"type\":\"request\",");

    createMessage(paramsStream, args);

    qDebug() << params;

    m_agent->sendRequest(params);
}

void DebugService::createMessage(QTextStream& params, QVariantMap args)
{
    bool isFirst = true;
    foreach( QVariant variant, args) {

        if (!isFirst)
            params << ",";

        isFirst = false;

        if (variant.type() == QVariant::String) {
            params << "\"" << args.key(variant) << "\":\"" << variant.toString() << "\"";
        }

        if (variant.type() == QVariant::Int) {
            params << "\"" << args.key(variant) << "\":" << variant.toInt();
        }

        if (variant.type() == QVariant::Map) {
            params << "\"arguments\":{";
            createMessage(params,variant.toMap());
        }
    }

    params << "}" ;
}

void DebugService::run()
{
   // mutex.lock();
    runScript();
   // mutex.unlock();
    exec();
}
