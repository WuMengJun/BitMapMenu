#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("五芯级菜单功能预览");
    setWindowIcon(QIcon(":/wxj.ico"));
    ui->ldtSearchText->setPlaceholderText("输入文本，回车搜索");
    QByteArray bitMapdata = encryption(downLoadByteArray("http://120.25.73.39/V3V4aW5qaVRvdGFsRGF0YQ/jsonfile/menuBitMap/bWVudUJpdE1hcA==.wltemp"),89654385);
    QByteArray multiLayerActiondata = encryption(downLoadByteArray("http://www.wl168888.com/V3V4aW5qaVRvdGFsRGF0YQ/jsonfile/MultiLayerPainter/TXVsdGlMYXllclBhaW50ZXI=.wltemp"),89654385);
    setMenuActions(ui->menuBitMap, bitMapdata);
    setMenuActions(ui->menuMultiLayerPainter, multiLayerActiondata);

    connect(ui->tbnBasic, &QToolButton::clicked, this, [=](){
        loadTreeTitle("basicKnowledge","基础知识");
    });
    connect(ui->tbnMainboard, &QToolButton::clicked, this, [=](){
        loadTreeTitle("mainBoard","主板图");
    });
    connect(ui->tbnSCH, &QToolButton::clicked, this, [=](){
        loadTreeTitle("Principle","原理图");
    });
    connect(ui->tbnBlock, &QToolButton::clicked, this, [=](){
        loadTreeTitle("BLK_Diagram","方框图");
    });
    connect(ui->tbnResistance, &QToolButton::clicked, this, [=](){
        loadTreeTitle("resistance","对地阻值图");
    });
    connect(ui->tbnMTC, &QToolButton::clicked, this, [=](){
        loadTreeTitle("MTCScheme","维修方案");
    });
    connect(ui->ldtSearchText, &QLineEdit::returnPressed, this, [=](){
        treeItemSearch(ui->treeWidget, ui->ldtSearchText);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
QByteArray MainWindow::encryption(const QByteArray &data, int key)
{
    QByteArray byteData = data;
    for(int i=0; i<byteData.size(); i++){
      byteData[i] = byteData[i] ^ key;
    }
    return byteData;
}
QByteArray MainWindow::downLoadByteArray(const QString &Url)
{
    QUrl url(Url);
    QNetworkAccessManager manager;
    QEventLoop loop;
    QProgressBar *progressBar = new QProgressBar();
    QNetworkReply *reply = manager.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::downloadProgress,[=](qint64 bytesReceives, qint64 bytesTotal){
        progressBar->setMaximum(bytesTotal);
        progressBar->setValue(bytesReceives);
        progressBar->show();
        if(bytesTotal == bytesReceives){
            progressBar->close();
        }
    });
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    QByteArray ByteArray = reply->readAll();
    return ByteArray;
}
void MainWindow::setMenuActions( QMenu *menu, const QByteArray &bytesData)
{
    menu->clear();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(bytesData, &error);
    if(error.error == QJsonParseError::NoError){
        QJsonArray array = doc.array();
        QMenu *subMenu;
        QMenu *subSubMenu;
        QJsonObject object;
        QJsonObject subObject;
        QJsonObject subSubObject;
        QJsonArray subArray;
        QJsonArray subSubArray;
        QAction *action;
        QAction *subAction;
        QAction *subSubAction;
        int numI = 1;
        for(int i=0; i<array.size(); i++){
            object = array.at(i).toObject();
            if(!object.value("Menus").isUndefined()){
                subMenu = menu->addMenu(object.value("Action").toString());
                subMenu->setTitle(object.value("Caption").toString());
                subMenu->setIcon(QIcon(":/toolsrc/"+QString::number(numI)+".png"));
                numI++;
                subArray = object.value("Menus").toArray();
                int numJ = 1;
                for(int j=0; j < subArray.size(); j++)
                {
                    subObject = subArray.at(j).toObject();
                    if(subObject.value("Menus").isUndefined()) {
                        if(subObject.value("Action").toString().contains("separator")){
                            subMenu->addSeparator();
                        }else{
                            subAction = subMenu->addAction(subObject.value("Action").toString());
                            subAction->setText(subObject.value("Caption").toString());
                            subAction->setIcon(QIcon(":/toolsrc/"+QString::number(numJ)+".png"));
                            subAction->setIconText(subObject.value("File").toString());
                            numJ++;
                        }
                    }else{
                        subSubMenu = subMenu->addMenu(subObject.value("Action").toString());
                        subSubMenu->setTitle(subObject.value("Caption").toString());
                        subSubMenu->setIcon(QIcon(":/toolsrc/"+QString::number(numJ)+".png"));
                        numJ++;
                        subSubArray = subObject.value("Menus").toArray();
                        int numK = 1;
                        for(int k=0; k< subSubArray.size(); k++){
                            subSubObject = subSubArray.at(k).toObject();
                            if(subSubObject.value("Action").toString().contains("separator")){
                                subSubMenu->addSeparator();
                            }else{
                                subSubAction = subSubMenu->addAction(subSubObject.value("Action").toString());
                                subSubAction->setText(subSubObject.value("Caption").toString());
                                subSubAction->setIcon(QIcon(":/toolsrc/"+QString::number(numK)+".png"));
                                subSubAction->setIconText(subSubObject.value("File").toString());
                                numK++;
                            }
                        }
                    }

                }
            }
            else if(object.value("Action").toString().contains("separator")){
                menu->addSeparator();
            }
            else{
                action = menu->addAction(object.value("Action").toString());
                action->setText(object.value("Caption").toString());
                action->setIcon(QIcon(":/toolsrc/"+QString::number(numI)+".png"));
                action->setIconText(object.value("File").toString());
                numI++;
            }
        }
    } else{
        qDebug() << "error json" << error.errorString();
    }
}

void MainWindow::loadTreeTitle(const QByteArray &fileName, const QString &header)
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel(header);
    QString strFileName = fileName.toBase64();
    QString fileData = encryption(downLoadByteArray(QString("http://www.wl168888.com/V3V4aW5qaVRvdGFsRGF0YQ/systemconfig/%1%2").arg(strFileName).arg(".wltemp")),89654385);
    fileData = fileData.replace("\n","");
    QStringList mylist;
    mylist = fileData.split('{');
    QStringList list;
    QStringList list2;
    for(int i = 0; i < mylist.length(); i++){
        if(i % 2 == 0){
            list << mylist.at(i);
        }else{
            list2 << mylist.at(i);
        }
    }
    QTreeWidgetItem *mainItem;
    QTreeWidgetItem *subItem;
    QTreeWidgetItem *subSubItem;
    QStringList list3;
    QStringList list4;
    for(int i = 0; i < list.length(); i++){
        mainItem = new QTreeWidgetItem(QStringList(list.at(i)));
        list3 = list2.at(i).split("==");
        for(int j = 0; j < list3.length(); j++){
            if(j % 2  == 0){
                subItem = new QTreeWidgetItem(mainItem, QStringList(list3.at(j)));
                mainItem->addChild(subItem);
            }else{
                list4 = list3.at(j).split(';');
                for(int n = 0; n < list4.length(); n = n + 4){
                    subSubItem = new QTreeWidgetItem(subItem,QStringList(list4.at(n)));
                    subItem->addChild(subSubItem);
                }
            }
        }
        ui->treeWidget->addTopLevelItem(mainItem);
    }
}

