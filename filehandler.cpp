#include <QFileDialog>

#include "filehandler.h"

FileBrowser::FileBrowser(QWidget *parent) :
    QWidget(parent)
{
}

QString FileBrowser::openFile()
{
    return QFileDialog::getOpenFileName(this, tr("Open File"), "/Users/ajana/labs/debugger/test", tr ("JS files (*.js)"));
}

FileHandler::FileHandler(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    m_fileBrowser = new FileBrowser();
}

FileHandler::~FileHandler()
{
    delete m_fileBrowser;
}

void FileHandler::openFile()
{
    QString filePath = m_fileBrowser->openFile();

    if (filePath.isNull())
        return;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    if (!m_contents.length()) {
        m_contents.clear();
    }
    m_lines = 0;

    QString line;
    do {
         line = in.readLine();
         m_contents.append(line);
         m_contents.append(QString('\n'));
         m_lines++;
    } while (!line.isNull());

    m_filepath = filePath;

    emit fileLoaded();
}

QString FileHandler::filename(QString filepath)
{
    return QFileInfo(filepath).fileName();
}
