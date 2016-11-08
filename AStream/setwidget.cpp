#include "setwidget.h"

#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QFrame>
#include <QPainter>
#include <QFile>
#include <QSettings>
#include <QComboBox>
#include <QFileDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QSpinBox>

#include "colorWidget.h"

#include <QDebug>

setWidget::setWidget(int w, int h, QWidget *parent) : tranWidget(w, h, parent), isPaletteShown(false)
{

	try
	{
		 createSubCom();
		 setObjectName();
		 setGeo();
		 readConfig();
		 connectSignal();
		 setColor(QColor(255, 255, 255, 255));
	}
	catch (std::bad_alloc &)
	{
		abort();
	}

}

setWidget::~setWidget()
{
	//saveGeneral();
	//saveHotKey();
	//saveWindowLrc();
	//saveDeskLrc();
	//saveNetwork();
}

bool setWidget::eventFilter(QObject *obj, QEvent *event)
{
	auto editor = dynamic_cast<QLineEdit *>(obj);
	if (editor != nullptr)
	{
		if (event->type() != QEvent::KeyPress)
		{
			return QWidget::eventFilter(obj, event);
		}

		auto k = dynamic_cast<QKeyEvent *>(event);

		QString s;
		if (k->modifiers() != Qt::NoModifier)
		{
			bool flag = false;
			int mod = k->modifiers();

			if ((mod&Qt::ControlModifier) == Qt::ControlModifier)
			{
				s += "Ctrl+";
				flag = true;
			}
			if ((mod&Qt::AltModifier) == Qt::AltModifier)
			{
				s += "Alt+";
				flag = true;
			}
			if ((mod&Qt::ShiftModifier) == Qt::ShiftModifier)
			{
				s += "Shift+";
				flag = true;
			}

			if (flag)
			{
				auto &vector = allSequence.find(editor).value();
				vector[0] = mod;
				vector[1] = k->key();

				auto result = allKeys.find(k->key());
				if (result != allKeys.end())
				{
					s += result.value();
					lastText.find(editor).value() = s;
					editor->setText(s);
				}
			}

		}

		event->accept();
		return true;
	}

	auto widget = dynamic_cast<QWidget *>(obj);
	if (widget != nullptr)
	{
		if (event->type() != QEvent::ShowToParent)
		{
			return QWidget::eventFilter(obj, event);
		}

		if (widget == generalWidget)
		{
			autoPlayBt->setChecked(general.autoPlay);
			playHello->setChecked(general.playHello);
			keepIcon->setChecked(general.keepIcon);
			noKeepIcon->setChecked(!general.keepIcon);
			defaultPlayer->setChecked(general.isDefault);
			autoSave->setChecked(general.autoSave);
			lrcEdit->setText(general.lrcRoute);
			downEdit->setText(general.downLoadRoute);
			screenShotEdit->setText(general.saveRoute);
		}
		else if (widget == hotkeyWidget)
		{
			playPauseEd->setText(hotkeys.playPause);
			prevEd->setText(hotkeys.prevSong);
			nextEd->setText(hotkeys.nextSong);
			loudEd->setText(hotkeys.higher);
			quietEd->setText(hotkeys.lower);
			muteEd->setText(hotkeys.mute);
			screenShotEd->setText(hotkeys.screenShot);
			globalHotBt->setChecked(hotkeys.isHotKey);
		}
		else if (widget == windowLrcWidget)
		{
			fontBox->setCurrentIndex(fontBox->findText(windowlrc.fontName));
			fontSizeBox->setCurrentIndex(fontSizeBox->findText(QString::number(windowlrc.pixelSize)));
			fontStyleBox->setCurrentIndex(windowlrc.type);
			tempUnplay = windowlrc.unPlay;
			tempPlayed = windowlrc.played;
			resetWindowView();
		}
		else if (widget == desktopLrcWidget)
		{
			defontBox->setCurrentIndex(defontBox->findText(desklrc.fontName));
			defontSizeBox->setCurrentIndex(defontSizeBox->findText(QString::number(desklrc.pixelSize)));
			defontStyleBox->setCurrentIndex(desklrc.type);
			deTempUnplay[0] = desklrc.unPlay[0];
			deTempPlayed[0] = desklrc.played[0];
			deTempUnplay[1] = desklrc.unPlay[1];
			deTempPlayed[1] = desklrc.played[1];
			deTempUnplay[2] = desklrc.unPlay[2];
			deTempPlayed[2] = desklrc.played[2];
			resetDeskView();
		}
		else if (widget == advanceWidget)
		{
			typeBox->setCurrentIndex(network.proxyType);
			addrEdit->setText(network.address);
			portEdit->setText(network.port);
			userEdit->setText(network.userName);
			passwordEdit->setText(network.passWord);

			maxNum->setValue(network.resultNum);
		}

	}

	return QWidget::eventFilter(obj, event);
}

void setWidget::paintEvent(QPaintEvent *e)
{
	tranWidget::paintEvent(e);
	QPainter painter(this);
	painter.fillRect(0, 30, 160, height(), QColor(102, 204, 250, 180));
}

