//#include "Kinalyzer\ImageCache.h"
#include "StereoViewLayer.h"
//#include "VarTrace\vartracestore.h"

using namespace StereoView;

static void paintRect(double x1, double y1, double x2, double y2)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(x1, y1);
	glTexCoord2f(0, 1);
	glVertex2f(x1, y2);
	glTexCoord2f(1, 1);
	glVertex2f(x2, y2);
	glTexCoord2f(1, 0);
	glVertex2f(x2, y1);	
	glEnd();
}

float Layer::xToLocal(float x, float from, float to)
{
	QSizeF s = getSize();
	return ((x-from)/(to-from)*screenSize.width() - screenFrame.left())*s.width()/screenFrame.width();
}

//float Layer::xToLocal(Kin::Hand *h, float from, float to)
//{
//	return xToLocal(h->x(), -1, 1);
//}

float Layer::yToLocal(float y, float from, float to)
{
	QSizeF s = getSize();
	return ((y-from)/(to-from)*screenSize.height() - screenFrame.top())*s.height()/screenFrame.height();
}

//float Layer::yToLocal(Kin::Hand *h, float from, float to)
//{
//	return yToLocal(h->y(), 1, -1);
//}

// Test Layer

TestLayer::TestLayer(QObject *parent) 
	: Layer(parent)
{
	setup.fit = true;
	setup.flat = true;
	setup.clip = true;
	setup.osd = true;
}

void TestLayer::paintGL(int eye)
{	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor4f(
		eye > 0 ? 0.5 : 0, 
		eye <= 0 ? 0.5 : 0, 
		eye > 0 ? 0.5 : 0, 
		1);
	glVertex2f(0, 0);
	glVertex2f(0, 1);
	glVertex2f(1, 1);
	glVertex2f(1, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor4f(1, 1, 1, 1);
	for (int i = -1; i <= 11; ++i) {
		for (int j = -1; j <= 11; ++j) {
			if (i < 0 || j < 0 || i >= 10 || j >= 10) glColor4f(1, 1, 0, 1); else glColor4f(1, 1, 1, 1);
			if (i+1 <= 10) {
				glVertex2f(i/10., j/10.);
				glVertex2f((i+1)/10., j/10.);
			}
			if (j+1 <= 10) {
				glVertex2f(i/10., j/10.);
				glVertex2f(i/10., (j+1)/10.);
			}
		}
	}	
	glEnd();
}

// TestLayer 3D

TestLayer3D::TestLayer3D(QObject *parent)
	: Layer(parent)
{
	setup.fit = true;
	setup.flat = false;
	setup.clip = true;
	setup.osd = false;
	setup.zNear = 0.1;
	setup.zZero = 2;
	setup.zFar = 10;
}

void TestLayer3D::paintGL(int eye)
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor4f(1, 1, 1, 1);

	int mi = 6, mj = 6;
	for (int k = -3; k <= 3; ++k) {
		for (int i = -mi; i <= mi; ++i) {
			for (int j = -mj; j <= mj; ++j) {
				if (i + 1 <= mi) {
					glVertex3f((i)/(float)mi, (j)/(float)mj, (k)*0.5);
					glVertex3f((i+1)/(float)mi, (j)/(float)mj, (k)*0.5);
				}
				if (j + 1 <= mj) {
					glVertex3f((i)/(float)mi, (j)/(float)mj, (k)*0.1);
					glVertex3f((i)/(float)mi, (j+1)/(float)mj, (k)*0.1);
				}
				if (k + 1 <= 3) {
					glVertex3f((i)/(float)mi, (j)/(float)mj, (k)*0.1);
					glVertex3f((i)/(float)mi, (j)/(float)mj, (k+1)*0.1);
				}
			}
		}
	}
	glEnd();
}

QSizeF TestLayer3D::getSize()
{
	return QSizeF(1, 1);
}

// Buffered view layer

BufferedViewLayer::BufferedViewLayer(QObject *parent) 
	: Layer(parent)
{
	frameBuf[0] = new char[32*32*3];
	frameBuf[1] = new char[32*32*3];
	frameBufW = frameBufH = 32;
	texView[0] = texView[1] = 0;
	doFrameRefresh = 0;
	switchSides = false;

	setup.clip = true;
	setup.fit = true;
	setup.flat = true;
	setup.osd = true;
	setup.zZero = 0;
}

