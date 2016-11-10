#ifndef SETWIDGET_H
#define SETWIDGET_H

#include "tranWidget.h"
#include "setStructs.h"
#include "paletteWidget.h"

#include <QMap>
#include <QVector>

class QPushButton;
class QLineEdit;
class QLabel;
class QRadioButton;
class QFrame;
class QCheckBox;
class QComboBox;
class colorWidget;
class QSpinBox;

class setWidget : public tranWidget
{
	Q_OBJECT
public:
	setWidget(int,int,QWidget *parent = 0);
	void getSet();
	~setWidget();
protected:
	generalSet general;
	hotKeys hotkeys;
	windowLrc windowlrc;
	deskLrc desklrc;
	networkSet network;
	QColor tempUnplay, tempPlayed;
	QColor deTempUnplay[3], deTempPlayed[3];
	QMap<int, QString> allKeys;
	paletteWidget *palette;
	colorWidget *unplayWidget;
	colorWidget *playedWidget;
	QMap<QLineEdit *, QString> lastText;
	QMap<QLineEdit *, QVector<int>>allSequence;
	QLabel *basicSetLa;
	QPushButton *generalSetBt;
	QWidget *currentWidget;
	QWidget *generalWidget;
	QWidget *hotkeyWidget;
	QWidget *windowLrcWidget;
	QWidget *desktopLrcWidget;
	QLabel *advanceSetLa;
	QPushButton *proxySetBt;
	QWidget *advanceWidget;
	QLabel *lrcSetLa;
	QPushButton *windowLrcBt;
	QPushButton *desktopLrcBt;
	QPushButton *hotkeyButton;
	QPushButton *submitBt;
	QPushButton *cancelBt;
	QPushButton *applyBt;
	QPushButton *closeBt;
	QColor *currentColor;
	bool isPaletteShown;
signals:
	void resetGeneral(generalSet &);
signals:
	void resetHotKey(hotKeys &);
signals:
	void resetWindowLrc(windowLrc &);
signals:
	void resetDeskLrc(deskLrc &);
signals:
	void resetNetwork(networkSet &);
protected:
	void routeChange();
	void applyChange();
	void resetWindowView();
	void resetDeskView();
	void selectColor();
protected:
	void hotKeyTextChange(const QString &);
	void readHotKey();
	void readGeneral();
	void readWindowLrc();
	void readDeskLrc();
	void readNetwork();
	void saveGeneral();
	void saveHotKey();
	void saveWindowLrc();
	void saveDeskLrc();
	void saveNetwork();
protected:
	virtual bool eventFilter(QObject *, QEvent *)override;
	virtual void paintEvent(QPaintEvent *)override;
	void createSubCom();
	void setObjectName();
	void setGeo();
	void readConfig();
	void connectSignal();
	void resetCurrentWidget();
protected:
	QLabel *proxyLa;
	QLabel *proxyTypeLa;
	QLabel *addrLa;
	QLabel *portLa;
	QLabel *userLa;
	QLabel *passwordLa;
	QLineEdit *addrEdit;
	QLineEdit *portEdit;
	QLineEdit *userEdit;
	QLineEdit *passwordEdit;
	QComboBox *typeBox;
	QFrame *proxyLine;
	QLabel *searchLa;
	QFrame *searchLine;
	QLabel *resultNum;
	QSpinBox *maxNum;
protected:
	QLabel *destyleSetLa;
	QLabel *defontLa;
	QLabel *defontSizeLa;
	QLabel *defontStyleLa;
	QLabel *decolorLa;
	QLabel *deviewLa;
	QLabel *deviewPic;
	QFrame *destyleFrame;
	QFrame *deviewFrame;
	QComboBox *defontBox;
	QComboBox *defontSizeBox;
	QComboBox *defontStyleBox;
	QPushButton *deunplayBt;
	QPushButton *deplayedBt;
	QPushButton *dedefaultBt;
protected:
	QLabel *styleSetLa;
	QLabel *fontLa;
	QLabel *fontSizeLa;
	QLabel *fontStyleLa;
	QLabel *colorLa;
	QLabel *viewLa;
	QLabel *viewPic;
	QFrame *styleFrame;
	QFrame *viewFrame;
	QComboBox *fontBox;
	QComboBox *fontSizeBox;
	QComboBox *fontStyleBox;
	QPushButton *unplayBt;
	QPushButton *playedBt;
	QPushButton *defaultBt;
protected:
	QLabel *playCtrlLa;
	QLabel *playPauseLa;
	QLabel *prevLa;
	QLabel *nextLa;
	QLabel *loudLa;
	QLabel *quietLa;
	QLabel *muteLa;
	QLabel *otherLa;
	QLabel *screenShotLa;
	QLineEdit *playPauseEd;
	QLineEdit *prevEd;
	QLineEdit *nextEd;
	QLineEdit *loudEd;
	QLineEdit *quietEd;
	QLineEdit *muteEd;
	QLineEdit *screenShotEd;
	QFrame *otherLine;
	QFrame *playCtrlLine;
	QCheckBox *globalHotBt;
	QPushButton *resetBt;
protected:	
	QLabel *onStartLa;
	QFrame *onStartLine;
	QCheckBox *autoPlayBt;
	QCheckBox *playHello;
	QLabel *onCloseLa;
	QFrame *onCloseLine;
	QRadioButton *keepIcon;
	QRadioButton *noKeepIcon;
	QLabel *fileAssociateLa;
	QFrame *fileBt;
	QCheckBox *defaultPlayer;
	QCheckBox *autoSave;
	QLabel *lrcRouteLa;
	QLabel *downRouteLa;
	QLabel *screenRouteLa;
	QLineEdit *lrcEdit;
	QLineEdit *downEdit;
	QLineEdit *screenShotEdit;
	QPushButton *lrcButton;
	QPushButton *downButton;
	QPushButton *skinButton;
	QFrame *routeLine;
	QLabel *routeLa;
};

#endif // SETWIDGET_H