void setWidget::createSubCom()
{
	basicSetLa=new QLabel(this);
	generalSetBt=new QPushButton(this);
	generalWidget=new QWidget(this);
	hotkeyWidget = new QWidget(this);
	hotkeyButton=new QPushButton(this);

	windowLrcWidget = new QWidget(this);
	desktopLrcWidget = new QWidget(this);
	lrcSetLa = new QLabel(this);
	windowLrcBt = new QPushButton(this);
	desktopLrcBt = new QPushButton(this);

	submitBt = new QPushButton(u8"确定",this);
	cancelBt = new QPushButton(u8"取消",this);
	applyBt = new QPushButton(u8"应用",this);
	closeBt = new QPushButton(this);

	routeLine = new QFrame(generalWidget);
	routeLa=new QLabel(u8"目录设置",generalWidget);

	onStartLa = new QLabel(u8"程序启动时",generalWidget);
	lrcRouteLa = new QLabel(u8"歌词目录", generalWidget);
	downRouteLa = new QLabel(u8"下载目录",generalWidget);
	screenRouteLa = new QLabel(u8"截图目录", generalWidget);
	onCloseLa = new QLabel(u8"关闭主面板时", generalWidget);
	fileAssociateLa = new QLabel(u8"文件关联", generalWidget);
	fileBt = new QFrame(generalWidget);
	onStartLine = new QFrame(generalWidget);
	onCloseLine = new QFrame(generalWidget);
	keepIcon = new QRadioButton(u8"最小化到系统托盘，不退出程序", generalWidget);
	noKeepIcon = new QRadioButton(u8"退出程序", generalWidget);
	lrcEdit = new QLineEdit( generalWidget);
	downEdit = new QLineEdit(generalWidget);
	screenShotEdit = new QLineEdit(generalWidget);
	lrcButton = new QPushButton(u8"更改目录", generalWidget);
	downButton = new QPushButton(u8"更改目录", generalWidget);
	skinButton = new QPushButton(u8"更改目录", generalWidget);
	autoPlayBt = new QCheckBox(u8"自动播放音乐", generalWidget);
	playHello = new QCheckBox(u8"播放问候语", generalWidget);
	defaultPlayer = new QCheckBox(u8"设为默认播放器", generalWidget);
	autoSave = new QCheckBox(u8"自动保存截图", generalWidget);

	playCtrlLa = new QLabel(u8"播放设置",hotkeyWidget);
	playPauseLa = new QLabel(u8"播放/暂停", hotkeyWidget);
	prevLa = new QLabel(u8"上一首", hotkeyWidget);
	nextLa = new QLabel(u8"下一首", hotkeyWidget);
	loudLa = new QLabel(u8"增大音量", hotkeyWidget);
	quietLa = new QLabel(u8"减小音量", hotkeyWidget);
	muteLa = new QLabel(u8"静音", hotkeyWidget);
	otherLa = new QLabel(u8"其它功能", hotkeyWidget);
	screenShotLa = new QLabel(u8"截取屏幕", hotkeyWidget);

	playPauseEd = new QLineEdit(hotkeyWidget);
	prevEd = new QLineEdit(hotkeyWidget);
	nextEd = new QLineEdit(hotkeyWidget);
	loudEd = new QLineEdit(hotkeyWidget);
	quietEd = new QLineEdit(hotkeyWidget);
	muteEd = new QLineEdit(hotkeyWidget);
	screenShotEd = new QLineEdit(hotkeyWidget);

	otherLine = new QFrame(hotkeyWidget);
	playCtrlLine = new QFrame(hotkeyWidget);
	globalHotBt = new QCheckBox(u8"启用全局快捷键",hotkeyWidget);
	resetBt = new QPushButton(u8"恢复默认",hotkeyWidget);

	styleSetLa = new QLabel(u8"样式设置", windowLrcWidget);
	fontLa = new QLabel(u8"字体", windowLrcWidget);
	fontSizeLa = new QLabel(u8"字号", windowLrcWidget);
	fontStyleLa = new QLabel(u8"字型", windowLrcWidget);
	colorLa = new QLabel(u8"颜色", windowLrcWidget);
	viewLa = new QLabel(u8"预览", windowLrcWidget);
	viewPic = new QLabel(windowLrcWidget);
	styleFrame = new QFrame(windowLrcWidget);
	viewFrame = new QFrame(windowLrcWidget);
	fontBox = new QComboBox(windowLrcWidget);
	fontSizeBox = new QComboBox(windowLrcWidget);
	fontStyleBox = new QComboBox(windowLrcWidget);
	unplayBt = new QPushButton(u8"未播放",windowLrcWidget);
	playedBt = new QPushButton(u8"已播放",windowLrcWidget);
	defaultBt = new QPushButton(u8"恢复默认方案",windowLrcWidget);

	destyleSetLa = new QLabel(u8"样式设置", desktopLrcWidget);
	defontLa = new QLabel(u8"字体", desktopLrcWidget);
	defontSizeLa = new QLabel(u8"字号", desktopLrcWidget);
	defontStyleLa = new QLabel(u8"字型", desktopLrcWidget);
	decolorLa = new QLabel(u8"颜色", desktopLrcWidget);
	deviewLa = new QLabel(u8"预览", desktopLrcWidget);
	deviewPic = new QLabel(desktopLrcWidget);
	destyleFrame = new QFrame(desktopLrcWidget);
	deviewFrame = new QFrame(desktopLrcWidget);
	defontBox = new QComboBox(desktopLrcWidget);
	defontSizeBox = new QComboBox(desktopLrcWidget);
	defontStyleBox = new QComboBox(desktopLrcWidget);
	deunplayBt = new QPushButton(u8"未播放", desktopLrcWidget);
	deplayedBt = new QPushButton(u8"已播放", desktopLrcWidget);
	dedefaultBt = new QPushButton(u8"恢复默认方案", desktopLrcWidget);

	advanceSetLa=new QLabel(this);
	proxySetBt = new QPushButton(this);
	advanceWidget=new QWidget(this);

	proxyLa = new QLabel(u8"代理设置",advanceWidget);
	proxyTypeLa = new QLabel(u8"代理类型：", advanceWidget);
	addrLa = new QLabel(u8"代理地址：", advanceWidget);
	portLa = new QLabel(u8"端口：", advanceWidget);
	userLa = new QLabel(u8"用户名：", advanceWidget);
	passwordLa = new QLabel(u8"密码：", advanceWidget);
	addrEdit = new QLineEdit(advanceWidget);
	portEdit = new QLineEdit(advanceWidget);
	userEdit = new QLineEdit(advanceWidget);
	passwordEdit = new QLineEdit(advanceWidget);
	typeBox = new QComboBox(advanceWidget);
	proxyLine = new QFrame(advanceWidget);

	searchLa = new QLabel(u8"搜索设置", advanceWidget);
	searchLine=new QFrame(advanceWidget);
	resultNum = new QLabel(u8"搜索结果数", advanceWidget);
	maxNum = new QSpinBox(advanceWidget);

	palette = new paletteWidget(526, 288, 0);
	palette->hide();

}

void setWidget::setObjectName()
{
	basicSetLa->setObjectName("basicSetLa");
	generalSetBt->setObjectName("generalSetBt");
	hotkeyButton->setObjectName("hotkeySetBt");
	closeBt->setObjectName("closeBt");
	lrcSetLa->setObjectName("lrcSetLa");
	windowLrcBt->setObjectName("windowLrcBt");
	desktopLrcBt->setObjectName("desktopLrcBt");
	advanceSetLa->setObjectName("advanceSetLa");
	proxySetBt->setObjectName("proxySetBt");
}

