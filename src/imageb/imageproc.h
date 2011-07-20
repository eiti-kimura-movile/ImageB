#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//qt includes
#include <QStringList>
#include <QString>

#include "NumMeth.h"

using namespace std;

class ImageProc{

    public:
        ImageProc();
        
        //*******operacoes com vetor
        vector<double> loadFile(string);
        double mean(vector<double>);
        vector<double> norm(vector<double>);
        double min(vector<double>);
        double max(vector<double>);
        
        //*******operacoes com matrix
        Matrix loadFileM(string);
        Matrix loadFileM2(string);  
        Matrix loadMatrixFiles(QStringList, int, int);
        double mean(Matrix);
        double min(Matrix);
        double max(Matrix);
        void norm(Matrix&);
        void resize(Matrix&,int);
        void normTo255(Matrix&);
        void normTo255(double, double, Matrix&);
        void normToSize( Matrix&, int);
        Matrix createWave(Matrix, Matrix, Matrix);
        Matrix zeros(int);
        void adjustMatrixPow2( Matrix& );//ajusta tamanho do vetor para as fft's
    
        Matrix getCol(Matrix, int);
        void setCol(int, Matrix, Matrix&);
        Matrix getLine(Matrix, int);
        void setLine(int, Matrix, Matrix&);
 
        Matrix getVectorRef(double, double, double);// cria vetor de referencia R de 10mm a 90mm
        
        //*******transforma rapida e transformada inversa de fourier 
        void fft( Matrix& RealA, Matrix& ImagA);
        void ifft( Matrix& RealA, Matrix& ImagA);
        
        void fft2( Matrix& RealA, Matrix& ImagA);
        void ifft2( Matrix& RealA, Matrix& ImagA);
        //transformada de hilbert para calculo da envoltoria
        void hilbert( Matrix& mat );
        void hilbert2( Matrix& mat );  //faz hilbert das colunas e depois nas linhas sobrepostos
        void hilbert2D( Matrix& mat ); //hilbert em 2 dimensões
        
        void applyGain(Matrix, Matrix&);
        void applyGain(Matrix& mat, int v_start, int v_end);
        void createGainVector(Matrix&, int, int, int);
        void setAttenuation(int, int, double, Matrix&);
        void limiarCut(double,Matrix&);
        void timeCut(int, int, Matrix&);
        
        void meshgrid(Matrix&, Matrix&, Matrix);
        void meshgrid2D(Matrix&, Matrix&, Matrix, Matrix);
        void pol2cart( Matrix, Matrix, Matrix&, Matrix&);
        void pol2cart( double, Matrix, Matrix&, Matrix&);
         
        void adjustYAxis(Matrix&);//ajusta dados para que fiquem
      
        //escreve dados da matrix em arquivo
        void writeMatrixData(Matrix mat, string filename );
        
        //calcula o módulo de um número complexo
        Matrix modcomplex(Matrix real, Matrix imag);
         
        //******colormap jet
        Matrix createColorMap();
    
    private:
        vector<double> matrix2vector(Matrix);
        Matrix vector2matrix(vector<double>);
};

#endif

