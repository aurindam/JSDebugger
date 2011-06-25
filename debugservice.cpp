#include <QFile>
#include <QFileInfo>

#include "debugservice.h"
#include "debuggeragent.h"
#include "v8.h"

v8::Handle<v8::Value> print(const v8::Arguments& args) {
    //    bool first = true;
    //    for (int i = 0; i < args.Length(); i++) {
    //        v8::HandleScope handle_scope;
    //        if (first) {
    //            first = false;
    //        } else {
    //            printf(" ");
    //        }
    //        v8::String::Utf8Value str(args[i]);
    //        const char* cstr = ToCString(str);
    //        printf("%s", cstr);
    //    }
    //    printf("\n");
    //    fflush(stdout);
        return v8::Undefined();
}

DebugService::DebugService(QObject *parent):
    QThread(parent), m_scriptSource(0), m_scriptName(0)
{
    m_agent = new DebuggerAgent(this);
    connect(m_agent, SIGNAL(response(QString)), this, SLOT(response(QString)));
}

DebugService::~DebugService()
{
    v8::V8::Dispose();
}

bool DebugService::setupExecutionEnvironment(QString& scriptSource)
{
    QFile file(scriptSource);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray contents(file.readAll());

    m_scriptSource = v8::String::New(contents.data(), contents.length());
    if (m_scriptSource.IsEmpty())
        return false;

    m_scriptName = v8::String::New(QFileInfo(scriptSource).fileName().toUtf8().constData());

    return true;
}

void DebugService::run()
{
    v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

    global->Set(v8::String::New("print"), v8::FunctionTemplate::New(print));

    v8::Handle<v8::Context> context = v8::Context::New(NULL, global);
    // Enter the newly created execution environment.
    v8::Context::Scope context_scope(context);

    m_debugContext = v8::Persistent<v8::Context>::New(context);

    v8::Locker locker;

    bool report_exceptions = true;

    v8::Handle<v8::Script> script;
    {
        // Compile script in try/catch context.
        v8::TryCatch try_catch;
        script = v8::Script::Compile(m_scriptSource, m_scriptName);
        if (script.IsEmpty()) {
            // Print errors that happened during compilation.
            if (report_exceptions)
                reportException(&try_catch);
            return;
        }
    }

    {
        v8::TryCatch try_catch;

        script->Run();
        if (try_catch.HasCaught()) {
            if (report_exceptions)
                reportException(&try_catch);
            return;
        }
    }
    exec();
}


const char* ToCString(const v8::String::Utf8Value& value) {
    return *value ? *value : "<string conversion failed>";
}



void DebugService::reportException(v8::TryCatch* try_catch) {
//    v8::HandleScope handle_scope;
//    v8::String::Utf8Value exception(try_catch->Exception());
//    const char* exception_string = ToCString(exception);
//    v8::Handle<v8::Message> message = try_catch->Message();
//    if (message.IsEmpty()) {
//        // V8 didn't provide any extra information about this error; just
//        // print the exception.
//        printf("%s\n", exception_string);
//    } else {
//        // Print (filename):(line number): (message).
//        v8::String::Utf8Value filename(message->GetScriptResourceName());
//        const char* filename_string = ToCString(filename);
//        int linenum = message->GetLineNumber();
//        printf("%s:%i: %s\n", filename_string, linenum, exception_string);
//        // Print line of source code.
//        v8::String::Utf8Value sourceline(message->GetSourceLine());
//        const char* sourceline_string = ToCString(sourceline);
//        printf("%s\n", sourceline_string);
//        // Print wavy underline (GetUnderline is deprecated).
//        int start = message->GetStartColumn();
//        for (int i = 0; i < start; i++) {
//            printf(" ");
//        }
//        int end = message->GetEndColumn();
//        for (int i = start; i < end; i++) {
//            printf("^");
//        }
//        printf("\n");
//    }
}

void DebugService::response(QString result)
{

}
