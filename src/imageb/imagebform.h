#ifndef IMAGEBFORM_H
#define IMAGEBFORM_H

#include <QMainWindow>
#include <QDir>

#include "imageproc.h"
#include "imageb.h"

#include "frmProcSinal.h"
#include "frmCorteSinal.h"
#include "frmSFSAFT.h"
#include "frmBrightContrast.h"

#include "setupsinal.h"
#include "module/moduleMenuAction.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;

using namespace std;

//! A class to control the main window ImabeB
class ImageBForm : public QMainWindow
{
        Q_OBJECT

    public:
        //! the default constructor
        ImageBForm();
    
    private slots:
        //! opens a RF single data or a JPEG Image
        void open();
        //! opens a list of RF data
        void open_files();
    
        void zoomIn();
        void zoomOut();
        void normalSize();
        void fitToWindow();
    
        //! saves the image in jpg format
        void saveImage();
        //! it exports data in Matrix format
        void exportImage();
    
        void setColorMapGray();
        void setColorMapJet();
        void setColorMapPink();
    
        //! shows data in Cartesian coordinates
        void setNormalMode();
        //! shows focused data
        void setFocusMode();
        //! shows data in Polar coordinates
        void Cart2PolarImage();
    
        void setupSignal();
        void signalCut();
        void about();
        
        //! increases brightness
        void bright();
        
        //! perform SF algorithm
        void SFSAFT();
    
        void updateImage();
        void reloadImage(Matrix);
        void updateSFImage();
     
        //! clear the loaded plug-ins
        void clearPlugins();  
        //! undo one action performed to a plug-in
        void undoPluginAction();
        //! loads all plug-in from plugin directory
        void loadPlugins(); //carrega todos os plugins do diretório
        //! executes plugin main method
        void run();//executa operação dos plugins

    private:
        void createActions();
        void createMenus();
        void createToolBars();
        void updateActions();
        void scaleImage(double factor);
        void adjustScrollBar(QScrollBar *scrollBar, double factor);
    
        //! class for image processing
        ImageProc imgproc;
        //! class for process ultrasound images
        ImageB imageB;
        //! the color map used in image
        int colormap;
        //! image shown in the screen
        QImage image;
        //! matrix data files
        Matrix mat, mat_proc, mat_undo;

        QLabel *imageLabel;
        QScrollArea *scrollArea;
        double scaleFactor;

        QAction *openAct;
        QAction *openNAct;
        QAction *saveImageAct;
        QAction *exportImageAct;
        
        QAction *exitAct;
        QAction *zoomInAct;
        QAction *zoomOutAct;
        QAction *normalSizeAct;
        QAction *fitToWindowAct;
        
        QAction *grayAct;
        QAction *jetAct;
        QAction *pinkAct;
         
        QAction *imgSquareAct;
        QAction *imgSAFTAct;
        QAction *imgPolarAct;
        
        QAction *imgSetupAct;
        QAction *signalCutAct;
        QAction *imgBright;
        QAction *signalSFSAFT;
        
        QAction *aboutAct;
        QAction *aboutQtAct;
        
        QAction *refreshPluginAct;
        QAction *undoPluginAct;

        QMenu *fileMenu;
        QMenu *viewMenu;
        QMenu *signalMenu;
        QMenu *colorMapMenu;
        QMenu *helpMenu;
        QMenu *pluginMenu;
        
        QToolBar *fileToolBar;
        QToolBar *imageToolBar;
        
        bool focusMode;
        
        //! list with all moduleMenuAction, represents the actions from plug-ins 
        QList <moduleMenuAction*> lista; //lista com todas as QActions
};

#endif