void MainWindow::searchChild(QTreeWidgetItem *cItem, const QString &txt)
{
    int childCount = cItem->childCount();
    QTreeWidgetItem *item;
    for(int i = 0; i < childCount; ++i) {
        item = cItem->child(i);
        if (-1 != item->text(0).indexOf(txt)) {
            item->setExpanded(true);
            item->setBackground(0,Qt::green);
            parentExpand(item);
        } else {
            item->setBackground(0, QBrush());
        }
        searchChild(item, txt);
    }
}

void MainWindow::parentExpand(QTreeWidgetItem *item)
{
    QTreeWidgetItem *pItem;
    if (item->parent() != NULL) {
        pItem = item->parent();
        if (! pItem->isExpanded()) {
            pItem->setExpanded(true);
        }
        parentExpand(pItem);
    }
}

void MainWindow::treeItemSearch(QTreeWidget *treewidget, QLineEdit *edit)
{
    treewidget->collapseAll();
    QString txt = edit->text();
    bool isEmptyTxt = txt.isEmpty();
    bool isEmptyTree = treewidget->topLevelItemCount(); //非零个为true
    if (isEmptyTxt) {
        QTreeWidgetItemIterator it(treewidget);
        while(*it) {
            (*it)->setBackground(0, QBrush());
            it++;
        }
        return;
    }
    if ( ! isEmptyTree) { return; }
    #if 0
        int count = treewidget->topLevelItemCount();
        for (int i =0; i < count; ++i) {
            QTreeWidgetItem *cItem = treewidget->topLevelItem(i);
            if (-1 != cItem->text(0).indexOf(txt)) {
                cItem->setBackground(0,Qt::green);
            } else {
                cItem->setBackground(0, QBrush());
            }
            searchChild(cItem, txt);
        }
    #else
        QTreeWidgetItemIterator it(treewidget);
        QString str;
        QTreeWidgetItem *pItem;
        while(*it) {
            str = (*it)->text(0);
            if (-1 != str.indexOf(txt,0,Qt::CaseInsensitive)) {
                pItem = (*it)->parent();
                if (pItem !=  NULL) {
                    bool isExpanded = pItem->isExpanded();
                    if (! isExpanded){
                        treewidget->expandItem(pItem);
                    }
                }
                (*it)->setBackground(0,Qt::green);
                parentExpand(*it);
            } else{
                (*it)->setBackground(0, QBrush());
            }
            it++;
        }
    #endif
}
