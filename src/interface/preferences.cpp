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
#include "preferences.h"
#include "scribblearea.h";

Preferences::Preferences()
 {
     contentsWidget = new QListWidget;
     contentsWidget->setViewMode(QListView::IconMode);
     contentsWidget->setIconSize(QSize(96, 84));
     contentsWidget->setMovement(QListView::Static);
     contentsWidget->setMaximumWidth(128);
     contentsWidget->setSpacing(12);

     pagesWidget = new QStackedWidget;
     pagesWidget->addWidget(new GeneralPage(this));
     pagesWidget->addWidget(new FilesPage(this));
     pagesWidget->addWidget(new TimelinePage(this));
     //pagesWidget->addWidget(new QueryPage);

     QPushButton *closeButton = new QPushButton(tr("Close"));

     createIcons();
     contentsWidget->setCurrentRow(0);

     connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

     QHBoxLayout *horizontalLayout = new QHBoxLayout;
     horizontalLayout->addWidget(contentsWidget);
     horizontalLayout->addWidget(pagesWidget, 1);

     QHBoxLayout *buttonsLayout = new QHBoxLayout;
     buttonsLayout->addStretch(1);
     buttonsLayout->addWidget(closeButton);

     QVBoxLayout *mainLayout = new QVBoxLayout;
     mainLayout->addLayout(horizontalLayout);
     mainLayout->addStretch(1);
     mainLayout->addSpacing(12);
     mainLayout->addLayout(buttonsLayout);
     setLayout(mainLayout);

     setWindowTitle(tr("Preferences"));
 }

 void Preferences::createIcons()
 {
     QListWidgetItem *generalButton = new QListWidgetItem(contentsWidget);
     generalButton->setIcon(QIcon(":icons/prefspencil.png"));
     generalButton->setText(tr("General"));
     generalButton->setTextAlignment(Qt::AlignHCenter);
     generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		 
		 QListWidgetItem *filesButton = new QListWidgetItem(contentsWidget);
     filesButton->setIcon(QIcon(":icons/prefs-files.png"));
     filesButton->setText(tr("Snapshots"));
     filesButton->setTextAlignment(Qt::AlignHCenter);
     filesButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		 
		 QListWidgetItem *timelineButton = new QListWidgetItem(contentsWidget);
     timelineButton->setIcon(QIcon(":icons/prefstimeline.png"));
     timelineButton->setText(tr("Time Line"));
     timelineButton->setTextAlignment(Qt::AlignHCenter);
     timelineButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

     /*QListWidgetItem *queryButton = new QListWidgetItem(contentsWidget);
     queryButton->setIcon(QIcon(":/images/query.png"));
     queryButton->setText(tr("Query"));
     queryButton->setTextAlignment(Qt::AlignHCenter);
     queryButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);*/

     connect(contentsWidget,
             SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
             this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
 }

 void Preferences::changePage(QListWidgetItem *current, QListWidgetItem *previous)
 {
     if (!current)
         current = previous;

     pagesWidget->setCurrentIndex(contentsWidget->row(current));
 }



GeneralPage::GeneralPage(QWidget *parent) : QWidget(parent) {
	QSettings settings("Pencil","Pencil");
	QVBoxLayout *lay = new QVBoxLayout();
	
	QGroupBox* windowOpacityBox = new QGroupBox(tr("Window opacity"));
	QGroupBox* backgroundBox = new QGroupBox(tr("Background"));
	QGroupBox* appearanceBox = new QGroupBox(tr("Appearance"));
	QGroupBox* displayBox = new QGroupBox(tr("Rendering"));
	QGroupBox* editingBox = new QGroupBox(tr("Editing"));
	
	QLabel *windowOpacityLabel = new QLabel(tr("Opacity"));
	QSlider *windowOpacityLevel = new QSlider(Qt::Horizontal);
	windowOpacityLevel->setMinimum(30);
	windowOpacityLevel->setMaximum(100);
	int value = settings.value("windowOpacity").toInt();
	windowOpacityLevel->setValue( 100 - value );
	
	QButtonGroup* backgroundButtons = new QButtonGroup();
	QRadioButton* checkerBackgroundButton = new QRadioButton(); 
	QRadioButton* whiteBackgroundButton = new QRadioButton();
	QRadioButton* greyBackgroundButton = new QRadioButton();
	QRadioButton* dotsBackgroundButton = new QRadioButton();
	QRadioButton* weaveBackgroundButton = new QRadioButton();
	QPixmap previewCheckerboard(32,32);
	QPixmap previewWhite(32,32);
	QPixmap previewGrey(32,32);
	QPixmap previewDots(32,32);
	QPixmap previewWeave(32,32);
	QPainter painter(&previewCheckerboard);
	painter.fillRect( QRect(0,0,32,32), ScribbleArea::getBackgroundBrush("checkerboard") );
	painter.end();
	painter.begin(&previewDots);
	painter.fillRect( QRect(0,0,32,32), ScribbleArea::getBackgroundBrush("dots") );
	painter.end();
	painter.begin(&previewWeave);
	painter.fillRect( QRect(0,0,32,32), ScribbleArea::getBackgroundBrush("weave") );
	painter.end();
	previewWhite.fill( Qt::white );
	previewGrey.fill( Qt:: lightGray );
	checkerBackgroundButton->setIcon( previewCheckerboard );
	whiteBackgroundButton->setIcon( previewWhite );
	greyBackgroundButton->setIcon( previewGrey );
	dotsBackgroundButton->setIcon( previewDots );
	dotsBackgroundButton->setIcon( previewWeave );
	backgroundButtons->addButton(checkerBackgroundButton);
	backgroundButtons->addButton(whiteBackgroundButton);
	backgroundButtons->addButton(greyBackgroundButton);
	backgroundButtons->addButton(dotsBackgroundButton);
	backgroundButtons->addButton(weaveBackgroundButton);
	backgroundButtons->setId(checkerBackgroundButton, 1);
	backgroundButtons->setId(whiteBackgroundButton, 2);
	backgroundButtons->setId(greyBackgroundButton, 3);
	backgroundButtons->setId(dotsBackgroundButton, 4);
	backgroundButtons->setId(weaveBackgroundButton, 5);
	QHBoxLayout *backgroundLayout = new QHBoxLayout();
	backgroundBox->setLayout(backgroundLayout);
	backgroundLayout->addWidget(checkerBackgroundButton);
	backgroundLayout->addWidget(whiteBackgroundButton);
	backgroundLayout->addWidget(greyBackgroundButton);
	backgroundLayout->addWidget(dotsBackgroundButton);
	backgroundLayout->addWidget(weaveBackgroundButton);
	if( settings.value("background").toString() == "checkerboard" ) checkerBackgroundButton->setChecked(true);
	if( settings.value("background").toString() == "white" ) whiteBackgroundButton->setChecked(true);
	if( settings.value("background").toString() == "grey" ) greyBackgroundButton->setChecked(true);
	if( settings.value("background").toString() == "dots" ) dotsBackgroundButton->setChecked(true);
	if( settings.value("background").toString() == "weave" ) weaveBackgroundButton->setChecked(true);
	
	QCheckBox *shadowsBox = new QCheckBox(tr("Shadows"));
	shadowsBox->setChecked(false); // default
	if (settings.value("shadows").toString()=="true") shadowsBox->setChecked(true);
	
	QCheckBox *toolCursorsBox = new QCheckBox(tr("Tool Cursors"));
	toolCursorsBox->setChecked(true); // default
	if (settings.value("toolCursors").toString()=="false") toolCursorsBox->setChecked(false);

	QCheckBox *aquaBox = new QCheckBox(tr("Aqua Style"));
	aquaBox->setChecked(false); // default
	if (settings.value("style").toString()=="aqua") aquaBox->setChecked(true);
	
	QCheckBox *antialiasingBox = new QCheckBox(tr("Antialiasing"));
	antialiasingBox->setChecked(true); // default
	if (settings.value("antialiasing").toString()=="false") antialiasingBox->setChecked(false);
	
	QButtonGroup *gradientsButtons = new QButtonGroup();
	QRadioButton* gradient1Button = new QRadioButton(tr("None")); 
	QRadioButton* gradient2Button = new QRadioButton(tr("Quick"));
	QRadioButton* gradient3Button = new QRadioButton(tr("Gradient1"));
	QRadioButton* gradient4Button = new QRadioButton(tr("Gradient2"));
	gradientsButtons->addButton(gradient1Button);
	gradientsButtons->addButton(gradient2Button);
	gradientsButtons->addButton(gradient3Button);
	gradientsButtons->addButton(gradient4Button);
	gradientsButtons->setId(gradient1Button, 1);
	gradientsButtons->setId(gradient2Button, 2);
	gradientsButtons->setId(gradient3Button, 3);
	gradientsButtons->setId(gradient4Button, 4);
	QGroupBox* gradientsBox = new QGroupBox(tr("Gradients"));
	QHBoxLayout *gradientsLayout = new QHBoxLayout();
	gradientsBox->setLayout(gradientsLayout);
	gradientsLayout->addWidget(gradient1Button);
	gradientsLayout->addWidget(gradient2Button);
	gradientsLayout->addWidget(gradient3Button);
	gradientsLayout->addWidget(gradient4Button);
	if( settings.value("gradients").toString() == "1" ) gradient1Button->setChecked(true);
	if( settings.value("gradients").toString() == "2" ) gradient2Button->setChecked(true);
	if( settings.value("gradients").toString() == "" )  gradient2Button->setChecked(true); // default
	if( settings.value("gradients").toString() == "3" ) gradient3Button->setChecked(true);
	if( settings.value("gradients").toString() == "4" ) gradient4Button->setChecked(true);
	
	
	/*QCheckBox *gradientsBox = new QCheckBox(tr("Gradients"));
	gradientsBox->setChecked(true); // default
	if (settings.value("gradients").toString()=="0") gradientsBox->setChecked(false);*/
	
	QLabel *curveOpacityLabel = new QLabel(tr("Vector curve opacity"));
	QSlider *curveOpacityLevel = new QSlider(Qt::Horizontal);
	curveOpacityLevel->setMinimum(0);
	curveOpacityLevel->setMaximum(100);
	curveOpacityLevel->setValue( 100 - settings.value("curveOpacity").toInt() );
	
	QGridLayout *windowOpacityLayout = new QGridLayout();
	windowOpacityBox->setLayout(windowOpacityLayout);
	windowOpacityLayout->addWidget(windowOpacityLabel, 0, 0);
	windowOpacityLayout->addWidget(windowOpacityLevel, 0, 1);
	
	QVBoxLayout *appearanceLayout = new QVBoxLayout();
	appearanceBox->setLayout(appearanceLayout);
	appearanceLayout->addWidget(shadowsBox);
	appearanceLayout->addWidget(toolCursorsBox);
#ifdef Q_WS_MAC	
	appearanceLayout->addWidget(aquaBox);
#endif

	QGridLayout *displayLayout = new QGridLayout();
	displayBox->setLayout(displayLayout);
	displayLayout->addWidget(antialiasingBox, 0, 0);
	displayLayout->addWidget(gradientsBox, 1, 0);
	displayLayout->addWidget(curveOpacityLabel, 2, 0);
	displayLayout->addWidget(curveOpacityLevel, 3, 0);
	
	QLabel *curveSmoothingLabel = new QLabel(tr("Vector curve smoothing"));
	QSlider *curveSmoothingLevel = new QSlider(Qt::Horizontal);
	curveSmoothingLevel->setMinimum(1);
	curveSmoothingLevel->setMaximum(100);
	value = settings.value("curveSmoothing").toInt();
	curveSmoothingLevel->setValue( value );
	
	QCheckBox *highResBox = new QCheckBox(tr("Tablet high-resolution position"));
	if (settings.value("highResPosition")=="true") highResBox->setChecked(true);
	else highResBox->setChecked(false);
	
	QGridLayout *editingLayout = new QGridLayout();
	editingBox->setLayout(editingLayout);
	editingLayout->addWidget(curveSmoothingLabel, 0, 0);
	editingLayout->addWidget(curveSmoothingLevel, 1, 0);
	editingLayout->addWidget(highResBox, 2, 0);
	
	//QLabel *fontSizeLabel = new QLabel(tr("Labels font size"));
	//QDoubleSpinBox *fontSize = new QDoubleSpinBox();

	/*fontSize->setMinimum(4);
	fontSize->setMaximum(20);
	frameSize->setMinimum(4);
	frameSize->setMaximum(20);
	
	fontSize->setFixedWidth(50);
	frameSize->setFixedWidth(50);
	lengthSize->setFixedWidth(50);
	
	if (settings.value("drawLabel")=="false") drawLabel->setChecked(false);
	else drawLabel->setChecked(true);
	fontSize->setValue(settings.value("labelFontSize").toInt()); 
	frameSize->setValue(settings.value("frameSize").toInt());
	if (settings.value("labelFontSize").toInt()==0) fontSize->setValue(12);
	if (settings.value("frameSize").toInt()==0) frameSize->setValue(6);
	lengthSize->setText(settings.value("length").toString());
	if (settings.value("length").toInt()==0) lengthSize->setText("240");

	connect(fontSize, SIGNAL(valueChanged(int)), this, SIGNAL(fontSizeChange(int)));
	connect(frameSize, SIGNAL(valueChanged(int)), this, SIGNAL(frameSizeChange(int)));
	connect(lengthSize, SIGNAL(textChanged(QString)), this, SIGNAL(lengthSizeChange(QString)));
	connect(drawLabel, SIGNAL(stateChanged(int)), this, SIGNAL(labelChange(int)));*/
	
	lay->addWidget(windowOpacityBox);
	lay->addWidget(appearanceBox);
	lay->addWidget(backgroundBox);
	lay->addWidget(displayBox);
	lay->addWidget(editingBox);
	
	connect(windowOpacityLevel, SIGNAL(valueChanged(int)), parent, SIGNAL(windowOpacityChange(int)));
	connect(backgroundButtons, SIGNAL(buttonClicked(int)), parent, SIGNAL(backgroundChange(int)));
	connect(gradientsButtons, SIGNAL(buttonClicked(int)), parent, SIGNAL(gradientsChange(int)));
	connect(shadowsBox, SIGNAL(stateChanged(int)), parent, SIGNAL(shadowsChange(int)));
	connect(toolCursorsBox, SIGNAL(stateChanged(int)), parent, SIGNAL(toolCursorsChange(int)));
	connect(aquaBox, SIGNAL(stateChanged(int)), parent, SIGNAL(styleChange(int)));
	connect(antialiasingBox, SIGNAL(stateChanged(int)), parent, SIGNAL(antialiasingChange(int)));
	connect(curveOpacityLevel, SIGNAL(valueChanged(int)), parent, SIGNAL(curveOpacityChange(int)));
	connect(curveSmoothingLevel, SIGNAL(valueChanged(int)), parent, SIGNAL(curveSmoothingChange(int)));
	connect(highResBox, SIGNAL(stateChanged(int)), parent, SIGNAL(highResPositionChange(int)));
	
	/*lay->addWidget(fontSizeLabel);
	lay->addWidget(fontSize);
	lay->addWidget(frameSizeLabel);
	lay->addWidget(frameSize);
	lay->addWidget(lengthSizeLabel);
	lay->addWidget(lengthSize);*/
	setLayout(lay);
}


TimelinePage::TimelinePage(QWidget *parent) : QWidget(parent) {
	QSettings settings("Pencil","Pencil");
	
	QVBoxLayout *lay = new QVBoxLayout();
	
	QGroupBox* timeLineBox = new QGroupBox(tr("Time Line"));
	QCheckBox *drawLabel = new QCheckBox(tr("Draw timeline labels"));
	QLabel *fontSizeLabel = new QLabel(tr("Labels font size"));
	QSpinBox *fontSize = new QSpinBox();
	QLabel *frameSizeLabel = new QLabel(tr("Frame size in Pixels"));
	QSpinBox *frameSize = new QSpinBox(this);
	QLabel *lengthSizeLabel = new QLabel(tr("Timeline size in Frames"));
	QLineEdit *lengthSize = new QLineEdit(this);
	lengthSize->setInputMask("9999");

	QCheckBox *scrubBox = new QCheckBox(tr("Short scrub"));
	scrubBox->setChecked(false); // default
	if (settings.value("shortScrub").toBool()) scrubBox->setChecked(true);
	
	fontSize->setMinimum(4);
	fontSize->setMaximum(20);
	frameSize->setMinimum(4);
	frameSize->setMaximum(20);
	
	fontSize->setFixedWidth(50);
	frameSize->setFixedWidth(50);
	lengthSize->setFixedWidth(50);
	
	if (settings.value("drawLabel")=="false") drawLabel->setChecked(false);
	else drawLabel->setChecked(true);
	fontSize->setValue(settings.value("labelFontSize").toInt()); 
	frameSize->setValue(settings.value("frameSize").toInt());
	if (settings.value("labelFontSize").toInt()==0) fontSize->setValue(12);
	if (settings.value("frameSize").toInt()==0) frameSize->setValue(6);
	lengthSize->setText(settings.value("length").toString());
	if (settings.value("length").toInt()==0) lengthSize->setText("240");

	connect(fontSize, SIGNAL(valueChanged(int)), parent, SIGNAL(fontSizeChange(int)));
	connect(frameSize, SIGNAL(valueChanged(int)), parent, SIGNAL(frameSizeChange(int)));
	connect(lengthSize, SIGNAL(textChanged(QString)), parent, SIGNAL(lengthSizeChange(QString)));
	connect(drawLabel, SIGNAL(stateChanged(int)), parent, SIGNAL(labelChange(int)));
	connect(scrubBox, SIGNAL(stateChanged(int)), parent, SIGNAL(scrubChange(int)));
	
	//lay->addWidget(drawLabel);
	//lay->addWidget(fontSizeLabel);
	//lay->addWidget(fontSize);
	lay->addWidget(frameSizeLabel);
	lay->addWidget(frameSize);
	lay->addWidget(lengthSizeLabel);
	lay->addWidget(lengthSize);
	lay->addWidget(scrubBox);
	timeLineBox->setLayout(lay);
	
	QVBoxLayout *lay2 = new QVBoxLayout();
	lay2->addWidget(timeLineBox);
	lay2->addStretch(1);
	setLayout(lay2);
}

FilesPage::FilesPage(QWidget *parent) : QWidget(parent) {
	QSettings settings("Pencil","Pencil");
	
	QVBoxLayout *lay = new QVBoxLayout();
	
	QGroupBox* autosaveBox = new QGroupBox(tr("Auto-Snapshot Documents"));
	QCheckBox *autosaveCheckBox = new QCheckBox(tr("Enable auto-snapshot"));
	QLabel *autosaveNumberLabel = new QLabel(tr("Number of modifications before auto-snapshot:"));
	QSpinBox *autosaveNumberBox = new QSpinBox();
	
	QLabel *snapshotDirectoryLabel = new QLabel(tr("Specify Snapshot Directory:"));
	QLineEdit *snapshotDirectoryBox = new QLineEdit(this);
	snapshotDirectoryBox->setFixedWidth(350);

	autosaveNumberBox->setMinimum(5);
	autosaveNumberBox->setMaximum(200);
	autosaveNumberBox->setFixedWidth(50);
	
	autosaveCheckBox->setChecked(false);
	if (settings.value("autosave")=="true") autosaveCheckBox->setChecked(true);
	
	autosaveNumberBox->setValue(settings.value("autosaveNumber").toInt()); 
	if (settings.value("autosaveNumber").toInt()==0) autosaveNumberBox->setValue(20);
	
	snapshotDirectoryBox->setText(settings.value("snapshotDir").toString());

	connect(autosaveNumberBox, SIGNAL(valueChanged(int)), parent, SIGNAL(autosaveNumberChange(int)));
	connect(autosaveCheckBox, SIGNAL(stateChanged(int)), parent, SIGNAL(autosaveChange(int)));
	connect(snapshotDirectoryBox, SIGNAL(textChanged(QString)), parent, SIGNAL(snapshotDirChange(QString)));
	
	lay->addWidget(autosaveCheckBox);
	lay->addWidget(autosaveNumberLabel);
	lay->addWidget(autosaveNumberBox);
	lay->addWidget(snapshotDirectoryLabel);
	lay->addWidget(snapshotDirectoryBox);

	autosaveBox->setLayout(lay);
	
	QVBoxLayout *lay2 = new QVBoxLayout();
	lay2->addWidget(autosaveBox);
	lay2->addStretch(1);
	setLayout(lay2);
}


