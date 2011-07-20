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

    #ifndef BModeWidget_H
    #define BModeWidget_H

    #include <QPixmap>
    #include <QWidget>
    #include <QMainWindow>
    
    #include "consumer.h"
    #include "producer.h"
    #include "imgproc/frmProcSinal.h"
    #include "frmSettings.h"
    
    #include "imgproc/setupsinal.h" 
    #include "imgproc/imageb.h"
    #include "imgproc/imageproc.h"
    
    using namespace std;
    

    class BModeWidget : public QMainWindow
    {
        Q_OBJECT

    public:
        //BModeWidget(QWidget *parent = 0);
        BModeWidget();
    
    protected:
        void paintEvent(QPaintEvent *event);
        void resizeEvent(QResizeEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void wheelEvent(QWheelEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void closeEvent(QCloseEvent *event);

    private slots:
        void updatePixmap(const QPixmap &image, double scaleFactor);
        void setNumFPS(int fps);
        void play_pause();
            
        void setColorMapGray();
        void setColorMapJet();
        void setColorMapPink();
       
        void setupSignal();
        void settings();

    private:
        void zoom(double zoomFactor);
        void scroll(int deltaX, int deltaY);
    
        void createToolBar();
        void createActions();
    
        Producer producer;
        Consumer consumer;
    
        int n_fps; //número de frames por segundo
    
        QPixmap pixmap;
        QPoint pixmapOffset;
        QPoint lastDragPos;
        double centerX;
        double centerY;
        double pixmapScale;
        double curScale;
    
        ImageB imageB;
        ImageProc imgproc;
    
        QToolBar *playerToolBar;
        QToolBar *imageToolBar;
        
        QActionGroup *actGroup;
        
        QAction *settingAct;
        QAction *playAct;
        QAction *imgSetupAct;
        
        QAction *pinkAct;
        QAction *grayAct;
        QAction *jetAct;

    };

    #endif
