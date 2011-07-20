#include <QtGui>
#include <QMouseEvent>
#include "imagebform.h"

using namespace std;

//! constants for color maps
int gray=0, jet=1, pink=2;

//!global variable to Signal Setup
SetupSinal signal_setup; 
//! global variable to data acquired information
Acquisition aq;

ImageBForm::ImageBForm()
{
        imageLabel = new QLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);
        
        scrollArea = new QScrollArea;
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidget(imageLabel);
        setCentralWidget(scrollArea);

        createActions();
        createMenus();
        createToolBars();
        loadPlugins();

        colormap = gray;//seta default colormap
        focusMode = false; //modo de exibição normal ou focalizado

        setWindowTitle(tr("ImageB - US Image Viewer 1.0")); 
        resize(640, 480);
}

void ImageBForm::open_files()
{
  
   QStringList files;

   files = QFileDialog::getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            QDir::currentPath(),
                            "Signal Data (*.csv *.dat)");

  if (files.size() <= 0)
     return;
  
  files.sort(); //ordena os arquivos selecionados por nome

  //abre todos os arquivos, processa e normaliza-os entre 0 - 255  
  mat = imageB.getDataMatrix(files, files.size(), 1024);
  imgproc.norm(mat);
  
  int col = (int) round(mat.nCol()/2);//seleciona uma coluna da metade da matriz
  if (col ==0) col = 1;
  
  
  //esconde o botão de processamento polar para cartesiano
  imgPolarAct->setVisible(false);
  
  //configuração de ganho e corte por limiar
  signal_setup.setRefSignal(imgproc.getCol(mat,col));
  signal_setup.setRFData(imgproc.getCol(mat,col));
  signal_setup.setGain(mat.nRow(),0,100);
  signal_setup.setAttenuation(0,0,-30);
  signal_setup.setThreshold(0);
  signal_setup.applyTimeCut(1,0);
  signal_setup.applyGainLimiar();
  
  aq.elements = files.size();
  imageB.setSignalSetup(signal_setup);//seta as configurações para processamento
  
  mat_proc = imageB.procData2BMode(mat);
  
  //objeto retorna uma imagem redimensionada 
  image = imageB.getBModeImage(600,400, mat_proc, colormap);
  
  imageLabel->setPixmap(QPixmap::fromImage(image));//seta a imagem a ser exibida
  
  scaleFactor = 1.0;

  fitToWindowAct->setEnabled(true);
  imgSAFTAct->setEnabled(true);
  imgSquareAct->setEnabled(true);
  imgSetupAct->setEnabled(true);
  signalCutAct->setEnabled(true); 
  focusMode = false;
  
  updateActions();

  if (!fitToWindowAct->isChecked())
     imageLabel->adjustSize();

}


 void ImageBForm::open()
   {
   
        QString filename = QFileDialog::getOpenFileName(this,
                                        tr("Open File"), QDir::currentPath());
        focusMode = false;
        imageB.removeSignalSetup(); //cancela o processamento de ganho e corte por limiar

        if (filename.isEmpty())
           return;

        if ( filename.endsWith(".jpg",Qt::CaseInsensitive) ) //arquivo JPG
           {
             image = QImage(filename);
             long max_pixel = 640*480; //número máximo de pontos aceitáveis pelo programa imageB
             long image_pixel = (image.width()*image.height());
             
             if ( image_pixel > max_pixel )
             {
               QMessageBox::critical(this, "ImageB US-Image Viewer",
                              QString("ImageB can not open images larger than 307.200 pixels (640x480)")+
                                      "\n The image your are trying to open has dismension: " + QString::number(image.width()) + "x" +
                                      QString::number(image.height()) + "\n and " + QString::number(image_pixel) + " pixels. Try to open a smaller file.");
              
                mat_proc.set(150);
             }             
             else
             {
                 mat_proc = imageB.convertRGB2Gray(image);
                
                 //objeto retorna uma imagem redimensionada
                 image = imageB.getBModeImage(image.width(), image.height() , mat_proc, colormap );
             }
           } 
        else
          {
            //trata como arquivo de sinais de rf
          
            mat = imageB.loadMatrixData( filename.toStdString() );
            aq.rf_matrix = mat;
            
            /*se a imagem tiver dimensõe compatíveis com o processamento
            cart2pol então habilita botão de conversão de coordenada*/ 
            if ( mat.nCol() != 81 || mat.nRow() != 1401 )
               imgPolarAct->setVisible(false);
            else
               imgPolarAct->setVisible(true);
            
            
            mat_proc = imageB.procData2BMode(mat);
            
            //objeto retorna uma imagem redimensionada 
            image = imageB.getBModeImage(600, 400 , mat_proc, colormap );
        }
      
        imageLabel->setPixmap(QPixmap::fromImage(image));
        scaleFactor = 1.0;

        fitToWindowAct->setEnabled(true);
        updateActions();
        
        //habilita somente quando abrir imagem composta por sinais de vários arquivos 
        imgSAFTAct->setEnabled(false);
        imgSquareAct->setEnabled(false);
        imgSetupAct->setEnabled(false);
        signalCutAct->setEnabled(false);
        
        if (!fitToWindowAct->isChecked())
           imageLabel->adjustSize();
    }
  
  
  
    void ImageBForm::saveImage()
    {
 
      QString filename = QFileDialog::getSaveFileName(
                    this,
                    "Choose a filename to save under",
                    QDir::currentPath(),
                    "Images (*.jpg)");
    
      if (!filename.isEmpty())
          image.save(filename,"JPG",100);
    }
  
  
    void ImageBForm::exportImage()
    {
 
      QString filename = QFileDialog::getSaveFileName(
                    this,
                    "Choose a filename to save under",
                    QDir::currentPath(),
                    "Data File (*.dat)");
    
      if (!filename.isEmpty())
            imgproc.writeMatrixData(mat_proc,filename.toStdString());
    }
  
    void ImageBForm::zoomIn()
    {
        scaleImage(1.25);
    }

    void ImageBForm::zoomOut()
    {
        scaleImage(0.8);
    }

    void ImageBForm::normalSize()
    {
        imageLabel->adjustSize();
        scaleFactor = 1.0;
    }

    void ImageBForm::fitToWindow()
    {
        bool fitToWindow = fitToWindowAct->isChecked();
        scrollArea->setWidgetResizable(fitToWindow);
        if (!fitToWindow) {
            normalSize();
        }
        updateActions();
    }

  
    void ImageBForm::setColorMapGray()
    {
        if ( grayAct->isChecked() )
         {
            colormap = gray;
            //objeto retorna uma imagem redimensionada 
            image = imageB.getBModeImage(600,400, mat_proc, colormap );
            imageLabel->setPixmap(QPixmap::fromImage(image));
            
            //if ( !pinkAct->isChecked() &&  !jetAct->isChecked() &&)
         }
       
       pinkAct->setChecked(false);
       jetAct->setChecked(false);
       grayAct->setChecked(true);
    }
  
    void ImageBForm::setColorMapJet()
    {
       if ( jetAct->isChecked() )
            colormap = jet;
       else
            colormap = gray;
        
       //objeto retorna uma imagem redimensionada 
       image = imageB.getBModeImage(600,400, mat_proc, colormap );
       imageLabel->setPixmap(QPixmap::fromImage(image));
       pinkAct->setChecked(false);
       grayAct->setChecked(false);
       
       if (colormap == gray)
           grayAct->setChecked(true);
    }
  
    void ImageBForm::setColorMapPink()
    {
       if ( pinkAct->isChecked() )
            colormap = pink;
       else
            colormap = gray;
        
       //objeto retorna uma imagem redimensionada 
       image = imageB.getBModeImage(600,400, mat_proc, colormap );
       imageLabel->setPixmap(QPixmap::fromImage(image)); 
       jetAct->setChecked(false);
       grayAct->setChecked(false);
    }
  
  
   void ImageBForm::setNormalMode()
   {  
      mat_proc = imageB.procData2BMode(mat);
      image = imageB.getBModeImage(600,400, mat_proc, colormap );
      imageLabel->setPixmap(QPixmap::fromImage(image));
      focusMode = false;
   }
 
  
 
   void ImageBForm::setFocusMode()
   {
    
      aq.rf_matrix = mat; //objeto aquisição global
      mat_proc = imageB.getBModeFocusMatrix(600,400, aq, colormap);
      
      imgproc.writeMatrixData(mat_proc,"IMAGEF.txt");
      image = imageB.getBModeImage(600,400, mat_proc, colormap );
      imageLabel->setPixmap(QPixmap::fromImage(image));
      
      focusMode = true;
   }
 
    void ImageBForm::setupSignal()
    {
       frmProcSinal *frmProc = new frmProcSinal(this);
       connect(frmProc, SIGNAL(setupChanged()), this, SLOT( updateImage() ));
     
       frmProc->hide(); 
       frmProc->setWindowModality (Qt::ApplicationModal); //seta a janela como modal
       frmProc->show();//exibe janela
            
    }
  
    void ImageBForm::signalCut()
    {
      //cout << "signal cut" << endl;
      frmCorteSinal *frmCorte = new frmCorteSinal(this);
      connect(frmCorte, SIGNAL(setupChanged()), this, SLOT( updateImage() ));
     
      frmCorte->hide(); 
      frmCorte->setWindowModality (Qt::ApplicationModal); //seta a janela como modal
      frmCorte->show();//exibe janela
    }
   
    void ImageBForm::updateImage()
    {
       imageB.setSignalSetup(signal_setup);//seta as configurações para processamento
    
      if (focusMode)
        {
          aq.rf_matrix = mat; //objeto aquisição global
          mat_proc = imageB.getBModeFocusMatrix(600,400, aq, colormap); 
        }
      else
        {
           mat_proc = imageB.procData2BMode(mat);
        }
       
       //objeto retorna uma imagem redimensionada 
       image = imageB.getBModeImage(600,400, mat_proc, colormap);
       imageLabel->setPixmap(QPixmap::fromImage(image));//seta a imagem a ser exibida
    }
 
    void ImageBForm::updateSFImage()
    {
      cout << "updating SF image..." << endl;
      //cout << aq.elements << "," << aq.d << "," << aq.deltat << "," << aq.c << "," << aq.theta << endl;
      
      imgPolarAct->setVisible(true);
      mat_proc = imageB.procData2BMode(aq.rf_matrix); 
      image = imageB.getBModeImage(600,400, mat_proc, colormap);
      imageLabel->setPixmap(QPixmap::fromImage(image));//seta a imagem a ser exibida
    }
  
    void ImageBForm::about()
    {
        QMessageBox::about(this, tr("About Image Viewer"),
                tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
                   "and QScrollArea to display an image. QLabel is typically used "
                   "for displaying a text, but it can also display an image. "
                   "QScrollArea provides a scrolling view around another widget. "
                   "If the child widget exceeds the size of the frame, QScrollArea "
                   "automatically provides scroll bars. </p><p>The example "
                   "demonstrates how QLabel's ability to scale its contents "
                   "(QLabel::scaledContents), and QScrollArea's ability to "
                   "automatically resize its contents "
                   "(QScrollArea::widgetResizable), can be used to implement "
                   "zooming and scaling features. </p><p>In addition the example "
                   "shows how to use QPainter to print an image.</p>"));
    }

    void ImageBForm::createActions()
    {
        openAct = new QAction( QIcon("images/open.png"), tr("&Open Image DAT..."), this);
        openAct->setShortcut(tr("Ctrl+O"));
        connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    
        openNAct = new QAction(QIcon("images/open_rf.png"), tr("&Open RF Files..."), this);
        openNAct->setShortcut(tr("Ctrl+E"));
        connect(openNAct, SIGNAL(triggered()), this, SLOT(open_files()));
    
        saveImageAct = new QAction(QIcon("images/save.png"), tr("&Save Image As..."), this);
        saveImageAct->setShortcut(tr("Ctrl+S"));
        saveImageAct->setEnabled(false);
        connect(saveImageAct, SIGNAL(triggered()), this, SLOT(saveImage()));
    
        exportImageAct = new QAction(QIcon("images/paste.png"), tr("Ex&port Image"), this);
        exportImageAct->setShortcut(tr("Ctrl+P"));
        exportImageAct->setEnabled(false);
        connect(exportImageAct, SIGNAL(triggered()), this, SLOT(exportImage()));

        exitAct = new QAction(tr("E&xit"), this);
        exitAct->setShortcut(tr("Ctrl+Q"));
        connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

        zoomInAct = new QAction(QIcon("images/zoom_in.png"), tr("Zoom &In (25%)"), this);
        zoomInAct->setShortcut(tr("Ctrl++"));
        zoomInAct->setEnabled(false);
        connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

        zoomOutAct = new QAction(QIcon("images/zoom_out.png"), tr("Zoom &Out (25%)"), this);
        zoomOutAct->setShortcut(tr("Ctrl+-"));
        zoomOutAct->setEnabled(false);
        connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

        normalSizeAct = new QAction(tr("&Normal Size"), this);
        normalSizeAct->setShortcut(tr("Ctrl+N"));
        normalSizeAct->setEnabled(false);
        connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

        fitToWindowAct = new QAction(tr("&Fit to Window"), this);
        fitToWindowAct->setEnabled(false);
        fitToWindowAct->setCheckable(true);
        fitToWindowAct->setShortcut(tr("Ctrl+F"));
        connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));
        
        imgBright = new QAction(tr("&Brightness/Contrast"), this);
        connect(imgBright, SIGNAL(triggered()), this, SLOT(bright()));
        
        signalSFSAFT = new QAction(tr("&Process SF Data"), this);
        connect(signalSFSAFT, SIGNAL(triggered()), this, SLOT(SFSAFT()));
        
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
        
        imgPolarAct = new QAction(QIcon("images/image_polar.png"), tr("&Cart. to Polar Image"), this);
        imgPolarAct->setVisible(false);
        imgPolarAct->setCheckable(true);
        connect(imgPolarAct, SIGNAL(triggered()), this, SLOT(Cart2PolarImage()));
        
        imgSquareAct = new QAction(QIcon("images/image_square.png"), tr("&Normal Mode"), this);
        imgSquareAct->setEnabled(false);
        connect(imgSquareAct, SIGNAL(triggered()), this, SLOT( setNormalMode()));
        
        imgSAFTAct = new QAction(QIcon("images/image_saft.png"), tr("&Focus on Reception using SAFT"), this);
        imgSAFTAct->setEnabled(false);
        connect(imgSAFTAct, SIGNAL(triggered()), this, SLOT( setFocusMode() ));

        imgSetupAct = new QAction(QIcon("images/setup_signal.png"), tr("Setup Signa&l"), this);
        imgSetupAct->setEnabled(false);
        connect(imgSetupAct, SIGNAL(triggered()), this, SLOT( setupSignal() ));
        
        signalCutAct = new QAction(tr("Time Signal C&ut"), this);
        signalCutAct->setEnabled(false);
        connect(signalCutAct, SIGNAL(triggered()), this, SLOT( signalCut() ));
        
        refreshPluginAct = new QAction(tr("Re&fresh Plugins"), this);
        connect(refreshPluginAct, SIGNAL(triggered()), this, SLOT( loadPlugins() ));

        undoPluginAct = new QAction(tr("Undo Plugin Action"), this);
        connect(undoPluginAct, SIGNAL(triggered()), this, SLOT( undoPluginAction() ));


        aboutAct = new QAction(tr("&About"), this);
        connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

        aboutQtAct = new QAction(tr("About &Qt"), this);
        connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    }

    void ImageBForm::createMenus()
    {
        fileMenu = new QMenu(tr("&File"), this);
        fileMenu->addAction(openAct);
        fileMenu->addAction(openNAct);
        fileMenu->addSeparator();
        fileMenu->addAction(saveImageAct);
        fileMenu->addAction(exportImageAct);
        fileMenu->addSeparator();  
        fileMenu->addAction(exitAct);

        viewMenu = new QMenu(tr("&View"), this);
        viewMenu->addAction(zoomInAct);
        viewMenu->addAction(zoomOutAct);
        viewMenu->addAction(normalSizeAct);
        viewMenu->addSeparator();
        viewMenu->addAction(fitToWindowAct);
    
        signalMenu = new QMenu(tr("Si&gnal"), this);
        signalMenu->addAction(imgSetupAct);
        signalMenu->addAction(signalCutAct);
        signalMenu->addSeparator();
        signalMenu->addAction(signalSFSAFT);
        signalMenu->addSeparator();  
        signalMenu->addAction(imgBright);
 
        colorMapMenu = viewMenu->addMenu(tr("&Colormap"));
        colorMapMenu->addAction(grayAct);
        colorMapMenu->addAction(jetAct);
        colorMapMenu->addAction(pinkAct);

        pluginMenu = new QMenu(tr("&Plugin"), this);
        pluginMenu->addAction(refreshPluginAct);
        pluginMenu->addAction(undoPluginAct);
        pluginMenu->addSeparator();
    
        helpMenu = new QMenu(tr("&Help"), this);
        helpMenu->addAction(aboutAct);
        helpMenu->addAction(aboutQtAct);

        menuBar()->addMenu(fileMenu);
        menuBar()->addMenu(viewMenu);
        menuBar()->addMenu(signalMenu);
        menuBar()->addMenu(pluginMenu);
        menuBar()->addMenu(helpMenu);
    }

    void ImageBForm::createToolBars()
    {
        fileToolBar = addToolBar(tr("File"));
        fileToolBar->addAction(openAct);
        fileToolBar->addAction(openNAct);
    
        imageToolBar = addToolBar(tr("Image"));
        imageToolBar->addAction(grayAct);
        imageToolBar->addAction(jetAct);
        imageToolBar->addAction(pinkAct);
        imageToolBar->addSeparator();
        imageToolBar->addAction(imgSquareAct);
        imageToolBar->addAction(imgSAFTAct);
        imageToolBar->addSeparator();
        imageToolBar->addAction(imgSetupAct);
        imageToolBar->addSeparator();
        imageToolBar->addAction(imgPolarAct);
    }
  
    void ImageBForm::updateActions()
    {
        zoomInAct->setEnabled(!fitToWindowAct->isChecked());
        zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
        normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
        
        saveImageAct->setEnabled(true);
        exportImageAct->setEnabled(true);
    }

    void ImageBForm::scaleImage(double factor)
    {
        Q_ASSERT(imageLabel->pixmap());
        scaleFactor *= factor;
        imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

        adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
        adjustScrollBar(scrollArea->verticalScrollBar(), factor);

        zoomInAct->setEnabled(scaleFactor < 3.0);
        zoomOutAct->setEnabled(scaleFactor > 0.333);
    }

    void ImageBForm::adjustScrollBar(QScrollBar *scrollBar, double factor)
    {
        scrollBar->setValue(int(factor * scrollBar->value()
                                + ((factor - 1) * scrollBar->pageStep()/2)));
    }

  
    void ImageBForm::reloadImage(Matrix pixel_data)
    {
       mat_proc = pixel_data;
    
       //preserva tamanho original da imagem
       if (!image.isNull())
           image = imageB.getBModeImage(image.width(), image.height(), mat_proc, colormap );
       else
           image = imageB.getBModeImage(600, 400, mat_proc, colormap ); 
       
       imageLabel->setPixmap(QPixmap::fromImage(image));
    }
  
    void ImageBForm::bright()
    {
    
       frmBrightContrast *frmBC = new frmBrightContrast(this);
       connect(frmBC, SIGNAL(reloadImage(Matrix)), this, SLOT( reloadImage(Matrix) ));
    
       frmBC->hide();
       frmBC->setImageData(mat_proc);//seta a matriz que compoe a imagem
       frmBC->setWindowModality (Qt::ApplicationModal); //seta a janela como modal
       frmBC->show();//exibe janela
    /*
      QImage img = image;
      //img = (imageLabel->pixmap())->toImage();
    
      for (int i=0;i<img.width();i++)
      {
           for (int j=0;j<img.height();j++)
           {
             QRgb c = img.pixel(i,j);
             int r = qRed(c);
             int g = qGreen(c);
             int b = qBlue(c);
             float perc = 0.3;  
             float v =0;
           
             v = r + (r*perc);
             if (v < 0) r = 0;
               else if (v > 255) r = 255;
                    else r = (int)round(v);
            
             v = g + (g*perc);
             if (v < 0) g = 0;
               else if (v > 255) g = 255;
                    else g = (int)round(v);
                      
             v = b + (b*perc);
             if (b < 0) b = 0;
               else if (v > 255) b = 255;
                    else b = (int)round(v);
                      
            img.setPixel(i,j,qRgb(r,g,b));
           }
     }
   
      
      image = img;
      imageLabel->setPixmap(QPixmap::fromImage(img));//seta a imagem a ser exibida
      */
    }
  
  
    void ImageBForm::Cart2PolarImage()
    {
       
      if ( imgPolarAct->isChecked() )
      {
        aq.theta = 40;
        mat_proc = imageB.createPolarImage(mat);
      } 
      else 
        mat_proc = mat;
      
      mat_proc = imageB.procData2BMode(mat_proc);
   
      //objeto retorna uma imagem redimensionada 
      image = imageB.getBModeImage(600,400, mat_proc, colormap);
  
      imageLabel->setPixmap(QPixmap::fromImage(image));//seta a imagem a ser exibida
    }
  
  
    void ImageBForm::SFSAFT()
    {
      //cout << "signal cut" << endl;
      frmSFSAFT *frmSF = new frmSFSAFT(this);
      connect(frmSF, SIGNAL(finishedSFSAFT()), this, SLOT( updateSFImage() ));
     
      frmSF->hide(); 
      frmSF->setWindowModality (Qt::ApplicationModal); //seta a janela como modal
      frmSF->show();//exibe janela
    }
  
  
    //carrega todos os plugins do diretório
    void ImageBForm::loadPlugins()
    {
         clearPlugins(); //limpa todos os plugins carregados da memória
    
         typedef moduleBase *(*MyPrototype)();
      
         QDir pluginsDir = QDir(qApp->applicationDirPath()); //caminho da aplicação
         pluginsDir.cd("plugins"); //entra no diretório plugins
        
         //percorre todos os arquivos do diretório
         foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
              QString filePath = pluginsDir.absoluteFilePath(fileName); 
      
              QLibrary myLib(filePath);
              MyPrototype myFunction;
         
              myFunction = (MyPrototype) myLib.resolve("Create");
              
              if (myFunction)
                  cout << myFunction()->getName() << endl;
              else
                  cout << "ERROR Open Function" << endl;
              
              moduleMenuAction *mnuAction; //declara action como ponteiro
              mnuAction = new moduleMenuAction(QString(myFunction()->getName()), this); //instancia objeto com o nome do plugin
              mnuAction->setModuleBase(myFunction());//seta o plugin no action
              connect(mnuAction, SIGNAL(triggered()), this, SLOT(run()));
              pluginMenu->addAction(mnuAction); //adiciona action como item de menu
              lista.append(mnuAction); //registra na lista o action
           }   
    }
  
  
  //tratamento da execução dos plugins
  void ImageBForm::run()
  {
   moduleMenuAction *menuAction = qobject_cast<moduleMenuAction *>(sender()); //identifica qual moduleMenuAction foi clicado
   cout << "Executing Module:" << menuAction->getModuleBase()->getName() << endl; //executa a função
  
   mat_undo = mat_proc;
   
   menuAction->getModuleBase()->setData(mat_proc); //set data to plugin
   menuAction->getModuleBase()->process(); //process data
   mat_proc = menuAction->getModuleBase()->getData(); //get processed data
   
   
   //plugin wants to show a message box
   if (menuAction->getModuleBase()->showMessage())
     {
        QMessageBox::information(this,"plug-in: " + QString(menuAction->getModuleBase()->getDlgTitle()),
                              QString(menuAction->getModuleBase()->getMessage().c_str()));
     }
   
   //preserva tamanho original da imagem
   if (!image.isNull())
       image = imageB.getBModeImage(image.width(), image.height(), mat_proc, colormap );
   else
       image = imageB.getBModeImage(600, 400, mat_proc, colormap ); 
   
   imageLabel->setPixmap(QPixmap::fromImage(image));
   
  }


  void ImageBForm::clearPlugins()
  {
    for (int i = 0; i < lista.size(); ++i) {
         moduleMenuAction *mn = lista.at(i);
         delete(mn);
    }
    lista.clear();
  }


  void ImageBForm::undoPluginAction()
  {
    //preserva tamanho original da imagem
    if (!image.isNull())
        image = imageB.getBModeImage(image.width(), image.height(), mat_undo, colormap );
    else
        image = imageB.getBModeImage(600, 400, mat_undo, colormap ); 
   
    imageLabel->setPixmap(QPixmap::fromImage(image));
    
    mat_proc = mat_undo;
  }