/*

Pencil - Traditional Animation Software
Copyright (C) 2005-2007 Patrick Corrieri & Pascal Naidon

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/
#include <QtGui>
#include "editor.h"
#include "mainwindow.h"
#include "object.h"
#include <interfaces.h>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <stdio.h>
#include <QStringList>
#include <QDir>

MainWindow::MainWindow() {

    
	editor = new Editor(this);
	//Object* object = new Object();
	//object->defaultInitialisation();
	//editor->setObject( object );
	editor->newObject();

	arrangePalettes();
	
	//editor->getTimeLine()->close();
	
	createMenus();
	loadPlugins();
	readSettings();

    /*
     *Check if the snapshot directory exists or not
     *if yes-> continue and put snapshots in QCombobox
     *if not-> exit and continue like normal paint operations
     */

    //QDir directory("/home/grad/workspace/ece1724-pencil/ece1724-pencil/snap");
    QDir directory(QDir::currentPath() + "/snapshots");
    if (!directory.exists()) {
            qDebug() << "Directory Doesn't exists";
        }
    else {
         QStringList files = directory.entryList(QDir::Files);
         //Debugging purpose only     
         //qDebug() << "Directory exists";
         /*foreach(QString itm, files)
         {
                qDebug() << itm;
         }*/
        QWidget *window = new QWidget;
        window->setWindowTitle("Snapshots");
        QGridLayout *layout = new QGridLayout;
        QComboBox *qbox;
        QPushButton *button;
        qbox = new QComboBox();
        layout->addWidget(qbox,0,0,1,2);
        foreach(QString itm, files)
        {
            qbox->addItem(itm);
        }

        button = new QPushButton("Load Snapshot");
        layout->addWidget(button,2,0,1,2);

        window->setLayout(layout);
        //currently commented out, so that QT window doesnt show up -- will deal with this later
        //window->show();
    }
    
    QDir myDir("snapshots");
    QStringList list = myDir.entryList (QStringList("snapshotOperations*.log"));	// filter only c++ files
    list.sort();
    int high=0, current=0;
    for(int i=0; i<list.size(); i++)
    {
      sscanf(list.at(i).toUtf8().constData(),"snapshotOperations%d.log", &current);
      high = (current > high) ? current : high;
    }
    
    if(high>0)
    {
      qDebug() << "Highest snapshot number found was " << high;
      editor->restoreSnapshotOnStartup(QString("snapshots/snap%1").arg(high), QString("snapshots/snapshotOperations%1.log").arg(high));
    }
}

void MainWindow::on_pushButton_clicked()
{

    qDebug() << "Hurray the Select button is being clicked";
    //QMessageBox::information(this,"tittle",qbox->currentText());

}

void MainWindow::arrangePalettes() {
	setCentralWidget(editor);
	addDockWidget(Qt::RightDockWidgetArea, editor->getPalette());
	//editor->getPalette()->close();
	addDockWidget(Qt::LeftDockWidgetArea, editor->getToolSet()->drawPalette);
	addDockWidget(Qt::LeftDockWidgetArea, editor->getToolSet()->optionPalette);
	addDockWidget(Qt::LeftDockWidgetArea, editor->getToolSet()->displayPalette);
	//addDockWidget(Qt::LeftDockWidgetArea, editor->getToolSet()->onionPalette);
	//addDockWidget(Qt::LeftDockWidgetArea, editor->getToolSet()->keyPalette);

	//addDockWidget(Qt::BottomDockWidgetArea, editor->getToolSet()->timePalette);
	addDockWidget(Qt::BottomDockWidgetArea, editor->getTimeLine());

	editor->getToolSet()->drawPalette->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	editor->getToolSet()->optionPalette->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	//editor->getToolSet()->keyPalette->setFeatures(QDockWidget::NoDockWidgetFeatures);
	//editor->getToolSet()->onionPalette->setFeatures(QDockWidget::NoDockWidgetFeatures);
	editor->getToolSet()->displayPalette->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	editor->getToolSet()->keyPalette->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	editor->getToolSet()->onionPalette->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	editor->getToolSet()->timePalette->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	editor->getTimeLine()->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
}

