#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QAction>
#include <QMenu>
#include <QByteArray>
#include <QIcon>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QEventLoop>
#include <QProgressBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QLineEdit>
#include <QBrush>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QByteArray encryption(const QByteArray &data, int key);
    QByteArray downLoadByteArray(const QString &Url);
    void setMenuActions(QMenu *menu, const QByteArray &bytesData);

    void loadTreeTitle(const QByteArray &fileName, const QString &header);

    void searchChild(QTreeWidgetItem *cItem, const QString &txt);

    void parentExpand(QTreeWidgetItem *item);

    void treeItemSearch(QTreeWidget * treewidget, QLineEdit *edit);


private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