void setWidget::setGeo()
{
	basicSetLa->setGeometry(10, 40, 128, 26);
	generalSetBt->setGeometry(10, 70, 128, 26);
	hotkeyButton->setGeometry(10, 110, 128, 26);
	generalWidget->setGeometry(180, 40, 400,350);
	hotkeyWidget->setGeometry(180, 40, 400, 350);
	windowLrcWidget->setGeometry(180, 40, 400, 350);
	desktopLrcWidget->setGeometry(180, 40, 400, 350);
	advanceWidget->setGeometry(180, 40, 400, 350);
	closeBt->setGeometry(577,0,23,18);

	submitBt->setGeometry(280,400,80,25);
	cancelBt->setGeometry(380,400,80,25);
	applyBt->setGeometry(480,400,80,25);

	onStartLa->setGeometry(10, 8,60, 20);
	onStartLine->setGeometry(80, 10, 310, 16);
	autoPlayBt->setGeometry(10, 40, 190, 16);
	playHello->setGeometry(210, 40, 190, 16);
	onCloseLa->setGeometry(10, 78, 75, 20);
	onCloseLine->setGeometry(90, 80, 300, 16);
	keepIcon->setGeometry(10, 110, 190, 16);

	noKeepIcon->setGeometry(210,110, 190, 16);
	fileAssociateLa->setGeometry(10,148,50,20);
	fileBt->setGeometry(70, 150, 320, 16);
	defaultPlayer->setGeometry(10,180, 190, 16);
	autoSave->setGeometry(210, 180, 190, 16);

	routeLa->setGeometry(10,218,50,20);
	routeLine->setGeometry(70,220,320,16);

	lrcRouteLa->setGeometry(10, 250, 54, 25);
	downRouteLa->setGeometry(10,285,54, 25);
	screenRouteLa->setGeometry(10, 320, 54, 25);

	lrcEdit->setGeometry(70, 250, 220, 25);
	downEdit->setGeometry(70, 285, 220, 25);
	screenShotEdit->setGeometry(70, 320, 220, 25);

	lrcButton->setGeometry(300, 250, 75, 25);
	downButton->setGeometry(300, 285, 75, 25);
	skinButton->setGeometry(300, 320, 75, 25);

	playCtrlLa->setGeometry(10, 10, 50, 16);
	playCtrlLine->setGeometry(60,10,310,16);
	playPauseLa->setGeometry(10,33,60,25);
	prevLa->setGeometry(10,67,54,25);
	nextLa->setGeometry(10,101,54,25);
	loudLa->setGeometry(10,135,54,25);
	quietLa->setGeometry(10,169,54,25);
	muteLa->setGeometry(10,203,54,25);
	otherLa->setGeometry(10,240,50,20);
	otherLine->setGeometry(60,240,310,20);
	screenShotLa->setGeometry(10,263,54,25);
	globalHotBt->setGeometry(10,297,160,20);
	resetBt->setGeometry(280, 297, 80, 25);
	playPauseEd->setGeometry(140,33,220,25);
	prevEd->setGeometry(140,67,220,25);
	nextEd->setGeometry(140,101,220,25);
	loudEd->setGeometry(140,135,220,25);
	quietEd->setGeometry(140,169,220,25);
	muteEd->setGeometry(140,203,220,25);
	screenShotEd->setGeometry(140,263,220,25);

	lrcSetLa->setGeometry(10,180,128,26);
	windowLrcBt->setGeometry(10, 210, 128, 26);
	desktopLrcBt->setGeometry(10, 240, 128, 26);

	styleSetLa->setGeometry(10, 10, 50, 16);
	fontLa->setGeometry(10, 40, 40, 20);
	fontSizeLa->setGeometry(150, 40, 30, 20);
	fontStyleLa->setGeometry(260, 40, 30, 20);
	colorLa->setGeometry(10, 90, 30, 20);
	viewLa->setGeometry(10, 140, 30, 20);
	viewPic->setGeometry(10, 180, 360, 100);
	styleFrame->setGeometry(60, 10, 310, 16);
	viewFrame->setGeometry(40, 140, 330, 16);
	fontBox->setGeometry(40, 38, 80, 25);
	fontSizeBox->setGeometry(180, 38, 60, 25);
	fontStyleBox->setGeometry(290, 38, 80, 25);
	unplayBt->setGeometry(50, 85, 80, 30);
	playedBt->setGeometry(150, 85, 80, 30);
	defaultBt->setGeometry(10, 310, 110, 25);

	destyleSetLa->setGeometry(10, 10, 50, 16);
	defontLa->setGeometry(10, 40, 40, 20);
	defontSizeLa->setGeometry(150, 40, 30, 20);
	defontStyleLa->setGeometry(260, 40, 30, 20);
	decolorLa->setGeometry(10, 90, 30, 20);
	deviewLa->setGeometry(10, 140, 30, 20);
	deviewPic->setGeometry(10, 180, 360, 100);
	destyleFrame->setGeometry(60, 10, 310, 16);
	deviewFrame->setGeometry(40, 140, 330, 16);
	defontBox->setGeometry(40, 38, 80, 25);
	defontSizeBox->setGeometry(180, 38, 60, 25);
	defontStyleBox->setGeometry(290, 38, 80, 25);
	deunplayBt->setGeometry(50, 85, 80, 30);
	deplayedBt->setGeometry(150, 85, 80, 30);
	dedefaultBt->setGeometry(10, 310, 110, 25);

	advanceSetLa->setGeometry(10, 340, 128, 26);
	proxySetBt->setGeometry(10,380,128,26);

	proxyLa->setGeometry(10,10,50,16);
	proxyTypeLa->setGeometry(10, 40, 60, 20);
	addrLa->setGeometry(10, 80, 60, 20);
	portLa->setGeometry(200, 80, 60, 20);
	userLa->setGeometry(10, 120, 60, 20);
	passwordLa->setGeometry(200, 120,60, 20);
	addrEdit->setGeometry(80, 80, 110, 20);
	portEdit->setGeometry(250, 80, 110, 20);
	userEdit->setGeometry(80, 120, 110, 20);
	passwordEdit->setGeometry(250, 120, 110, 20);
	typeBox->setGeometry(80, 40, 115, 20);
	proxyLine->setGeometry(60, 10, 310, 16);

	searchLa->setGeometry(10, 150, 50, 16);
	searchLine->setGeometry(60, 150, 310, 16);
	resultNum->setGeometry(10, 190, 60, 16); 
	maxNum->setGeometry(80, 186, 60, 25);

}

