#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QWidget>
#include <QDeclarativeItem>

class FileBrowser : public QWidget
{
public:
    explicit FileBrowser(QWidget *parent = 0);
    QString openFile();
};

class FileHandler : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY (int m_lines READ lines)
    Q_PROPERTY (QString m_filepath READ filepath)
    Q_PROPERTY (QString m_contents READ contents)

public:
    explicit FileHandler(QDeclarativeItem *parent = 0);
    ~FileHandler();

signals:
    void fileLoaded();

public slots:
    void openFile();
    QString filename(QString filepath);
    int lines() { return m_lines; }
    QString filepath() { return m_filepath; }
    QString contents() { return m_contents; }

private:
    FileBrowser *m_fileBrowser;
    int m_lines;
    QString m_filepath;
    QString m_contents;
};

#endif // FILEHANDLER_H