void MainWindow::createMenus() {
	// ---------- Actions -------------
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	connect(exitAct, SIGNAL(triggered()), editor, SLOT(close()));

	newAct = new QAction(tr("&New"), this);
	newAct->setShortcut(tr("Ctrl+N"));
	connect(newAct, SIGNAL(triggered()), editor, SLOT(newDocument()));

	openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), editor, SLOT(openDocument()));

    saveAct = new QAction(tr("Save &As..."), this);
	saveAct->setShortcut(tr("Ctrl+Shift+S"));
	connect(saveAct, SIGNAL(triggered()), editor, SLOT(saveDocument()));

	savAct = new QAction(tr("&Save"), this);
	savAct->setShortcut(tr("Ctrl+S"));
	connect(savAct, SIGNAL(triggered()), editor, SLOT(saveForce()));

	exportXAct = new QAction(tr("&X-Sheet..."), this);
	exportXAct->setShortcut(tr("Ctrl+Alt+X"));
	connect(exportXAct, SIGNAL(triggered()), editor, SLOT(exportX()));

	exportAct = new QAction(tr("&Image Sequence..."), this);
	exportAct->setShortcut(tr("Ctrl+Alt+S"));
	connect(exportAct, SIGNAL(triggered()), editor, SLOT(exportSeq()));

	exportMovAct = new QAction(tr("&Movie..."), this);
	exportMovAct->setShortcut(tr("Ctrl+Alt+M"));
	connect(exportMovAct, SIGNAL(triggered()), editor, SLOT(exportMov()));

	exportFlashAct = new QAction(tr("&Flash/SWF..."), this);
	exportFlashAct->setShortcut(tr("Ctrl+Alt+F"));
	connect(exportFlashAct, SIGNAL(triggered()), editor, SLOT(exportFlash()));

	exportFlashAct = new QAction(tr("&Flash/SWF..."), this);
	exportFlashAct->setShortcut(tr("Ctrl+Alt+F"));
	connect(exportFlashAct, SIGNAL(triggered()), editor, SLOT(exportFlash()));
	
	exportPaletteAct = new QAction(tr("Palette..."), this);
	connect(exportPaletteAct, SIGNAL(triggered()), editor, SLOT(exportPalette()));
	
	importPaletteAct = new QAction(tr("Palette..."), this);
	connect(importPaletteAct, SIGNAL(triggered()), editor, SLOT(importPalette()));
	
    importAct = new QAction(tr("&Import image..."), this);
    importAct->setShortcut(tr("Ctrl+I"));
    connect(importAct, SIGNAL(triggered()), editor, SLOT(importImage()));

    importSndAct = new QAction(tr("&Import sound..."), this);