void setWidget::readConfig()
{
	QFile qss("data/qss/setWidget.qss");
	qss.open(QIODevice::ReadOnly);
	if (qss.isOpen())
	{
		setStyleSheet(qss.readAll());
	}

	fileBt->setFrameShape(QFrame::HLine);
	onCloseLine->setFrameShape(QFrame::HLine);
	onStartLine->setFrameShape(QFrame::HLine);
	routeLine->setFrameShape(QFrame::HLine);
	otherLine->setFrameShape(QFrame::HLine);
	playCtrlLine->setFrameShape(QFrame::HLine);
	styleFrame->setFrameShape(QFrame::HLine);
	viewFrame->setFrameShape(QFrame::HLine);
	destyleFrame->setFrameShape(QFrame::HLine);
	deviewFrame->setFrameShape(QFrame::HLine);
	proxyLine->setFrameShape(QFrame::HLine);
	searchLine->setFrameShape(QFrame::HLine);

	windowLrcWidget->hide();
	desktopLrcWidget->hide();
	hotkeyWidget->hide();
	advanceWidget->hide();
	currentWidget = generalWidget;

	fontSizeBox->addItem("14");
	fontSizeBox->addItem("16");
	fontSizeBox->addItem("18");
	fontSizeBox->addItem("20");
	fontSizeBox->addItem("22");
	fontSizeBox->addItem("24");
	fontSizeBox->addItem("26");
	fontSizeBox->addItem("28");
	fontSizeBox->addItem("30");

	fontBox->addItem(u8"微软雅黑");
	fontBox->addItem(u8"黑体");
	fontBox->addItem(u8"宋体");
	fontBox->addItem(u8"楷体");
	fontBox->addItem(u8"隶书");

	fontStyleBox->addItem(u8"常规");
	fontStyleBox->addItem(u8"加粗");
	fontStyleBox->addItem(u8"斜体");
	fontStyleBox->addItem(u8"加粗 斜体");

	defontSizeBox->addItem("26");
	defontSizeBox->addItem("28");
	defontSizeBox->addItem("30");
	defontSizeBox->addItem("32");
	defontSizeBox->addItem("34");
	defontSizeBox->addItem("36");
	defontSizeBox->addItem("38");

	defontBox->addItem(u8"微软雅黑");
	defontBox->addItem(u8"黑体");
	defontBox->addItem(u8"宋体");
	defontBox->addItem(u8"楷体");
	defontBox->addItem(u8"隶书");

	defontStyleBox->addItem(u8"常规");
	defontStyleBox->addItem(u8"加粗");
	defontStyleBox->addItem(u8"斜体");
	defontStyleBox->addItem(u8"加粗 斜体");

	typeBox->addItem(u8"不使用代理");
	typeBox->addItem(u8"使用SOCKS5代理");
	typeBox->addItem(u8"使用HTTP代理");

	passwordEdit->setEchoMode(QLineEdit::Password);

	//readGeneral();
	//readHotKey();
	//readWindowLrc();
	readDeskLrc();
	readNetwork();
}

void setWidget::hotKeyTextChange(const QString &str)
{
	auto editor = dynamic_cast<QLineEdit *>(sender());
	auto &text=lastText.find(editor).value();
	if (str != text)
	{
		editor->setText(text);
	}
}

void setWidget::readHotKey()
{

	for (int i = 0x01000030, j = 1; i < 0x0100003c; i++, j++)
	{
		allKeys.insert(i, "F" + QString::number(j));
	}

	for (int i = 0x30, j = 0; i < 0x3a; j++, i++)
	{
		allKeys.insert(i, QString::number(j));
	}

	for (int i = 0x41, j = 0; i < 0x5b; j++, i++)
	{
		QString t;
		t += i;
		allKeys.insert(i, t);
	}

	allKeys.insert(0x01000012, "Left");
	allKeys.insert(0x01000013, "Up");
	allKeys.insert(0x01000014, "Right");
	allKeys.insert(0x01000015, "Down");
	allKeys.insert(0x2d, "-");
	allKeys.insert(0x3d, "=");
	allKeys.insert(0x5b, "[");
	allKeys.insert(0x5d, "]");
	allKeys.insert(0x60, "`");
	allKeys.insert(0x3b, ";");
	allKeys.insert(0x3b, ";");
	allKeys.insert(0x27, "'");
	allKeys.insert(0x5c, "\\");
	allKeys.insert(0x2c, ",");
	allKeys.insert(0x2f, "/");

	QSettings gs("data/setWidget.ini", QSettings::IniFormat);
	hotkeys.higher = gs.value("HotKey/higher").toString();
	hotkeys.isHotKey = gs.value("HotKey/isHotKey").toBool();
	hotkeys.lower = gs.value("HotKey/lower").toString();
	hotkeys.mute = gs.value("HotKey/mute").toString();
	hotkeys.nextSong = gs.value("HotKey/nextSong").toString();
	hotkeys.playPause = gs.value("HotKey/playPause").toString();
	hotkeys.prevSong = gs.value("HotKey/prevSong").toString();
	hotkeys.screenShot = gs.value("HotKey/screenShot").toString();
	
	playPauseEd->installEventFilter(this);
	prevEd->installEventFilter(this);
	nextEd->installEventFilter(this);
	loudEd->installEventFilter(this);
	quietEd->installEventFilter(this);
	muteEd->installEventFilter(this);
	screenShotEd->installEventFilter(this);

	lastText.insert(playPauseEd, hotkeys.playPause);
	lastText.insert(prevEd, hotkeys.prevSong);
	lastText.insert(nextEd, hotkeys.nextSong);
	lastText.insert(loudEd, hotkeys.higher);
	lastText.insert(quietEd, hotkeys.lower);
	lastText.insert(muteEd, hotkeys.mute);
	lastText.insert(screenShotEd, hotkeys.screenShot);

	allSequence.insert(playPauseEd, std::move(QVector<int>(2)));
	allSequence.insert(prevEd, std::move(QVector<int>(2)));
	allSequence.insert(nextEd, std::move(QVector<int>(2)));
	allSequence.insert(loudEd, std::move(QVector<int>(2)));
	allSequence.insert(quietEd, std::move(QVector<int>(2)));
	allSequence.insert(muteEd, std::move(QVector<int>(2)));
	allSequence.insert(screenShotEd, std::move(QVector<int>(2)));

	auto getKeyCode = [this,&gs](const QString &str,QVector<int> &vector)
	{
		auto list = gs.value(str).toString().split('+');
		vector[0] = list[0].toInt();
		vector[1] = list[1].toInt();
	};
	
	getKeyCode("HotKey/playKey",hotkeys.playKeys);
	getKeyCode("HotKey/prevKey", hotkeys.prevKeys);
	getKeyCode("HotKey/nextKey", hotkeys.nextKeys);
	getKeyCode("HotKey/higherKey", hotkeys.higherKeys);
	getKeyCode("HotKey/lowerKey", hotkeys.lowerKeys);
	getKeyCode("HotKey/muteKey", hotkeys.muteKeys);
	getKeyCode("HotKey/screenShotKey", hotkeys.screenShotKeys);

}

