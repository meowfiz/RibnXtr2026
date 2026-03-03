

#ifndef MYDOCKWIDGET_H
#define MYDOCKWIDGET_H

#include	<qscrollarea.h>
#include <QDockWidget>
 //#include <QMainWindow>
//#include <qmotifstyle.h>
#include	<qsplitter.h>
#include	<qmessagebox.h>
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(QMenu)


class MyDockWidget : public QDockWidget
{
    Q_OBJECT

    QAction *closableAction;
    QAction *movableAction;
    QAction *floatableAction;
    QAction *floatingAction;
    QAction *verticalTitleBarAction;

    QActionGroup *allowedAreasActions;
    QAction *allowLeftAction;
    QAction *allowRightAction;
   /* QAction *allowTopAction;
    QAction *allowBottomAction;
*/
    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
 /*   QAction *topAction;
    QAction *bottomAction;*/

    QAction *changeSizeHintsAction;

    QMenu *tabMenu;
    QMenu *splitHMenu;
    QMenu *splitVMenu;

    QAction *windowModifiedAction;
	QString wName;
	QWidget *origTitleBarWidget;
	QWidget *tmpTitleBarWidget;

	QScrollArea *scrollArea;
	QWidget *tmpWidget;
	int ww,hh;
	
	
public:
	MyDockWidget(const QString &name, const QString &_title, bool placeOnLeft = false, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	
	void	setScrollWidget (QWidget *w);

    QMenu *menu;
    void setCustomSizeHint(const QSize &size);
	void place(Qt::DockWidgetArea area, bool place);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void resizeEvent(QResizeEvent *e);
		virtual void mousePressEvent(QMouseEvent* event);
	 void hideEvent  ( QCloseEvent * );
	 void closeEvent  ( QCloseEvent * );

private:
    void allow(Qt::DockWidgetArea area, bool allow);


private slots:
	
    void changeClosable(bool on);
    void changeMovable(bool on);
    void changeFloatable(bool on);
    void changeFloating(bool on);
    void changeVerticalTitleBar(bool on);
    void updateContextMenu();

    void allowLeft(bool a);
    void allowRight(bool a);
    void allowTop(bool a);
    void allowBottom(bool a);

    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);

    void splitInto(QAction *action);
    void tabInto(QAction *action);
		
	public slots:

		void SlotSetTitleBarVisibility(bool on);
			void	showHideContent();
			bool	isContentVisible(){return scrollArea->widget()->isVisible();};
			void	setContentVisible(bool on);
			void	slotDockLocationChanged(Qt::DockWidgetArea);
signals:
		void	closeButtonClicked(QString name);
		void	dockLocationChanged(Qt::DockWidgetArea,QString name);
};


class MyTitleBar : public QWidget
{
    Q_OBJECT
public:
    MyTitleBar(QWidget *parent = 0);

    QSize sizeHint() const { return minimumSizeHint(); }
    QSize minimumSizeHint() const;
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
	void	resizeEvent(QResizeEvent *e);
public slots:
    void updateMask();
   
   signals:
		void	showHideContent();
    

private:
    QPixmap leftPm, centerPm, rightPm;
};


//--------------------------
class MyViewWidget: public QWidget
{
	Q_OBJECT
	public:
		MyViewWidget( QString _title, QWidget *parent = 0, Qt::WindowFlags flags=0);
		
		void setSubWidget(int nr,QWidget *w);
		QWidget *getSubWidget(int nr);

		


	private:
		
	QSplitter *MainHorizontalSplitter;
	QSplitter *MainVerticalSplitter;
	QSplitter *VerticalSplitter1,*VerticalSplitter2;
	QSplitter *HorizontalSplitter1,*HorizontalSplitter2;
	QSplitter *HistogramVerticalSplitter;

	
		QWidget *myWidget[5];
	int currentLayout;

		void ShowAndResetFixedSizeOfDockWidgets(char *tab);
		void FreeAllWidgets();
		

		public slots:

		void SlotSetLayout(int nr);
	
	signals:
		
		void SignalBlockViewWidgets(bool a);

		


		


};


#endif // MYDOCKWIDGET_H
