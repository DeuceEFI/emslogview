#include <QDebug>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "egraph.h"
#include "qgraph.h"
#include "logloader.h"
#include "datapacketdecoder.h"
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	decoder = 0;
	loader = 0;
	connect(ui->actionOpen_Log,SIGNAL(triggered()),this,SLOT(fileOpenClicked()));
	connect(ui->actionClose_Log,SIGNAL(triggered()),this,SLOT(fileCloseClicked()));
	connect(ui->actionQuit,SIGNAL(triggered()),this,SLOT(fileQuitClicked()));
	graph = new QGraph(this);
	this->setCentralWidget(graph);
	//graph->setGeometry(100,100,800,600);
	graph->show();
	dataSelectionScreen = new DataSelectionScreen();
	connect(dataSelectionScreen,SIGNAL(itemEnabled(QString)),this,SLOT(itemEnabled(QString)));
	connect(dataSelectionScreen,SIGNAL(itemDisabled(QString)),this,SLOT(itemDisabled(QString)));
	connect(ui->actionSelect_Dialog,SIGNAL(triggered()),this,SLOT(selectDialogClicked()));

	dataSelectionScreen->setVisible(false);
	this->setAttribute(Qt::WA_DeleteOnClose,true);
}
void MainWindow::logProgress(quint64 pos,quint64 total)
{
	m_progressDialog->setValue(100.0 * ((double)pos / (double)total));
}

MainWindow::~MainWindow()
{
	//delete ui;
	//delete dataSelectionScreen;
}
void MainWindow::payloadDecoded(QVariantMap map)
{
	if (variantList.size() == 0)
	{
		//First incoming map.
	}
	variantList.append(map);
}
void MainWindow::itemEnabled(QString name)
{
	QList<float> list;
	for (int i=0;i<variantList.size();i++)
	{
		list.append(variantList[i][name].toFloat());
	}
	graph->addData(list,name);
}

void MainWindow::itemDisabled(QString name)
{
	graph->removeData(name);
}

void MainWindow::boxClicked(bool checked)
{
	QCheckBox *sendbox = qobject_cast<QCheckBox*>(sender());
	if (!sendbox)
	{
		return;
	}
	QString name = sendbox->text();
	if (checked)
	{
		QList<float> list;
		for (int i=0;i<variantList.size();i++)
		{
			list.append(variantList[i][name].toFloat());
		}
		graph->addData(list,name);
	}
	else
	{
		graph->removeData(name);
	}
}
void MainWindow::selectDialogClicked()
{
	dataSelectionScreen->show();
	QApplication::postEvent(dataSelectionScreen, new QEvent(QEvent::Show));
	QApplication::postEvent(dataSelectionScreen, new QEvent(QEvent::WindowActivate));
}

void MainWindow::threadDone()
{
	m_progressDialog->hide();
	delete m_progressDialog;
	qDebug() << variantList.size() << "records loaded";
	for (QVariantMap::const_iterator i=variantList[0].constBegin();i!=variantList[0].constEnd();i++)
	{
		dataSelectionScreen->addItem(i.key());
	}
	dataSelectionScreen->show();
	dataSelectionScreen->setGeometry(100,100,400,600);
	/*QList<float> list;
	for (int i=0;i<variantList.size();i++)
	{
		list.append(variantList[i]["RPM"].toFloat());
	}
	graph->addData(list,"RPM");

	list.clear();
	for (int i=0;i<variantList.size();i++)
	{
		list.append(variantList[i]["TPS"].toFloat());
	}
	graph->addData(list,"TPS");

	list.clear();
	for (int i=0;i<variantList.size();i++)
	{
		list.append(variantList[i]["EGO"].toFloat());
	}
	graph->addData(list,"EGO");

	*/

	/*list.clear();
	for (int i=0;i<variantList.size();i++)
	{
		list.append(variantList[i]["Lambda"].toFloat());
	}
	graph->addData(list,"Lambda");

	list.clear();
	for (int i=0;i<variantList.size();i++)
	{
		float lambda = variantList[i]["Lambda"].toFloat();
		float VE = variantList[i]["VEMain"].toFloat();
		float ego = variantList[i]["EGO"].toFloat();
		float newval = (VE / lambda) * ego;
		list.append(newval);
	}
	graph->addData(list,"IntendedVE");*/
}
void MainWindow::fileOpenClicked()
{
	QString filename = QFileDialog::getOpenFileName(this,"Select log file to open");
	if (filename == "")
	{
		return;
	}
	fileCloseClicked();
	//decoder = new FEDataPacketDecoder();
	loader = new LogLoader();
	//connect(loader,SIGNAL(incomingDatalogPacket(QByteArray)),decoder,SLOT(decodePayload(QByteArray)));
	connect(loader,SIGNAL(done()),this,SLOT(threadDone()));
	connect(loader,SIGNAL(loadProgress(quint64,quint64)),this,SLOT(logProgress(quint64,quint64)));
	//connect(decoder,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
	connect(loader,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
	loader->loadFile(filename);
	m_progressDialog = new QProgressDialog("Loading file....","Cancel",0,100);
	m_progressDialog->show();

}

void MainWindow::fileCloseClicked()
{
	graph->clear();
	if (decoder)
	{
		disconnect(decoder,SIGNAL(payloadDecoded(QVariantMap)),this,SLOT(payloadDecoded(QVariantMap)));
		delete decoder;
		decoder = 0;
	}
	if (loader)
	{
		disconnect(loader,SIGNAL(incomingDatalogPacket(QByteArray)),decoder,SLOT(decodePayload(QByteArray)));
		disconnect(loader,SIGNAL(done()),this,SLOT(threadDone()));
		delete loader;
		loader = 0;
	}
}

void MainWindow::fileQuitClicked()
{

}
