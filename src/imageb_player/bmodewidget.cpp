/****************************************************************************
    **
    ** Copyright (C) 2004-2005 Trolltech AS. All rights reserved.
    **
    ** This file is part of the documentation of the Qt Toolkit.
    **
    ** This file may be used under the terms of the GNU General Public
    ** License version 2.0 as published by the Free Software Foundation
    ** and appearing in the file LICENSE.GPL included in the packaging of
    ** this file.  Please review the following information to ensure GNU
    ** General Public Licensing requirements will be met:
    ** http://www.trolltech.com/products/qt/opensource.html
    **
    ** If you are unsure which license is appropriate for your use, please
    ** review the following information:
    ** http://www.trolltech.com/products/qt/licensing.html or contact the
    ** sales department at sales@trolltech.com.
    **
    ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
    ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
    **
    ****************************************************************************/

    #include <QtGui>
    #include <math.h>
    #include "bmodewidget.h"
    #include "imgproc/acquisition.h"
    
    const int BufferSize = 10;
    QImage imgbuffer[BufferSize];
    QSemaphore freeBytes(BufferSize);
    QSemaphore usedBytes;
    Matrix buffer[BufferSize];
    
    //!global variable to Signal Setup
    SetupSinal signal_setup; 
    //! global variable to data acquired information
    Acquisition aq;


    const double DefaultCenterX = -0.637011f;
    const double DefaultCenterY = -0.0395159f;
    const double DefaultScale = 0.00403897f;

    const double ZoomInFactor = 0.8f;
    const double ZoomOutFactor = 1 / ZoomInFactor;
    const int ScrollStep = 20;

    //! constants for color maps
    int gray=0, jet=1, pink=2;

    BModeWidget::BModeWidget()
    {
        centerX = DefaultCenterX;
        centerY = DefaultCenterY;
        pixmapScale = DefaultScale;
        curScale = DefaultScale;

        setWindowTitle(tr("Ultrasound B-Mode Imaging..."));
        setCursor(Qt::CrossCursor);
        resize(600, 400);
         
        createActions();
        createToolBar();
    
        QStringList files;
    
        //seleciona arquivos para processamento
        files = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            QDir::currentPath(),
                            "Signal Data (*.csv *.dat)");
        if (files.size() <= 0)
           return;
        
        files.sort(); //ordena os arquivos selecionados por nome 
        
        //abre todos os arquivos, processa e normaliza-os entre 0 - 255  
        Matrix mat = imageB.getDataMatrix(files, files.size(), 1024);
        imgproc.norm(mat);
        
        int col = (int) round(mat.nCol()/2);//seleciona uma coluna da metade da matriz
        if (col ==0) col = 1;
        
        //configuração de ganho e corte por limiar
        signal_setup.setRefSignal(imgproc.getCol(mat,col));
        signal_setup.setRFData(imgproc.getCol(mat,col));
        signal_setup.setGain(mat.nRow(),0,100);
        signal_setup.setAttenuation(0,0,-30);
        signal_setup.setThreshold(0);
        signal_setup.applyTimeCut(1,0);
        signal_setup.applyGainLimiar();
        
        aq.elements = files.size();
        
        qRegisterMetaType<QImage>("QImage");
        qRegisterMetaType<QPixmap>("QPixmap");
        
        //connect(&consumer, SIGNAL(renderedImage(const QImage &, double)),
        //        this, SLOT(updatePixmap(const QImage &, double)));
        
        connect(&consumer, SIGNAL(renderedImage(const QPixmap &, double)),
                this, SLOT(updatePixmap(const QPixmap &, double)));

        connect(&consumer, SIGNAL(numFramesPerSecond(int)),
                this, SLOT(setNumFPS(int)));

        n_fps = 0;
        producer.setSource(files);
        producer.start();
        consumer.start();
    }

  
    void BModeWidget::closeEvent(QCloseEvent *event)
    {
  
      producer.stop();
      qDebug() << "STOP Producer";

      consumer.stop();
      qDebug() << "STOP Consumer";
    
      event->accept();
    }
  
  
    void BModeWidget::paintEvent(QPaintEvent * /* event */)
    {
        QPainter painter(this);
        painter.fillRect(rect(), Qt::black);

        if (pixmap.isNull()) {
            painter.setPen(Qt::white);
            painter.drawText(rect(), Qt::AlignCenter,
                             tr("Rendering initial image, please wait..."));
            return;
        }

        if (curScale == pixmapScale) {
            painter.drawPixmap(pixmapOffset, pixmap);
        } else {
            double scaleFactor = pixmapScale / curScale;
            int newWidth = int(pixmap.width() * scaleFactor);
            int newHeight = int(pixmap.height() * scaleFactor);
            int newX = pixmapOffset.x() + (pixmap.width() - newWidth) / 2;
            int newY = pixmapOffset.y() + (pixmap.height() - newHeight) / 2;

            painter.save();
            painter.translate(newX, newY);
            painter.scale(scaleFactor, scaleFactor);
            painter.drawPixmap(0, 0, pixmap);
            painter.restore();
        }

        QString text = "Use mouse wheel to zoom."
                          "Press and hold left mouse button to scroll. fps=" + QString::number(n_fps,10);
      
        QFontMetrics metrics = painter.fontMetrics();
        int textWidth = metrics.width(text);

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 127));
        painter.drawRect((width() - textWidth) / 2 - 5, 0, textWidth + 10,
                         metrics.lineSpacing() + 5);
        painter.setPen(Qt::white);
        painter.drawText((width() - textWidth) / 2,
                         metrics.leading() + metrics.ascent(), text);
    }

    void BModeWidget::resizeEvent(QResizeEvent * /* event */)
    {
        update();
    }

    void BModeWidget::keyPressEvent(QKeyEvent *event)
    {
        switch (event->key()) {
        case Qt::Key_Plus:
            zoom(ZoomInFactor);
            break;
        case Qt::Key_Minus:
            zoom(ZoomOutFactor);
            break;
        case Qt::Key_Left:
            scroll(-ScrollStep, 0);
            break;
        case Qt::Key_Right:
            scroll(+ScrollStep, 0);
            break;
        case Qt::Key_Down:
            scroll(0, -ScrollStep);
            break;
        case Qt::Key_Up:
            scroll(0, +ScrollStep);
            break;
        case Qt::Key_F1:
           {
            //mostra/esconde barra de ferramentas ao pressionar F1
            playerToolBar->setVisible( !playerToolBar->isVisible() );
            imageToolBar->setVisible( !imageToolBar->isVisible() ); 
           }
            break;
        default:
            QWidget::keyPressEvent(event);
        }
        
        cout << event->text().toStdString() << endl;
    }

    void BModeWidget::wheelEvent(QWheelEvent *event)
    {
        int numDegrees = event->delta() / 8;
        double numSteps = numDegrees / 15.0f;
        zoom(pow(ZoomInFactor, numSteps));
    }

    void BModeWidget::mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton)
            lastDragPos = event->pos();
    }

    void BModeWidget::mouseMoveEvent(QMouseEvent *event)
    {
        if (event->buttons() & Qt::LeftButton) {
            pixmapOffset += event->pos() - lastDragPos;
            lastDragPos = event->pos();
            update();
        }
    }

    void BModeWidget::mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            pixmapOffset += event->pos() - lastDragPos;
            lastDragPos = QPoint();

            int deltaX = (width() - pixmap.width()) / 2 - pixmapOffset.x();
            int deltaY = (height() - pixmap.height()) / 2 - pixmapOffset.y();
            scroll(deltaX, deltaY);
        }
    }

    void BModeWidget::updatePixmap(const QPixmap &image, double scaleFactor)
    {
        if (!lastDragPos.isNull())
            return;

        pixmap = image;
        pixmapScale = scaleFactor;
        update();
    }

    void BModeWidget::setNumFPS(int fps)
    {
       this->n_fps = fps;
       update();
    }
  
    void BModeWidget::zoom(double zoomFactor)
    {
        curScale *= zoomFactor;
        update();
    }

    void BModeWidget::scroll(int deltaX, int deltaY)
    {
        centerX += deltaX * curScale;
        centerY += deltaY * curScale;
        update();
    }
  
    void BModeWidget::setColorMapGray()
    {
       consumer.setColorMap(gray);
    }

    void BModeWidget::setColorMapJet()
    {
      consumer.setColorMap(jet);
    }
  
    void BModeWidget::setColorMapPink()
    {
     consumer.setColorMap(pink);
    }


  
    void BModeWidget::createActions()
    {
        settingAct = new QAction( QIcon("images/sample_setting.png"), tr("&Sampling Settings"), this);
        settingAct->setShortcut(tr("Ctrl+D"));
        connect(settingAct, SIGNAL(triggered()), this, SLOT(settings()));
    
        imgSetupAct = new QAction(QIcon("images/setup_signal.png"), tr("Setup Signa&l"), this);
        imgSetupAct->setShortcut(tr("Ctrl+S"));
        imgSetupAct->setEnabled(true);
        connect(imgSetupAct, SIGNAL(triggered()), this, SLOT( setupSignal() ));
    
        playAct = new QAction(QIcon("images/pause.png"), tr("Pause/Disconnect"), this);
        playAct->setEnabled(true);
        playAct->setShortcut(tr("Ctrl+P"));
        connect(playAct, SIGNAL(triggered()), this, SLOT( play_pause() ));
    
        actGroup = new QActionGroup(this); //agrupamento de check buttons
    
        grayAct = new QAction(QIcon("images/colormap_gray.png"), tr("&Gray Scale"), this);
        grayAct->setEnabled(true);
        grayAct->setCheckable(true);
        grayAct->setChecked(true);
        connect(grayAct, SIGNAL(triggered()), this, SLOT(setColorMapGray()));
        
        jetAct = new QAction(QIcon("images/colormap_jet.png"),tr("&Jet Style"), this);
        jetAct->setEnabled(true);
        jetAct->setCheckable(true);
        connect(jetAct, SIGNAL(triggered()), this, SLOT(setColorMapJet()));
        
        pinkAct = new QAction(QIcon("images/colormap_pink.png"), tr("&Red"), this);
        pinkAct->setEnabled(true);
        pinkAct->setCheckable(true);
        connect(pinkAct, SIGNAL(triggered()), this, SLOT(setColorMapPink()));
        
        actGroup->addAction(grayAct);
        actGroup->addAction(jetAct);
        actGroup->addAction(pinkAct);
    }
  
  
    void BModeWidget::createToolBar()
    {
        playerToolBar = addToolBar(tr("Player"));
        playerToolBar->addAction(settingAct);
        playerToolBar->addAction(playAct);
    
        imageToolBar = addToolBar(tr("Image"));
        imageToolBar->addAction(imgSetupAct);
        imageToolBar->addSeparator();
        imageToolBar->addActions(actGroup->actions());
    }
  
  
    void BModeWidget::play_pause()
    {
       //troca o ícone e o tip
       if ( ! producer.isRunning() )
         {
          playAct->setIcon( QIcon("images/pause.png") );
          playAct->setToolTip("Pause/Disconnect");
         }
       else
         {
          playAct->setIcon( QIcon("images/play.png") );
          playAct->setToolTip("Play/Connect");
         }

       producer.play_pause(); //pára/inicia o processo
       
    }
  
    void BModeWidget::setupSignal()
    {
       //pausa o processo se estiver rodando para efetuar a configuração 
       if ( producer.isRunning() )
           this->play_pause();
    
       frmProcSinal *frmProc = new frmProcSinal(this);
       //connect(frmProc, SIGNAL(setupChanged()), this, SLOT( update() ));
     
       frmProc->hide(); 
       frmProc->setWindowModality (Qt::ApplicationModal); //seta a janela como modal
       frmProc->show();//exibe janela
    }
  
    void BModeWidget::settings()
    {
      //pausa o processo se estiver rodando para efetuar a configuração 
       if ( producer.isRunning() )
           this->play_pause();
  
     
       frmSettings *frmDataSettings = new frmSettings(this);
     
       frmDataSettings->hide(); 
       frmDataSettings->setWindowModality (Qt::ApplicationModal); //seta a janela como modal
       frmDataSettings->show();//exibe janela
    }