void setWidget::readGeneral()
{
	lrcEdit->setEnabled(false);
	downEdit->setEnabled(false);
	screenShotEdit->setEnabled(false);

	QSettings gs("data/setWidget.ini", QSettings::IniFormat);
	general.autoPlay = gs.value("Gen/autoPlay").toBool();
	general.autoSave = gs.value("Gen/autoSave").toBool();
	general.isDefault = gs.value("Gen/isDefault").toBool();
	general.keepIcon = gs.value("Gen/keepIcon").toBool();
	general.playHello = gs.value("Gen/playHello").toBool();
	general.lrcRoute = gs.value("Gen/lrcRoute").toString();
	general.downLoadRoute = gs.value("Gen/downLoadRoute").toString();
	general.saveRoute = gs.value("Gen/saveRoute").toString();

}

void setWidget::readWindowLrc()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	windowlrc.fontName = gs.value("WindowLrc/winFontName").toString();
	windowlrc.type = static_cast<windowLrc::fontType>(gs.value("WindowLrc/winFontType").toString().toInt());
	windowlrc.pixelSize = gs.value("WindowLrc/winPixelSize").toString().toInt();

	auto list = gs.value("WindowLrc/winUnplayColor").toString().split('+');
	windowlrc.unPlay.setRgb(list[0].toInt(), list[1].toInt(), list[2].toInt());
	list = gs.value("WindowLrc/winPlayedColor").toString().split('+');
	windowlrc.played.setRgb(list[0].toInt(), list[1].toInt(), list[2].toInt());

	fontBox->setCurrentIndex(fontBox->findText(windowlrc.fontName));
	fontSizeBox->setCurrentIndex(fontSizeBox->findText(QString::number(windowlrc.pixelSize)));
	fontStyleBox->setCurrentIndex(windowlrc.type);

	tempUnplay = windowlrc.unPlay;
	tempPlayed = windowlrc.played;

	resetWindowView();

}

void setWidget::readDeskLrc()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	desklrc.fontName = gs.value("DeskLrc/deFontName").toString();
	desklrc.type = static_cast<windowLrc::fontType>(gs.value("DeskLrc/deFontType").toString().toInt());
	desklrc.pixelSize = gs.value("DeskLrc/dePixelSize").toString().toInt();

	auto list = gs.value("DeskLrc/deUnplayColor").toString().split('+');
	desklrc.unPlay[0].setRgb(list[0].toInt(), list[1].toInt(), list[2].toInt());
	desklrc.unPlay[1].setRgb(list[3].toInt(), list[4].toInt(), list[5].toInt());
	desklrc.unPlay[2].setRgb(list[6].toInt(), list[7].toInt(), list[8].toInt());

	list = gs.value("DeskLrc/dePlayedColor").toString().split('+');
	desklrc.played[0].setRgb(list[0].toInt(), list[1].toInt(), list[2].toInt());
	desklrc.played[1].setRgb(list[3].toInt(), list[4].toInt(), list[5].toInt());
	desklrc.played[2].setRgb(list[6].toInt(), list[7].toInt(), list[8].toInt());

	defontBox->setCurrentIndex(defontBox->findText(desklrc.fontName));
	defontSizeBox->setCurrentIndex(defontSizeBox->findText(QString::number(desklrc.pixelSize)));
	defontStyleBox->setCurrentIndex(desklrc.type);

	deTempUnplay[0] = desklrc.unPlay[0];
	deTempUnplay[1] = desklrc.unPlay[1];
	deTempUnplay[2] = desklrc.unPlay[2];

	deTempPlayed[0] = desklrc.played[0];
	deTempPlayed[1] = desklrc.played[1];
	deTempPlayed[2] = desklrc.played[2];

	unplayWidget = new colorWidget(250, 205, deTempUnplay);
	playedWidget = new colorWidget(250, 205, deTempPlayed);
	unplayWidget->hide();
	playedWidget->hide();

	resetDeskView();
}

void setWidget::readNetwork()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	network.proxyType = gs.value("NetworkSet/proxyType").toInt();
	network.address = gs.value("NetworkSet/address").toString();
	network.port = gs.value("NetworkSet/port").toString();
	network.userName = gs.value("NetworkSet/userName").toString();
	network.passWord = gs.value("NetworkSet/passWord").toString();
	network.resultNum = gs.value("NetworkSet/resultNum").toInt();

	typeBox->setCurrentIndex(network.proxyType);
	addrEdit->setText(network.address);
	portEdit->setText(network.port);
	userEdit->setText(network.userName);
	passwordEdit->setText(network.passWord);

	maxNum->setValue(network.resultNum);
}

void setWidget::saveWindowLrc()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	gs.setValue("WindowLrc/winFontName", windowlrc.fontName);
	gs.setValue("WindowLrc/winUnplayColor", QString::number(windowlrc.unPlay.red())+'+'+ QString::number(windowlrc.unPlay.green()) 
		+ '+' + QString::number(windowlrc.unPlay.blue()) );
	gs.setValue("WindowLrc/winPlayedColor", QString::number(windowlrc.played.red()) + '+' + QString::number(windowlrc.played.green())
		+ '+' + QString::number(windowlrc.played.blue()));
	gs.setValue("WindowLrc/winFontType", QString::number(windowlrc.type));
	gs.setValue("WindowLrc/winPixelSize", QString::number(windowlrc.pixelSize));

}

void setWidget::saveDeskLrc()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	gs.setValue("DeskLrc/deFontName", desklrc.fontName);

	gs.setValue("DeskLrc/deUnplayColor", QString::number(desklrc.unPlay[0].red()) + '+' + QString::number(desklrc.unPlay[0].green())
		+ '+' + QString::number(desklrc.unPlay[0].blue())
	+ QString::number(desklrc.unPlay[1].red()) + '+' + QString::number(desklrc.unPlay[1].green())
		+ '+' + QString::number(desklrc.unPlay[1].blue())
	+ QString::number(desklrc.unPlay[2].red()) + '+' + QString::number(desklrc.unPlay[2].green())
		+ '+' + QString::number(desklrc.unPlay[2].blue())
	);

	gs.setValue("DeskLrc/dePlayedColor", QString::number(desklrc.played[0].red()) + '+' + QString::number(desklrc.played[0].green())
		+ '+' + QString::number(desklrc.played[0].blue())
	+ QString::number(desklrc.played[1].red()) + '+' + QString::number(desklrc.played[1].green())
		+ '+' + QString::number(desklrc.played[1].blue())
	+ QString::number(desklrc.played[2].red()) + '+' + QString::number(desklrc.played[2].green())
		+ '+' + QString::number(desklrc.played[2].blue())
	);

	gs.setValue("DeskLrc/deFontType", QString::number(desklrc.type));
	gs.setValue("DeskLrc/dePixelSize", QString::number(desklrc.pixelSize));
}

