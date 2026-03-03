//#include "StereoViewWidget.h"
//#include <gl/GLU.h>
////#include "VarTrace/vartracestore.h"
////#include "Kinalyzer/ImageCache.h"
//#include <QRectF>
//
//using namespace StereoView;
//
//static void myFrustum(double left, double right, double bottom, double top, double zero, double znear, double zfar)
//{
//	double a = znear / zero;
//	glFrustum(a*left, a*right, a*bottom, a*top, znear, zfar);
//}
//
//StereoViewWidget::StereoViewWidget(QWidget *parent, QGLWidget * shareWidget) :
//QGLWidget(QGLFormat(QGL::SampleBuffers), parent, shareWidget, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
//fsInfoFont("Calibri", 90), useFakeUser(false)
//{
//	img3D = NULL;
//	memset(painterParams, 0, 500 * sizeof(double));
//	animMode = false;
//	forceHalfMode = false;
//	stereoMode = 1;//side-by-side
//
//	//img=new QImage(this->width(),this->height(),QImage::Format_RGB32);
//	prevWidth = 0;
//	prevHeight = 0;
//	//imgTmp=new QImage(1,1,QImage::Format_RGB32);
//	paintWithMousePointer = false;
//
//	headPos[0] = 0;
//	headPos[1] = 0;
//	headPos[2] = 3000; // approx. 2x widths of the avg. screen
//	eyesShift[0] = -50; // left eye
//	eyesShift[1] = 50; // right eye
//
//	posLeft[0] = 0;
//	posRight[0] = 0;
//	posLeft[1] = 0;
//	posRight[1] = 0;
//	scaleFactorX = 0;
//	scaleFactorY = 0;
//	stereoViewLeft = 0;
//	stereoViewRight = 1;
//
//	repaintTimer.setInterval(30);
//	repaintTimer.setSingleShot(false);
//	connect(&repaintTimer, SIGNAL(timeout()), this, SLOT(repaintTimeout()));
//	kinSet = new Kin::DevicesSet();
//	kinProducer = new Kin::ProducerNetworkUDP(kinSet, 0x8787);
//	kinInter = new Kin::Inter3();
//	connect(kinInter, SIGNAL(leadingUserChanged(Kin::User *)), this, SLOT(leadingUserChanged(Kin::User *)));
//
//	fakeUser.state = Kin::State_Leading;
//	fakeUser.id = 1;
//
//	for (int i = 0; i < 4; ++i) {
//		UpperMenuItem *it = new UpperMenuItem(i * 200, 300, 150, 150);
//		if (i == 0 || i == 3) it->selected = true;
//		upMenu.append(it);
//	}
//
//	menuCameraLastTool = 3;
//	menuActorLastTool = 2;
//
//	plotsView = new VarTrace::View(this);
//	plotsView->hide();
//
//	sndBip = new QSound("sounds/bip-short.wav");
//	sndClick = new QSound("sounds/click1.wav");
//	sndUnClick = new QSound("sounds/click3.wav");
//
//	//plotsView = new VarTrace::View(this);
//	//plotsView->show();
//
//	frameIsGood = true;
//	facingCamera = true;
//
//	//appendLayer(new TestLayer());
//	//appendLayer(new TestLayer3D());
//	menuLayer = new KinMenuLayer();
//	appendLayer(menuLayer);
//	connect(menuLayer, SIGNAL(mainHandPress(int, int, Qt::MouseButton, bool)), this, SIGNAL(mainHandPress(int, int, Qt::MouseButton, bool)));
//	connect(menuLayer, SIGNAL(mainHandRelease(int, int, Qt::MouseButton)), this, SIGNAL(mainHandRelease(int, int, Qt::MouseButton)));
//	connect(menuLayer, SIGNAL(mainHandMoved(int, int, Qt::MouseButton)), this, SIGNAL(mainHandMoved(int, int, Qt::MouseButton)));
//	connect(menuLayer, SIGNAL(interactWithCurrentActor()), this, SIGNAL(interactWithCurrentActor()));
//	connect(menuLayer, SIGNAL(interactWithCamera()), this, SIGNAL(interactWithCamera()));
//	connect(kinInter, SIGNAL(leadingUserChanged(Kin::User *)), menuLayer, SLOT(leadingUserChanged(Kin::User *)));
//
//	setMouseTracking(true);
//}
////--------------------------------
//StereoViewWidget::~StereoViewWidget()
//{
//	//delete plotsView;	
//	deleteImg3D();
//}
//
//void StereoViewWidget::appendLayer(StereoView::Layer *layer)
//{
//	layersNew.append(layer);
//}
//
//void StereoViewWidget::prependLayer(StereoView::Layer *layer)
//{
//	layersPre.append(layer);
//}
//
//void StereoViewWidget::setBufferedViewLayer(StereoView::BufferedViewLayer *layer)
//{
//	menuLayer->view = layer;
//	prependLayer(layer);
//}
//
//void StereoViewWidget::initializeGL()
//{
//	glMatrixMode(GL_MODELVIEW);
//	//glEnable(GL_CULL_FACE);
//
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glShadeModel(GL_SMOOTH);
//	//glEnable(GL_DEPTH_TEST);
//
//	glClearColor(0, 0, 0, 1.f);
//
//	cursorTexId = ImageCache::loadGLTexture("images/cursor.png");
//	handCursorTex = ImageCache::loadGLTexture("images/cursor-real.png");
//	handCursorGrabTex = ImageCache::loadGLTexture("images/cursor-real-grab.png");
//
//	initNewLayers();
//
//	connect(menuLayer->itHold, SIGNAL(triggered()), kinInter, SLOT(leaveLeading()));
//	connect(menuLayer->itPlay, SIGNAL(triggered()), this, SIGNAL(generateAnimation()));
//	connect(menuLayer->itStop, SIGNAL(triggered()), this, SIGNAL(abortAnimation()));
//	connect(menuLayer->itReset, SIGNAL(triggered()), this, SIGNAL(resetAllActorsTransformations()));
//
//	//menu.show();
//	doMenuReload = true;
//
//	kinProducer->start();
//}
//
//void StereoViewWidget::initNewLayers()
//{
//	while (!layersNew.empty()) {
//		Layer *layer = layersNew.front();
//		layersNew.pop_front();
//		layer->ctx = this;
//		layer->initGL();
//		layers.push_back(layer);
//	}
//
//	while (!layersPre.empty()) {
//		Layer *layer = layersPre.back();
//		layersPre.pop_back();
//		layer->ctx = this;
//		layer->initGL();
//		layers.push_front(layer);
//	}
//}
//
//void StereoViewWidget::resizeGL(int w, int h)
//{
//	foreach(StereoView::Layer *layer, layers) {
//		layer->onResize(w, h);
//	}
//
//	/*for (int i = 0; i < upMenu.length(); ++i) {
//	float ww = 150;
//	upMenu.at(i)->w = upMenu.at(i)->h = ww;
//	upMenu.at(i)->x1 = upMenu.at(i)->x0 = w/2 + ww*4*(i/(upMenu.length() - 1.0) - 0.5);
//	}*/
//}
//
//void StereoViewWidget::paintGL()
//{
//	initNewLayers();
//
//	menuLayer->paintLabels = !animMode;
//
//	if (!kinSet->devices.isEmpty()) {
//		frameIsGood = kinInter->update(kinSet->devices.first(), kinProducer);
//	}
//
//	Kin::User *user = kinInter->getLeading();
//
//	if (!user) {
//		if (useFakeUser) {
//			fakeUser.hands[1].updatePos(2.*mousePos[0] / width() - 1, 1 - 2.*mousePos[1] / height());
//			fakeUser.hands[1].valid = true;
//			user = &fakeUser;
//		}
//	}
//	else {
//		//TRACE_VAR(user->getFacingCameraFactor());
//		if (user->state.dur() > 1000) {
//			if (!user->hands[0].valid && !user->hands[1].valid && user->hands[1].valid.dur() > 2000) {
//				kinInter->leaveLeading();
//			}
//			if (!frameIsGood && frameIsGood.dur() > 2000) {
//				kinInter->leaveLeading();
//			}
//		}
//	}
//
//	//if (user) {
//	for (int i = layers.size() - 1; i >= 0; --i) {
//		StereoView::Layer *layer = layers.at(i);
//		if (!layer->getSetup().visible) continue;
//		if (layer->onUserUpdate(user, user == &fakeUser)) break;
//	}
//	//}		
//
//
//	foreach(StereoView::Layer *layer, layers) {
//		if (!layer->getSetup().visible) continue;
//		layer->paintGLBegin();
//	}
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	foreach(StereoView::Layer *layer, layers) {
//		QSizeF size = layer->getSize();
//		LayerSetup setup = layer->getSetup();
//
//		if (!setup.visible) continue;
//
//		for (int eye = 0; eye < 2; ++eye) {
//			QPointF corner(0, 0);
//			float cam[3] = {
//				headPos[0] + eyesShift[eye],
//				headPos[1],
//				headPos[2]
//			};
//
//			float screenWidth = width();
//			float screenHeight = height();
//			float frameWidth, frameHeight;
//
//			//stereoMode == 0: nonstereo, left eye
//			//stereoMode == 1: half projection, squashed horizontaly												
//			//stereoMode == 3: dual projection, aspect ratio intact			
//
//			if (stereoMode == 3) screenWidth /= 2;
//			float scaleX = 1;
//			if (stereoMode == 1) {
//				scaleX = 0.5;
//			}
//
//			if (screenWidth*size.height() <= size.width()*screenHeight) {
//				frameWidth = screenWidth;
//				frameHeight = size.height()*screenWidth / size.width();
//				corner.setX(0);
//				corner.setY((screenHeight - frameHeight) / 2);
//			}
//			else {
//				frameWidth = size.width()*screenHeight / size.height();
//				frameHeight = screenHeight;
//				corner.setX((screenWidth - frameWidth) / 2);
//				corner.setY(0);
//			}
//
//			QRectF *frame, *sframe;
//
//			layer->screenSize.setWidth(screenWidth);
//			layer->screenSize.setHeight(screenHeight);
//			frame = layer->renderFrame + eye;
//			sframe = &layer->screenFrame;
//			if (setup.fit) {
//				frame->setLeft(corner.x() + eye*screenWidth);
//				frame->setTop(corner.y());
//				frame->setWidth(frameWidth*scaleX);
//				frame->setHeight(frameHeight);
//				sframe->setLeft(corner.x());
//				sframe->setTop(corner.y());
//				sframe->setWidth(frameWidth);
//				sframe->setHeight(frameHeight);
//			}
//			else {
//				frame->setLeft(eye*screenWidth);
//				frame->setTop(0);
//				frame->setWidth(size.width());
//				frame->setHeight(size.height());
//				sframe->setLeft(0);
//				sframe->setTop(0);
//				sframe->setWidth(size.width());
//				sframe->setHeight(size.height());
//			}
//
//			float stereoShift = setup.zZero*eyesShift[eye] * size.width() / screenWidth;
//
//			if (stereoMode == 0) {
//				stereoShift = 0;
//				// nonstereo, left eye only
//				if (eye == 1) break;
//			}
//
//			glMatrixMode(GL_PROJECTION);
//			glLoadIdentity();
//
//			if (setup.fit) {
//				if (setup.clip) {
//					if (box == NULL)
//					{
//
//						glViewport(
//							scaleX*(corner.x() + eye*screenWidth),
//							corner.y(),
//							scaleX*frameWidth,
//							frameHeight);
//					}
//					else
//					{
//						//	glViewport(box[0], box[1], box[2], box[3]);
//					}
//					if (setup.flat) {
//						gluOrtho2D(0, size.width(), size.height(), 0);
//						glMatrixMode(GL_MODELVIEW);
//						glLoadIdentity();
//						glTranslated(stereoShift, 0, 0);
//					}
//					else {
//						float xScale = size.width() / screenWidth;
//						float yScale = size.height() / screenHeight;
//						float zScale = xScale;
//
//						myFrustum(
//							-size.width() / 2 - cam[0] * xScale,
//							size.width() / 2 - cam[0] * xScale,
//							size.height() / 2 - cam[1] * yScale,
//							-size.height() / 2 - cam[1] * yScale,
//							setup.zZero + zScale*cam[2],
//							setup.zNear,
//							setup.zFar
//							);
//
//						glMatrixMode(GL_MODELVIEW);
//						glLoadIdentity();
//						glTranslated(
//							xScale*cam[0],
//							yScale*cam[1],
//							-zScale*cam[2] - setup.zZero);
//					}
//				}
//				else {
//
//					if (box == NULL)
//					{
//
//						glViewport(
//							scaleX*eye*screenWidth,
//							0,
//							scaleX*screenWidth,
//							screenHeight);
//						float shx = -size.width()*corner.x() / frameWidth;
//						float shy = -size.height()*corner.y() / frameHeight;
//						gluOrtho2D(
//							shx,
//							size.width()*screenWidth / frameWidth + shx,
//							size.height()*screenHeight / frameHeight + shy,
//							shy);
//					}
//					else
//					{
//						glViewport(box[0], box[1], box[2], box[3]);
//						gluOrtho2D(0, box[2], box[3], 0);
//					}
//					glMatrixMode(GL_MODELVIEW);
//					glLoadIdentity();
//					glTranslated(stereoShift, 0, 0);
//				}
//			}
//			else {
//				if (setup.clip) {
//					glViewport(eye*screenWidth, 0,
//						size.width(), size.height());
//					gluOrtho2D(0, size.width(), size.height(), 0);
//					glMatrixMode(GL_MODELVIEW);
//					glLoadIdentity();
//				}
//				else {
//					glViewport(eye*screenWidth, 0, screenWidth, screenHeight);
//					gluOrtho2D(0, screenWidth, screenHeight, 0);
//					glMatrixMode(GL_MODELVIEW);
//					glLoadIdentity();
//				}
//			}
//
//			layer->paintGL(eye);
//		}
//	}
//
//
//	foreach(StereoView::Layer *layer, layers) {
//		if (!layer->getSetup().visible) continue;
//
//		layer->paintGLEnd();
//	}
//
//	frameno += 1;
//	if (frameno % 120 == 0) {
//		kinSet->forgetFrames(kinProducer->getLocalTime(), 10000);
//	}
//
//	/*if (doMenuReload) {
//	doMenuReload = false;
//	menu.load("config.ini");
//	}
//
//	if (!kinSet->devices.isEmpty()) {
//	kinInter->update(kinSet->devices.first(), kinProducer);
//	}
//
//	glViewport(0, 0, width(), height());
//
//	if (doFrameRefresh) {
//	doFrameRefresh = false;
//	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
//	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
//	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
//	for (int i = 0; i < 2; ++i) {
//	glBindTexture(GL_TEXTURE_2D, texView[i]);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufW, frameBufH, 0, GL_RGB, GL_UNSIGNED_BYTE, frameBuf[i]);
//	}
//	glPopClientAttrib();
//	}
//	glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);
//
//	float frameWidth;
//	float frameHeight;
//	float cornerX;
//	float cornerY;
//
//	int screenWidth = getStereoMode() != 3 ? width() : width()/2;
//	int screenHeight = height();
//	float scaleX = getStereoMode() != 0 && getStereoMode() != 3 ? 0.5 : 1;
//
//	if (screenWidth*frameBufH <= frameBufW*screenHeight) {
//	// frame eq or wider than screen
//	frameWidth = screenWidth;
//	frameHeight = (float)frameBufH*screenWidth/frameBufW;
//	cornerX = 0;
//	cornerY = (screenHeight - frameHeight)/2;
//	} else {
//	frameWidth = (float)frameBufW*screenHeight/frameBufH;
//	frameHeight = screenHeight;
//	cornerX = (screenWidth - frameWidth)/2;
//	cornerY = 0;
//	}
//
//	fakeUser.hands[1].updatePos(
//	mousePos[0]*2.0/frameBufW - 1,
//	1 - mousePos[1]*2.0/frameBufH,
//	0);
//
//	for (int i = 0; i < (getStereoMode() == 0 ? 1 : 2); ++i) {
//	// image
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0, 1, 1, 0);
//	glViewport(
//	scaleX*(cornerX + i*screenWidth),
//	cornerY,
//	scaleX*frameWidth,
//	frameHeight
//	);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, texView[i == 0 ? stereoViewLeft : stereoViewRight]);
//	glBegin(GL_QUADS);
//	glColor4f(1, 1, 1, 1);
//	glTexCoord2f(0, 1);
//	glVertex2f(0, 0);
//	glTexCoord2f(0, 0);
//	glVertex2f(0, 1);
//	glTexCoord2f(1, 0);
//	glVertex2f(1, 1);
//	glTexCoord2f(1, 1);
//	glVertex2f(1, 0);
//	glEnd();
//	// cursor:
//
//	if (paintWithMousePointer && !kinInter->getLeading()) {
//	float curx = mousePos[0]/(float)frameBufW;
//	float cury = mousePos[1]/(float)frameBufH;
//
//	float curw = cursorImg.width()/(float)frameBufW;
//	float curh = cursorImg.height()/(float)frameBufH;
//	glBindTexture(GL_TEXTURE_2D, cursorTexId);
//	glBegin(GL_QUADS);
//	glTexCoord2f(0, 0);
//	glVertex2f(curx, cury);
//	glTexCoord2f(0, 1);
//	glVertex2f(curx, cury + curh);
//	glTexCoord2f(1, 1);
//	glVertex2f(curx + curw, cury + curh);
//	glTexCoord2f(1, 0);
//	glVertex2f(curx + curw, cury);
//	glEnd();
//	}
//
//	// menu:
//	float fbw, fbh;
//	if (frameBufW > frameBufH) {
//	fbw = frameBufW/(float)frameBufH;
//	fbh = 1;
//	} else {
//	fbw = 1;
//	fbh = frameBufH/(float)frameBufW;
//	}
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(-fbw, fbw, fbh, -fbh);
//
//	glEnable(GL_TEXTURE_2D);
//	menu.paintGL(getStereoMode() == 0 ? 0 : 0.1*(2*i - 1));
//	}
//
//	glViewport(0, 0, width(), height());
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(0, width(), 0, height());
//
//	frameno += 1;
//	if (frameno % 120 == 0) {
//	kinSet->forgetFrames(kinProducer->getLocalTime(), 10000);
//	}
//
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glTranslatef(width()/2, 100, 0);
//	glScalef(10, 10, 1);
//	glRotatef(frameno*10, 0, 0, 1);
//	glBegin(GL_QUADS);
//	glColor4f(1, 0, 0, 1);
//	glVertex2f(-1, 1);
//	glColor4f(1, 1, 0, 1);
//	glVertex2f(-1, -1);
//	glColor4f(0, 1, 1, 1);
//	glVertex2f(1, -1);
//	glColor4f(0, 0, 1, 1);
//	glVertex2f(1, 1);
//	glEnd();
//
//	//glViewport(0, 0, width(), height());
//	glDisable(GL_TEXTURE_2D);
//	glPopMatrix();
//	glBegin(GL_QUADS);
//
//	if (kinSet->devices.count() > 0) {
//	float sc = 1;
//	glColor4f(0, 1, 0, 1);
//	int leadingId = 0;
//	if (kinInter->getLeading()) {
//	leadingId = kinInter->getLeading()->id;
//	float c = kinInter->getLeading()->state.dur()/1500.0;
//	if (c > 0.5) {
//	sc = sc - (c - 0.5)*0.5;
//	if (sc < 0.5) {
//	sc = 0.5;
//	}
//	}
//	}
//	Kin::Device *d = kinSet->devices.first();
//	if (d->frames.count() > 0) {
//	Kin::Frame *f = d->frames.last();
//	Kin::UserSkeleton *sk;
//	for (int j = 0; j < f->usersTabLen; ++j) {
//	sk = f->usersTab + j;
//	if (leadingId && sk->uid != leadingId) {
//	continue;
//	}
//	for (int k = 0; k < sk->jointsTabLen; ++k) {
//	Kin::Joint *j = sk->jointsTab + k;
//	glVertex2f(sc*j->pos2[0] - 5, height() - sc*j->pos2[1] + 5);
//	glVertex2f(sc*j->pos2[0] - 5, height() - sc*j->pos2[1] - 5);
//	glVertex2f(sc*j->pos2[0] + 5, height() - sc*j->pos2[1] - 5);
//	glVertex2f(sc*j->pos2[0] + 5, height() - sc*j->pos2[1] + 5);
//	}
//	}
//	}
//	}
//	glEnd();
//
//	if (upMenuDisplay || upMenuDisplay.dur() < 1000) {
//	float ya = upMenuDisplay.getLazyValue();
//
//	glColor4f(1, 1, 1, ya*0.5);
//	glDisable(GL_TEXTURE_2D);
//
//	//drawRect(0, height() - ya*100, width(), 100);
//	{
//	float y = height() - ya*300;
//	glBegin(GL_QUADS);
//	glColor4f(0, 0, 0, ya*0.7);
//	glVertex2f(0 + width(), y + 300);
//	glVertex2f(0, y + 300);
//	glColor4f(0, 0, 0, 0);
//	glVertex2f(0, y);
//	glVertex2f(width(), y);
//	glEnd();
//	}
//
//	for (int i = 0; i < upMenu.length(); ++i) {
//	UpperMenuItem *m = upMenu.at(i);
//	if (m->over && m->over.dur() > 1000 && !m->selected) {
//	sndBip->play();
//	m->selected = true;
//	switch (i) {
//	case 0:
//	upMenu.at(1)->selected = false;
//	emit interactWithCamera();
//	menuActorLastTool = upMenu.at(2)->selected ? 2 : 3;
//	upMenu.at(2)->selected = menuCameraLastTool == 2 ? true : false;
//	upMenu.at(3)->selected = menuCameraLastTool == 3 ? true : false;
//	break;
//	case 1:
//	upMenu.at(0)->selected = false;
//	emit interactWithCurrentActor();
//	menuCameraLastTool = upMenu.at(2)->selected ? 2 : 3;
//	upMenu.at(2)->selected = menuActorLastTool == 2 ? true : false;
//	upMenu.at(3)->selected = menuActorLastTool == 3 ? true : false;
//	break;
//	case 2:
//	upMenu.at(3)->selected = false;
//	break;
//	case 3:
//	upMenu.at(2)->selected = false;
//	break;
//	}
//	}
//	}
//
//	for (int i = 0; i < upMenu.length(); ++i) {
//	UpperMenuItem *m = upMenu.at(i);
//	float overa = m->over.getFuzzy(350);
//	float sela = m->selected.getFuzzy(350);
//	float size = 0.9 - 0.1*(m->selected ? 2*sela : overa);
//
//	static QString name;
//	switch (i) {
//	case 0: name = "ico-camera.png"; break;
//	case 1: name = "ico-actor.png"; break;
//	case 2: name = "ico-move.png"; break;
//	case 3: name = "ico-rotate.png"; break;
//	}
//
//	if (m->selected) {
//	glColor4f(0, 1, 0, 0.9);
//	} else {
//	glColor4f(1 - sela, 1, 1 - sela, 0.5 + 0.4*overa);
//	}
//	drawRectTextured(m->x0 + ya*(m->x1 - m->x0) - m->w*size/2,
//	height() - (m->y0 + ya*(m->y1 - m->y0) - m->h*size/2),
//	m->w*size,
//	m->h*size,
//	name
//	);
//	}
//	}
//
//	Kin::User *u = kinInter->getLeading();
//	if (u == NULL) {
//	//u = &fakeUser;
//	} else {
//	fakeUser.hands[1].valid = false;
//	}
//
//	bool actorMode = upMenu.at(1)->selected;
//
//	if (u) for (int s = 0; s < 2; ++s) {
//	Kin::Hand *h = u->hands + s;
//	float vis = 1;
//	if (!h->valid) {
//	h->clicked = false;
//	if (h->valid.dur() > 1000) {
//	continue;
//	}
//	vis = 1 - h->valid.dur()/1000.0;
//	} else {
//	vis = h->valid.dur()/500.0;
//	if (vis > 1) vis = 1;
//
//	if (upMenuDisplay || upMenuDisplay.dur() < 500) {
//	mainHandStationary = false;
//	} else if (s == 1 && vis >= 1) {
//	QPoint mousePt((h->x()+1)*frameBufW/2, (1-h->y())*frameBufH/2);
//
//	float hvar = h->var(1000);
//	mainHandStationary = hvar < 0.001;
//	//TRACE_COND(mainHandStationary);
//	//TRACE_COND(!mainHandStationary);
//
//	Qt::MouseButton but = upMenu.at(2)->selected ? Qt::MidButton : Qt::LeftButton;
//
//	if (mainHandStationary && mainHandStationary.dur() > 700) {
//	if (h->clicked.dur() > 2000) {
//	if (h->clicked) {
//	h->clicked = false;
//	sndUnClick->play();
//	} else if (h->y() > -0.85) {
//	h->clicked = true;
//	sndClick->play();
//	}
//	if (h->clicked) {
//	if (actorMode) {
//	//emit signalActorPicker(mousePt.x(), frameBufH-mousePt.y(), 0);
//	emit mainHandPress(mousePt.x(), mousePt.y(), Qt::LeftButton, true);
//	}
//	emit mainHandPress(mousePt.x(), mousePt.y(), but, false);
//	} else {
//
//	emit mainHandRelease(mousePt.x(), mousePt.y(), but);
//	}
//	}
//	}
//	if (h->clicked) {
//	emit mainHandMoved(mousePt.x(), mousePt.y(), but);
//	} else if (h->y() > -0.7) {
//	emit mainHandMoved(mousePt.x(), mousePt.y(), Qt::NoButton);
//	}
//	}
//	}
//
//	if (!actorMode || !h->clicked && h->y() > -0.9) {
//	float hx = 48; //hookx
//	float hy = 48; //hooky
//	float hw = 96; //width
//	float hh = 96; //height
//	if (actorMode) {
//	hx = 42*96/256;
//	hy = 96 - 24*96/256;
//	}
//	if (h->side < 0) {
//	hx = hw - hx;
//	}
//
//	GLuint htex = actorMode ?
//	ImageCache::loadGLTexture("cursor-point-transtip.png", false)
//	: h->clicked ?
//	handCursorGrabTex
//	: handCursorTex;
//
//	float xx = (h->x()+1)*frameWidth/2 + cornerX;
//	float yy = (h->y()+1)*frameHeight/2 + cornerY;
//
//	if (s == 1 && vis >= 1) {
//	for (int i = 0; i < upMenu.length(); ++i) {
//	UpperMenuItem *m = upMenu.at(i);
//	float dx = xx - m->x1;
//	float dy = yy - (height() - m->y1 + m->h/2);
//	m->over = upMenuDisplay && upMenuDisplay.dur() > 500 ? dx*dx + dy*dy < m->w*m->w/3 : false;
//	}
//
//	if (upMenuDisplay) {
//	upMenuDisplay = h->y() > 0.55;
//	} else if (!h->clicked && !mainHandStationary) {
//	upMenuDisplay = h->y() > 0.7;
//	}
//	}
//
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, htex);
//	glBegin(GL_QUADS);
//	glColor4f(1, 1, 1, vis*(upMenuDisplay.getLazyValue()*0.35 + 0.65));
//	glTexCoord2f(h->side > 0 ? 0 : 1, 0);
//	glVertex2f(xx - hx, yy - hy + hh);
//	glTexCoord2f(h->side > 0 ? 0 : 1, 1);
//	glVertex2f(xx - hx, yy - hy);
//	glTexCoord2f(h->side > 0 ? 1 : 0, 1);
//	glVertex2f(xx - hx + hw, yy - hy);
//	glTexCoord2f(h->side > 0 ? 1 : 0, 0);
//	glVertex2f(xx - hx + hw, yy - hy + hh);
//	glEnd();
//	}
//	}
//
//
//	glPushMatrix();
//	glLoadIdentity();
//	glDisable(GL_TEXTURE_2D);
//	fsInfoText = kinInter->info;
//
//	if (fsInfoText.dur() < 2900) {
//	float a = (fsInfoText.dur()-2000.0);
//	if (a < 0) {
//	a = 1;
//	} else {
//	a = 1 - a/900.0;
//	}
//	glColor4f(0, 0, 0, a*0.6);
//	drawRect(0 + (1-a)*(1-a)*width(), 0, width(), 170);
//	glColor4f(0, 1, 0, a*0.9);
//	renderText(20 + (1-a)*(1-a)*width(), height() - 50, fsInfoText, fsInfoFont);
//	}
//	glPopMatrix();*/
//}
////------------
//void StereoViewWidget::deleteImg3D()
//{
//	if (img3D != NULL)
//	{
//		delete img3D;
//		img3D = NULL;
//	}
//}
////------------
//void StereoViewWidget::grabCurrent(QString filename)
//{
//	int size[2] = { width(), height() };
//	//char *fb = new char[(size[0]+2)*(size[1]+2)*3]; // +2 cols and +2 rows for optional padding			
//
//	if (img3D == NULL)
//	{
//		img3D = new QImage(size[0], size[1], QImage::Format_ARGB32);
//	}
//	else
//	if ((img3D->size().width() != size[0]) || (img3D->size().height() != size[1]))
//	{
//		delete img3D;
//		img3D = new QImage(size[0], size[1], QImage::Format_ARGB32);
//	}
//
//	//QImage Image(size[0], size[1], 32);
//	//unsigned char *pixelData = Image.bits();
//	unsigned char *pixelData = img3D->bits();
//	makeCurrent();
//	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
//	glPixelStorei(GL_PACK_ALIGNMENT, 1);
//	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
//	//glReadPixels( 0, 0, size[0], size[1], GL_RGB, GL_UNSIGNED_BYTE, fb);		
//	glReadPixels(0, 0, size[0], size[1], GL_BGRA_EXT, GL_UNSIGNED_BYTE, img3D->bits());
//	glPopClientAttrib();
//
//	for (int y = 0; y < size[1] / 2; ++y) {
//		unsigned char *pixelLine1 = pixelData + y*size[0] * 4;
//		unsigned char *pixelLine2 = pixelData + (size[1] - 1 - y)*size[0] * 4;
//		for (int x = 0; x < 4 * size[0]; ++x) {
//			unsigned char t = pixelLine1[x];
//			pixelLine1[x] = pixelLine2[x];
//			pixelLine2[x] = t;
//		}
//	}
//
//	//Image.save(filename,"PNG");
//	img3D->save(filename, "JPG", 98);
//
//	//delete [] fb;
//}
////---------
//QImage * StereoViewWidget::getCurrentStereoImage()
//{
//	int size[2] = { width(), height() };
//	//char *fb = new char[(size[0]+2)*(size[1]+2)*3]; // +2 cols and +2 rows for optional padding			
//
//	if (img3D == NULL)
//	{
//		img3D = new QImage(size[0], size[1], QImage::Format_ARGB32);
//	}
//	else
//	if ((img3D->size().width() != size[0]) || (img3D->size().height() != size[1]))
//	{
//		delete img3D;
//		img3D = new QImage(size[0], size[1], QImage::Format_ARGB32);
//	}
//
//	//QImage Image(size[0], size[1], 32);
//	//unsigned char *pixelData = Image.bits();
//	unsigned char *pixelData = img3D->bits();
//	makeCurrent();
//	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
//	glPixelStorei(GL_PACK_ALIGNMENT, 1);
//	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
//	//glReadPixels( 0, 0, size[0], size[1], GL_RGB, GL_UNSIGNED_BYTE, fb);		
//	glReadPixels(0, 0, size[0], size[1], GL_BGRA_EXT, GL_UNSIGNED_BYTE, img3D->bits());
//	glPopClientAttrib();
//
//	for (int y = 0; y < size[1] / 2; ++y) {
//		unsigned char *pixelLine1 = pixelData + y*size[0] * 4;
//		unsigned char *pixelLine2 = pixelData + (size[1] - 1 - y)*size[0] * 4;
//		for (int x = 0; x < 4 * size[0]; ++x) {
//			unsigned char t = pixelLine1[x];
//			pixelLine1[x] = pixelLine2[x];
//			pixelLine2[x] = t;
//		}
//	}
//
//	return img3D;
//	//Image.save(filename,"PNG");
//	//img3D->save(filename,"JPG",98);
//
//	//delete [] fb;
//}
//
////------------------------------------
//void StereoViewWidget::slotMousePointerMoved(int x, int y)
//{
//	/*if ((mousePos[0]!=x)||(mousePos[1]!=y))
//	{
//	mousePos[0]=x;
//	mousePos[1]=y;
//	fakeUser.hands[1].valid = true;
//	paintWithMousePointer=true;
//	update();
//	} */
//}
////---------------------------------
//void StereoViewWidget::slotMousePointerMoved(int x, int y, int button)
//{
//	/* if ((mousePos[0]!=x)||(mousePos[1]!=y))
//	{
//	mousePos[0]=x;
//	mousePos[1]=y;
//	paintWithMousePointer=true;
//	if (button==0) update();
//	}*/
//}
//
////---------------------
//
//void StereoViewWidget::showEvent(QShowEvent *)
//{
//	repaintTimer.start();
//}
//
//void StereoViewWidget::hideEvent(QHideEvent *)
//{
//	repaintTimer.stop();
//	emit signalWidgetChanged();
//}
//
//void StereoViewWidget::mouseMoveEvent(QMouseEvent *ev)
//{
//	mousePos[0] = ev->x();
//	mousePos[1] = ev->y();
//	update();
//}
//
//void StereoViewWidget::mouseReleaseEvent(QMouseEvent *ev)
//{
//	useFakeUser = !useFakeUser;
//	qWarning() << ("StereoViewWidget: mouse controlled fake user is: %s\n", useFakeUser ? "on" : "off");
//	/*float frameWidth, frameHeight, cornerX, cornerY;
//	int screenWidth = getStereoMode() != 3 ? width() : width()/2;
//	int screenHeight = height();
//	float scaleX = getStereoMode() != 0 && getStereoMode() != 3 ? 0.5 : 1;
//	if (screenWidth*frameBufH <= frameBufW*screenHeight) {
//	// frame eq or wider than screen
//	frameWidth = screenWidth;
//	frameHeight = (float)frameBufH*screenWidth/frameBufW;
//	cornerX = 0;
//	cornerY = (screenHeight - frameHeight)/2;
//	} else {
//	frameWidth = (float)frameBufW*screenHeight/frameBufH;
//	frameHeight = screenHeight;
//	cornerX = (screenWidth - frameWidth)/2;
//	cornerY = 0;
//	}
//	float xx = (ev->x() - cornerX)*frameBufW/frameWidth;
//	float yy = (ev->y() - cornerY)*frameBufH/frameHeight;
//	if (xx < 0 || yy < 0 || xx >= frameBufW || yy >= frameBufH) return;
//	emit signalActorPicker(xx, yy, 0);*/
//}
////---------------
//void StereoViewWidget::closeEvent(QCloseEvent *)
//{
//	repaintTimer.stop();
//	qWarning() << ("close!");
//	emit signalWidgetChanged();
//}
//
//void StereoViewWidget::setStereoView(int left, int right)
//{
//	this->stereoViewLeft = left < 0 ? 0 : left > 1 ? 1 : left;
//	this->stereoViewRight = right < 0 ? 0 : right > 1 ? 1 : right;
//}
//
//int StereoViewWidget::getStereoViewLeft()
//{
//	return stereoViewLeft;
//}
//
//int StereoViewWidget::getStereoViewRight()
//{
//	return stereoViewRight;
//}
//
//void StereoViewWidget::drawRect(float x, float y, float w, float h)
//{
//	glBegin(GL_QUADS);
//	glTexCoord2f(0, 0);
//	glVertex2f(x, y + h);
//	glTexCoord2f(0, 1);
//	glVertex2f(x, y);
//	glTexCoord2f(1, 1);
//	glVertex2f(x + w, y);
//	glTexCoord2f(1, 0);
//	glVertex2f(x + w, y + h);
//	glEnd();
//}
//
//void StereoViewWidget::drawRectTextured(float x, float y, float w, float h, GLuint tex)
//{
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	drawRect(x, y, w, h);
//}
//
//void StereoViewWidget::drawRectTextured(float x, float y, float w, float h, const QString &tex)
//{
//	drawRectTextured(x, y, w, h, ImageCache::loadGLTexture(tex, false));
//}
//
////-----------------
//void StereoViewWidget::updateInputRes(int w, int h)
//{
//
//	int bW = this->width();
//	int bH = this->height();
//
//	if (stereoMode == 1) { // half
//		bW = bW / 2;
//		w /= 2;
//	}
//
//	double scalexF = (double)bW / (w);
//	double scaleyF = (double)bH / (h);
//
//	double smallS = (double)w / h;
//	double bigS = (double)bW / bH;
//
//	newW, newH;
//	if (bigS>smallS)
//	{
//		newH = bH;
//		newW = (newH*w) / h;
//	}
//	else
//	{
//		newW = bW;
//		newH = (newW*h) / w;
//
//	}
//
//
//
//	if ((prevWidth != w) || (prevHeight != h))
//	{
//		//pbuffer->size();;
//		//delete imgTmp;
//		//imgTmp=new QImage(newW,newH,QImage::Format_RGB32);
//		//img->fill(Qt::black);
//
//	}
//
//	int posx, posy;
//	posy = (bH - newH) / 2;	  posx = (bW - newW) / 2;
//	posLeft[0] = posx;	 posLeft[1] = posy;
//
//	posRight[0] = posx + bW;	 posRight[1] = posy;
//	scaleFactorX = (double)(newW) / w;
//	scaleFactorY = (double)(newH) / h;
//	if (stereoMode == 1)
//	{
//		scaleFactorX *= 0.5;
//	}
//	else if (stereoMode == 2)
//	{
//		scaleFactorX *= 0.25;
//	}
//
//	prevWidth = w;
//	prevHeight = h;
//}
//
////-------------------------------------
//
///*void StereoViewWidget::SetLeftImage (int w,int h)
//{
//int bW=img->width()/2;
//int bH=img->height();
//
//double scalexF=(double)bW/(w);
//double scaleyF=(double)bH/(h);
//
//
//double smallS=(double)w/h;
//double bigS=(double)bW/bH;
//
//int newW,newH;
//if (bigS>smallS)
//{
//newH=bH;
//newW=(newH*w)/h;
//}
//else
//{
//newW=bW;
//newH=(newW*h)/w;
//
//}
//
//QPainter p(img);
//int posy=(bH-newH)/2;
//int posx=(bW-newW)/2;
//
//p.drawImage(posx,posy,*imgTmp);
//}
////-------------------------------
//void StereoViewWidget::SetRightImage (int w,int h)
//{
//
//int bW=img->width()/2;
//int bH=img->height();
//
//double scalexF=(double)bW/(w);
//double scaleyF=(double)bH/(h);
//
//double smallS=(double)w/h;
//double bigS=(double)bW/bH;
//
//int newW,newH;
//if (bigS>smallS)
//{
//newH=bH;
//newW=(newH*w)/h;
//}
//else
//{
//newW=bW;
//newH=(newW*h)/w;
//
//}
//
//QPainter p(img);	 int posy=(bH-newH)/2;	 int posx=(bW-newW)/2;
//
//
//p.drawImage(posx+bW,posy,*imgTmp);
//}*/
////-------------------------------
//void StereoViewWidget::updateWidget()
//{
//	//update();
//	//repaint();
//}
////-----------------------------------
//
//void StereoViewWidget::setNewPainterParams(double *params)
//{
//	//type, r,g,b,posx,posy,sizex,sizey
//	for (int i = 0; i<params[0] * 9 + 1; i++)
//	{
//		painterParams[i] = params[i];
//	}
//
//}
//
//void StereoViewWidget::repaintTimeout()
//{
//	update();
//}
//
//void StereoViewWidget::reloadMenu()
//{
//	doMenuReload = true;
//}
//
//void StereoViewWidget::menuShow()
//{
//	menu.show();
//	update();
//}
//
//void StereoViewWidget::menuHide()
//{
//	menu.hide();
//}
//
//void StereoViewWidget::menuNext()
//{
//	menu.highlightNext();
//}
//
//void StereoViewWidget::menuSelect()
//{
//	menu.select();
//}
//
//void StereoViewWidget::leadingUserChanged(Kin::User *u)
//{
//	mainHandStationary = false;
//	upMenuDisplay = false;
//	if (u) {
//		qWarning() << ("Leading user: %d", u->id);
//	}
//	else {
//		qWarning() << ("Leading user: NONE");
//	}
//}
