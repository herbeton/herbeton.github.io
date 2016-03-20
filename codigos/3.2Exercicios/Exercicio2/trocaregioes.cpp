#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;
  Mat imagem1, imagem2, imagem3, imagem4, imagemFinal;
  Vec3b val;

  //Transformou a matriz em formato de matriz
  image= imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)//caso a imagem seja diferende de bolhas.png
    cout << "nao abriu bolhas.png" << endl;

  namedWindow("Imagem Final",WINDOW_AUTOSIZE);
  int rows = image.rows;
  int cols = image.cols;
  std::cout << "Linhas: "<<rows << std::endl;
  std::cout << "Colunas: "<<cols << std::endl;

  //Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
  imagem1 = image(Rect(0,0,rows/2,cols/2));
  imagem2 = image(Rect(rows/2,rows/2,rows/2,cols/2));
  imagem3 = image(Rect(rows/2,0,rows/2,cols/2));
  imagem4 = image(Rect(0,rows/2,rows/2,cols/2));
  imagemFinal = image;
  // imshow("Imagem 1", imagem1);
  // imshow("Imagem 2", imagem2);
  // imshow("Imagem 3", imagem3);
  // imshow("Imagem 4", imagem4);
  // imshow("Image", image);
  // waitKey();

  imagem1.copyTo(imagemFinal(Rect(rows/2,0,rows/2,cols/2)));
  imagem2.copyTo(imagemFinal(Rect(0,rows/2,rows/2,cols/2)));
  imagem3.copyTo(imagemFinal(Rect(rows/2,rows/2,rows/2,cols/2)));
  imagem4.copyTo(imagemFinal(Rect(0,0,rows/2,cols/2)));
  imshow("Imagem Final", imagemFinal);
  waitKey();



  return 0;
}
