#ifndef MyQtObjects_H
#define MyQtObjects_H
	
#include "qslider.h"
#include "qwidget.h"
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qobject.h"
#include "qspinbox.h"
#include "qlineedit.h"
#include "qsettings.h"
#include "qcombobox.h"

const char* const CONFIG_NAME = "config.ini";

class AutoSaveObject
{

protected:
	QString groupName, fieldName;
	QString groupName2, customFieldName;
	int nr;
	QString configIniName;
	int flag;
	int flags[10];
public:
	void setFlag(int nr, int value) { flags[nr] = value; };
	int getFlag(int nr) { return flags[nr]; };
	int GetNumber() { return nr; };
	AutoSaveObject() { for (int i = 0;i < 10;i++) { flags[i] = 0; };customFieldName = ""; };
	virtual void AutoSaveOn() = 0;
	virtual void AutoSaveOff() = 0;
	virtual void LoadSavedSettings() {};
};

class QPushButton2 :public QPushButton, public AutoSaveObject
{
	Q_OBJECT

private:
	QList<QPushButton2*>* buttonList;


	//int buttonNumber;
	void init(QList<QPushButton2*>* _list)
	{
		setObjectName("QPushButton2");
		groupName = "Data-Init";
		
		fieldName = "LastButtonClicked";
		buttonList = _list;
		//buttonNumber = buttonCount++;
		configIniName = CONFIG_NAME;
		nr = buttonList->size();
		buttonList->append(this);
		this->setToolTip("ButtnoNr: " + QString::number(nr));
	};
public:

	void AutoSaveOn()
	{
		connect(this, SIGNAL(clicked()), this, SLOT(SlotSetCurrentButtonAsLastClicked()));
	}
	void AutoSaveOff()
	{
		disconnect(this, SIGNAL(clicked()), this, SLOT(SlotSetCurrentButtonAsLastClicked()));
	}


	void emitClicked()
	{
		emit clicked();
	};

	QPushButton2(QList<QPushButton2*>* _list, QWidget* parent = nullptr) : QPushButton(parent)
	{
		init(_list);

	};

	QPushButton2(QList<QPushButton2*>* _list, const QString& text, QWidget* parent = nullptr) : QPushButton(text, parent)
	{
		init(_list);
	};

	QPushButton2(QList<QPushButton2*>* _list, const QIcon& icon, const QString& text, QWidget* parent = nullptr) : QPushButton(icon, text, parent)
	{
		init(_list);
	};

	//static const QPushButton2 *lastClickedButton;
signals:

	void signalLastButtonClickedTextChanged(QString txt);
public slots:

	void SlotSetCurrentButtonAsLastClicked()
	{
		//lastClickedButton = this;
		//currentButton = buttonNumber;

		QSettings sett(configIniName, QSettings::IniFormat);
		sett.beginGroup(groupName);
		sett.setValue(fieldName, nr);
		sett.setValue("LastButtonClickedText", this->text());
		emit signalLastButtonClickedTextChanged(this->text());
		sett.endGroup();
	}

};




//--------------------------------------


class QCheckBox2 :public QCheckBox, public AutoSaveObject
{
	Q_OBJECT

private:
	QList<QCheckBox2*>* list;

	
	void init(QList<QCheckBox2*>* _list,QString _customFieldName="")
	{
	
		groupName = "checkBoxes";
		groupName2 = groupName+"Custom";
		customFieldName = _customFieldName;
		fieldName = "box_";

		list = _list;

		//buttonNumber = buttonCount++;
		configIniName = CONFIG_NAME;
		nr = 0;// list->size();
		if (list->size()>0)
		{
			if (_list->last()->customFieldName.isEmpty())
				nr = _list->last()->nr + 1;
			else
				nr = _list->last()->nr;//nie zwiêkszam indeksu - bêdzie przezroczyste - nie zmieni numeracji dotychczas wprowadzonych!
		}

		list->append(this);
	};

public:

	