//    importAct->setShortcut(tr("Ctrl+I"));
    connect(importSndAct, SIGNAL(triggered()), editor, SLOT(importSound()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), editor, SLOT(about()));

	helpMe = new QAction(tr("&Help"), this);
	helpMe->setShortcut(tr("Ctrl+?"));
	connect(helpMe, SIGNAL(triggered()), editor, SLOT(helpBox()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	undoAct = new QAction(tr("Undo"), this);
	undoAct->setShortcut(tr("Ctrl+Z"));
	connect(undoAct, SIGNAL(triggered()), editor, SLOT(undo()));

	redoAct = new QAction(tr("Redo"), this);
	redoAct->setShortcut(tr("Ctrl+Shift+Z"));
	connect(redoAct, SIGNAL(triggered()), editor, SLOT(redo()));

	copyAct = new QAction(tr("Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
	connect(copyAct, SIGNAL(triggered()), editor, SLOT(copy()));

	pasteAct = new QAction(tr("Paste"), this);
	pasteAct->setShortcut(tr("Ctrl+V"));
	connect(pasteAct, SIGNAL(triggered()), editor, SLOT(paste()));

	selectAllAct = new QAction(tr("Select All"), this);
	selectAllAct->setShortcut(tr("Ctrl+A"));
	connect(selectAllAct, SIGNAL(triggered()), editor, SIGNAL(selectAll()));

	preferencesAct = new QAction(tr("Preferences"), this);
	connect(preferencesAct, SIGNAL(triggered()), editor, SLOT(showPreferences()));


	newBitmapLayerAct = new QAction(tr("New Bitmap Layer"), this);
	connect(newBitmapLayerAct, SIGNAL(triggered()), editor, SLOT(newBitmapLayer()));

	newVectorLayerAct = new QAction(tr("New Vector Layer"), this);
	connect(newVectorLayerAct, SIGNAL(triggered()), editor, SLOT(newVectorLayer()));

	newSoundLayerAct = new QAction(tr("New Sound Layer"), this);
	connect(newSoundLayerAct, SIGNAL(triggered()), editor, SLOT(newSoundLayer()));

	newCameraLayerAct = new QAction(tr("New Camera Layer"), this);
	connect(newCameraLayerAct, SIGNAL(triggered()), editor, SLOT(newCameraLayer()));

	deleteLayerAct = new QAction(tr("Delete Current Layer"), this);
	connect(deleteLayerAct, SIGNAL(triggered()), editor, SLOT(deleteCurrentLayer()));


	dockAllPalettesAct = new QAction(tr("Dock All Palettes"), this);
	connect(dockAllPalettesAct, SIGNAL(triggered()), editor, SLOT(dockAllPalettes()));

	detachAllPalettesAct = new QAction(tr("Detach All Palettes"), this);
	connect(detachAllPalettesAct, SIGNAL(triggered()), editor, SLOT(detachAllPalettes()));
	
	// --------------- Menus ------------------
	
	importMenu = new QMenu(tr("Import"), this);
	importMenu->addAction(importPaletteAct);

	exportMenu = new QMenu(tr("Export"), this);
	exportMenu->addAction(exportXAct);
	exportMenu->addAction(exportAct);
	exportMenu->addAction(exportMovAct);
	exportMenu->addAction(exportFlashAct);
	exportMenu->addSeparator();
	exportMenu->addAction(exportPaletteAct);

	openRecentMenu = new QMenu(tr("Open recent..."), this);

	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(newAct);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(openRecentMenu);
	fileMenu->addAction(savAct);
	fileMenu->addAction(saveAct);
	fileMenu->addSeparator();
	fileMenu->addMenu(importMenu);
	fileMenu->addMenu(exportMenu);

	editMenu = new QMenu(tr("&Edit"), this);
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addSeparator();
	editMenu->addAction(copyAct);
	editMenu->addAction(pasteAct);
	editMenu->addSeparator();
	editMenu->addAction(selectAllAct);
	editMenu->addSeparator();
	editMenu->addAction(importAct);
	editMenu->addAction(importSndAct);
	editMenu->addSeparator();
	editMenu->addAction(preferencesAct);

	layerMenu = new QMenu(tr("&Layer"), this);
	layerMenu->addAction(newBitmapLayerAct);
	layerMenu->addAction(newVectorLayerAct);
	layerMenu->addAction(newSoundLayerAct);
	layerMenu->addAction(newCameraLayerAct);
	layerMenu->addSeparator();
	layerMenu->addAction(deleteLayerAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(helpMe);
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);

	windowsMenu = new QMenu(tr("Windows"), this);
	windowsMenu->addAction(dockAllPalettesAct);
	windowsMenu->addAction(detachAllPalettesAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(editMenu);
	menuBar()->addMenu(layerMenu);
	menuBar()->addMenu(helpMenu);
	menuBar()->addMenu(windowsMenu);
}

void MainWindow::loadPlugins() {
	qDebug() << "MainWindow loadplugins" << this << this->thread();
	// foreach (QObject *plugin, QPluginLoader::staticInstances()) populateMenus(plugin); // static plugins
	QDir pluginsDir = QDir(qApp->applicationDirPath());
 #if defined(Q_OS_WIN)
     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
         pluginsDir.cdUp();
 #elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
     }
 #endif
	pluginsDir.cd("plugins");

	qDebug() << "Plugin dir = " << pluginsDir.dirName();
	/*foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		qDebug() << "loader " << loader.thread();
		qDebug() << "plugin " << fileName << plugin << plugin->thread();
		if (plugin) {
			plugin->moveToThread(this->thread());
			populateMenus(plugin);
			//pluginFileNames += fileName;
		}
	}*/

	//brushMenu->setEnabled(!brushActionGroup->actions().isEmpty());
	//shapesMenu->setEnabled(!shapesMenu->actions().isEmpty());
	//filterMenu->setEnabled(!filterMenu->actions().isEmpty());
}

void MainWindow::populateMenus(QObject *plugin) {
	qDebug() << "MainWindow populateMenus" << this << this->thread();
	qDebug() << "MainWindow populateMenus" << plugin << plugin->thread();
	/*BrushInterface *iBrush = qobject_cast<BrushInterface *>(plugin);
	if (iBrush) addToMenu(plugin, iBrush->brushes(), brushMenu, SLOT(changeBrush()), brushActionGroup);

	ShapeInterface *iShape = qobject_cast<ShapeInterface *>(plugin);
	if (iShape) addToMenu(plugin, iShape->shapes(), shapesMenu, SLOT(insertShape()));

	FilterInterface *iFilter = qobject_cast<FilterInterface *>(plugin);
	if (iFilter) addToMenu(plugin, iFilter->filters(), filterMenu, SLOT(applyFilter()));*/
	
	ExportInterface *exportPlugin = qobject_cast<ExportInterface *>(plugin);
	if (exportPlugin) addToMenu(plugin, exportPlugin->name(), exportMenu, SLOT(exportFile()));
}

void MainWindow::addToMenu(QObject *plugin, const QString text, QMenu *menu, const char *member, QActionGroup *actionGroup) {
	qDebug() << "MainWindow populateMenus" << this << this->thread();
	qDebug() << "MainWindow populateMenus" << plugin << plugin->thread();
	qDebug() << "addToMenu 1";
	QAction *action = new QAction(text, plugin);
	qDebug() << "addToMenu 2";
	connect(action, SIGNAL(triggered()), this, member);
	menu->addAction(action);
	if (actionGroup) {
		action->setCheckable(true);
		actionGroup->addAction(action);
	}
}

void MainWindow::exportFile() {
	QAction *action = qobject_cast<QAction *>(sender());
	ExportInterface *exportPlugin = qobject_cast<ExportInterface *>(action->parent());
	if(exportPlugin) {
		//exportPlugin->exportFile();
	} else {
		qDebug() << "exportPlugin is null";
	}
	//const QImage image = iFilter->filterImage(action->text(), paintArea->image(), this);
	//paintArea->setImage(image);
}

void MainWindow::setOpacity(int opacity) {
	QSettings settings("Pencil","Pencil");
	settings.setValue("windowOpacity", 100-opacity);
	setWindowOpacity(opacity/100.0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (editor->maybeSave()) {
		writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::keyPressEvent( QKeyEvent *e ) {
	switch (e->key()) {
	case Qt::Key_H:
		editor->switchVisibilityOfLayer(editor->currentLayer);
		break;
	case Qt::Key_D:
		editor->scrubForward();
		break;
	case Qt::Key_Right:
		//editor->scrubForward();
		break;
	case Qt::Key_S:
		editor->scrubBackward();
		break;
	case Qt::Key_Left:
		//editor->scrubBackward();
		break;
	case Qt::Key_Up:
		//editor->previousLayer();
		break;
	case Qt::Key_Down:
		//editor->nextLayer();
		break;
	case Qt::Key_Space:
		editor->play();
		break;
 	case Qt::Key_Alt:
		//editor->altPress();
		break;
	case Qt::Key_Return:
		editor->addKey();
		break;
	default:
	QWidget::keyPressEvent(e);
    }
}

void MainWindow::keyReleaseEvent( QKeyEvent *e ) {
	switch (e->key()) {
    case Qt::Key_Alt:
		//editor->altRelease();
	break;
		default:
	QWidget::keyPressEvent(e);
    }
}

void MainWindow::readSettings() {
	QSettings settings("Pencil", "Pencil");
	QRect desktopRect = QApplication::desktop()->screenGeometry();
	desktopRect.adjust(80,80,-80,-80);
	//QPoint pos = settings.value("editorPosition", QPoint( qMax(0, (desktopRect.width()-800)/2), qMax(0, (desktopRect.height()-600)/2) )).toPoint();
	QPoint pos = settings.value("editorPosition", desktopRect.topLeft() ).toPoint();
	//QSize size = settings.value("editorSize", QSize(800, 600)).toSize();
	QSize size = settings.value("editorSize", desktopRect.size() ).toSize();
	move(pos);
	resize(size);
	
	editor->restorePalettesSettings(true, true, true);
	
	QString myPath = settings.value("lastFilePath", QVariant(QDir::homePath())).toString();
	addRecentFile(myPath);

	setOpacity(100-settings.value("windowOpacity").toInt());	
	//initialiseStyle();
}

void MainWindow::writeSettings() {
	QSettings settings("Pencil", "Pencil");
	settings.setValue("editorPosition", pos());
	settings.setValue("editorSize", size());
	
	Palette* colourPalette = editor->getPalette();
	if(colourPalette != NULL) {
		settings.setValue("colourPalettePosition", colourPalette->pos());
		settings.setValue("colourPaletteSize", colourPalette->size());
		settings.setValue("colourPaletteFloating", colourPalette->isFloating());
	}
	
	TimeLine* timelinePalette = editor->getTimeLine();
	if(timelinePalette != NULL) {
		settings.setValue("timelinePalettePosition", timelinePalette->pos());
		settings.setValue("timelinePaletteSize", timelinePalette->size());
		settings.setValue("timelinePaletteFloating", timelinePalette->isFloating());
	}
	
	QDockWidget* drawPalette = editor->getToolSet()->drawPalette;
	if(drawPalette != NULL) {
		settings.setValue("drawPalettePosition", drawPalette->pos());
		settings.setValue("drawPaletteSize", drawPalette->size());
		settings.setValue("drawPaletteFloating", drawPalette->isFloating());
	}

	QDockWidget* optionPalette = editor->getToolSet()->optionPalette;
	if(optionPalette != NULL) {
		settings.setValue("optionPalettePosition", optionPalette->pos());
		settings.setValue("optionPaletteSize", optionPalette->size());
		settings.setValue("optionPaletteFloating", optionPalette->isFloating());
	}

	QDockWidget* displayPalette = editor->getToolSet()->displayPalette;
	if(optionPalette != NULL) {
		settings.setValue("displayPalettePosition", displayPalette->pos());
		settings.setValue("displayPaletteSize", displayPalette->size());
		settings.setValue("displayPaletteFloating", displayPalette->isFloating());
	}
			
}

void MainWindow::addRecentFile(QString filePath) {
	QAction* openThisFileAct = new QAction(filePath, this);
	connect(openThisFileAct, SIGNAL(triggered()), editor, SLOT(openRecent()));
	openRecentMenu->addAction(openThisFileAct);
}