void setWidget::saveNetwork()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	gs.setValue("NetworkSet/proxyType",QString::number(network.proxyType));
	gs.setValue("NetworkSet/address",network.address);
	gs.setValue("NetworkSet/port",network.port);
	gs.setValue("NetworkSet/userName",network.userName);
	gs.setValue("NetworkSet/passWord",network.passWord);
	gs.setValue("NetworkSet/resultNum",QString::number(network.resultNum));

}

void setWidget::saveGeneral()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	gs.setValue("Gen/autoPlay", general.autoPlay);
	gs.setValue("Gen/autoSave", general.autoSave);
	gs.setValue("Gen/isDefault", general.isDefault);
	gs.setValue("Gen/keepIcon", general.keepIcon);
	gs.setValue("Gen/playHello", general.playHello);
	gs.setValue("Gen/lrcRoute", general.lrcRoute);
	gs.setValue("Gen/downLoadRoute", general.downLoadRoute);
	gs.setValue("Gen/saveRoute", general.saveRoute);

}

void setWidget::saveHotKey()
{
	QSettings gs("data/setWidget.ini", QSettings::IniFormat);

	gs.setValue("HotKey/playPause", hotkeys.playPause);
	gs.setValue("HotKey/prevSong", hotkeys.prevSong);
	gs.setValue("HotKey/nextSong", hotkeys.nextSong);
	gs.setValue("HotKey/higher", hotkeys.higher);
	gs.setValue("HotKey/lower", hotkeys.lower);
	gs.setValue("HotKey/mute", hotkeys.mute);
	gs.setValue("HotKey/screenShot", hotkeys.screenShot);
	gs.setValue("HotKey/isHotKey", QString::number(static_cast<int>(hotkeys.isHotKey)));

	gs.setValue("HotKey/playKey",QString::number(hotkeys.playKeys[0])+'+'+ QString::number(hotkeys.playKeys[1]));
	gs.setValue("HotKey/prevKey", QString::number(hotkeys.prevKeys[0]) + '+' + QString::number(hotkeys.prevKeys[1]));
	gs.setValue("HotKey/nextKey", QString::number(hotkeys.nextKeys[0]) + '+' + QString::number(hotkeys.nextKeys[1]));
	gs.setValue("HotKey/higherKey", QString::number(hotkeys.higherKeys[0]) + '+' + QString::number(hotkeys.higherKeys[1]));
	gs.setValue("HotKey/lowerKey", QString::number(hotkeys.lowerKeys[0]) + '+' + QString::number(hotkeys.lowerKeys[1]));
	gs.setValue("HotKey/muteKey", QString::number(hotkeys.muteKeys[0]) + '+' + QString::number(hotkeys.muteKeys[1]));
	gs.setValue("HotKey/screenShotKey", QString::number(hotkeys.screenShotKeys[0]) + '+' + QString::number(hotkeys.screenShotKeys[1]));

}

void setWidget::connectSignal()
{
	connect(closeBt, &QPushButton::clicked, this, &QWidget::close);
	connect(generalSetBt, &QPushButton::clicked, this, &setWidget::resetCurrentWidget);
	connect(hotkeyButton, &QPushButton::clicked, this, &setWidget::resetCurrentWidget);
	connect(windowLrcBt, &QPushButton::clicked, this, &setWidget::resetCurrentWidget);
	connect(desktopLrcBt, &QPushButton::clicked, this, &setWidget::resetCurrentWidget);
	connect(proxySetBt, &QPushButton::clicked, this, &setWidget::resetCurrentWidget);

	connect(lrcButton, &QPushButton::clicked, this, &setWidget::routeChange);
	connect(downButton, &QPushButton::clicked, this, &setWidget::routeChange);
	connect(skinButton, &QPushButton::clicked, this, &setWidget::routeChange);

	connect(cancelBt, &QPushButton::clicked, this, &QWidget::hide);
	connect(applyBt, &QPushButton::clicked, this, &setWidget::applyChange);
	connect(submitBt, &QPushButton::clicked, [this]()
	{
		applyChange();
		hide();
	}
	);

	connect(playPauseEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);
	connect(prevEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);
	connect(nextEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);
	connect(loudEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);
	connect(quietEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);
	connect(muteEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);
	connect(screenShotEd, &QLineEdit::textEdited, this, &setWidget::hotKeyTextChange);

	connect(resetBt, &QPushButton::clicked, [this]()
	{
		hotkeys.playPause = "Ctrl+F5";
		hotkeys.prevSong = "Ctrl+Left";
		hotkeys.nextSong = "Ctrl+Right";
		hotkeys.higher = "Ctrl+Up";
		hotkeys.lower = "Ctrl+Down";
		hotkeys.mute = "Ctrl+M";
		hotkeys.screenShot = "Ctrl+Q";
		hotkeys.isHotKey = true;

		hotkeys.playKeys = std::move(QVector<int>{67108864, 16777249});
		hotkeys.prevKeys = std::move(QVector<int>{67108864, 16777234});
		hotkeys.nextKeys = std::move(QVector<int>{67108864, 16777236});
		hotkeys.higherKeys = std::move(QVector<int>{67108864, 16777235});
		hotkeys.lowerKeys = std::move(QVector<int>{67108864, 16777237});
		hotkeys.muteKeys = std::move(QVector<int>{67108864, 77});
		hotkeys.screenShotKeys = std::move(QVector<int>{67108864, 81});

		playPauseEd->setText(hotkeys.playPause);
		prevEd->setText(hotkeys.prevSong);
		nextEd->setText(hotkeys.nextSong);
		loudEd->setText(hotkeys.higher);
		quietEd->setText(hotkeys.lower);
		muteEd->setText(hotkeys.mute);
		screenShotEd->setText(hotkeys.screenShot);
		globalHotBt->setChecked(hotkeys.isHotKey);

		allSequence.find(loudEd).value() = hotkeys.higherKeys;
		allSequence.find(quietEd).value() = hotkeys.lowerKeys;
		allSequence.find(muteEd).value() = hotkeys.muteKeys;
		allSequence.find(nextEd).value() = hotkeys.nextKeys;
		allSequence.find(playPauseEd).value() = hotkeys.playKeys;
		allSequence.find(prevEd).value() = hotkeys.prevKeys;
		allSequence.find(screenShotEd).value() = hotkeys.screenShotKeys;

	}
	);

	connect(defaultBt, &QPushButton::clicked, [this]()
	{
		windowlrc.fontName = u8"微软雅黑";
		windowlrc.pixelSize = 22;
		windowlrc.type = windowLrc::fontType::none;
		windowlrc.played = QColor(251, 219, 131);
		windowlrc.unPlay = QColor(255, 255,255);
		fontBox->setCurrentIndex(fontBox->findText(windowlrc.fontName));
		fontSizeBox->setCurrentIndex(fontSizeBox->findText(QString::number(windowlrc.pixelSize)));
		fontStyleBox->setCurrentIndex(windowlrc.type);
		tempUnplay = windowlrc.unPlay;
		tempPlayed = windowlrc.played;
		resetWindowView();
	}
	);

	connect(dedefaultBt, &QPushButton::clicked, [this]()
	{
		desklrc.fontName = u8"黑体";
		desklrc.pixelSize = 32;
		desklrc.type = windowLrc::fontType::none;

		desklrc.played [0]= QColor(102, 204, 250);
		desklrc.played[1] = QColor(255, 255, 255);
		desklrc.played[2] = QColor(102, 204, 250);

		desklrc.unPlay[0] = QColor(0, 51, 153);
		desklrc.unPlay[1] = QColor(80, 170, 250);
		desklrc.unPlay[2] = QColor(0, 102, 255);

		defontBox->setCurrentIndex(defontBox->findText(desklrc.fontName));
		defontSizeBox->setCurrentIndex(defontSizeBox->findText(QString::number(desklrc.pixelSize)));
		defontStyleBox->setCurrentIndex(desklrc.type);

		deTempUnplay[0] = desklrc.unPlay[0];
		deTempUnplay[1] = desklrc.unPlay[1];
		deTempUnplay[2] = desklrc.unPlay[2];

		deTempPlayed[0] = desklrc.played[0];
		deTempPlayed[1] = desklrc.played[1];
		deTempPlayed[2] = desklrc.played[2];

		resetDeskView();
	}
	);

	connect(fontBox, &QComboBox::currentTextChanged, this, &setWidget::resetWindowView);
	connect(fontSizeBox, &QComboBox::currentTextChanged, this, &setWidget::resetWindowView);
	connect(fontStyleBox, &QComboBox::currentTextChanged, this, &setWidget::resetWindowView);

	connect(defontBox, &QComboBox::currentTextChanged, this, &setWidget::resetDeskView);
	connect(defontSizeBox, &QComboBox::currentTextChanged, this, &setWidget::resetDeskView);
	connect(defontStyleBox, &QComboBox::currentTextChanged, this, &setWidget::resetDeskView);

	connect(unplayBt,&QPushButton::clicked,this,&setWidget::selectColor);
	connect(playedBt, &QPushButton::clicked, this, &setWidget::selectColor);

	connect(palette, &paletteWidget::colorSetted, [this](QColor c)
	{
		isPaletteShown = false;
		*currentColor = c;
		resetWindowView();
		unplayWidget->resetColor();
		playedWidget->resetColor();
	}
	);
	connect(palette, &paletteWidget::closed, [this](){isPaletteShown = false;});

	connect(deunplayBt, &QPushButton::clicked, unplayWidget, &colorWidget::show);
	connect(deplayedBt, &QPushButton::clicked, playedWidget, &colorWidget::show);

	connect(unplayWidget->getFirst(), &QPushButton::clicked, this, &setWidget::selectColor);
	connect(unplayWidget->getSecond(), &QPushButton::clicked, this, &setWidget::selectColor);
	connect(unplayWidget->getThird(), &QPushButton::clicked, this, &setWidget::selectColor);

	connect(playedWidget->getFirst(), &QPushButton::clicked, this, &setWidget::selectColor);
	connect(playedWidget->getSecond(), &QPushButton::clicked, this, &setWidget::selectColor);
	connect(playedWidget->getThird(), &QPushButton::clicked, this, &setWidget::selectColor);

	connect(unplayWidget, &colorWidget::colorApply, this, &setWidget::resetDeskView);
	connect(playedWidget, &colorWidget::colorApply, this, &setWidget::resetDeskView);

	generalWidget->installEventFilter(this);
	hotkeyWidget->installEventFilter(this);
	windowLrcWidget->installEventFilter(this);
	desktopLrcWidget->installEventFilter(this);
	advanceWidget->installEventFilter(this);

}