	void AutoSaveOn()
	{
		connect(this, SIGNAL(stateChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}
	void AutoSaveOff()
	{
		disconnect(this, SIGNAL(stateChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}


	QCheckBox2(QList<QCheckBox2*>* _list, QWidget* parent = nullptr, QString _customFieldName = "") : QCheckBox(parent)
	{
		init(_list, _customFieldName);

	};


	QCheckBox2(QList<QCheckBox2*>* _list, const QString& text, QWidget* parent, QString _customFieldName = "") :QCheckBox(text, parent)
	{
		init(_list, _customFieldName);
	}

	void LoadSavedSettings()
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);		sett.beginGroup(finalGroupName);	
		if (sett.contains(name))
		{
			int val = sett.value(name).toInt();
			this->setChecked(val);
		}
		sett.endGroup();
	}
public slots:

	void SlotSaveSettings(int state)
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);
		sett.beginGroup(finalGroupName);
		sett.setValue(name, state);
		sett.endGroup();
	}


};


//--------------------------------------


class QComboBox2 :public QComboBox, public AutoSaveObject
{
	Q_OBJECT

private:
	QList<QComboBox2*>* list;


	void init(QList<QComboBox2*>* _list, QString _customFieldName = "")
	{
		groupName = "comboBoxes";
		groupName2 = groupName + "Custom";
		fieldName = "comboBox_";
		customFieldName = _customFieldName;
		list = _list;


		configIniName = CONFIG_NAME;
		nr = 0;// list->size();
		if (list->size() > 0)
		{
			if (_list->last()->customFieldName.isEmpty())
				nr = _list->last()->nr + 1;
			else
				nr = _list->last()->nr;//nie zwiêkszam indeksu - bêdzie przezroczyste - nie zmieni numeracji dotychczas wprowadzonych!
		}
		list->append(this);
	};

public:

	

	void AutoSaveOn()
	{
		connect(this, SIGNAL(activated(int)), this, SLOT(SlotSaveSettings(int)));
		connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}
	void AutoSaveOff()
	{
		disconnect(this, SIGNAL(activated(int)), this, SLOT(SlotSaveSettings(int)));
		disconnect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}

	QComboBox2(QList<QComboBox2*>* _list, QWidget* parent = nullptr, QString _customFieldName = "") : QComboBox(parent)
	{
		init(_list, _customFieldName);

	};

	void LoadSavedSettings()
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;
		QSettings sett(configIniName, QSettings::IniFormat);		sett.beginGroup(finalGroupName);	//	QString name = fieldName + QString::number(nr);
		if (sett.contains(name))
		{
			int val = sett.value(name).toInt();
			this->setCurrentIndex(val);
		}
		sett.endGroup();
	}

public slots:

	void SlotSaveSettings(int state)
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;
		QSettings sett(configIniName, QSettings::IniFormat);
		sett.beginGroup(finalGroupName);
		sett.setValue(name, state);
		sett.endGroup();
	}


};



//--------------------------------------
//--------------------------------------


class QSlider2 :public QSlider, public AutoSaveObject
{
	Q_OBJECT

private:
	QList<QSlider2*>* list;

	void init(QList<QSlider2*>* _list, QString _customFieldName = "")
	{
		setObjectName ("QSlider2");
		groupName = "sliders";
		groupName2 = groupName + "Custom";
		fieldName = "slider_";
		customFieldName = _customFieldName;
		list = _list;		configIniName = CONFIG_NAME;		
		
		nr = 0;// list->size();
		if (list->size() > 0)
		{
			if (_list->last()->customFieldName.isEmpty())
				nr = _list->last()->nr + 1;
			else
				nr = _list->last()->nr;//nie zwiêkszam indeksu - bêdzie przezroczyste - nie zmieni numeracji dotychczas wprowadzonych!
		}
		
		list->append(this);
	};


public:

	

	void AutoSaveOn()
	{
		connect(this, SIGNAL(valueChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}
	void AutoSaveOff()
	{
		disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}

	QSlider2(QList<QSlider2*>* _list, QWidget* parent = nullptr, QString _customFieldName = "") : QSlider(parent)
	{
		init(_list, customFieldName);

	};

	QSlider2(QList<QSlider2*>* _list, Qt::Orientation orientation, QWidget* parent = nullptr, QString _customFieldName = "") :QSlider(orientation, parent)
	{
		init(_list, customFieldName);
	}
	void LoadSavedSettings()
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);		sett.beginGroup(finalGroupName);	//	QString name = fieldName + QString::number(nr);
		if (sett.contains(name))
		{
			int val = sett.value(name).toInt();
			this->setValue(val);
		}
		sett.endGroup();
	}
public slots:

