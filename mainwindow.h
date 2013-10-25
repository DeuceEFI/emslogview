#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVariantMap>
#include <QScrollArea>
#include "datapacketdecoder.h"
#include "logloader.h"
#include "dataselectionscreen.h"
#include "qgraph.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
	QList<QVariantMap> variantList;
	QGraph *graph;
	FEDataPacketDecoder *decoder;
	LogLoader *loader;
	DataSelectionScreen *dataSelectionScreen;
private slots:
	void payloadDecoded(QVariantMap map);
	void threadDone();
	void boxClicked(bool checked);
	void fileOpenClicked();
	void fileCloseClicked();
	void fileQuitClicked();
	void itemEnabled(QString name);
	void itemDisabled(QString name);
	void selectDialogClicked();
};

#endif // MAINWINDOW_H