void setWidget::routeChange()
{
	auto route = QFileDialog::getExistingDirectory(this);
	auto sendBt = dynamic_cast<QPushButton *>(sender());

	if (sendBt == lrcButton)
	{
		lrcEdit->setText(route);
	}
	else if (sendBt == downButton)
	{
		downEdit->setText(route);
	}
	else if (sendBt == skinButton)
	{
		screenShotEdit->setText(route);
	}
}

void setWidget::applyChange()
{
	if (currentWidget == generalWidget)
	{
		general.autoPlay = autoPlayBt->isChecked();
		general.autoSave = autoSave->isChecked();
		general.downLoadRoute = downEdit->text();
		general.isDefault = defaultBt->isChecked();
		general.keepIcon = keepIcon->isChecked();
		general.lrcRoute = lrcEdit->text();
		general.playHello = playHello->isChecked();
		general.saveRoute = screenShotEdit->text();

		emit resetGeneral(general);
	}
	else if (currentWidget == hotkeyWidget)
	{
		hotkeys.higher = loudEd->text();
		hotkeys.isHotKey = globalHotBt->isChecked();
		hotkeys.lower = quietEd->text();
		hotkeys.mute = muteEd->text();
		hotkeys.nextSong = nextEd->text();
		hotkeys.playPause = playPauseEd->text();
		hotkeys.prevSong = prevEd->text();
		hotkeys.screenShot = screenShotEd->text();

		hotkeys.higherKeys = allSequence.find(loudEd).value();
		hotkeys.lowerKeys = allSequence.find(quietEd).value();
		hotkeys.muteKeys = allSequence.find(muteEd).value();
		hotkeys.nextKeys = allSequence.find(nextEd).value();
		hotkeys.playKeys = allSequence.find(playPauseEd).value();
		hotkeys.prevKeys = allSequence.find(prevEd).value();
		hotkeys.screenShotKeys= allSequence.find(screenShotEd).value();

		emit resetHotKey(hotkeys);
	}
	else if (currentWidget == windowLrcWidget)
	{
		windowlrc.fontName = fontBox->currentText();
		windowlrc.pixelSize = fontSizeBox->currentText().toInt();
		windowlrc.played = tempPlayed;
		windowlrc.unPlay = tempUnplay;
		windowlrc.type =static_cast<windowLrc::fontType>( fontStyleBox->currentIndex());

		emit resetWindowLrc(windowlrc);
	}
	else if (currentWidget == desktopLrcWidget)
	{
		desklrc.fontName = defontBox->currentText();
		desklrc.pixelSize = defontSizeBox->currentText().toInt();
		desklrc.played[0] = deTempPlayed[0];
		desklrc.unPlay[0] = deTempUnplay[0];
		desklrc.played[1] = deTempPlayed[1];
		desklrc.unPlay[1] = deTempUnplay[1];
		desklrc.played[2] = deTempPlayed[2];
		desklrc.unPlay[2] = deTempUnplay[2];
		desklrc.type = static_cast<windowLrc::fontType>(defontStyleBox->currentIndex());

		emit resetDeskLrc(desklrc);
	}
	else if (currentWidget == advanceWidget)
	{
		network.address = addrEdit->text();
		network.passWord = passwordEdit->text();
		network.port = portEdit->text();
		network.proxyType = typeBox->currentIndex();
		network.userName = userEdit->text();
		network.resultNum = maxNum->value();

		emit resetNetwork(network);
	}


}