	void SlotSaveSettings(int state)
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);
		sett.beginGroup(finalGroupName);
		sett.setValue(name, state);
		sett.endGroup();
	}


};
//-----------------------------
//--------------------------------------


class QSpinBox2 :public QSpinBox, public AutoSaveObject
{
	Q_OBJECT

private:
	QList<QSpinBox2*>* list;

	void init(QList<QSpinBox2*>* _list, QString _customFieldName = "")
	{
		groupName = "spinBoxes";
		groupName2 = groupName + "Custom";
		fieldName = "spinBox_";
		customFieldName = _customFieldName;
		list = _list;		configIniName = CONFIG_NAME;	
		
		nr = 0;// list->size();
		if (list->size() > 0)
		{
			if (_list->last()->customFieldName.isEmpty())
				nr = _list->last()->nr + 1;
			else
				nr = _list->last()->nr;//nie zwiêkszam indeksu - bêdzie przezroczyste - nie zmieni numeracji dotychczas wprowadzonych!
		}
		
		list->append(this);
	};


public:

	
	void AutoSaveOn()
	{
		connect(this, SIGNAL(valueChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}
	void AutoSaveOff()
	{
		disconnect(this, SIGNAL(valueChanged(int)), this, SLOT(SlotSaveSettings(int)));
	}

	QSpinBox2(QList<QSpinBox2*>* _list, QWidget* parent = nullptr, QString _customFieldName = "") : QSpinBox(parent)
	{
		init(_list, _customFieldName);

	};

	void LoadSavedSettings()
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);		sett.beginGroup(finalGroupName);		//QString name = fieldName + QString::number(nr);
		if (sett.contains(name))
		{
			int val = sett.value(name).toInt();
			this->setValue(val);
		}
		sett.endGroup();
	}

public slots:

	void SlotSaveSettings(int state)
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;
		QSettings sett(configIniName, QSettings::IniFormat);
		sett.beginGroup(finalGroupName);
		sett.setValue(name, state);
		sett.endGroup();
	}


};
//----------------------------

class QLineEdit2 :public QLineEdit, public  AutoSaveObject
{
	Q_OBJECT

private:
	QList<QLineEdit2*>* list;

	void init(QList<QLineEdit2*>* _list, QString _customFieldName = "")
	{
		groupName = "lineEdits";
		groupName2 = groupName + "Custom";
		fieldName = "lineEdit_";
		customFieldName = _customFieldName;
		list = _list;


		configIniName = CONFIG_NAME;
		nr = 0;// list->size();
		if (list->size() > 0)
		{
			if (_list->last()->customFieldName.isEmpty())
				nr = _list->last()->nr + 1;
			else
				nr = _list->last()->nr;//nie zwiêkszam indeksu - bêdzie przezroczyste - nie zmieni numeracji dotychczas wprowadzonych!
		}
		list->append(this);
	};

public:

	

	void AutoSaveOn()
	{
		connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(SlotSaveSettings(const QString&)));
	}
	void AutoSaveOff()
	{
		disconnect(this, SIGNAL(textChanged(const QString&)), this, SLOT(SlotSaveSettings(const QString&)));
	}

	QLineEdit2(QList<QLineEdit2*>* _list, QWidget* parent = nullptr, QString _customFieldName = "") : QLineEdit(parent)
	{
		init(_list, _customFieldName);

	};

	QLineEdit2(QList<QLineEdit2*>* _list, const QString& text, QWidget* parent, QString _customFieldName = "") :QLineEdit(text, parent)
	{
		init(_list, _customFieldName);
	}

	void LoadSavedSettings()
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);		sett.beginGroup(finalGroupName);		//QString name = fieldName + QString::number(nr);
		if (sett.contains(name))
		{
			QString val = sett.value(name).toString();
			this->setText(val);
		}
		sett.endGroup();
	}
public slots:

	void SlotSaveSettings(const QString& state)
	{
		QString finalGroupName = (customFieldName.isEmpty()) ? groupName : groupName2;
		QString name = (customFieldName.isEmpty()) ? fieldName + QString::number(nr) : customFieldName;

		QSettings sett(configIniName, QSettings::IniFormat);
		sett.beginGroup(finalGroupName);
		sett.setValue(name, state);
		sett.endGroup();
	}


};





#endif //MyQtObjects_H
//////////////////////////////////////////////////////
