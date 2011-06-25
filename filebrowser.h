#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QWidget>

class File : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int m_lines READ lines)
    Q_PROPERTY(QString m_contents READ contents)

public:
    explicit File(QWidget *parent = 0);

signals:
    void fileLoaded();

public slots:
    void browse();
    int lines() const { return m_lines; }
    QString contents() const { return m_contents; }

private:
    void readFile(QString& fileName);


private:
    int m_lines;
    QString m_contents;
};

#endif // FILEDIALOG_H