void setWidget::resetWindowView()
{
	QPixmap map(30, 22);
	QPainter painter(&map);
	painter.fillRect(0, 0, 30, 22, tempUnplay);
	unplayBt->setIcon(map);
	unplayBt->setIconSize(QSize(30, 22));
	painter.fillRect(0, 0, 30, 22, tempPlayed);
	playedBt->setIcon(map);
	playedBt->setIconSize(QSize(30, 22));

	QPixmap buffer(360, 58);
	QPixmap view(360, 100);
	QPainter viewPainter(&buffer);
	QPainter viewPainter2(&view);
	buffer.fill(Qt::black);
	view.fill(Qt::black);

	QFont font;
	if (fontStyleBox->currentIndex() == 1)
	{
		font.setBold(true);
	}
	else if (fontStyleBox->currentIndex() == 2)
	{
		font.setItalic(true);
	}
	else  if (fontStyleBox->currentIndex() == 3)
	{
		font.setItalic(true);
		font.setBold(true);
	}

	int size = fontSizeBox->currentText().toInt();
	font.setFamily(fontBox->currentText());
	font.setPixelSize(size);
	viewPainter.setFont(font);

	viewPainter.setPen(tempUnplay);
	viewPainter.drawText(0, 0, 360, 58, Qt::AlignVCenter | Qt::AlignLeft, u8"yy音乐，传播好声音");
	viewPainter.setPen(tempPlayed);
	viewPainter.drawText(0, 0, font.pixelSize() * 5, 58, Qt::AlignVCenter | Qt::AlignLeft, u8"yy音乐，传播好声音");

	viewPainter2.drawPixmap((360 - (font.pixelSize() * 9)) / 2, 21, buffer);
	viewPic->setPixmap(view);
}

void setWidget::resetDeskView()
{
	QPixmap map(30,22);
	QPainter painter(&map);
	QLinearGradient gd;
	gd.setStart(0, 0);
	gd.setFinalStop(0, 22);

	gd.setColorAt(0, deTempUnplay[0]);
	gd.setColorAt(0.5, deTempUnplay[1]);
	gd.setColorAt(1, deTempUnplay[2]);
	painter.fillRect(0, 0, 30, 22, gd);
	deunplayBt->setIcon(map);
	deunplayBt->setIconSize(QSize(30, 22));

	gd.setColorAt(0, deTempPlayed[0]);
	gd.setColorAt(0.5, deTempPlayed[1]);
	gd.setColorAt(1, deTempPlayed[2]);
	painter.fillRect(0, 0, 30,22, gd);
	deplayedBt->setIcon(map);
	deplayedBt->setIconSize(QSize(30,22));

	QPixmap buffer(360,58);
	QPixmap view(360, 100);
	QPainter viewPainter(&buffer);
	QPainter viewPainter2(&view);
	buffer.fill(Qt::black);
	view.fill(Qt::black);

	gd.setStart(0, 0);
	gd.setFinalStop(0, 58);
	gd.setColorAt(0.2, deTempUnplay[0]);
	gd.setColorAt(0.5, deTempUnplay[1]);
	gd.setColorAt(0.8, deTempUnplay[2]);

	QFont font;
	if (defontStyleBox->currentIndex() == 1)
	{
		font.setBold(true);
	}
	else if (defontStyleBox->currentIndex() == 2)
	{
		font.setItalic(true);
	}
	else  if (defontStyleBox->currentIndex() == 3)
	{
		font.setItalic(true);
		font.setBold(true);
	}

	int size = defontSizeBox->currentText().toInt();
	font.setFamily(defontBox->currentText());
	font.setPixelSize(size);
	viewPainter.setPen(QPen(gd, 0));
	viewPainter.setFont(font);
	viewPainter.drawText(0, 0,360, 58, Qt::AlignVCenter | Qt::AlignLeft, u8"yy音乐，传播好声音");

	gd.setColorAt(0.2, deTempPlayed[0]);
	gd.setColorAt(0.5, deTempPlayed[1]);
	gd.setColorAt(0.8, deTempPlayed[2]);
	viewPainter.setPen(QPen(gd, 0));
	viewPainter.drawText(0, 0, font.pixelSize()*5, 58, Qt::AlignVCenter | Qt::AlignLeft, u8"yy音乐，传播好声音");

	viewPainter2.drawPixmap((360-(font.pixelSize() * 9))/2, 21, buffer);
	deviewPic->setPixmap(view);
}

void setWidget::selectColor()
{
	if (isPaletteShown)
	{
		return;
	}
	else
	{
		isPaletteShown = true;
	}
	
	auto ptr = sender();

	if (ptr== unplayBt)
	{
		currentColor = &tempUnplay;
	}
	else if(ptr==playedWidget)
	{
		currentColor = &tempPlayed;
	}
	else if (ptr ==unplayWidget->getFirst() )
	{
		currentColor = &unplayWidget->getColor(0);
	}
	else if (ptr == unplayWidget->getSecond())
	{
		currentColor = &unplayWidget->getColor(1);
	}
	else if (ptr == unplayWidget->getThird())
	{
		currentColor = &unplayWidget->getColor(2);
	}
	else if (ptr == playedWidget->getFirst())
	{
		currentColor = &playedWidget->getColor(0);
	}
	else if (ptr == playedWidget->getSecond())
	{
		currentColor = &playedWidget->getColor(1);
	}
	else if (ptr == playedWidget->getThird())
	{
		currentColor = &playedWidget->getColor(2);
	}

	palette->show();
}

void setWidget::resetCurrentWidget()
{
	auto sendBt = sender();
	currentWidget->hide();
	if (sendBt == generalSetBt)
	{
		currentWidget = generalWidget;
	}
	else if (sendBt == hotkeyButton)
	{
		currentWidget = hotkeyWidget;
	}
	else if (sendBt == windowLrcBt)
	{
		currentWidget = windowLrcWidget;
	}
	else if (sendBt == desktopLrcBt)
	{
		currentWidget = desktopLrcWidget;
	}
	else if (sendBt == proxySetBt)
	{
		currentWidget=advanceWidget;
	}

	currentWidget->show();
}