BufferedViewLayer::~BufferedViewLayer()
{
	delete [] frameBuf[0];
	delete [] frameBuf[1];
}

void BufferedViewLayer::initGL()
{
	glGenTextures(2, texView);
	for (int i = 0; i < 2; ++i) {
		glBindTexture(GL_TEXTURE_2D, texView[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
}

void BufferedViewLayer::paintGL(int eye)
{
	eye = eye <= 0 ? 0 : 1;
	if (switchSides) {
		eye = 1 - eye;
	}	
	if (doFrameRefresh) {
		for (int i = 0; i < doFrameRefresh; ++i) {
			glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		 
			glBindTexture(GL_TEXTURE_2D, texView[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufW, frameBufH, 0, GL_RGB, GL_UNSIGNED_BYTE, frameBuf[i]);		 
			glPopClientAttrib();	
		}
		doFrameRefresh = 0;
	}
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texView[eye]);
	paintRect(0, frameBufH, frameBufW, 0);
}

// menu:

//KinMenuItem::KinMenuItem(QObject *parent)
//	: QObject(parent)
//{
//}
//
//KinMenuItem::KinMenuItem(QString label, QString texPath, float xp, float yp, int border, bool checkable, int group) 
//	: label(label), xp(xp), yp(yp), over(false), checked(false), border(border%4), checkable(checkable), checkGroup(group), clickBlock(false)
//{	
//	tex = ImageCache::loadGLTexture(texPath);	
//}
//
//float KinMenuItem::x(float visible) 
//{	
//	switch (border) {
//	case 0: return xp;
//	case 1: return 1 + xp - 2*visible*xp;
//	case 2: return xp;
//	case 3: return -1 + -xp + 2*visible*xp;
//	default: return 0.5;
//	}
//}
//
//float KinMenuItem::y(float visible) 
//{
//	switch (border) {
//	case 0: return -1 + -yp + 2*visible*yp;
//	case 1: return yp;
//	case 2: return 1 + yp - 2*visible*yp;
//	case 3: return yp;
//	default: return 0.5;
//	}
//}
//
//void KinMenuItem::click()
//{
//	emit triggered();
//}
//
//KinMenuLayer::KinMenuLayer(QObject *parent)
//	: Layer(parent), infoFont("Calibri", 90), noUserInfoFont("Calibri", 20), 
//	handStationary(false), toolUsing(false), view(NULL), zoomMode(false),
//	mainHand(1), otherHand(0)
//{
//	paintLabels = true;
//
//	setup.zZero = -0.4; // negative: in front of the screen;   positive: behind the screen
//	setup.flat = true;
//	setup.clip = false;
//	setup.fit = true;
//	setup.visible = true;
//	setup.osd = true;
//	userActive = false;
//}
//
//KinMenuLayer::~KinMenuLayer()
//{
//	foreach (KinMenuItem *item, items) {
//		delete item;
//	}
//}
//
//void KinMenuLayer::initGL()
//{
//	items.append(itCamera = new KinMenuItem("Camera mode", "images/ico-camera.png", -0.40, 0.2, 0, true, 0));
//	items.append(itActor = new KinMenuItem("Actor mode", "images/ico-actor.png", -0.125, 0.15, 0, true, 0));
//	items.append(itRotate = new KinMenuItem("Rotate", "images/ico-rotate.png", 0.125, 0.15, 0, true, 1));
//	items.append(itMove = new KinMenuItem("Move", "images/ico-move.png", 0.40, 0.2, 0, true, 1));
//
//	items.append(itReset = new KinMenuItem("Reset", "images/xbutton.png", -0.45, 0.26, 2, false, -1));
//	items.append(itPlay = new KinMenuItem("Hide", "images/ico-hide.png", -0.15, 0.23, 2, false, -1));
//	items.append(itStop = new KinMenuItem("Show", "images/ico-unhide.png", 0.15, 0.23, 2, false, -1));
//	items.append(itHold = new KinMenuItem("Hold", "images/hbutton.png", 0.45, 0.26, 2, false, -1));	
//
//	itCamera->checked = true;
//	itRotate->checked = true;
//	itCameraTool = itRotate;
//	itActorTool = itMove;
//
//	sndBip = new QSound("sounds/bip-short.wav");
//	sndClick = new QSound("sounds/click1.wav");	 
//	sndUnClick = new QSound("sounds/click3.wav");
//}
//
//void KinMenuLayer::paintGL(int eye)
//{	
//	QSizeF s = getSize();	
//
//	// draw menu buttons:
//	float visibleLevel = menuVisible.getLazyValue();
//	if (visibleLevel > 0.001) {
//		glDisable(GL_TEXTURE_2D);
//		glColor4f(0, 0, 0, visibleLevel*0.5);
//		paintRect(-s.width(), 0, 2*s.width(), s.height()); // wide for stereo mode
//
//		float itsize = 0.15;	
//		foreach (KinMenuItem *item, items) {
//			float x = (item->x(visibleLevel)*s.width() + s.width())/2;
//			float y = (item->y(visibleLevel)*s.height() + s.height())/2;
//
//			glEnable(GL_TEXTURE_2D);
//			glBindTexture(GL_TEXTURE_2D, item->tex);
//		
//			float c;
//			if (item->checkable) {
//				c = item->checked.getFuzzy(100, 300);
//			} else {
//				c = item->clicked.getFuzzy(300);
//			}
//			glColor4f(1 - c, 1, 1 - c, item->checked ? 1 : item->over.getFuzzy(700, 700, 1, 0.6));
//			paintRect(x - itsize/2, y - itsize/2, x + itsize/2, y + itsize/2);
//		}
//	}
//
//	// paint info text
//	/*if (zoomMode.dur() < 1000) {
//		if (zoomMode) {
//			infoText = "Zoom mode";
//		}
//		zoomMode.touch();
//	} 
//
//	if (paintLabels) {
//		glDisable(GL_TEXTURE_2D);
//		float a = (infoText.dur()-2000.0);*/
//		if (a < 0) {
//			a = 1;
//		} else {
//			a = 1 - a/900.0;
//		}
//		glPushMatrix();
//		glScalef(1./1280, 1./1280, 1);
//		if (infoText.dur() < 2900) {
//			glColor4f(0, 0, 0, a*0.6);
//			paintRect(0, 950, 0 + a*a*1920, 1150);
//			glColor4f(1, 1, 1, a*0.9);	
//			ctx->renderText(45 - 1280 + a*a*1280, 1100, 0, infoText, infoFont);
//		}
//		glColor4f(1, 1, 1, 0.8);
//		if (!userActive) {
//			ctx->renderText(20, 20, 0, QString("No user active"), noUserInfoFont);
//		}
//		glPopMatrix();
//	}
//
//	// draw cursor:
//	
//	for (int i = 0; i < 2; ++i) {
//		paintCursor(cursors + i, 0.09);
//	}
//}
//
//QSizeF KinMenuLayer::getSize()
//{
//	return QSizeF(width/(double)height, 1);	
//}
//
//bool KinMenuLayer::onUserUpdate(Kin::User *u, bool fake)
//{
//	if (!u) {
//		toolUsing = false;
//		menuVisible = false;
//		handStationary = false;
//		cursors[mainHand].visible = false;
//		cursors[otherHand].visible = false;
//		userActive = false;
//		return false;
//	}
//	userActive = !fake;
//
//	for (int i = 0; i < 2; ++i) {
//		cursors[i].x = xToLocal(u->hands + i);
//		cursors[i].y = yToLocal(u->hands + i);		
//		cursors[i].side = u->hands[i].side;
//		//cursors[i].visible = u->hands[i].valid;
//	}
//
//	if (!u->hands[mainHand].valid) {
//		return false;
//	}
//	//Kin::Hand *h = u->hands + mainHand;
//	//Kin::Hand *hOther = u->hands + otherHand;
//	float realx = h->x(), realy = h->y();
//
//	//setup.zZero = realy;
//	//qWarning()<<("Zzero: %f", realy);	
//
//	/*float visibleLevel = menuVisible.getLazyValue();		
//	
//	cursors[mainHand].visible = true;
//	cursors[otherHand].visible = false;*/
//
//	if (menuVisible || visibleLevel > 0.75) {
//		menuVisible = fabs(realy) > 0.5;		
//		handStationary = false;
//		toolUsing = false;
//		zoomMode = false;
//	} else {
//		if (!toolUsing) {
//			menuVisible = fabs(realy) > 0.75;
//			if (menuVisible) {
//				foreach (KinMenuItem *item, items) {
//					item->clickBlock = false;
//				}
//			}
//		}
//
//		if (!menuVisible && view != NULL) {
//			viewHandUpdate(h, hOther);
//		}
//	}
//
//	if (menuVisible && visibleLevel > 0.7) {
//		setCursorMode(CUR_GRAB);
//		cursors[otherHand].type = CUR_HIDDEN;
//		cursors[otherHand].visible = false;
//		menuHandUpdate(h, hOther);
//	} else {
//		if (toolUsing) {			
//			if (!zoomMode) {
//				cursors[otherHand].type = CUR_HIDDEN;
//				cursors[otherHand].visible = false;
//			}
//			if (itActor->checked) {
//				setCursorMode(CUR_HIDDEN);
//			} else {
//				if (zoomMode) {
//					cursors[otherHand].visible = true;
//				}
//				setCursorMode(CUR_GRABBED);
//			}
//		} else {			
//			if (itActor->checked) {
//				setCursorMode(CUR_PICK);
//			} else {	
//				cursors[otherHand].visible = fabs(hOther->y()) < 0.5 && u->hands[1].x() - u->hands[0].x() > 0.05;
//				setCursorMode(CUR_GRAB);
//			}			
//		}
//	}
//
//	return true;
//}
//
//bool KinMenuLayer::menuHandUpdate(Kin::Hand *h, Kin::Hand *hOther)
//{	
//	QSizeF s = getSize();
//	float locx = xToLocal(h);
//	float locy = yToLocal(h);
//	KinMenuItem *mitem = NULL;
//	float mdist = 1e10;
//	foreach (KinMenuItem *item, items) {		
//		float x = (item->x(1)*s.width() + s.width())/2;
//		float y = (item->y(1)*s.height() + s.height())/2;
//		float dx = x - locx;
//		float dy = y - locy;
//		float dist = dx*dx + dy*dy;
//		if (dist < mdist) {
//			mdist = dist;
//			mitem = item;
//		}
//	}
//	if (mdist > 0.1*0.1) {
//		mitem = NULL;
//	}
//
//	foreach (KinMenuItem *item, items) {
//		item->over = item == mitem;
//		if (!item->over) {
//			item->clickBlock = false;
//		}
//	}
//	foreach (KinMenuItem *item, items) {
//		if (item->over && item->over.dur() > 1000) {
//			selectItem(item);
//		}
//	}
//	return true;
//}
//
//bool KinMenuLayer::viewHandUpdate(Kin::Hand *h, Kin::Hand *hOther)
//{
//	handStationary = h->var(1000) < 0.001;
//	handOtherStationary = hOther->var(1000) < 0.002;
//	int viewX = view->xToLocal(h->x());
//	int viewY = view->yToLocal(-h->y());
//	Qt::MouseButton but = itMove->checked ? Qt::MidButton : Qt::LeftButton;	
//
//	if (handStationary && handStationary.dur() > 700) {
//		if (toolUsing.dur() > 2000) {
//			if (toolUsing) {
//				toolUsing = false;
//				sndUnClick->play();
//			} else if (h->y() > -0.85) {
//				toolUsing = true;
//			}
//					
//			if (toolUsing) {
//				if (itActor->checked) {
//					pickedActor = false;
//					emit mainHandPress(viewX, viewY, Qt::LeftButton, true);
//					if (pickedActor) {
//						sndClick->play();
//						emit mainHandPress(viewX, viewY, but, false);
//					} else {						
//						toolUsing = false;
//					}
//				} else {
//					if (cursors[otherHand].visible.getLazyValue() > 0.9 && handOtherStationary && handOtherStationary.dur() > 500) {
//						zoomMode = true;						
//					}
//					sndClick->play();
//					if (zoomMode) {
//						emit mainHandPress(view->frameBufW/2, view->frameBufH - 1 - h->dist(hOther)*view->frameBufH/2.1f, Qt::RightButton, false);
//					} else {
//						emit mainHandPress(viewX, viewY, but, false);
//					}
//				}
//			} else {				
//				if (zoomMode) {
//					emit mainHandRelease(view->frameBufW/2, view->frameBufH - 1 - h->dist(hOther)*view->frameBufH/2.1f, Qt::RightButton);
//				} else {
//					emit mainHandRelease(viewX, viewY, but);
//				}
//				zoomMode = false;
//			}					
//		}
//	}			
//	if (toolUsing) { 
//		if (zoomMode) {
//			emit mainHandMoved(view->frameBufW/2, view->frameBufH - 1 - h->dist(hOther)*view->frameBufH/2.1f, Qt::RightButton);
//		} else {
//			emit mainHandMoved(viewX, viewY, but);
//		}
//	} else if (h->y() > -0.7) { // ??
//		emit mainHandMoved(viewX, viewY, Qt::NoButton);
//	}
//	return true;
//}
//
//void KinMenuLayer::paintCursor(HandCursor *cur, float size)
//{
//	QString texName;
//
//	float vis = cur->visible.getLazyValue();
//	if (vis <= 0.001) return;
//
//	float hookx;
//	float hooky;
//	float hw = 1; //width
//	float hh = 1; //height
//
//	switch (cur->type) { 	
//	case CUR_GRAB:
//		hookx = hooky = 0.5;
//		texName = "images/cursor-real.png";
//		break;
//	case CUR_GRABBED:
//		hookx = hooky = 0.5;
//		texName = "images/cursor-real-grab.png";
//		break;
//	case CUR_PICK:
//		hookx = 42/256.0;
//		hooky = 24/256.0;
//		texName = "images/cursor-point-transtip.png";
//		break;
//	case CUR_HIDDEN:
//	default:
//		break;
//	}
//	
//	if (cur->side < 0) {
//		hookx = hw - hookx;
//	}
//
//	if (texName.isEmpty()) return;
//
//	hookx *= size;
//	hooky *= size;
//	hw *= size;
//	hh *= size;
//
//	float xx = cur->x;
//	float yy = cur->y;
//	GLuint tex = ImageCache::loadGLTexture(texName);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	
//	glBegin(GL_QUADS);
//	glColor4f(1, 1, 1, vis*(menuVisible.getLazyValue()*0.35 + 0.65));
//		glTexCoord2f(cur->side > 0 ? 0 : 1, 1);
//		glVertex2f(xx - hookx, yy - hooky + hh);
//		glTexCoord2f(cur->side > 0 ? 0 : 1, 0);
//		glVertex2f(xx - hookx, yy - hooky);
//		glTexCoord2f(cur->side > 0 ? 1 : 0, 0);
//		glVertex2f(xx - hookx + hw, yy - hooky);
//		glTexCoord2f(cur->side > 0 ? 1 : 0, 1);
//		glVertex2f(xx - hookx + hw, yy - hooky + hh);		
//	glEnd();
//}
//
//void KinMenuLayer::selectItem(KinMenuItem *item)
//{
//
//	if (!item->checkable) {
//		if (!item->clickBlock) {
//			item->clickBlock = true;
//			item->clicked.touch();
//			sndBip->play();
//			item->click();
//		}		
//		return;
//	}
//	if (item->checked) {
//		return;
//	}
//	item->checked = true;
//	sndBip->play();
//	//infoText = item->label;
//	foreach (KinMenuItem *item2, items) {
//		if (item2 == item) continue;
//		if (item2->checkable && item2->checkGroup == item->checkGroup) {
//			item2->checked = false;
//		}
//	}
//	if (item == itCamera) {
//		emit interactWithCamera();
//		selectItem(itCameraTool);
//	} else if (item == itActor) {
//		emit interactWithCurrentActor();
//		selectItem(itActorTool);
//	} else if (item == itMove || item == itRotate) {
//		if (itCamera->checked) {
//			itCameraTool = item;
//		} else if (itActor->checked) {
//			itActorTool = item;
//		}
//	}
//}
//
//void KinMenuLayer::leadingUserChanged(Kin::User *u)
//{	
//	if (u) {
//		infoText = "User selected";
//	} else {
//		infoText = "Inactivated";
//	}
//}
