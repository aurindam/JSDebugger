#include <QFileDialog>
#include <QTextStream>
#include "filebrowser.h"

File::File(QWidget *parent) :
    QWidget(parent)
{
}

void File::browse()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), "/home", tr("JS files (*.js)"));

    readFile(fileName);
}

void File::readFile(QString &fileName)
{
    //Check if user has selected a file
    if (fileName.isNull())
        return;

    fileName.replace('\\','/');
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    if (!m_contents.length()) {
        m_contents.clear();
        m_lines = 0;
    }

    QString line;
    do {
         line = in.readLine();
         m_contents.append(line);
         m_contents.append(QString('\n'));
         m_lines++;
    } while (!line.isNull());

    emit fileLoaded();
}
