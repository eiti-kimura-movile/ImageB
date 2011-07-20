 #include <QApplication>

    #include "bmodewidget.h"

    int main(int argc, char *argv[])
    {
        QApplication app(argc, argv);
        BModeWidget widget;
        widget.show();
        return app.exec();
    }